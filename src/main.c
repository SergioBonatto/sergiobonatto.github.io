#include <emscripten.h>
#include <stdbool.h>
#include <time.h>

#include "config.h"
#include "render.h"
#include "ui.h"

static float runtime = 0.0f;
static bool is_dark = true;
static const struct theme *cur_theme;

enum page_state {
	PAGE_HOME,
	PAGE_BLOG_INDEX,
	PAGE_ARTICLE
};

static enum page_state cur_page = PAGE_HOME;

static void render_home(void)
{
	add_image("public/pfp.avif", NULL, 1.0f);
	add_paragraph(msg_whoami);
	add_paragraph(msg_bio);
}

static void render_blog(void)
{
	int i;

	add_paragraph("Blog Index");
	for (i = 0; i < posts_count; i++) {
		add_blog_entry(posts[i].title, posts[i].date, i);
	}
}

EMSCRIPTEN_KEEPALIVE
void open_article(int index)
{
	if (index < 0 || index >= posts_count)
		return;

	cur_page = PAGE_ARTICLE;
	clear_feed();
	load_article(posts[index].slug);
}

EMSCRIPTEN_KEEPALIVE
void switch_page(bool blog)
{
	enum page_state next_page = blog ? PAGE_BLOG_INDEX : PAGE_HOME;

	if (cur_page == next_page)
		return;

	cur_page = next_page;
	clear_feed();

	if (cur_page == PAGE_BLOG_INDEX)
		render_blog();
	else
		render_home();
}

EMSCRIPTEN_KEEPALIVE
void toggle_theme(void)
{
	is_dark = !is_dark;
	cur_theme = is_dark ? &theme_dark : &theme_light;

	update_theme_toggle_label(is_dark ? ":light" : ":dark");
	update_theme_colors(cur_theme->bg, cur_theme->text,
			    cur_theme->dim_text, cur_theme->scanline);
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

	cur_theme = &theme_dark;

	init_graphics(cur_theme->bg, cur_theme->text, cur_theme->dim_text, UI_HEADER_HEIGHT);

	apply_style("#feed", css_feed);
	add_theme_toggle(":light", css_theme_toggle);
	add_nav_link(":blog", css_nav_blog, "nav-blog");
	add_nav_link(":home", css_nav_home, "nav-home");

	render_home();

	t = time(NULL);
	tm = *localtime(&t);
	year = tm.tm_year + 1900;

	add_footer(year, css_footer);

	emscripten_set_main_loop(main_tick, 0, 1);

	return 0;
}
