#include "tetris.h"

const int tet_shapes[TET_NUM][TET_ROWS][TET_COLS] = {
  { 2, 2, {{1,1,0,0}, {1,1,0,0}} },
  { 0, 0, {{1,1,1,1}, {0,0,0,0}} },
  { 0, 0, {{1,1,1,1}, {0,0,0,0}} },
  { 0, 0, {{1,1,1,1}, {0,0,0,0}} },
  { 0, 0, {{1,1,1,1}, {0,0,0,0}} },
  { 0, 0, {{1,1,1,1}, {0,0,0,0}} },
  { 0, 0, {{1,1,1,1}, {0,0,0,0}} }
};

Tet * rand_tet();
Tet create_tet(TetType type);

void erase_tet(Tet * tet, Board * board);
void place_tet(Tet * tet, Board * board);

Board * create_board(int rows, int cols);
void destroy_board(Board * board);

void update_tet_pos(Tet * tet, Board * board);

int * complete_rows(Board * board);
void clear_rows(Board * board, int rows[]);

void drop_tet(TetrisGame * game, Tet * tet);
void lower_falling(TetrisGame * game);

void render_frame(TetrisGame * game);
void tick(TetrisGame * game);
void run(TetrisGame * game);

void render_frame(TetrisGame * game) {
  next_frame(game->render);
}

void run(TetrisGame * game) {
  int ch;

  while((ch = getch()) != 'q') {
    tick(game);
    render_frame(game);
  }
}

void tick(TetrisGame * game) {
  if(game->ticks % game->speed_ticks * 10 == 0) {
    Tet * tet = rand_tet();
    drop_tet(game, tet);
  }

  lower_falling(game);
  game->ticks++;
}

void lower_falling(TetrisGame * game) {
  game->falling->pos.row += 1;

  if(game->falling->dir_left) {
    game->falling->pos.col += 2;

    if(game->falling->pos.col > 9) {
      game->falling->dir_left ^= 1;
    }
  } else {
    game->falling->pos.col -= 2;

    if(game->falling->pos.col < 0) {
      game->falling->dir_left ^= 1;
    }
  }
  update_tet_pos(game->falling, game->board);
}

void drop_tet(TetrisGame * game, Tet * tet) {
  game->falling = tet;
  tet->pos.row = 0;
  tet->pos.col = game->board->cols/2 - tet->shape.cols/2;
  update_tet_pos(tet, game->board);
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

void update_tet_pos(Tet * tet, Board * board) {
  if(tet->placed) {
    erase_tet(tet, board);
  }

  place_tet(tet, board);
  tet->placed = 1;
}

TetrisGame * create_game() {
  TetrisGame * game = malloc(sizeof(TetrisGame));

  game->paused = 0;
  game->falling = NULL;
  game->speed_ticks = 800;
  game->ticks = 0;
  game->board = create_board(B_ROWS, B_COLS);
  game->render = init_render(game->board);

  return game;
}

Tet * rand_tet() {
  // TODO make random
  Tet * tet = malloc(sizeof(Tet));
  *tet = create_tet(I);

  return tet;
}

Tet create_tet(TetType type) {
  // TODO do all pieces
  Tet tet;

  tet.type = I;
  tet.pos.row = -1;
  tet.pos.col = -1;
  tet.placed = 0;
  tet.dir_left = 0;
  memcpy(&tet.shape, tet_shapes[I], sizeof(TetShape));

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

void destroy_game(TetrisGame * game) {
  destroy_board(game->board);
  destroy_render(game->render);

  free(game->render);
  free(game);
}

void destroy_board(Board * board) {
  for (int i = 0; i < board->rows; ++i) {
    free(board->bitmap[i]);
  }
  free(board->bitmap);
  free(board);
}
