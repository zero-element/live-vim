#ifndef __UTILS_H
#define __UTILS_H

#include <stddef.h>

#define max(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define min(a, b) \
    ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#define calc_row(width, len) (len ? (len - 1) / width + 1 : 1)
#define calc_x(width, len) (len / width)
#define calc_y(width, len) (len % width)

size_t next_size(size_t x);

#endif