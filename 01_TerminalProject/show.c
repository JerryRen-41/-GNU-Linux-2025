#define _GNU_SOURCE
#include <ncurses.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    noecho();

    int height = LINES - 2;
    int width  = COLS - 2;

    // 外框窗口
    WINDOW *border_win = newwin(height, width, 1, 1);
    box(border_win, 0, 0);
    mvwprintw(border_win, 0, 2, " %s ", argv[1]);
    wrefresh(border_win);

    // 内容窗口
    WINDOW *content_win = newwin(height - 2, width - 2, 2, 2);
    scrollok(content_win, TRUE);
    keypad(content_win, TRUE);

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        endwin();
        perror("fopen");
        return 1;
    }

    char *buffer = NULL;
    size_t bufsize = 0;
    int input;

    // 初始化显示一屏内容
    for (int i = 0; i < height - 2 && getline(&buffer, &bufsize, file) > 0; i++) {
        wprintw(content_win, "%.*s\n", width - 2, buffer);
    }
    wrefresh(content_win);

    // 交互：空格滚动一行，ESC退出
    while ((input = wgetch(content_win)) != 27) {
        if (input == ' ') {
            if (getline(&buffer, &bufsize, file) > 0) {
                wprintw(content_win, "%.*s\n", width - 2, buffer);
                wrefresh(content_win);
            }
        }
    }

    free(buffer);
    fclose(file);
    delwin(content_win);
    delwin(border_win);
    endwin();

    return 0;
}

