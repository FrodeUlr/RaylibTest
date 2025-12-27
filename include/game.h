#ifndef GAME_H
#define GAME_H
#include "config.h"
#include "gametypes.h"
#include "player.h"

struct Level;
typedef struct Game {
  GameState gameState;
  struct Level *level;
  Player **players;
  size_t playerCount;
  bool running;
} Game;

void start_game(Game *game, Config *config);
void draw_ui(Player *players[], int player_count, int screen_width,
             int screen_height);
void initialize_players(Game *game, int player_count);
void free_game(Game *game);
void set_camera(Game *game, int screen_width, int screen_height);
void draw_viewports(Rectangle *game_viewport, Game *game, struct Level *level);

#endif
