#include <emscripten.h>
#include <string.h>
#include <stdio.h>

#include "router.h"
#include "config.h"
#include "state.h"
#include "pages.h"
#include "ui.h"

#define ROUTE_POST_PREFIX "#/post/"

void switch_to_404(void)
{
	if (state.page == PAGE_404)
		return;

	state.page = PAGE_404;
	clear_feed();
	page_render_404();
}

EMSCRIPTEN_KEEPALIVE
void open_article(int index)
{
	char buf[256];

	if (index < 0 || index >= posts_count)
		return;

	state.page = PAGE_ARTICLE;
	clear_feed();
	load_article(index);

	snprintf(buf, sizeof(buf), "#/post/%s", posts[index].slug);
	ui_sync_url(buf);

	update_seo_metadata(posts[index].title, posts[index].description, buf);
}

int open_article_by_slug(const char *slug)
{
	int i;

	for (i = 0; i < posts_count; i++) {
		if (strcmp(posts[i].slug, slug) == 0) {
			open_article(i);
			return 0;
		}
	}
	return -1;
}

EMSCRIPTEN_KEEPALIVE
void switch_page(bool blog)
{
	enum page_state next_page = blog ? PAGE_BLOG_INDEX : PAGE_HOME;

	if (state.page == next_page)
		return;

	state.page = next_page;
	clear_feed();

	if (state.page == PAGE_BLOG_INDEX) {
		page_render_blog();
		ui_sync_url("#/blog");
	} else {
		page_render_home();
		ui_sync_url("#/");
	}
}

EMSCRIPTEN_KEEPALIVE
void handle_route(const char *path)
{
	if (!path || !*path)
		return;

	if (strcmp(path, "#/") == 0 || strcmp(path, "#/home") == 0 ||
	    strcmp(path, "") == 0) {
		switch_page(false);
	} else if (strcmp(path, "#/blog") == 0) {
		switch_page(true);
	} else if (strncmp(path, ROUTE_POST_PREFIX,
		   strlen(ROUTE_POST_PREFIX)) == 0) {
		if (open_article_by_slug(path + strlen(ROUTE_POST_PREFIX)) != 0) {
			switch_to_404();
		}
	} else {
		switch_to_404();
	}
}
