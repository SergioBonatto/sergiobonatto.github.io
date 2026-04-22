#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ui.h"
#include "config.h"
#include "contents_data.h"
#include "math.h"
#include "buffer.h"

static bool is_digit_ascii(char c) {
	return c >= '0' && c <= '9';
}

static int fast_atoi(const char **s, const char *end) {
	int res = 0;
	while (*s < end && is_digit_ascii(**s)) {
		res = res * 10 + (**s - '0');
		(*s)++;
	}
	return res;
}

static float fast_atof(const char **s, const char *end) {
	float res = 0.0f;
	while (*s < end && is_digit_ascii(**s)) {
		res = res * 10.0f + (**s - '0');
		(*s)++;
	}
	if (*s < end && **s == '.') {
		(*s)++;
		float frac = 0.1f;
		while (*s < end && is_digit_ascii(**s)) {
			res += (**s - '0') * frac;
			frac /= 10.0f;
			(*s)++;
		}
	}
	return res;
}

static const char *find_inline_math_end(const char *p, const char *end) {
	return memchr(p, '$', (size_t)(end - p));
}

static const char *find_display_math_end(const char *p, const char *end) {
	while (p < end) {
		const char *hit = memchr(p, '$', (size_t)(end - p));
		if (!hit || hit + 1 >= end) return NULL;
		if (hit[1] == '$') return hit;
		p = hit + 1;
	}
	return NULL;
}

static bool is_valid_graph_color_name(const char *name) {
	if (!name || name[0] != '-' || name[1] != '-')
		return false;

	for (const unsigned char *p = (const unsigned char *)name + 2; *p; p++) {
		if ((*p >= 'a' && *p <= 'z') ||
		    (*p >= 'A' && *p <= 'Z') ||
		    (*p >= '0' && *p <= '9') ||
		    *p == '-' || *p == '_') {
			continue;
		}
		return false;
	}

	return true;
}

static bool is_front_matter_delim(const char *line, size_t len) {
	return len == 3 && line[0] == '-' && line[1] == '-' && line[2] == '-';
}

static void render_graph_shortcode(const char *p, size_t len) {
	float pcts[16], opacs[16];
	const char *colors[16];
	int styles[16];
	char color_names[1024];
	char *cn_ptr = color_names;
	char *cn_end = color_names + sizeof(color_names) - 1;
	int h, w, n = 0;
	const char *end = p + len;

	h = fast_atoi(&p, end);
	if (p < end && *p == ',') p++;
	w = fast_atoi(&p, end);
	if (h <= 0 || w <= 0 || h > 4096 || w > 4096)
		return;

	while (p < end && n < 16) {
		while (p < end && (*p == ';' || *p == ' ')) p++;
		if (p >= end) break;

		pcts[n] = fast_atof(&p, end);
		if (p < end && *p == ',') p++;

		colors[n] = cn_ptr;
		while (p < end && *p != ',' && cn_ptr < cn_end) *cn_ptr++ = *p++;
		if (p < end && *p != ',')
			return;
		*cn_ptr++ = '\0';
		if (!is_valid_graph_color_name(colors[n]))
			return;

		if (p < end && *p == ',') p++;
		opacs[n] = fast_atof(&p, end);
		if (pcts[n] < 0.0f || pcts[n] > 1.0f || opacs[n] < 0.0f || opacs[n] > 1.0f)
			return;
		if (p < end && *p == ',') p++;

		if (p < end) {
			switch (*p) {
			case 's': styles[n] = BAR_SEG_SOLID;   break;
			case 'h': styles[n] = BAR_SEG_HATCHED; break;
			default:  styles[n] = BAR_SEG_EMPTY;   break;
			}
			while (p < end && *p != ';') p++;
		}
		n++;
	}

	if (n > 0) add_bar(h, w, pcts, colors, opacs, styles, n);
}

