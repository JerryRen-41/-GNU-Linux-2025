#include "mbuf.h"

int mbuf_reserve(mbuf *buf, size_t newcap) {
    if (newcap <= buf->cap)
        return 0;

    size_t cap = buf->cap ? buf->cap : 16;
    while (cap < newcap)
        cap *= 2;

    char *newdata = realloc(buf->data, cap);
    if (!newdata)
        return -1;

    buf->data = newdata;
    buf->cap = cap;
    return 0;
}

