#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "buffer.h"

Buffer g_html_buf;

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
    
    int remaining = BUFFER_CAPACITY - b->len;
    int written = vsnprintf(b->data + b->len, remaining, fmt, args);
    
    va_end(args);

    if (written < 0 || written >= remaining) {
        b->overflow = true;
    } else {
        b->len += written;
    }
}

void buf_escape(Buffer *b, const char *str, size_t len) {
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
            buf_append(b, entity);
        } else {
            if (b->len < BUFFER_CAPACITY - 1) {
                b->data[b->len++] = c;
            } else {
                b->overflow = true;
                break;
            }
        }
    }
    b->data[b->len] = '\0';
}
