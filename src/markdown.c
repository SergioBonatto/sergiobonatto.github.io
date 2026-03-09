#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ui.h"
#include "config.h"
#include "contents_data.h"

/*
 * Kernel-style: Zero-copy markdown parser using static buffers
 * to avoid heap fragmentation and runtime overhead.
 */

static void render_line_view(const char *line, size_t len)
{
	static char buf[4096];
	size_t copy_len;

	if (len == 0) {
		add_paragraph("");
		return;
	}

	/* Heading # */
	if (line[0] == '#') {
		const char *text = line;
		size_t text_len = len;

		while (text_len > 0 && (*text == '#' || *text == ' ')) {
			text++;
			text_len--;
		}
		
		copy_len = text_len < sizeof(buf) - 1 ? text_len : sizeof(buf) - 1;
		memcpy(buf, text, copy_len);
		buf[copy_len] = '\0';
		add_paragraph(buf);
		return;
	}

	/* Image ![alt](url) */
	if (len > 4 && line[0] == '!' && line[1] == '[') {
		const char *alt_start = line + 2;
		const char *alt_end = NULL;
		size_t i;
		
		for (i = 0; i < len - 2; i++) {
			if (alt_start[i] == ']') {
				alt_end = alt_start + i;
				break;
			}
		}

		if (alt_end && *(alt_end + 1) == '(') {
			const char *url_start = alt_end + 2;
			const char *url_end = NULL;
			const char *p;

			for (p = url_start; p < line + len; p++) {
				if (*p == ')') {
					url_end = p;
					break;
				}
			}

			if (url_end) {
				static char url_buf[1024], alt_buf[1024];
				size_t alt_len = alt_end - alt_start;
				size_t url_len = url_end - url_start;

				size_t a_copy = alt_len < 1023 ? alt_len : 1023;
				size_t u_copy = url_len < 1023 ? url_len : 1023;

				memcpy(alt_buf, alt_start, a_copy);
				alt_buf[a_copy] = '\0';
				memcpy(url_buf, url_start, u_copy);
				url_buf[u_copy] = '\0';

				add_image(url_buf, alt_buf, 1.0f);
				return;
			}
		}
	}

	/* Default paragraph */
	copy_len = len < sizeof(buf) - 1 ? len : sizeof(buf) - 1;
	memcpy(buf, line, copy_len);
	buf[copy_len] = '\0';
	add_paragraph(buf);
}

EMSCRIPTEN_KEEPALIVE
void render_markdown(const char *content)
{
	const char *cursor;
	int in_frontmatter = 0;
	int separator_count = 0;

	if (!content)
		return;

	cursor = content;
	while (*cursor) {
		const char *next_line = strchr(cursor, '\n');
		size_t line_len = next_line ? (size_t)(next_line - cursor) : strlen(cursor);
		size_t actual_len = line_len;

		if (actual_len > 0 && cursor[actual_len - 1] == '\r')
			actual_len--;

		if (actual_len >= 3 && strncmp(cursor, "---", 3) == 0) {
			separator_count++;
			in_frontmatter = (separator_count == 1);
		} else if (!in_frontmatter) {
			render_line_view(cursor, actual_len);
		}

		if (!next_line)
			break;
		cursor = next_line + 1;
	}
}

void load_article(const char *slug)
{
	const char *body = get_article_body(slug);
	if (body)
		render_markdown(body);
}
