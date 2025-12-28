#ifndef PLAYER_H
#define PLAYER_H
#include "external/raylib.h"
#include "gametypes.h"
#include "textures.h"
#include <stddef.h>

struct Level;

typedef struct PlayerKey {
  KeyboardKey left;
  KeyboardKey right;
  KeyboardKey up;
  KeyboardKey down;
  KeyboardKey jump;
} PlayerKey;

typedef struct Player {
  char *name;
  Color color;
  Vector2 startPosition;
  Vector2 position;
  PlayerType playerType;
  PlayerKey keys;
  float speed;
  Vector2 velocity;
  Vector2 accelerationVector;
  float acceleration;
  float maxSpeed;
  float radius;
  char number;
  TextureDef idle;
  TextureDef run;
} Player;

void generate_player(Player *player, char *name, PlayerType playerType, float x,
                     float y, Color color);

void set_player_keys(Player *player);

void set_player_texture(Player *player);

void render_players(Player *players[], size_t playerCount, struct Level *level);

void update_position(Player *players[], int plaerCount, struct Level *level);

void reset_player_movement(Player *player);

void two_player_collision(Player *player1, Player *player2,
                          struct Level *level);

void players_collision(Player *players[], int playerCount, struct Level *level);

bool collides_with_level(float x, float y, float radius, struct Level *level);

bool is_blocked(char tile);

bool check_level_completion(Player *players[], struct Level *level,
                            size_t playerCount);

bool check_kill_zone(Player *players[], struct Level *level,
                     size_t playerCount);

#endif
