#ifndef PROJECT_SRC_BUFFER_H
#define PROJECT_SRC_BUFFER_H

#include <stddef.h>
#include <stdarg.h>
#include <stdbool.h>

#define BUFFER_CAPACITY (512 * 1024) 

typedef struct {
    char data[BUFFER_CAPACITY];
    size_t len;
    bool overflow;
} Buffer;

extern Buffer g_html_buf;

void buf_reset(Buffer *b);
void buf_append(Buffer *b, const char *str);
void buf_append_len(Buffer *b, const char *str, size_t len);
void buf_printf(Buffer *b, const char *fmt, ...);
void buf_escape(Buffer *b, const char *str, size_t len);

#endif
