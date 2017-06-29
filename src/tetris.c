#include "tetris.h"

const int tet_shapes[TET_NUM][TET_ROWS][TET_COLS] = {
  {{0,1,0,0}, {1,1,1,0}},
  {{1,1,1,1}},
  {{1,1,1,1}},
  {{1,1,1,1}},
  {{1,1,1,1}},
  {{1,1,1,1}},
  {{1,1,1,1}}
};


void render(WINDOW *win, Game *game) {
  char ** bitmap = game->board->bitmap;
  render_bitmap(win, bitmap, game->board->rows, game->board->cols);
}

Game * create_game() {
  Game * game = malloc(sizeof(Game));

  game->paused = 0;
  game->falling = NULL;
  game->speed_ticks = 800;
  game->ticks = 0;
  game->board = create_board(B_ROWS, B_COLS);

  return game;
}

void destroy_game(Game * game) {
  destroy_board(game->board);
  free(game);
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

void destroy_board(Board * board) {
  for (int i = 0; i < board->rows; ++i) {
    free(board->bitmap[i]);
  }
  free(board->bitmap);
  free(board);
}

void tick(Game * game) {
  if(game->ticks % game->speed_ticks == 0) {
    Tet tet = rand_tet();
    drop_tet(game, &tet);
    game->ticks = 0;
  }
}

void drop_tet(Game * game, Tet * tet) {
  game->falling = tet;
  tet->pos.row = 0;
  tet->pos.col = game->board->cols/2 - tet->shape.cols/2;
  update_tet(tet, game->board);
}

void erase_tet(Tet * tet, Board * board) {
  for (int r = 0; r < TET_ROWS; ++r) {
    for (int c = 0; c < TET_COLS; ++c) {
      if(tet->shape.bitmap[r][c]) {
        int row = r + tet->pos.row;
        int col = c + tet->pos.col;

        board->bitmap[r][c] = 0;
      }
    }
  }
}

void place_tet(Tet * tet, Board * board) {
  for (int r = 0; r < TET_ROWS; ++r) {
    for (int c = 0; c < TET_COLS; ++c) {
      if(tet->shape.bitmap[r][c]) {
        int row = r + tet->pos.row;
        int col = c + tet->pos.col;

        board->bitmap[row][col] = 'x';
      }
    }
  }
}

void update_tet(Tet * tet, Board * board) {
  if(tet->placed) {
    erase_tet(tet, board);
  }

  place_tet(tet, board);
  tet->placed = 1;
}

Tet create_tet(TetType type) {
  // TODO do all pieces
  Tet tet;

  tet.type = I;
  tet.pos.row = -1;
  tet.pos.col = -1;
  tet.placed = 0;
  tet.shape.rows = 1;
  tet.shape.cols = 4;
  memcpy(tet.shape.bitmap, tet_shapes[I], sizeof(int[TET_ROWS][TET_COLS]));

  return tet;
}

Tet rand_tet() {
  // TODO make random
  return create_tet(I);
}
