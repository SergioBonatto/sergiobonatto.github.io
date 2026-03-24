#ifndef PROJECT_SRC_SYS_H
#define PROJECT_SRC_SYS_H

#include <stddef.h>
#include <stdbool.h>

/* 
 * SYSTEM INTERFACE (Platform Abstraction Layer)
 */

void sys_set_html(const char *selector, const char *html);
void sys_set_text(const char *selector, const char *text);
void sys_set_style(const char *selector, const char *css);
void sys_append_child(const char *parent_sel, const char *tag, const char *html);
void sys_scroll_to_bottom(const char *selector);

void sys_update_url(const char *path);
void sys_get_url_hash(char *buf, size_t max_len);
void sys_init_router(void);

void sys_set_meta(const char *title, const char *desc, const char *url);
void sys_console_log(const char *msg);
double sys_now(void);

#endif
