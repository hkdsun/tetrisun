#ifndef TETRIS_H
#define TETRIS_H

#include <stdlib.h>
#include <string.h>

#include "render.h"
#include "board.h"

#define B_COLS 10
#define B_ROWS 20

#define BOARD_COLS 10
#define BOARD_ROWS 20

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
  int dir_left;
} Tet;

typedef struct {
  int paused;
  int speed_ticks; // TODO change to ms
  int ticks;
  int rows;
  int cols;
  Tet * falling;
  Board * board;
  Render * render;
} TetrisGame;

TetrisGame * create_game();
void destroy_game(TetrisGame * game);

#endif /* ifndef TETRIS_H */
