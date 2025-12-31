#include "game.h"
#include <stdlib.h>

int main(void) {
  Config *config = NULL;
  config = malloc(sizeof(Config));
  if (config == NULL) {
    return -1;
  }
  load(config, "config.cfg");
  Game *game = NULL;
  game = malloc(sizeof(Game));
  if (game == NULL) {
    free(config);
    return -1;
  }
  game->gameState = MAIN_MENU;
  game->running = true;
  start_game(game, config);

  if (game != NULL) {
    free(game);
  }
  if (config != NULL) {
    free(config);
  }
  return 0;
}
