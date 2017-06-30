#include "tetris.h"

const TetShape tet_shapes[TET_NUM] = {
  { 1, 4, {{1,1,1,1}, {0,0,0,0}} }, // I
  { 2, 2, {{1,1,0,0}, {1,1,0,0}} }, // O
  { 2, 3, {{0,1,0,0}, {1,1,1,0}} }, // T
  { 2, 3, {{0,1,1,0}, {1,1,0,0}} }, // S
  { 2, 3, {{1,1,0,0}, {0,1,1,0}} }, // Z
  { 2, 3, {{1,1,1,0}, {0,0,1,0}} }, // IL
  { 2, 3, {{1,0,0,0}, {1,1,1,0}} }  // L
};

Tet * rand_tet();
Tet create_tet(TetType type);

void erase_tet(Tet * tet, Board * board);
void place_tet(Tet * tet, Board * board);

Board * create_board(int rows, int cols);
void destroy_board(Board * board);

void update_tet_pos(Tet * tet, Board * board, TetPos pos);

int * complete_rows(Board * board);
void clear_rows(Board * board, int rows[]);

void drop_tet(TetrisGame * game, Tet * tet);
int lower_falling(TetrisGame * game);

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

void print_board(Board * board) {
  char ** bitmap = board->bitmap;

  for (int i = 0; i < board->rows; ++i) {
    for (int j = 0; j < board->cols; ++j) {
      fprintf(stderr, "%d ", bitmap[i][j]);
    }
    fprintf(stderr, "\n");
  }
}

void tick(TetrisGame * game) {
  if(game->ticks % game->speed_ticks * 10 == 0) {
    Tet * tet = rand_tet();
    drop_tet(game, tet);
  }

  if(lower_falling(game)) {
    Tet * tet = rand_tet();
    drop_tet(game, tet);
  }

  game->ticks++;
}

int lower_falling(TetrisGame * game) {
  TetPos pos;

  pos.row = game->falling->pos.row + 1;
  pos.col = game->falling->pos.col;

  if(pos.row > BOARD_ROWS - game->falling->shape.rows) return 1;

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
  fprintf(stderr, "e %d %d\n", tet->pos.row, tet->pos.col);
  erase_tet(tet, board);

  tet->pos.row = new_pos.row;
  tet->pos.col = new_pos.col;

  fprintf(stderr, "n %d %d\n", tet->pos.row, tet->pos.col);
  place_tet(tet, board);
}

TetrisGame * create_game() {
  TetrisGame * game = malloc(sizeof(TetrisGame));

  game->paused = 0;
  game->falling = NULL;
  game->speed_ticks = 800;
  game->ticks = 0;
  game->board = create_board(B_ROWS, B_COLS);
  game->render = init_render(game->board);

  srand(time(NULL));

  return game;
}

Tet * rand_tet() {
  // TODO make random
  Tet * tet = malloc(sizeof(Tet));

  int shape = rand() % TET_NUM;
  *tet = create_tet(T);

  return tet;
}

Tet create_tet(TetType type) {
  // TODO do all pieces
  Tet tet;

  tet.type = I;
  tet.pos.row = -1;
  tet.pos.col = -1;
  tet.dir_left = 1;
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

void destroy_game(TetrisGame * game) {
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
