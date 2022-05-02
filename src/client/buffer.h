#ifndef __BUFFER_H
#define __BUFFER_H

#include "dstring.h"
#include "utils.h"

#include <stdint.h>

// 编辑区最大行数
#define BUFFER_LINES 10000

#define buffer_get_count(b) (b->count)

typedef struct {
    // 编辑区文本 以行为单位
    dstring lines[BUFFER_LINES];
    // 当前总文本行数
    uint16_t count;
} buffer;

buffer *buffer_init();
void buffer_release(buffer *b);
void buffer_insert_line(buffer *b, const dstring ds, int16_t row);
void buffer_insert_line_move(buffer *b, dstring ds, int16_t row);
void buffer_remove_line(buffer *b, int16_t row);
void buffer_concat_line(buffer *b, uint16_t row);
void buffer_trunc_line(buffer *b, uint16_t row, uint16_t col);
void buffer_input_char(buffer *b, uint16_t row, size_t col, char c);
void buffer_remove_char(buffer *b, uint16_t row, size_t col);
dstring buffer_get_line(buffer *b, uint16_t row);
size_t buffer_get_line_len(buffer *b, uint16_t row);

#endif