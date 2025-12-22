#ifndef GAME_H
#define GAME_H
#include "config.h"
#include "level.h"
#include "player.h"

typedef enum { MAIN_MENU, EXIT, LEVEL_ONE, LEVEL_TWO, GAME_OVER } GameState;
typedef struct Game {
  GameState gameState;
  Level *level;
  Player **players;
  size_t playerCount;
  bool running;
} Game;

void start_game(Game *game, Config *config);
void draw_ui(Player *players[], int player_count, int screen_width,
             int screen_height);
void initialize_players(Game *game, int player_count);
void free_game(Game *game);

#endif
