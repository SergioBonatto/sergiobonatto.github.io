#ifndef PROJECT_SRC_JS_API_H
#define PROJECT_SRC_JS_API_H

#include <stddef.h>
#include "config.h" // For struct theme

// From sys.c
void sys_set_html(const char *selector, const char *html);
void sys_set_text(const char *selector, const char *text);
void sys_set_style(const char *selector, const char *css);
void sys_append_child(const char *parent_sel, const char *tag, const char *html);
void sys_scroll_to_bottom(const char *selector);
void sys_update_theme_vars(int bg_idx, int tx_idx, int dim_idx, int accent_idx, const char *const *palette_ptr);
void sys_init_router(void);
void sys_update_url(const char *path);
void sys_get_url_hash(char *buf, size_t max_len);
void sys_set_meta(const char *title, const char *desc, const char *url);
void sys_console_log(const char *msg);
double sys_now(void);

// From render.c
void update_theme_colors(const struct theme *t, const char *const *palette);
void init_graphics(const struct theme *t, int header_h);
void render_update_strings(const char *label_ptr, int text_color_idx, const char *const *palette);
void apply_style(const char *selector_cstr, const char *style_cstr);
void draw_frame(float t);

// From ui.c
void add_theme_toggle(const char *label_cstr, const char *style_cstr);
void add_nav_link(const char *label_cstr, const char *style_cstr, const char *id_cstr);
void update_theme_toggle_label(const char *label_cstr);

#endif
