#include "buffer.h"
#include "dstring.h"
#include "editor.h"

#include <stdint.h>
#include <stdio.h>
#include <curses.h>
#include <stdlib.h>
#include <string.h>

#define EDITOR_SIZE sizeof(editor)

// 初始化编辑器
editor *editor_init(int height, int width) {
    editor *e;
    e = malloc(EDITOR_SIZE);
    e->buffer = buffer_init(width);
    e->height = height - 1, e->width = width;
    e->flag = 0;
    e->mode = EDITOR_MODE_NORMAL;
    e->x = 0, e->y = 0;
    e->row = 0, e->col = 0;
    e->top = 0;
    e->cmd = dstring_create();
    e->status = dstring_create();
    return e;
};

// 销毁编辑器
void editor_release(editor *e) {
    buffer_release(e->buffer);
    dstring_release(e->cmd);
    dstring_release(e->status);
    free(e);
};

// 打开文件
void editor_open_file(editor *e, char *file_name){
    // TODO level 1
};

// 保存文件
void editor_save_file(editor *e){
    // TODO level 1
};

// 判断普通模式输入是否合法
uint8_t _editor_judge_normal(const char *cmd) {
    // TODO level 2
    return 0;
};

// 判断命令模式输入是否合法
uint8_t _editor_judge_command(const char *cmd) {
    // TODO level 4
    int len = strlen(cmd);
    if (len > 2) return 0;
    int flag = 0;
    switch (cmd[0]) {
    case 'w':
        flag |= EDITOR_FLAG_SAVE;
        if (len == 2 && cmd[1] == 'q') flag |= EDITOR_FLAG_EXIT;
        break;
    case 'q':
        flag |= EDITOR_FLAG_EXIT;
        if (len == 2) return 0;
        break;
    }
    return flag;
};

// 执行普通模式命令
void _editor_exec_normal(editor *e){
    // TODO level 2
};

// 执行命令模式命令
void _editor_exec_command(editor *e){
    // TODO level 4
};

void _editor_move_curse(editor *e, uint16_t row, uint16_t col) {
    // 上翻
    if (row < e->top) e->top = row;
    // 下滚
    if (row >= e->top + e->height) {
        e->top = max(0, row - e->height + 1);
    }

    // 左右移均采用右对齐
    if (col <= e->left || col > e->left + e->width - 1) e->left = max(0, col - e->width + 1);

    e->row = row, e->col = col;
    e->x = e->row - e->top, e->y = e->col - e->left;
}

void _editor_key_left(editor *e) {
    if (e->col == 0) return;
    e->col--;
}

void _editor_key_right(editor *e) {
    if (e->col == buffer_get_line_len(e->buffer, e->row)) return;
    e->col++;
}

void _editor_key_up(editor *e) {
    if (e->row == 0) return;

    e->row--;

    // 调整列位置
    int end = buffer_get_line_len(e->buffer, e->row);
    if (e->col > end) {
        // 原先光标位置大于当前行长度 光标移动到当前行尾
        e->col = end;
    }
}

void _editor_key_down(editor *e) {
    if (e->row == buffer_get_count(e->buffer) - 1) return;

    e->row++;
    // 调整列位置
    int end = buffer_get_line_len(e->buffer, e->row);
    if (e->col > end) {
        // 原先光标位置大于当前行长度 光标移动到当前行尾
        e->col = end;
    }
}

