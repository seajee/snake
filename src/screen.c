#include "screen.h"

#include <curses.h>

void screen_start(void)
{
    initscr();
    noecho();
    timeout(0);
    curs_set(0);
    keypad(stdscr, TRUE);
}

void screen_end(void)
{
    endwin();
}
