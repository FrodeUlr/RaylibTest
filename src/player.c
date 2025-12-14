#include "../include/player.h"
#include "../include/raylib.h"
#include <math.h>

Player generate_player(char *name, float x, float y, Color color) {
  Player player;
  player.name = name;
  player.position = (Vector2){x, y};
  player.speed = 10.0f;
  player.acceleration = 0.0f;
  player.max_speed = 200.0f;
  player.radius = 15.0f;
  player.color = color;
  player.mass = 1.0f;
  return player;
}

bool is_right_key(bool TwoPlayer) {
  return (TwoPlayer && IsKeyDown(KEY_D)) ||
         (!TwoPlayer && IsKeyDown(KEY_RIGHT));
}
bool is_left_key(bool TwoPlayer) {
  return (TwoPlayer && IsKeyDown(KEY_A)) || (!TwoPlayer && IsKeyDown(KEY_LEFT));
}
bool is_up_key(bool TwoPlayer) {
  return (TwoPlayer && IsKeyDown(KEY_W)) || (!TwoPlayer && IsKeyDown(KEY_UP));
}
bool is_down_key(bool TwoPlayer) {
  return (TwoPlayer && IsKeyDown(KEY_S)) || (!TwoPlayer && IsKeyDown(KEY_DOWN));
}

void update_position(Player *player, bool TwoPlayer, Level level,
                     int screenWidth, int screenHeight) {
  int tile_rows = level.height;
  int tile_cols = level.width;
  int tile_w = screenWidth / tile_cols;
  int tile_h = screenHeight / tile_rows;

  float new_x = player->position.x;
  float new_y = player->position.y;
  float radius = player->radius;

  if (is_right_key(TwoPlayer))
    new_x += player->speed * GetFrameTime();
  if (is_left_key(TwoPlayer))
    new_x -= player->speed * GetFrameTime();
  if (is_up_key(TwoPlayer))
    new_y -= player->speed * GetFrameTime();
  if (is_down_key(TwoPlayer))
    new_y += player->speed * GetFrameTime();

  if (is_right_key(TwoPlayer) || is_left_key(TwoPlayer) ||
      is_up_key(TwoPlayer) || is_down_key(TwoPlayer)) {
    if (player->speed < player->max_speed) {
      player->acceleration += 0.1f;
      player->speed += player->acceleration;
    }
  } else {
    player->acceleration = 0.0f;
    player->speed = 10.0f;
  }

  int left_tile = (int)((new_x - radius) / tile_w);
  int right_tile = (int)((new_x + radius) / tile_w);
  int top_tile = (int)((player->position.y - radius) / tile_h);
  int bottom_tile = (int)((player->position.y + radius) / tile_h);

  if (!is_blocked(level.data[top_tile][left_tile]) &&
      !is_blocked(level.data[bottom_tile][left_tile]) &&
      !is_blocked(level.data[top_tile][right_tile]) &&
      !is_blocked(level.data[bottom_tile][right_tile])) {
    player->position.x = new_x;
  }

  left_tile = (int)((player->position.x - radius) / tile_w);
  right_tile = (int)((player->position.x + radius) / tile_w);
  top_tile = (int)((new_y - radius) / tile_h);
  bottom_tile = (int)((new_y + radius) / tile_h);

  if (!is_blocked(level.data[top_tile][left_tile]) &&
      !is_blocked(level.data[bottom_tile][left_tile]) &&
      !is_blocked(level.data[top_tile][right_tile]) &&
      !is_blocked(level.data[bottom_tile][right_tile])) {
    player->position.y = new_y;
  }
}

void check_collision(Player *player, int screenWidth, int screenHeight) {
  if (player->position.x < player->radius)
    player->position.x = player->radius;
  if (player->position.x > screenWidth - player->radius)
    player->position.x = screenWidth - player->radius;
  if (player->position.y < player->radius)
    player->position.y = player->radius;
  if (player->position.y > screenHeight - player->radius)
    player->position.y = screenHeight - player->radius;
}

void two_player_collision(Player *player1, Player *player2, Level level,
                          int screenWidth, int screenHeight) {
  float dx = player1->position.x - player2->position.x;
  float dy = player1->position.y - player2->position.y;
  float distance = sqrtf(dx * dx + dy * dy);
  float minDistance = player1->radius + player2->radius + 5;

  if (distance < minDistance) {
    float overlap = 0.5f * (minDistance - distance);
    float nx = dx / distance;
    float ny = dy / distance;

    float old1x = player1->position.x, old1y = player1->position.y;
    float old2x = player2->position.x, old2y = player2->position.y;

    player1->position.x += nx * overlap;
    player1->position.y += ny * overlap;
    player2->position.x -= nx * overlap;
    player2->position.y -= ny * overlap;

    if (collides_with_level(player1->position.x, player1->position.y,
                            player1->radius, level, screenWidth, screenHeight))
      player1->position.x = old1x, player1->position.y = old1y;
    if (collides_with_level(player2->position.x, player2->position.y,
                            player2->radius, level, screenWidth, screenHeight))
      player2->position.x = old2x, player2->position.y = old2y;
  }
}

bool is_blocked(char c) { return c == '#' || c == '@'; }

bool collides_with_level(float x, float y, float radius, Level level,
                         int screenWidth, int screenHeight) {
  int tile_rows = level.height;
  int tile_cols = level.width;
  int tile_w = screenWidth / tile_cols;
  int tile_h = screenHeight / tile_rows;

  int left_tile = (int)((x - radius) / tile_w);
  int right_tile = (int)((x + radius) / tile_w);
  int top_tile = (int)((y - radius) / tile_h);
  int bottom_tile = (int)((y + radius) / tile_h);

  if (left_tile < 0)
    left_tile = 0;
  if (right_tile >= tile_cols)
    right_tile = tile_cols - 1;
  if (top_tile < 0)
    top_tile = 0;
  if (bottom_tile >= tile_rows)
    bottom_tile = tile_rows - 1;

  for (int i = top_tile; i <= bottom_tile; i++) {
    for (int j = left_tile; j <= right_tile; j++) {
      if (is_blocked(level.data[i][j]))
        return true;
    }
  }
  return false;
}

void render_player(Player player) {
  float name_size = MeasureText(player.name, 20);
  float middle_x = player.position.x - name_size / 2;
  DrawText(player.name, middle_x, player.position.y - player.radius - 25, 20,
           WHITE);
  DrawCircleV(player.position, player.radius, player.color);
}

bool check_level_completion(Player player, Level level, int screenWidth,
                            int screenHeight) {
  int tile_rows = level.height;
  int tile_cols = level.width;
  int tile_w = screenWidth / tile_cols;
  int tile_h = screenHeight / tile_rows;

  int player_tile_x = (int)(player.position.x / tile_w);
  int player_tile_y = (int)(player.position.y / tile_h);

  if (level.data[player_tile_y][player_tile_x] == 'O') {
    return true;
  }
  return false;
}
