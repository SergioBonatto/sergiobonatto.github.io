#include <emscripten.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

#include "config.h"
#include "render.h"
#include "ui.h"
#include "graph.h"

static float runtime = 0.0f;
static bool is_dark = true;
static const struct theme *cur_theme;

enum page_state {
	PAGE_INITIAL,
	PAGE_HOME,
	PAGE_BLOG_INDEX,
	PAGE_ARTICLE
};

static enum page_state cur_page = PAGE_INITIAL;

static void render_home(void)
{
	const char *pfp = "public/pfp.avif";

	add_image(pfp, strlen(pfp), NULL, 0, 1.0f);
	
	add_paragraph(msg_whoami, strlen(msg_whoami));
	add_paragraph(msg_bio, strlen(msg_bio));
}

static void render_blog(void){
	int i;
	const char *header = "Blog Index";

	add_paragraph(header, strlen(header));
	for (i = 0; i < posts_count; i++) {
		add_blog_entry(posts[i].title, posts[i].date, i);
	}
}

EMSCRIPTEN_KEEPALIVE
void open_article(int index){
	if (index < 0 || index >= posts_count)
		return;

	cur_page = PAGE_ARTICLE;
	clear_feed();
	load_article(posts[index].slug);

	char buf[256];
	snprintf(buf, sizeof(buf), "#/post/%s", posts[index].slug);
	ui_sync_url(buf);
}

void open_article_by_slug(const char *slug) {
	for (int i = 0; i < posts_count; i++) {
		if (strcmp(posts[i].slug, slug) == 0) {
			open_article(i);
			return;
		}
	}
}

EMSCRIPTEN_KEEPALIVE
void switch_page(bool blog){
	enum page_state next_page = blog ? PAGE_BLOG_INDEX : PAGE_HOME;

	if (cur_page == next_page)
		return;

	cur_page = next_page;
	clear_feed();

	if (cur_page == PAGE_BLOG_INDEX) {
		render_blog();
		ui_sync_url("#/blog");
	} else {
		render_home();
		ui_sync_url("#/");
	}
}

EMSCRIPTEN_KEEPALIVE
void handle_route(const char *path) {
	if (!path || !*path) return;

	if (strcmp(path, "#/") == 0 || strcmp(path, "#/home") == 0 || strcmp(path, "") == 0) {
		switch_page(false);
	} else if (strcmp(path, "#/blog") == 0) {
		switch_page(true);
	} else if (strncmp(path, "#/post/", 7) == 0) {
		open_article_by_slug(path + 7);
	}
}

EMSCRIPTEN_KEEPALIVE
void toggle_theme(void)
{
	is_dark = !is_dark;
	cur_theme = is_dark ? &theme_dark : &theme_light;

	update_theme_toggle_label(is_dark ? ":light" : ":dark");
	update_theme_colors(cur_theme);
}

void main_tick(void)
{
	runtime += timing.tick_delta;
	draw_frame(runtime, msg_header, cur_theme->text, cur_theme->scanline);
}

int main(void)
{
	time_t t;
	struct tm tm;
	int year;
	char initial_hash[256];

	cur_theme = &theme_dark;

	init_graphics(cur_theme, UI_HEADER_HEIGHT);

	apply_style("#feed", css_feed);
	add_theme_toggle(":light", css_theme_toggle);
	add_nav_link(":blog", css_nav_blog, "nav-blog");
	add_nav_link(":home", css_nav_home, "nav-home");

	ui_init_router();
	ui_get_current_hash(initial_hash, sizeof(initial_hash));
	
	if (initial_hash[0] != '\0' && strcmp(initial_hash, "#/") != 0) {
		handle_route(initial_hash);
	} else {
		switch_page(false);
	}

	t = time(NULL);
	tm = *localtime(&t);
	year = tm.tm_year + 1900;

	add_footer(year, css_footer);

	emscripten_set_main_loop(main_tick, 0, 1);

	return 0;
}
