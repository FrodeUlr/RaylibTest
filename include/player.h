#ifndef PLAYER_H
#define PLAYER_H
#include "levels.h"
#include "raylib.h"

typedef struct Player {
  char *name;
  Vector2 position;
  float speed;
  float acceleration;
  float max_speed;
  float radius;
  Color color;
  float mass;
} Player;

Player generate_player(char *name, float x, float y, Color color);

void update_position(Player *player, bool TwoPlayer, Level level,
                     int screenWidth, int screenHeight);

void check_collision(Player *player, int screenWidth, int screenHeight);
void two_player_collision(Player *player1, Player *player2, Level level,
                          int screenWidth, int screenHeight);
bool is_blocked(char tile);
bool collides_with_level(float x, float y, float radius, Level level,
                         int screenWidth, int screenHeight);
void render_player(Player player);
bool check_level_completion(Player player, Level level, int screenWidth,
                            int screenHeight);
#endif
