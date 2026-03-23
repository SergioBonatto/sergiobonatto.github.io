#ifndef PROJECT_SRC_RENDER_H
#define PROJECT_SRC_RENDER_H

#include "config.h"

void init_graphics(const struct theme *theme, int header_height);
void apply_style(const char *selector, const char *css_text);
void update_theme_colors(const struct theme *theme, const char *const *palette);
void render_update_strings(const char *label, int text_color_idx,
			   const char *const *palette);
void draw_frame(float t);

#endif
