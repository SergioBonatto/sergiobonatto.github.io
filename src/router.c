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
	state.page = PAGE_404;
	ui_begin_render();
	page_render_404();
	ui_end_render();
}

EMSCRIPTEN_KEEPALIVE
void open_article(int index)
{
	char buf[256];

	if (index < 0 || index >= posts_count)
		return;

	state.page = PAGE_ARTICLE;
	ui_begin_render();
	load_article(index);
	ui_end_render();

	snprintf(buf, sizeof(buf), "#/post/%s", posts[index].slug);
	ui_sync_url(buf);

	update_seo_metadata(posts[index].title, posts[index].description, buf);
}

int open_article_by_slug(const char *slug)
{
	int index = find_post_index_by_slug(slug);
	if (index >= 0) {
		open_article(index);
		return 0;
	}
	return -1;
}

EMSCRIPTEN_KEEPALIVE
void handle_current_route(void)
{
	char path[256];

	ui_get_current_hash(path, sizeof(path));
	handle_route(path);
}

EMSCRIPTEN_KEEPALIVE
void switch_page(bool blog)
{
	enum page_state next_page = blog ? PAGE_BLOG_INDEX : PAGE_HOME;

	state.page = next_page;
	ui_begin_render();
	if (blog) {
		page_render_blog();
		ui_sync_url("#/blog");
	} else {
		page_render_home();
		ui_sync_url("#/");
	}
	ui_end_render();
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
