#ifndef PROJECT_SRC_MARKDOWN_H
#define PROJECT_SRC_MARKDOWN_H

#include "buffer.h"

void load_article(Buffer *b, int index);
void render_markdown_to_buffer(Buffer *b, const char *content);

#endif
