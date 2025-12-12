#include "raylib.h"

typedef struct Player {
  Vector2 position;
  float speed;
  float radius;
  Color color;
} Player;

void PlayerUpdatePosition(Player *player, bool TwoPlayer);
void TwoPlayerCollision(Player *player1, Player *player2);

void PlayerCheckCollision(Player *player, int screenWidth, int screenHeight);
