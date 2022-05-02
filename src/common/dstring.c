/*
 * 一个简易动态字符串实现
 * 已经实现了部分接口，你可以根据需要，额外添加功能
 * 动态字符串扩容缩容可能导致内存地址变化，调用相关函数时请注意不要忽略返回值
 */

#include "dstring.h"
#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define STR_MIN_SIZE 24
#define CHAR_SIZE sizeof(char)
#define DSTR_SIZE sizeof(dsh)

#define DSTR_TO_DSTRING(sh) ((char *)sh->data)

/* 创建动态字符串 */
dstring dstring_create() {
    dsh *sh;

    sh = malloc(DSTR_SIZE + STR_MIN_SIZE);
    sh->data[0] = '\0';

    sh->cap = STR_MIN_SIZE;
    sh->len = 0;

    return DSTR_TO_DSTRING(sh);
}

/* 拷贝构造动态字符串 */
dstring dstring_build(const char *str) {
    dsh *sh;
    size_t len = strlen(str);
    int newSize = max(next_size(len + 1), STR_MIN_SIZE);

    sh = malloc(DSTR_SIZE + newSize);
    memmove(sh->data, str, len + 1);

    sh->cap = newSize;
    sh->len = len;

    return DSTR_TO_DSTRING(sh);
}

/* 拷贝字符串切片
 * 
 * start为起始位置 len为拷贝长度 下标从0开始
 */
dstring dstring_build_slice(const char *str, size_t start, size_t len) {
    dsh *sh;
    if (len == 0) return dstring_create();

    int newSize = max(next_size(len + 1), STR_MIN_SIZE);

    sh = malloc(DSTR_SIZE + newSize);
    memmove(sh->data, str + start, len);

    sh->data[len] = '\0';
    sh->cap = newSize;
    sh->len = len;

    return DSTR_TO_DSTRING(sh);
}

inline size_t dstring_get_length(dstring ds) {
    return ((dsh *)(ds - DSTR_SIZE))->len;
};

/* 释放字符串 */
void dstring_release(dstring ds) {
    if (ds == NULL)
        return;
    free(ds - DSTR_SIZE);
}

/* 字符串扩容/缩容 */
dsh *_dstring_resize(dsh *sh, size_t len) {
    if (sh == NULL)
        return NULL;

    int newSize = max(next_size(len + 1), STR_MIN_SIZE);

    if (sh->cap == newSize)
        return sh;

    sh = realloc(sh, DSTR_SIZE + newSize);
    sh->cap = newSize;
    return sh;
}

dsh *_dstring_insert(dsh *sh, const char *str, size_t start, size_t len) {
    int dsLen = sh->len;

    int newLen = dsLen + len;
    sh = _dstring_resize(sh, newLen);
    // 拷贝源字符串末尾
    memmove(sh->data + start + len, sh->data + start, (dsLen - start + 1) * CHAR_SIZE);
    // 拷贝待插入字符串
    memmove(sh->data + start, str, len * CHAR_SIZE);

    sh->len = newLen;
    return sh;
}

/* 插入字符串
 * 
 * index==-1表示插入到末尾
*/
dstring dstring_insert(dstring ds, const char *str, int index) {
    if (ds == NULL || index < -1)
        return ds;

    int strLen = strlen(str);
    if (strLen == 0)
        return ds;

    dsh *sh = (dsh *)(ds - DSTR_SIZE);
    int dsLen = sh->len;
    if (index > dsLen) return ds;
    if (index == -1) index = dsLen;

    sh = _dstring_insert(sh, str, index, strLen);
    return DSTR_TO_DSTRING(sh);
}

/* 插入字符
 * 
 * index==-1表示插入到末尾
*/
dstring dstring_insert_char(dstring ds, char c, int index) {
    if (ds == NULL || index < -1)
        return ds;

    dsh *sh = (dsh *)(ds - DSTR_SIZE);
    int dsLen = sh->len;
    if (index > dsLen) return ds;
    if (index == -1) index = dsLen;

    sh = _dstring_insert(sh, &c, index, 1);
    return DSTR_TO_DSTRING(sh);
}

/* 追加字符串 */
dstring dstring_append(dstring ds, const char *str) {
    return dstring_insert(ds, str, -1);
}

/* 删除字符串 */
dstring dstring_remove(dstring ds, size_t start, size_t len) {
    if (len == 0) return ds;

    dsh *sh = (dsh *)(ds - DSTR_SIZE);
    int dsLen = sh->len;
    int end = start + len;
    if (end > dsLen) return ds;

    // 拷贝源字符串末尾
    memmove(sh->data + start, sh->data + end, (dsLen - end + 1) * CHAR_SIZE);
    int newLen = dsLen - len;
    sh = _dstring_resize(sh, newLen);

    sh->len = newLen;
    return DSTR_TO_DSTRING(sh);
}

/* 清空字符串 */
dstring dstring_clear(dstring ds) {
    dsh *sh = (dsh *)(ds - DSTR_SIZE);
    sh->len = 0;
    sh->data[0] = '\0';
    return DSTR_TO_DSTRING(sh);
}

/* 拷贝动态字符串 */
dstring dstring_copy(const dstring ds) {
    return dstring_build(ds);
}

/* 拷贝字符串切片
 *
 * 若长度超出源字符串，则拷贝start后全部内容
*/
dstring dstring_copy_slice(const dstring ds, size_t start, size_t len) {
    dsh *sh;
    if (len == 0) return dstring_create();
    size_t rawLen = dstring_get_length(ds);
    if (start >= rawLen) return dstring_create();
    if (rawLen < start + len) len = rawLen - start;

    int newSize = max(next_size(len + 1), STR_MIN_SIZE);

    sh = malloc(DSTR_SIZE + newSize);
    memmove(sh->data, ds + start, len);

    sh->data[len] = '\0';
    sh->cap = newSize;
    sh->len = len;

    return DSTR_TO_DSTRING(sh);
}

/* TODO 查找下一单词位置
 * 
 * direct==0表示向前查找 direct==1表示向后查找
 */
size_t dstring_next_word(dstring ds, size_t start, uint8_t direct) {
    // TODO level 2
    return 0;
};