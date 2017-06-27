#include <ncurses.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

chtype nsquare = (chtype) ' '|A_REVERSE;

#define BOARD_COLS 10
#define BOARD_ROWS 20

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(int argc, char *argv[])
{
  WINDOW *board_win;
  int startx, starty, width, height;
  int ch;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  timeout(50);
  refresh();

  height = BOARD_ROWS + 2;
  width = BOARD_COLS * 2 + 2;
  starty = (LINES - height) / 2;
  startx = (COLS - width) / 2;
  board_win = create_newwin(height, width, starty, startx);

  while((ch = getch()) != 'q') {
    wrefresh(board_win);
  }

  endwin();			/* End curses mode		  */
  return 0;
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
