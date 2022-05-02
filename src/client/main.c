#include "editor.h"

#include <stdio.h>
#include <ncurses.h>

void curses_init() {
    initscr();
    noecho();
    raw();
    keypad(stdscr, true);
}

int main(int argc, char *argv[]) {
    editor *e;

    curses_init();
    int row, col;
    getmaxyx(stdscr, row, col);
    e = editor_init(row, col);
    
    // 打开文件
    if (argc > 1) {
        editor_open_file(e, argv[1]);
    }

    while ((editor_get_flag(e) & EDITOR_FLAG_EXIT) == 0) {
        editor_update(e);
        int input = getch();
        editor_handle_input(e, input);
    }

    if (editor_get_flag(e) & EDITOR_FLAG_SAVE) {
        editor_save_file(e);
    }

    refresh();
    endwin();
    return 0;
}