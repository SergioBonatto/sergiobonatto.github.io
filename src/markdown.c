#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ui.h"
#include "config.h"
#include "contents_data.h"

/*
 * Kernel-style: Zero-copy markdown parser.
 * Directly passes pointers from the read-only data segment to the UI layer.
 */

static void render_line_view(const char *line, size_t len)
{
	/* Empty line check */
	if (len == 0) {
		add_paragraph(line, 0);
		return;
	}

	/* Heading # detection (skip hashes and spaces) */
	if (line[0] == '#') {
		const char *text = line;
		size_t text_len = len;

		while (text_len > 0 && (*text == '#' || *text == ' ')) {
			text++;
			text_len--;
		}
		
		add_paragraph(text, text_len);
		return;
	}

	/* Image detection: ![alt](url) */
	if (len > 4 && line[0] == '!' && line[1] == '[') {
		const char *alt_start = line + 2;
		const char *alt_end = NULL;
		size_t i;
		
		/* Find end of alt text */
		for (i = 0; i < len - 2; i++) {
			if (alt_start[i] == ']') {
				alt_end = alt_start + i;
				break;
			}
		}

		/* Check for URL start */
		if (alt_end && *(alt_end + 1) == '(') {
			const char *url_start = alt_end + 2;
			const char *url_end = NULL;
			const char *p;

			/* Find end of URL */
			for (p = url_start; p < line + len; p++) {
				if (*p == ')') {
					url_end = p;
					break;
				}
			}

			if (url_end) {
				add_image(url_start, (size_t)(url_end - url_start),
					  alt_start, (size_t)(alt_end - alt_start),
					  1.0f);
				return;
			}
		}
	}

	/* Default paragraph: Pass direct pointer and length */
	add_paragraph(line, len);
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

		/* Handle CRLF or LF */
		if (actual_len > 0 && cursor[actual_len - 1] == '\r')
			actual_len--;

		/* Frontmatter skipping logic */
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
