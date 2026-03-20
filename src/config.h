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

extern const char *nord_palette[16];

struct theme {
	int bg;
	int text;
	int dim_text;
	int accent;
	int scanline;
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

#define UI_HEADER_HEIGHT 180

extern const char *msg_header;
extern const char *msg_whoami;
extern const char *msg_bio;
extern const char *msg_seo;
extern const char *msg_404_title;
extern const char *msg_404_body;

#endif