// 普通模式输入处理
void _editor_handle_input_normal(editor *e, int c) {
    switch (c) {
    case ':':
        e->mode = EDITOR_MODE_COMMAND;
        e->cmd = dstring_clear(e->cmd);
        break;
    case 'i':
        // TODO level 2
        // 这里进入输入模式的实现并不正确 可以考虑如何修正
        e->mode = EDITOR_MODE_INSERT;
        e->cmd = dstring_clear(e->cmd);
        break;
    case KEY_BACKSPACE:
    case 27: // esc
        e->cmd = dstring_clear(e->cmd);
        break;
    case KEY_ENTER:
    case 10: // enter
        _editor_exec_normal(e);
        e->cmd = dstring_clear(e->cmd);
        break;
    case KEY_LEFT:
        _editor_key_left(e);
        break;
    case KEY_RIGHT:
        _editor_key_right(e);
        break;
    case KEY_UP:
        _editor_key_up(e);
        break;
    case KEY_DOWN:
        _editor_key_down(e);
        break;
    default:
        e->cmd = dstring_insert_char(e->cmd, (char)c, -1);
        // 若命令合法 直接触发执行
        if (_editor_judge_normal(e->cmd)) {
            _editor_exec_normal(e);
            e->cmd = dstring_clear(e->cmd);
        }
    }
    _editor_move_curse(e, e->row, e->col);
}

// 插入模式输入处理
void _editor_handle_input_insert(editor *e, int c) {
    switch (c) {
    case 27: // esc
        e->mode = EDITOR_MODE_NORMAL;
        break;
    case KEY_ENTER:
    case 10: // enter
    {
        buffer_trunc_line(e->buffer, e->row, e->col);
        e->row++, e->col = 0;

    } break;
    case KEY_BACKSPACE:
    case 127: // backspace
        // 无需换行
        if (e->y > 0) {
            e->col--;
            buffer_remove_char(e->buffer, e->row, e->col);
            _editor_move_curse(e, e->row, e->col);
            break;
        }

        // 处理换行
        if (e->col > 0) {
            e->col--;
            buffer_remove_char(e->buffer, e->row, e->col);
        } else if (e->row > 0) {
            e->col = buffer_get_line_len(e->buffer, e->row - 1);
            e->row--;
            buffer_concat_line(e->buffer, e->row);
        }
        break;
    case KEY_LEFT:
        _editor_key_left(e);
        break;
    case KEY_RIGHT:
        _editor_key_right(e);
        break;
    case KEY_UP:
        _editor_key_up(e);
        break;
    case KEY_DOWN:
        _editor_key_down(e);
        break;
    default:
        buffer_input_char(e->buffer, e->row, e->col++, (char)c);
    }
    _editor_move_curse(e, e->row, e->col);
}

// 命令模式输入处理
void _editor_handle_input_command(editor *e, int c) {
    switch (c) {
    case KEY_ENTER:
    case 10: // enter
        e->flag |= _editor_judge_command(e->cmd);
        // 非有效指令
        if ((e->flag & 0b11) == 0) {
            dsprintf(e->status, "Not an editor command: :%s", e->cmd);
        }
        break;
    case 27: // esc
        e->mode = EDITOR_MODE_NORMAL;
        e->cmd = dstring_clear(e->cmd);
        break;
    case KEY_BACKSPACE:
    case 127: // backspace
        e->cmd = dstring_remove(e->cmd, dstring_get_length(e->cmd) - 1, 1);
        break;
    default:
        e->cmd = dstring_insert_char(e->cmd, (char)c, -1);
    }
}

// 输入处理
void editor_handle_input(editor *e, int c) {
    switch (e->mode) {
    case EDITOR_MODE_NORMAL:
        _editor_handle_input_normal(e, c);
        break;
    case EDITOR_MODE_INSERT:
        _editor_handle_input_insert(e, c);
        break;
    case EDITOR_MODE_COMMAND:
        _editor_handle_input_command(e, c);
    }
};

// 刷新显示
void editor_update(editor *e) {
    int lineCount = buffer_get_count(e->buffer);

    buffer *b = e->buffer;
    dstring curLine = buffer_get_line(b, e->top);
    uint16_t curX = 0;

    // 打印编辑区
    for (; curX < e->height && curX + e->top < lineCount; ++curX) {
        curLine = buffer_get_line(b, e->top + curX);
        dstring slice = dstring_copy_slice(curLine, e->left, e->width);
        mvprintw(curX, 0, slice);
        dstring_release(slice);
        clrtoeol();
    }
    for (; curX < e->height; curX++) {
        move(curX, 0);
        clrtoeol();
    }

    // 打印状态栏
    // TODO level 0

    // 重置光标
    move(e->x, e->y);
};