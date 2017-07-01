#include "tetris.h"

const TetShape tet_shapes[TET_NUM] = {
  { 1, 4, {{1,1,1,1}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}} }, // I
  { 2, 2, {{1,1,0,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}} }, // O
  { 2, 3, {{0,1,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}} }, // T
  { 2, 3, {{0,1,1,0}, {1,1,0,0}, {0,0,0,0}, {0,0,0,0}} }, // S
  { 2, 3, {{1,1,0,0}, {0,1,1,0}, {0,0,0,0}, {0,0,0,0}} }, // Z
  { 2, 3, {{1,1,1,0}, {0,0,1,0}, {0,0,0,0}, {0,0,0,0}} }, // IL
  { 2, 3, {{1,0,0,0}, {1,1,1,0}, {0,0,0,0}, {0,0,0,0}} }  // L
};

Tet * rand_tet();
Tet create_tet(TetType type);

void erase_tet(Tet * tet, Board * board);
void place_tet(Tet * tet, Board * board);

Board * create_board(int rows, int cols);
void destroy_board(Board * board);

void update_tet_pos(Tet * tet, Board * board, TetPos pos);

int full_rows(Board * board, int * rows);
void clear_rows(Board * board, int * rows, int num_rows);

void drop_tet(TetrisGame * game, Tet * tet);
int lower_falling(TetrisGame * game);

int collision(Tet * tet, TetPos pos, Board * board);
int on_tet(Tet * tet, int row, int col);

TetPos tet_left(Tet * tet, Board * board, int min_col);
TetPos tet_right(Tet * tet, Board * board, int max_col);
TetPos tet_freefall(Tet * tet, Board * board);
void tet_rotate(Tet * tet, Board * board);

void destroy_tet(Tet * tet);
void render_frame(TetrisGame * game);

TetShape rotate_once(TetShape shape);

void render_frame(TetrisGame * game);
void tick(TetrisGame * game);
void handle_kb(TetrisGame * game, int ch);
void run(TetrisGame * game);

void run(TetrisGame * game) {
  int ch;

  while(!game->over && (ch = getch()) != 'q') {
    handle_kb(game, ch);
    tick(game);
    render_frame(game);
  }
}

void handle_kb(TetrisGame * game, int ch) {
  TetPos new_pos;
  switch(ch) {
    case KEY_LEFT:
      new_pos = tet_left(game->falling, game->board, 0);
      update_tet_pos(game->falling, game->board, new_pos);
      break;
    case KEY_RIGHT:
      new_pos = tet_right(game->falling, game->board, game->board->cols);
      update_tet_pos(game->falling, game->board, new_pos);
      break;
    case KEY_UP:
      tet_rotate(game->falling, game->board);
      break;
    case KEY_DOWN:
      new_pos = tet_freefall(game->falling, game->board);
      update_tet_pos(game->falling, game->board, new_pos);
      break;
  }
}

void tick(TetrisGame * game) {
  if(game->falling == NULL) {
    Tet * tet = rand_tet();
    drop_tet(game, tet);
  }

  if(game->ticks % game->speed_ticks == 0) {
    if(lower_falling(game)) {
      if(game->falling->pos.row == 0)
        game->over=1;
      destroy_tet(game->falling);
      Tet * tet = rand_tet();
      drop_tet(game, tet);
    }
  }

  int c_rows[game->board->rows];
  int nc_rows = full_rows(game->board, c_rows);
  if(nc_rows > 0)
    clear_rows(game->board, c_rows, nc_rows);

  game->ticks++;
}

void clear_row(Board * board, int c_row) {
  for (int i = 0; i < board->cols; ++i) {
    board->bitmap[c_row][i] = ' ';
  }

  for (int row = c_row; row > 0; --row) {
    for (int col = 0; col < board->cols; ++col) {
      board->bitmap[row][col] = board->bitmap[row - 1][col];
    }
  }
}

void clear_rows(Board * board, int * rows, int num_rows) {
  for (int i = 0; i < num_rows; ++i) {
    clear_row(board, rows[i]);
  }
}

int full_rows(Board * board, int * rows) {
  int num_full = 0;

  for (int row = 0; row < board->rows; ++row) {
    int row_full = true;

    for (int col = 0; col < board->cols; ++col) {
      if(!board->bitmap[row][col]) {
        row_full = false;
        break;
      }
    }

    if(row_full) {
      rows[num_full] = row;
      num_full++;
    }
  }

  return num_full;
}

TetShape rotate_once(TetShape shape) {
  TetShape new_shape;
  new_shape.rows = shape.cols;
  new_shape.cols = shape.rows;

  for (int i = 0; i < TET_ROWS; ++i) {
    for (int j = 0; j < TET_COLS; ++j) {
      new_shape.bitmap[i][j] = 0;
    }
  }

  int col = 0;
  for (int i = shape.rows - 1; i >= 0; --i) {
    for (int j = 0; j < shape.cols; ++j) {
      new_shape.bitmap[j][col] = shape.bitmap[i][j];
    }

    col++;
  }

  return new_shape;
}

void tet_rotate(Tet * tet, Board * board) {
  erase_tet(tet, board);

  TetShape shape = rotate_once(tet->shape);
  TetOrien orien = (tet->orientation + 1) % 4;

  tet->shape = shape;
  tet->orientation = orien;

  place_tet(tet, board);
}

TetPos tet_left(Tet * tet, Board * board, int min_col) {
  TetPos pos;

  pos.row = tet->pos.row;
  pos.col = tet->pos.col;

  if(pos.col - 1 >= min_col)
    pos.col -= 1;

  if(collision(tet, pos, board))
    pos.col++;

  return pos;
}

