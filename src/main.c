#include <stdlib.h>
#include "tetris.h"

int main(int argc, char *argv[]) {
  TetrisGame * game = create_game();
  run(game);
  destroy_game(game);
}
