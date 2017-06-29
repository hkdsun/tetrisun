#include "render.h"

const chtype square = (chtype) ' '|A_REVERSE;

void render_char(WINDOW * win, int row, int col, chtype character) {
  wmove(win, row, col);
  waddch(win, character);
  waddch(win, character);
}

void render_bitmap(WINDOW * win, char ** bitmap, int rows, int cols) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if(bitmap[i][j] == 'x') {
        render_char(win, i + 1, j * 2 + 1, square);
      }
    }
  }
}
