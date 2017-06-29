#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <string.h>

#include "render.h"

#define B_COLS 10
#define B_ROWS 20

#define TET_ROWS 2
#define TET_COLS 4
#define TET_NUM 7

extern const int tet_shapes[TET_NUM][TET_ROWS][TET_COLS];
typedef enum {
  I, O, T, S, Z, IL, L
} TetType;

typedef struct {
  int row;
  int col;
} TetPos;

typedef struct {
  int rows;
  int cols;
  int bitmap[TET_ROWS][TET_COLS];
} TetShape;

typedef struct {
  TetType type;
  TetPos pos;
  TetShape shape;
  int placed;
} Tet;

typedef struct {
  int rows;
  int cols;
  char ** bitmap;
} Board;

typedef struct {
  int paused;
  int speed_ticks; // TODO change to ms
  int ticks;
  Tet * falling;
  Board * board;
} Game;

Tet rand_tet();
Tet create_tet(TetType type);

void erase_tet(Tet * tet, Board * board);
void place_tet(Tet * tet, Board * board);

Game * create_game();
void destroy_game(Game * game);

Board * create_board(int rows, int cols);
void destroy_board(Board * board);

void update_tet(Tet * tet, Board * board);

int * complete_rows(Board * board);
void clear_rows(Board * board, int rows[]);
void tick(Game * game);
void advance(Game * game);
void drop_tet(Game * game, Tet * tet);
void render(WINDOW *win, Game *game);

#endif /* ifndef TETRIS_H */
