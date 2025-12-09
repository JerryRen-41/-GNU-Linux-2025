#ifndef MBUF_H
#define MBUF_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    char *data;
    size_t len;
    size_t cap;
} mbuf;

/* 初始化 */
#define mbuf_init(buf)      \
    do {                    \
        (buf)->data = NULL; \
        (buf)->len = 0;     \
        (buf)->cap = 0;     \
    } while (0)

/* 释放 */
#define mbuf_free(buf)      \
    do {                    \
        free((buf)->data);  \
        (buf)->data = NULL; \
        (buf)->len = 0;     \
        (buf)->cap = 0;     \
    } while (0)

/* 确保容量 */
int mbuf_reserve(mbuf *buf, size_t newcap);

/* 追加数据 */
static inline int mbuf_append(mbuf *buf, const char *s, size_t n) {
    if (buf->len + n > buf->cap) {
        if (mbuf_reserve(buf, buf->len + n) != 0)
            return -1;
    }
    memcpy(buf->data + buf->len, s, n);
    buf->len += n;
    return 0;
}

#endif

