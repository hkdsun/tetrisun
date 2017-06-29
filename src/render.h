#ifndef RENDER_H
#define RENDER_H

#include <ncurses.h>

extern const chtype square;

void render_bitmap(WINDOW * win, char ** bitmap, int rows, int cols);
void render_char(WINDOW * win, int row, int col, chtype character);

#endif /* ifndef RENDER_H */
