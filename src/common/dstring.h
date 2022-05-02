#ifndef __DSTRING_H
#define __DSTRING_H

#include <stddef.h>
#include <stdint.h>

typedef char *dstring;
typedef struct {
    size_t len;
    size_t cap;
    char data[];
} dsh;

dstring dstring_create();
dstring dstring_build(const char *str);
dstring dstring_build_slice(const char *str, size_t start, size_t len);
size_t dstring_get_length(dstring ds);
void dstring_release(dstring ds);
dstring dstring_insert(dstring ds, const char *str, int index);
dstring dstring_insert_char(dstring ds, char c, int index);
dstring dstring_append(dstring ds, const char *str);
dstring dstring_remove(dstring ds, size_t start, size_t len);
dstring dstring_clear(dstring ds);
dstring dstring_copy(const dstring ds);
dstring dstring_copy_slice(const dstring ds, size_t start, size_t len);
size_t dstring_next_word(dstring ds, size_t start, uint8_t direct);

// sprintf for dstring
#define dsprintf(raw, args...) \
    ({  char* temp_str = malloc(1024 * sizeof(char));\
        sprintf(temp_str, args);\
        dstring_release(raw);\
        raw = dstring_build(temp_str);\
        free(temp_str); })

#endif