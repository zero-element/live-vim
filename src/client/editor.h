#ifndef __EDITOR_H
#define __EDITOR_H

#include "buffer.h"
#include <stdint.h>

#define EDITOR_FLAG_EXIT 0x1
#define EDITOR_FLAG_SAVE 0x2
#define EDITOR_FLAG_ILLEGAL 0x4

#define EDITOR_MODE_NORMAL 0x1
#define EDITOR_MODE_INSERT 0x2
#define EDITOR_MODE_COMMAND 0x3

#define editor_get_flag(e) (e->flag)

typedef struct {
    // 文本编辑区缓存
    buffer *buffer;
    // 光标屏幕绝对坐标
    uint16_t x, y;
    // 光标文本实际坐标
    uint16_t row, col;
    // 编辑区域高宽
    uint16_t height, width;
    // 顶部文本行号
    uint16_t top;
    // 最左侧文本列号
    uint16_t left;
    // 已输入命令
    dstring cmd;
    // 状态提示
    dstring status;
    uint8_t flag;
    uint8_t mode;
} editor;

editor *editor_init(int height, int width);
void editor_release(editor *e);
void editor_open_file(editor *e, char *file_name);
void editor_save_file(editor *e);
void editor_handle_input(editor *e, int c);
void editor_update(editor *e);

#endif