#include <ncurses.h>
#include <math.h>

chtype nsquare = (chtype) ' '|A_REVERSE;

#define BOARD_COLS 10
#define BOARD_ROWS 20

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int ticks = 0;

void wfill_square(WINDOW * win, int row, int col) {
  int wcols, wrows;
  getmaxyx(win, wrows, wcols);

  int srows = (int) floor((float) wrows / (float) BOARD_ROWS);
  int scols = (int) floor((float) wcols / (float) BOARD_COLS);

  int r = srows * row;
  int c = scols * col - 1;

  for (int i = 0; i < srows; ++i) {
    for (int j = 0; j < scols; ++j) {
      wmove(win, 1+ r + i, c + j + 2);
      waddch(win, nsquare);
    }
  }
}

void wdel_square(WINDOW * win, int row, int col) {
  int wcols, wrows;
  getmaxyx(win, wrows, wcols);

  int srows = (int) floor((float) wrows / (float) BOARD_ROWS);
  int scols = (int) floor((float) wcols / (float) BOARD_COLS);

  int r = srows * row;
  int c = scols * col - 1;

  for (int i = 0; i < srows; ++i) {
    for (int j = 0; j < scols; ++j) {
      wmove(win, 1+ r + i, c + j + 2);
      waddch(win, ' ');
    }
  }
}

void wdraw_blocks(WINDOW * win, bool board[BOARD_ROWS][BOARD_COLS]) {
  for (int row = 0; row < BOARD_ROWS; ++row) {
    for (int col = 0; col < BOARD_COLS; ++col) {
      if(board[row][col]) {
        wfill_square(win, row, col);
      } else {
        wdel_square(win, row, col);
      }
    }
  }
}

void render_board(WINDOW * win, bool board[BOARD_ROWS][BOARD_COLS])
{
  wdraw_blocks(win, board);
}

void land_pieces(bool board[BOARD_ROWS][BOARD_COLS]) {
  for (int r = BOARD_ROWS - 1; r > 0; --r) {
    for (int c = 0; c < BOARD_COLS; ++c) {
      if(!board[r][c]) {
        board[r][c] = board[r-1][c];
        board[r-1][c] = false;
      }
    }
  }
}

void tick(bool board[BOARD_ROWS][BOARD_COLS]) {
  ticks += 1;
  if(ticks % 10 == 0) {
    land_pieces(board);
  }
}


int main(int argc, char *argv[])
{
  WINDOW *my_win;
  int startx, starty, width, height;
  int ch;

  bool board[BOARD_ROWS][BOARD_COLS];

  for (int r = 0; r < BOARD_ROWS; ++r) {
    for (int c = 0; c < BOARD_COLS; ++c) {
      board[r][c] = false;
    }
  }

  board[0][0] = true;
  board[0][2] = true;
  board[0][4] = true;
  board[0][6] = true;
  board[0][9] = true;

  board[2][0] = true;
  board[3][1] = true;
  board[5][8] = true;
  board[5][3] = true;
  board[5][1] = true;
  board[1][7] = true;
  board[0][9] = true;

  initscr();			/* Start curses mode 		*/
  cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
  keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

  height = BOARD_ROWS + 2;
  width = BOARD_COLS * 2 + 2;
  starty = (LINES - height) / 2;	/* Calculating for a center placement */
  startx = 2;
  refresh();
  my_win = create_newwin(height, width, starty, startx);

  timeout(50);

  while((ch = getch()) != 'q') {
    switch(ch) {
    case KEY_LEFT:
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty,--startx);
      break;
    case KEY_RIGHT:
      destroy_win(my_win);
      my_win = create_newwin(height, width, starty,++startx);
      break;
    case KEY_UP:
      destroy_win(my_win);
      my_win = create_newwin(height, width, --starty,startx);
      break;
    case KEY_DOWN:
      destroy_win(my_win);
      my_win = create_newwin(height, width, ++starty,startx);
      break;
    }
    tick(board);
    wrefresh(my_win);
    render_board(my_win, board);
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
  /* box(local_win, ' ', ' '); : This won't produce the desired
   * result of erasing the window. It will leave it's four corners
   * and so an ugly remnant of window.
   */
  wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  /* The parameters taken are
   * 1. win: the window on which to operate
   * 2. ls: character to be used for the left side of the window
   * 3. rs: character to be used for the right side of the window
   * 4. ts: character to be used for the top side of the window
   * 5. bs: character to be used for the bottom side of the window
   * 6. tl: character to be used for the top left corner of the window
   * 7. tr: character to be used for the top right corner of the window
   * 8. bl: character to be used for the bottom left corner of the window
   * 9. br: character to be used for the bottom right corner of the window
   */
  wrefresh(local_win);
  delwin(local_win);
}
