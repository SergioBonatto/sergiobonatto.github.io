#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ui.h"
#include "config.h"
#include "contents_data.h"

static int fast_atoi(const char **s) {
	int res = 0;
	while (**s >= '0' && **s <= '9') {
		res = res * 10 + (**s - '0');
		(*s)++;
	}
	return res;
}

static float fast_atof(const char **s) {
	float res = 0.0f;
	while (**s >= '0' && **s <= '9') {
		res = res * 10.0f + (**s - '0');
		(*s)++;
	}
	if (**s == '.') {
		(*s)++;
		float frac = 0.1f;
		while (**s >= '0' && **s <= '9') {
			res += (**s - '0') * frac;
			frac /= 10.0f;
			(*s)++;
		}
	}
	return res;
}

static void render_graph_shortcode(const char *p, size_t len) {
	float pcts[16], opacities[16];
	const char *colors[16];
	int styles[16];
	char color_names[256];
	char *cn_ptr = color_names;
	int h, w, n = 0;
	const char *end = p + len;

	h = fast_atoi(&p);
	if (p < end && *p == ',') p++;
	w = fast_atoi(&p);

	while (p < end && n < 16) {
		while (p < end && (*p == ';' || *p == ' ')) p++;
		if (p >= end) break;

		pcts[n] = fast_atof(&p);
		if (p < end && *p == ',') p++;

		colors[n] = cn_ptr;
		while (p < end && *p != ',') {
			*cn_ptr++ = *p++;
		}
		*cn_ptr++ = '\0';

		if (p < end && *p == ',') p++;
		opacities[n] = fast_atof(&p);
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

	if (n > 0)
		add_bar(h, w, pcts, colors, opacities, styles, n);
}

static void render_line(const char *line, size_t len) {
	if (!len) {
		add_paragraph(line, 0);
		return;
	}

	switch (line[0]) {
	case '#': {
		const char *p = line;
		size_t l = len;
		while (l && (*p == '#' || *p == ' ')) {
			p++;
			l--;
		}
		add_paragraph(p, l);
		return;
	}
	case '!':
		if (len > 4 && line[1] == '[') {
			const char *alt_s = line + 2;
			const char *alt_e = memchr(alt_s, ']', len - 2);
			if (alt_e && alt_e[1] == '(') {
				const char *url_s = alt_e + 2;
				const char *url_e = memchr(url_s, ')', len - (url_s - line));
				if (url_e) {
					add_image(url_s, url_e - url_s, alt_s, alt_e - alt_s, 1.0f, 0, 0, 0);
					return;
				}
			}
		}
		break;
	case '[':
		if (len > 10 && line[1] == '[' && !strncmp(line + 2, "graph:", 6)) {
			const char *p = memchr(line, ']', len);
			if (p && p[1] == ']') {
				render_graph_shortcode(line + 8, (p - line) - 8);
				return;
			}
		}
		break;
	}

	add_paragraph(line, len);
}

EMSCRIPTEN_KEEPALIVE
void render_markdown(const char *content) {
	const char *cur = content;
	const char *next;
	size_t len;
	int fm_count = 0;
	bool in_code_block = false;
	const char *code_start = NULL;
	const char *lang_start = NULL;
	size_t lang_len = 0;

	if (!content)
		return;

	while (*cur) {
		next = strchr(cur, '\n');
		len = next ? (size_t)(next - cur) : strlen(cur);

		if (len > 0 && cur[len - 1] == '\r')
			len--;

		if (!in_code_block && len >= 3 && !strncmp(cur, "```", 3)) {
			in_code_block = true;
			lang_start = cur + 3;
			lang_len = len - 3;
			code_start = next ? next + 1 : NULL;
		} else if (in_code_block && len >= 3 && !strncmp(cur, "```", 3)) {
			if (code_start) {
				size_t code_len = cur - code_start;
				if (code_len > 0 && code_start[code_len - 1] == '\n')
					code_len--;
				add_code_block(lang_start, lang_len, code_start, code_len);
			}
			in_code_block = false;
			code_start = NULL;
		} else if (!in_code_block) {
			if (len >= 3 && cur[0] == '-' && cur[1] == '-' && cur[2] == '-') {
				fm_count++;
			} else if (fm_count % 2 == 0) {
				render_line(cur, len);
			}
		}

		if (!next)
			break;
		cur = next + 1;
	}
}

void load_article(int index) {
	const char *body = get_article_body(index);
	if (body)
		render_markdown(body);
}
