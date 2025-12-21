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
  PlayerType playerType;
  PlayerKey keys;
  float speed;
  Vector2 velocity;
  Vector2 accelerationVector;
  float acceleration;
  float maxSpeed;
  float radius;
  float mass;
  char number;
} Player;

void generate_player(Player *player, char *name, PlayerType playerType, float x,
                     float y, Color color);
void SetPlayerKeys(Player *player);
void render_players(Player *players[], size_t playerCount, Level *level);

void update_position(Player *players[], int plaerCount, Level *level);

void two_player_collision(Player *player1, Player *player2, Level *level);
void players_collision(Player *players[], int playerCount, Level *level);
bool collides_with_level(float x, float y, float radius, Level *level);
bool is_blocked(char tile);

bool check_level_completion(Player *players[], Level *level,
                            size_t playerCount);

#endif
