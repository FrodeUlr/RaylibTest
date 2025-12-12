#include "include/player.h"
#include "include/raylib.h"
#include <math.h>

void PlayerUpdatePosition(Player *player, bool TwoPlayer) {
  if ((IsKeyDown(KEY_RIGHT) && !TwoPlayer) || (IsKeyDown(KEY_D) && TwoPlayer))
    player->position.x += player->speed * GetFrameTime();
  if ((IsKeyDown(KEY_LEFT) && !TwoPlayer) || (IsKeyDown(KEY_A) && TwoPlayer))
    player->position.x -= player->speed * GetFrameTime();
  if ((IsKeyDown(KEY_UP) && !TwoPlayer) || (IsKeyDown(KEY_W) && TwoPlayer))
    player->position.y -= player->speed * GetFrameTime();
  if ((IsKeyDown(KEY_DOWN) && !TwoPlayer) || (IsKeyDown(KEY_S) && TwoPlayer))
    player->position.y += player->speed * GetFrameTime();
}

void PlayerCheckCollision(Player *player, int screenWidth, int screenHeight) {
  if (player->position.x < player->radius)
    player->position.x = player->radius;
  if (player->position.x > screenWidth - player->radius)
    player->position.x = screenWidth - player->radius;
  if (player->position.y < player->radius)
    player->position.y = player->radius;
  if (player->position.y > screenHeight - player->radius)
    player->position.y = screenHeight - player->radius;
}

void TwoPlayerCollision(Player *player1, Player *player2) {
  float dx = player1->position.x - player2->position.x;
  float dy = player1->position.y - player2->position.y;
  float distance = sqrtf(dx * dx + dy * dy);
  float minDistance = player1->radius + player2->radius;

  if (distance < minDistance) {
    float overlap = 0.5f * (minDistance - distance);
    float nx = dx / distance;
    float ny = dy / distance;

    player1->position.x += nx * overlap;
    player1->position.y += ny * overlap;
    player2->position.x -= nx * overlap;
    player2->position.y -= ny * overlap;
  }
}
