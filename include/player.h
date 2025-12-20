#ifndef PLAYER_H
#define PLAYER_H
#include "constants.h"
#include "level.h"
#include "raylib.h"
#include <stddef.h>

typedef struct PlayerKey {
  KeyboardKey left;
  KeyboardKey right;
  KeyboardKey up;
  KeyboardKey down;
  KeyboardKey jump;
} PlayerKey;

typedef enum { PLAYER_ONE, PLAYER_TWO } PlayerType;

typedef struct Player {
  char *name;
  Color color;
  Vector2 position;
  PlayerType player_type;
  PlayerKey keys;
  float speed;
  float acceleration;
  float max_speed;
  float radius;
  float mass;
  char number;
} Player;

Player generate_player(char *name, PlayerType player_type, float x, float y,
                       Color color);
void SetPlayerKeys(Player *player);
void render_players(Player *players[], size_t player_count, Level *level);

void update_position(Player *players[], int plaer_count, Level *level);

void two_player_collision(Player *player1, Player *player2, Level *level);
bool collides_with_level(float x, float y, float radius, Level *level);
bool is_blocked(char tile);

bool check_level_completion(Player *player[], Level *level,
                            size_t player_count);

#endif
