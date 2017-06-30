#include "render.h"

const chtype square = (chtype) ' '|A_REVERSE;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

Render * init_render(Board * board) {
  Render * render = malloc(sizeof(Render));
  int startx, starty, width, height;
  WINDOW * board_win;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(50);
  refresh();

  height = board->rows + 2;
  width = board->cols * 2 + 2;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;

  render->rows = board->rows;
  render->cols = board->cols;
  render->board = board;
  render->win = create_newwin(height, width, starty, startx);

  return render;
}

destroy_render(Render * render) {
  endwin();
  destroy_win(render->win);
  free(render);
}


void render_char(WINDOW * win, int row, int col, chtype character) {
  wmove(win, row, col);
  waddch(win, character);
  waddch(win, character);
}

void next_frame(Render * render) {
  char ** bitmap = render->board->bitmap;

  render_bitmap(render->win, bitmap, render->board->rows, render->board->cols);
  wrefresh(render->win);
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

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
  WINDOW *local_win;

  local_win = newwin(height, width, starty, startx);
  box(local_win, 0, 0);

  curs_set(0);
  wrefresh(local_win);

  return local_win;
}

void destroy_win(WINDOW *local_win)
{
  wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  wrefresh(local_win);
  delwin(local_win);
}