TetPos tet_right(Tet * tet, Board * board, int max_col) {
  TetPos pos;

  pos.row = tet->pos.row;
  pos.col = tet->pos.col;

  if(pos.col + 1 <= max_col - tet->shape.cols)
    pos.col++;

  if(collision(tet, pos, board))
    pos.col--;

  return pos;
}

TetPos tet_freefall(Tet * tet, Board * board) {
  TetPos pos = {tet->pos.row, tet->pos.col};

  while(!collision(tet, pos, board))
    pos.row += 1;
  pos.row--;

  return pos;
}

int on_tetris(Tet * tet, int row, int col) {
  for (int r = 0; r < tet->shape.rows; ++r) {
    for (int c = 0; c < tet->shape.cols; ++c) {
      if(tet->shape.bitmap[r][c]) {
        int occupied_row = r + tet->pos.row;
        int occupied_col = c + tet->pos.col;

        if(occupied_row == row && occupied_col == col)
          return 1;
      }
    }
  }

  return 0;
}

int collision(Tet * tet, TetPos pos, Board * board) {
  if(pos.row > BOARD_ROWS - tet->shape.rows) return 1;

  for (int r = 0; r < tet->shape.rows; ++r) {
    for (int c = 0; c < tet->shape.cols; ++c) {
      if(tet->shape.bitmap[r][c]) {
        int row = r + pos.row;
        int col = c + pos.col;
        if(board->bitmap[row][col] && !on_tetris(tet, row, col))
          return 1;
      }
    }
  }

  return 0;
}

int lower_falling(TetrisGame * game) {
  TetPos pos;

  pos.row = game->falling->pos.row + 1;
  pos.col = game->falling->pos.col;

  if(collision(game->falling, pos, game->board)) return 1;

  update_tet_pos(game->falling, game->board, pos);

  return 0;
}

void drop_tet(TetrisGame * game, Tet * tet) {
  game->falling = tet;

  tet->pos.row = 0;
  tet->pos.col = game->board->cols/2 - tet->shape.cols/2;

  update_tet_pos(tet, game->board, tet->pos);
}

void erase_tet(Tet * tet, Board * board) {
  for (int r = 0; r < tet->shape.rows; ++r) {
    for (int c = 0; c < tet->shape.cols; ++c) {
      if(tet->shape.bitmap[r][c]) {
        int row = r + tet->pos.row;
        int col = c + tet->pos.col;

        board->bitmap[row][col] = 0;
      }
    }
  }
}

void place_tet(Tet * tet, Board * board) {
  for (int r = 0; r < tet->shape.rows; ++r) {
    for (int c = 0; c < tet->shape.cols; ++c) {
      if(tet->shape.bitmap[r][c]) {
        int row = r + tet->pos.row;
        int col = c + tet->pos.col;

        board->bitmap[row][col] = 'x';
      }
    }
  }
}

void update_tet_pos(Tet * tet, Board * board, TetPos new_pos) {
  erase_tet(tet, board);

  tet->pos.row = new_pos.row;
  tet->pos.col = new_pos.col;

  place_tet(tet, board);
}

TetrisGame * create_game() {
  TetrisGame * game = malloc(sizeof(TetrisGame));

  game->paused = 0;
  game->falling = NULL;
  game->speed_ticks = 10;
  game->ticks = 0;
  game->over = 0;
  game->board = create_board(B_ROWS, B_COLS);
  game->render = init_render(game->board);

  srand(time(NULL));

  return game;
}

Tet * rand_tet() {
  Tet * tet = malloc(sizeof(Tet));

  int shape = rand() % TET_NUM;
  *tet = create_tet(shape);

  return tet;
}

Tet create_tet(TetType type) {
  Tet tet;

  tet.type = type;
  tet.pos.row = -1;
  tet.pos.col = -1;
  tet.dir_left = 1;
  tet.orientation = O_0;
  memcpy(&tet.shape, &tet_shapes[type], sizeof(TetShape));

  return tet;
}

Board * create_board(int rows, int cols) {
  Board * board = malloc(sizeof(Board));

  board->rows = rows;
  board->cols = cols;
  board->bitmap = calloc(rows, sizeof(char*));
  for (int i = 0; i < rows; ++i) {
    board->bitmap[i] = calloc(cols, sizeof(char));
  }

  return board;
}

void destroy_tet(Tet * tet) {
  free(tet);
}

void destroy_game(TetrisGame * game) {
  destroy_tet(game->falling);
  destroy_board(game->board);
  destroy_render(game->render);

  free(game);
}

void destroy_board(Board * board) {
  for (int i = 0; i < board->rows; ++i) {
    free(board->bitmap[i]);
  }
  free(board->bitmap);
  free(board);
}

void print_bitmap(int bitmap[TET_ROWS][TET_COLS]) {
  for (int i = 0; i < TET_ROWS; ++i) {
    for (int j = 0; j < TET_COLS; ++j) {
      fprintf(stderr, "%d ", bitmap[i][j]);
    }
    fprintf(stderr, "\n");
  }
  fprintf(stderr, "=========\n");
}

void print_board(Board * board) {
  char ** bitmap = board->bitmap;

  for (int i = 0; i < board->rows; ++i) {
    for (int j = 0; j < board->cols; ++j) {
      fprintf(stderr, "%d ", bitmap[i][j]);
    }
    fprintf(stderr, "\n");
  }
}

void render_frame(TetrisGame * game) {
  next_frame(game->render);
}
