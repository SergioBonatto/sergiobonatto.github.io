#ifndef PROJECT_SRC_CONFIG_H
#define PROJECT_SRC_CONFIG_H

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
};

extern const struct theme theme_dark;
extern const struct theme theme_light;

extern const char *css_theme_toggle;
extern const char *css_feed;
extern const char *css_footer;

#define UI_HEADER_HEIGHT 180

extern const char *msg_header;
extern const char *msg_whoami;
extern const char *msg_bio;

#endif
