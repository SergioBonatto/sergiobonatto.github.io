#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ui.h"
#include "config.h"

static void render_line(char *line)
{
	size_t len;

	if (!line)
		return;

	len = strlen(line);
	if (len == 0) {
		add_paragraph("");
		return;
	}

	/* Simple heading # */
	if (line[0] == '#') {
		char *text = line;
		while (*text == '#' || *text == ' ')
			text++;
		add_paragraph(text);
		return;
	}

	/* Simple image ![alt](url) */
	if (line[0] == '!' && line[1] == '[') {
		char *alt_start = line + 2;
		char *alt_end = strchr(alt_start, ']');
		if (alt_end) {
			*alt_end = '\0';
			char *url_start = strchr(alt_end + 1, '(');
			if (url_start) {
				url_start++;
				char *url_end = strchr(url_start, ')');
				if (url_end) {
					*url_end = '\0';
					add_image(url_start, alt_start, 1.0f);
					return;
				}
			}
		}
	}

	add_paragraph(line);
}

EMSCRIPTEN_KEEPALIVE
void render_markdown(const char *content)
{
	char *copy, *cursor;
	int in_frontmatter = 0;
	int separator_count = 0;

	if (!content)
		return;

	copy = strdup(content);
	if (!copy)
		return;

	cursor = copy;

	while (cursor && *cursor) {
		char *next_line = strchr(cursor, '\n');
		if (next_line) {
			*next_line = '\0';
			next_line++;
		}

		/* Check for Frontmatter separator "---" */
		int is_separator = (strncmp(cursor, "---", 3) == 0);
		if (is_separator && (cursor[3] == '\0' || cursor[3] == '\r' || cursor[3] == ' ')) {
			separator_count++;
			if (separator_count == 1)
				in_frontmatter = 1;
			else if (separator_count == 2)
				in_frontmatter = 0;
		} else if (!in_frontmatter) {
			render_line(cursor);
		}

		cursor = next_line;
	}

	free(copy);
}

EM_JS(void, fetch_article_internal, (const char *slug_cstr), {
	const slug = UTF8ToString(slug_cstr);
	const url = `contents/${slug}.md`;

	fetch(url)
		.then(res => {
			if (!res.ok) throw new Error(`HTTP ${res.status} loading ${url}`);
			return res.text();
		})
		.then(text => {
			if (typeof Module._render_markdown === 'function') {
				const length = lengthBytesUTF8(text) + 1;
				const ptr = Module._malloc(length);
				stringToUTF8(text, ptr, length);
				Module._render_markdown(ptr);
				Module._free(ptr);
			}
		})
		.catch(err => {
			console.error("Markdown Error:", err);
			if (typeof Module._add_paragraph === 'function') {
				const msg = `Error: Could not load '${slug}'.`;
				const length = lengthBytesUTF8(msg) + 1;
				const ptr = Module._malloc(length);
				stringToUTF8(msg, ptr, length);
				Module._add_paragraph(ptr);
				Module._free(ptr);
			}
		});
});

void load_article(const char *slug)
{
	fetch_article_internal(slug);
}
