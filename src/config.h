#ifndef PROJECT_SRC_CONFIG_H
#define PROJECT_SRC_CONFIG_H

#include <stdbool.h>

struct timing_config {
	float tick_delta;
	float flicker_speed;
	float flicker_base;
	float flicker_amp;
	float scanline_speed;
};

extern const struct timing_config timing;

struct theme {
	const char *bg;
	const char *text;
	const char *dim_text;
	const char *scanline;
	const char *accent;

	/* Nord Palette */
	const char *nord0;  /* #2E3440 - Polar Night */
	const char *nord1;  /* #3B4252 */
	const char *nord2;  /* #434C5E */
	const char *nord3;  /* #4C566A */
	const char *nord4;  /* #D8DEE9 - Snow Storm */
	const char *nord5;  /* #E5E9F0 */
	const char *nord6;  /* #ECEFF4 */
	const char *nord7;  /* #8FBCBB - Frost */
	const char *nord8;  /* #88C0D0 */
	const char *nord9;  /* #81A1C1 */
	const char *nord10; /* #5E81AC */
	const char *nord11; /* #BF616A - Aurora */
	const char *nord12; /* #D08770 */
	const char *nord13; /* #EBCB8B */
	const char *nord14; /* #A3BE8C */
	const char *nord15; /* #B48EAD */
};

extern const struct theme theme_dark;
extern const struct theme theme_light;

extern const char *css_theme_toggle;
extern const char *css_nav_home;
extern const char *css_nav_blog;
extern const char *css_feed;
extern const char *css_footer;

struct blog_post {
	const char *title;
	const char *date;
	const char *slug;
	const char *description;
};

/* Defined in contents_data.h (auto-generated) */
extern const struct blog_post posts[];
extern const int posts_count;

void load_article(const char *slug);

enum page_state {
	PAGE_INITIAL,
	PAGE_HOME,
	PAGE_BLOG_INDEX,
	PAGE_ARTICLE,
	PAGE_404
};

struct site_state {
	float runtime;
	bool is_dark;
	const struct theme *theme;
	enum page_state page;
};

extern struct site_state state;

#define UI_HEADER_HEIGHT 180

extern const char *msg_header;
extern const char *msg_whoami;
extern const char *msg_bio;
extern const char *msg_404_title;
extern const char *msg_404_body;

#endif
