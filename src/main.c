#include "include/game.h"

int main(void) {
  Config config;
  LoadConfig(&config, "config.cfg");
  start_game(&config);
  return 0;
}
