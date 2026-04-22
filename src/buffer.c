#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "buffer.h"

Buffer g_html_buf;

static void buf_append_char(Buffer *b, char c) {
    if (b->overflow) return;
    if (b->len + 1 >= BUFFER_CAPACITY) {
        b->overflow = true;
        return;
    }
    b->data[b->len++] = c;
    b->data[b->len] = '\0';
}

void buf_reset(Buffer *b) {
    b->len = 0;
    b->data[0] = '\0';
    b->overflow = false;
}

void buf_append_len(Buffer *b, const char *str, size_t len) {
    if (b->overflow || b->len + len >= BUFFER_CAPACITY) {
        b->overflow = true;
        return;
    }
    memcpy(b->data + b->len, str, len);
    b->len += len;
    b->data[b->len] = '\0';
}

void buf_append(Buffer *b, const char *str) {
    buf_append_len(b, str, strlen(str));
}

void buf_printf(Buffer *b, const char *fmt, ...) {
    if (b->overflow) return;

    va_list args;
    va_start(args, fmt);

    size_t remaining = BUFFER_CAPACITY - b->len;
    int written = vsnprintf(b->data + b->len, remaining, fmt, args);
    
    va_end(args);

    if (written < 0 || (size_t)written >= remaining) {
        b->overflow = true;
    } else {
        b->len += (size_t)written;
    }
}

static void buf_escape_impl(Buffer *b, const char *str, size_t len) {
    if (b->overflow) return;

    for (size_t i = 0; i < len; i++) {
        char c = str[i];
        const char *entity = NULL;

        switch (c) {
            case '&': entity = "&amp;"; break;
            case '<': entity = "&lt;"; break;
            case '>': entity = "&gt;"; break;
            case '"': entity = "&quot;"; break;
            case '\'': entity = "&#39;"; break;
        }

        if (entity) {
            buf_append_len(b, entity, strlen(entity));
        } else {
            buf_append_char(b, c);
        }
    }
}

void buf_escape(Buffer *b, const char *str, size_t len) {
    buf_escape_impl(b, str, len);
}

void buf_append_attr_escaped(Buffer *b, const char *str, size_t len) {
    buf_escape_impl(b, str, len);
}

bool buf_overflowed(const Buffer *b) {
    return b->overflow;
}
