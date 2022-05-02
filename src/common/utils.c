#include <stddef.h>

/* 下一次扩容容量 */
size_t next_size(size_t x) {
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;
    if (x + 1 == 0) return x;
    return x + 1;
}