void render_text(const char *text, size_t len) {
	const char *p = text;
	const char *end = text + len;
	const char *start = p;

	while (p < end) {
		if (*p == '$') {
			if (p > start) {
				buf_escape(&g_html_buf, start, p - start);
			}

			bool display = false;
			p++;
			if (p < end && *p == '$') {
				display = true;
				p++;
			}

			const char *math_start = p;
			const char *math_end = NULL;
			if (display) {
				math_end = find_display_math_end(p, end);
				if (math_end) {
					math_to_mathml(&g_html_buf, math_start, math_end - math_start, true);
					p = math_end + 2;
				}
			} else {
				math_end = find_inline_math_end(p, end);
				if (math_end) {
					math_to_mathml(&g_html_buf, math_start, math_end - math_start, false);
					p = math_end + 1;
				}
			}

			if (math_end) {
				start = p;
			} else {
				// No closing $, treat as literal
				buf_append(&g_html_buf, "$");
				if (display) buf_append(&g_html_buf, "$");
				start = math_start;
				p = math_start;
			}
		} else {
			p++;
		}
	}

	if (p > start) {
		buf_escape(&g_html_buf, start, p - start);
	}
}

static void render_line(const char *line, size_t len) {
	if (!len) {
		buf_append(&g_html_buf, "<p class=\"para\"> </p>");
		return;
	}

	switch (line[0]) {
	case '#': {
		const char *p = line;
		size_t l = len;
		int level = 0;
		while (l && *p == '#') {
			p++;
			l--;
			level++;
		}
		while (l && *p == ' ') {
			p++;
			l--;
		}
		buf_printf(&g_html_buf, "<h%d class=\"para\">", level <= 6 ? level : 6);
		render_text(p, l);
		buf_printf(&g_html_buf, "</h%d>", level <= 6 ? level : 6);
		return;
	}
	case '!': {
		if (len <= 4 || line[1] != '[')
			break;

		const char *alt_s = line + 2;
		const char *alt_e = memchr(alt_s, ']', len - 2);
		if (!alt_e || alt_e + 1 >= line + len || alt_e[1] != '(')
			break;

		const char *url_s = alt_e + 2;
		const char *url_e = memchr(url_s, ')', len - (url_s - line));
		if (!url_e)
			break;

		add_image(url_s, url_e - url_s, alt_s, alt_e - alt_s, 1.0f, 0, 0, 0);
		return;
	}
	case '[': {
		if (len <= 10 || line[1] != '[' || strncmp(line + 2, "graph:", 6) != 0)
			break;

		const char *p = memchr(line, ']', len);
		if (!p || p + 1 >= line + len || p[1] != ']')
			break;

		render_graph_shortcode(line + 8, (p - line) - 8);
		return;
	}
	}

	buf_append(&g_html_buf, "<p class=\"para\">");
	render_text(line, len);
	buf_append(&g_html_buf, "</p>");
}


EMSCRIPTEN_KEEPALIVE
void render_markdown(const char *content) {
	const char *cur = content;
	const char *next;
	size_t len;
	bool in_code = false;
	const char *code_start = NULL;
	const char *lang_start = NULL;
	size_t lang_len = 0;
	bool allow_front_matter = true;
	bool in_front_matter = false;

	if (!content) return;

	while (*cur) {
		next = strchr(cur, '\n');
		len = next ? (size_t)(next - cur) : strlen(cur);

		if (len > 0 && cur[len - 1] == '\r') len--;

		if (len >= 3 && !strncmp(cur, "```", 3)) {
			if (!in_code) {
				in_code = true;
				lang_start = cur + 3;
				lang_len = len - 3;
				code_start = next ? next + 1 : NULL;
			} else {
				if (code_start) {
					size_t clen = cur - code_start;
					if (clen > 0 && code_start[clen - 1] == '\n') clen--;
					add_code_block((struct str_view){lang_start, lang_len}, (struct str_view){code_start, clen});
				}
				in_code = false;
			}
		} else if (!in_code) {
			if (allow_front_matter && !in_front_matter && is_front_matter_delim(cur, len)) {
				in_front_matter = true;
			} else if (in_front_matter && is_front_matter_delim(cur, len)) {
				in_front_matter = false;
				allow_front_matter = false;
			} else if (!in_front_matter) {
				render_line(cur, len);
				if (len != 0)
					allow_front_matter = false;
			}
		}

		if (!next) break;
		cur = next + 1;
	}
}

void load_article(int index) {
	const char *body = get_article_body(index);
	if (body) render_markdown(body);
}
