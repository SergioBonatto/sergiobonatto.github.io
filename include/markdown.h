#ifndef PROJECT_SRC_MARKDOWN_H
#define PROJECT_SRC_MARKDOWN_H

#include <stddef.h>

void render_markdown(const char *content);
void render_text(const char *text, size_t len);
void load_article(int index);

#endif
