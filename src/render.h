#ifndef RENDER_H
#define RENDER_H

void init_graphics(const char* bg, const char* text, int header_height);
void apply_style(const char* selector, const char* css_text);
void update_theme_colors(const char* bg, const char* text, const char* scanline);
void draw_frame(float t, const char* label, const char* text_color, const char* scanline_color);

#endif
