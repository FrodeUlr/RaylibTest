#ifndef GAME_H
#define GAME_H
#include "config.h"

typedef enum { START, LEVEL_ONE, LEVEL_TWO, GAME_OVER } GameScreen;

void start_game(Config *config);

#endif
