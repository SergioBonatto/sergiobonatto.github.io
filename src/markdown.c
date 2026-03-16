#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include "ui.h"
#include "config.h"
#include "graph.h"
#include "contents_data.h"

static void render_graph_shortcode(const char *payload, size_t len){
	struct bar_segment segs[16];
	char buf[256];
	char *p, *tok, *saveptr1, *saveptr2;
	int h, w, n = 0;

	if (len >= sizeof(buf))
		return;

	memcpy(buf, payload, len);
	buf[len] = '\0';

	p = buf;
	tok = strtok_r(p, ";", &saveptr1);
	if (!tok)
		return;

	/* Parse height and width */
	h = atoi(tok);
	p = strchr(tok, ',');
	if (!p)
		return;
	w = atoi(p + 1);

	/* Parse segments */
	while ((tok = strtok_r(NULL, ";", &saveptr1)) && n < 16) {
		char *field;

		field = strtok_r(tok, ",", &saveptr2);
		if (!field) continue;
		segs[n].pct = (float)atof(field);

		field = strtok_r(NULL, ",", &saveptr2);
		if (!field) continue;
		segs[n].color_var = field;

		field = strtok_r(NULL, ",", &saveptr2);
		if (!field) continue;
		segs[n].opacity = (float)atof(field);

		field = strtok_r(NULL, ",", &saveptr2);
		if (!field) continue;

		switch (field[0]) {
		case 's': segs[n].style = BAR_SEG_SOLID;   break;
		case 'h': segs[n].style = BAR_SEG_HATCHED; break;
		default:  segs[n].style = BAR_SEG_EMPTY;   break;
		}
		n++;
	}

	if (n > 0)
		add_bar(h, w, segs, n);
}

static void render_line(const char *line, size_t len){
	const char *p, *alt_s, *alt_e, *url_s, *url_e;

	if (!len) {
		add_paragraph(line, 0);
		return;
	}

	/* Detect graph shortcode [[graph:...]] */
	if (len > 10 && !strncmp(line, "[[graph:", 8)) {
		p = memchr(line, ']', len);
		if (p && p[1] == ']') {
			render_graph_shortcode(line + 8, (p - line) - 8);
			return;
		}
	}

	if (line[0] == '#') {

		p = line;
		while (len && (*p == '#' || *p == ' ')) {
			p++;
			len--;
		}
		add_paragraph(p, len);
		return;
	}

	if (len > 4 && line[0] == '!' && line[1] == '[') {
		alt_s = line + 2;
		alt_e = memchr(alt_s, ']', len - 2);
		if (alt_e && alt_e[1] == '(') {
			url_s = alt_e + 2;
			url_e = memchr(url_s, ')', len - (url_s - line));
			if (url_e) {
				add_image(url_s, url_e - url_s, alt_s,
					  alt_e - alt_s, 1.0f);
				return;
			}
		}
	}

	add_paragraph(line, len);
}

EMSCRIPTEN_KEEPALIVE
void render_markdown(const char *content){
	const char *cur = content;
	const char *next;
	size_t len;
	int fm_count = 0;

	if (!content)
		return;

	while (*cur) {
		next = strchr(cur, '\n');
		len = next ? (size_t)(next - cur) : strlen(cur);

		if (len > 0 && cur[len - 1] == '\r')
			len--;

		if (len >= 3 && !strncmp(cur, "---", 3)) {
			fm_count++;
		} else if (fm_count % 2 == 0) {
			render_line(cur, len);
		}

		if (!next)
			break;
		cur = next + 1;
	}
}

void load_article(const char *slug){
	const char *body = get_article_body(slug);

	if (body)
		render_markdown(body);
}
