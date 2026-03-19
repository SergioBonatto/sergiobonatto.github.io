#ifndef PROJECT_SRC_ROUTER_H
#define PROJECT_SRC_ROUTER_H

#include <stdbool.h>

void handle_route(const char *path);
void switch_page(bool blog);
void open_article(int index);
int open_article_by_slug(const char *slug);
void switch_to_404(void);

#endif
