#ifndef RENDER_H
#define RENDER_H

#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <string.h>


#include "board.h"

extern const chtype square;

typedef struct {
  int rows;
  int cols;
  Board * board;
  WINDOW * win;
} Render;

Render * init_render(Board * board);
void destroy_render(Render * render);

void next_frame(Render * render);

void render_bitmap(WINDOW * win, char ** bitmap, int rows, int cols);
void render_char(WINDOW * win, int row, int col, chtype character);

#endif /* ifndef RENDER_H */
