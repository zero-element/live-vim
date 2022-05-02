#include "buffer.h"
#include "dstring.h"

#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE sizeof(buffer)
#define LINE_SIZE sizeof(dstring)

buffer *buffer_init() {
    buffer *b;
    b = malloc(BUFFER_SIZE);
    b->lines[0] = dstring_create();
    b->count = 1;
    return b;
};

void buffer_release(buffer *b) {
    for (int i = 0; i < b->count; i++) {
        dstring_release(b->lines[i]);
    }
    free(b);
};

/* 插入新行到指定行号 采用拷贝构造（拷贝原有字符串）
 * 
 * row为行号 下标从0开始
 */
void buffer_insert_line(buffer *b, const dstring ds, int16_t row) {
    if (row > b->count || row < -1 || b->count >= BUFFER_LINES) return;
    if (row == -1) row = b->count;

    memmove(b->lines + row + 1, b->lines + row, (b->count - row) * LINE_SIZE);
    b->count++;

    b->lines[row] = dstring_copy(ds);
};

/* 插入新行到指定行号 采用拷贝构造（仅拷贝指针）
 * 
 * row为行号 下标从0开始
 */
void buffer_insert_line_move(buffer *b, dstring ds, int16_t index) {
    if (index > b->count || index < -1 || b->count >= BUFFER_LINES) return;
    if (index == -1) index = b->count;

    memmove(b->lines + index + 1, b->lines + index, (b->count - index) * LINE_SIZE);
    b->count++;

    b->lines[index] = ds;
};

/* 删除指定行 */
void buffer_remove_line(buffer *b, int16_t row) {
    if (b->count == 0 || row >= b->count || row < -1) return;
    if (row == -1) row = b->count - 1;

    dstring_release(b->lines[row]);
    memmove(b->lines + row, b->lines + row + 1, (b->count - row - 1) * LINE_SIZE);
    b->count--;
};

/* 将指定行与下一行进行拼接 返回指定行拼接前长度 */
void buffer_concat_line(buffer *b, uint16_t row) {
    if (b->count < 2 || row >= b->count - 1) return;

    size_t pos = buffer_get_line_len(b, row);

    b->lines[row] = dstring_append(b->lines[row], b->lines[row + 1]);
    buffer_remove_line(b, row + 1);
};

/* 将指定行截断
 *
 * row为行号 col为行内截断位置 下标从0开始
 */
void buffer_trunc_line(buffer *b, uint16_t row, uint16_t col) {
    if (row >= b->count || col > buffer_get_line_len(b, row)) return;

    size_t end = buffer_get_line_len(b, row);

    // 拷贝当前行光标之后文本
    dstring slice = dstring_copy_slice(b->lines[row], col, end - col);
    b->lines[row] = dstring_remove(b->lines[row], col, end - col);

    // 插入新行
    buffer_insert_line_move(b, slice, row + 1);
};

/* 在指定行插入字符
 *
 * row为行号 col为行内插入位置 下标从0开始
 */
void buffer_input_char(buffer *b, uint16_t row, size_t col, char c) {
    if (row >= b->count) return;
    b->lines[row] = dstring_insert_char(b->lines[row], c, col);
}

/* 在指定行删除字符 */
void buffer_remove_char(buffer *b, uint16_t row, size_t col) {
    if (row >= b->count) return;
    b->lines[row] = dstring_remove(b->lines[row], col, 1);
}

/* 获取指定行字符串 */
inline dstring buffer_get_line(buffer *b, uint16_t row) {
    return b->lines[row];
};

/* 获取指定行字符串长度 */
inline size_t buffer_get_line_len(buffer *b, uint16_t row) {
    return dstring_get_length(b->lines[row]);
};