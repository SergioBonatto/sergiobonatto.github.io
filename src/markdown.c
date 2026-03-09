#include <emscripten.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ui.h"
#include "config.h"

static void render_line(char *line) {
	if (!line) return;

	size_t len = strlen(line);
	if (len == 0) {
		// Just add an empty line to preserve vertical rhythm
		add_paragraph("");
		return;
	}

	// Simple heading #
	if (line[0] == '#') {
		char *text = line;
		while (*text == '#' || *text == ' ') text++;
		add_paragraph(text);
		return;
	}

	// Simple image ![alt](url)
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

	// Default paragraph
	add_paragraph(line);
}

EMSCRIPTEN_KEEPALIVE
void render_markdown(const char *content) {
	if (!content) return;

	char *copy = strdup(content);
	char *cursor = copy;
	int in_frontmatter = 0;
	int separator_count = 0;

	// Process line by line manually to preserve empty lines
	while (cursor && *cursor) {
		char *next_line = strchr(cursor, '\n');
		if (next_line) {
			*next_line = '\0';
			next_line++;
		}

		// Check for Frontmatter separator "---" (ignoring potential \r or spaces)
		int is_separator = (strncmp(cursor, "---", 3) == 0);
		if (is_separator && (cursor[3] == '\0' || cursor[3] == '\r' || cursor[3] == ' ')) {
			separator_count++;
			if (separator_count == 1) {
				in_frontmatter = 1;
			} else if (separator_count == 2) {
				in_frontmatter = 0;
			}
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
			if (!res.ok) throw new Error("Post not found");
			return res.text();
		})
		.then(text => {
			if (Module._render_markdown) {
				const length = lengthBytesUTF8(text) + 1;
				const ptr = Module._malloc(length);
				stringToUTF8(text, ptr, length);
				Module._render_markdown(ptr);
				Module._free(ptr);
			} else {
				console.error("Markdown Loader Error: Module._render_markdown not found");
			}
		})
		.catch(err => {
			console.error("Markdown Loader Error:", err);
			const errorMsg = "Error: Could not load '" + slug + "'.";
			const length = lengthBytesUTF8(errorMsg) + 1;
			const ptr = Module._malloc(length);
			stringToUTF8(errorMsg, ptr, length);
			if (Module._add_paragraph) {
				Module._add_paragraph(ptr);
			}
			Module._free(ptr);
		});
});

void load_article(const char *slug) {
	fetch_article_internal(slug);
}
