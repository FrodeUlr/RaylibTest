#include "../include/player.h"
#include "../include/raylib.h"
#include <math.h>
#include <stdio.h>

Player generate_player(char *name, PlayerType player_type, float x, float y,
                       Color color) {
  Player player;
  player.name = name;
  player.player_type = player_type;
  player.position = (Vector2){x, y};
  player.speed = 10.0f;
  player.acceleration = 0.0f;
  player.max_speed = 200.0f;
  player.radius = 15.0f;
  player.color = color;
  player.mass = 1.0f;
  SetPlayerKeys(&player);
  return player;
}

void SetPlayerKeys(Player *player) {
  if (player->player_type == PLAYER_ONE) {
    player->keys.left = KEY_LEFT;
    player->keys.right = KEY_RIGHT;
    player->keys.up = KEY_UP;
    player->keys.down = KEY_DOWN;
    player->keys.jump = KEY_SPACE;
  } else if (player->player_type == PLAYER_TWO) {
    player->keys.left = KEY_A;
    player->keys.right = KEY_D;
    player->keys.up = KEY_W;
    player->keys.down = KEY_S;
    player->keys.jump = KEY_LEFT_SHIFT;
  }
}

void update_position(Player *player, Level *level) {
  float new_x = player->position.x;
  float new_y = player->position.y;
  float radius = player->radius;

  if (IsKeyDown(player->keys.right))
    new_x += player->speed * GetFrameTime();
  if (IsKeyDown(player->keys.left))
    new_x -= player->speed * GetFrameTime();
  if (IsKeyDown(player->keys.up))
    new_y -= player->speed * GetFrameTime();
  if (IsKeyDown(player->keys.down))
    new_y += player->speed * GetFrameTime();

  if (IsKeyDown(player->keys.right) || IsKeyDown(player->keys.left) ||
      IsKeyDown(player->keys.up) || IsKeyDown(player->keys.down)) {
    if (player->speed < player->max_speed) {
      player->acceleration += 0.1f;
      player->speed += player->acceleration;
      printf("Increased speed: %.2f -> \nIncreased acceleration %.2f -> \n",
             player->speed, player->acceleration);
    }
  } else {
    player->acceleration = 0.0f;
    player->speed = 10.0f;
  }

  // Convert position to tile coordinates, accounting for scaling and offset
  int left_tile = (int)((new_x - radius - level->offset_x) / level->tile_size);
  int right_tile = (int)((new_x + radius - level->offset_x) / level->tile_size);
  int top_tile =
      (int)((player->position.y - radius - level->offset_y) / level->tile_size);
  int bottom_tile =
      (int)((player->position.y + radius - level->offset_y) / level->tile_size);

  // Clamp tile indices
  left_tile = left_tile < 0 ? 0
                            : (left_tile >= level->columns ? level->columns - 1
                                                           : left_tile);
  right_tile =
      right_tile < 0
          ? 0
          : (right_tile >= level->columns ? level->columns - 1 : right_tile);
  top_tile =
      top_tile < 0 ? 0 : (top_tile >= level->rows ? level->rows - 1 : top_tile);
  bottom_tile = bottom_tile < 0 ? 0
                                : (bottom_tile >= level->rows ? level->rows - 1
                                                              : bottom_tile);

  if (!is_blocked(level->data[top_tile][left_tile]) &&
      !is_blocked(level->data[bottom_tile][left_tile]) &&
      !is_blocked(level->data[top_tile][right_tile]) &&
      !is_blocked(level->data[bottom_tile][right_tile])) {
    player->position.x = new_x;
  }

  left_tile =
      (int)((player->position.x - radius - level->offset_x) / level->tile_size);
  right_tile =
      (int)((player->position.x + radius - level->offset_x) / level->tile_size);
  top_tile = (int)((new_y - radius - level->offset_y) / level->tile_size);
  bottom_tile = (int)((new_y + radius - level->offset_y) / level->tile_size);

  left_tile = left_tile < 0 ? 0
                            : (left_tile >= level->columns ? level->columns - 1
                                                           : left_tile);
  right_tile =
      right_tile < 0
          ? 0
          : (right_tile >= level->columns ? level->columns - 1 : right_tile);
  top_tile =
      top_tile < 0 ? 0 : (top_tile >= level->rows ? level->rows - 1 : top_tile);
  bottom_tile = bottom_tile < 0 ? 0
                                : (bottom_tile >= level->rows ? level->rows - 1
                                                              : bottom_tile);

  if (!is_blocked(level->data[top_tile][left_tile]) &&
      !is_blocked(level->data[bottom_tile][left_tile]) &&
      !is_blocked(level->data[top_tile][right_tile]) &&
      !is_blocked(level->data[bottom_tile][right_tile])) {
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

void two_player_collision(Player *player1, Player *player2, Level *level,
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

bool collides_with_level(float x, float y, float radius, Level *level,
                         int screenWidth, int screenHeight) {
  int tile_w = screenWidth / level->columns;
  int tile_h = screenHeight / level->rows;

  int left_tile = (int)((x - radius) / tile_w);
  int right_tile = (int)((x + radius) / tile_w);
  int top_tile = (int)((y - radius) / tile_h);
  int bottom_tile = (int)((y + radius) / tile_h);

  if (left_tile < 0)
    left_tile = 0;
  if (right_tile >= level->columns)
    right_tile = level->rows - 1;
  if (top_tile < 0)
    top_tile = 0;
  if (bottom_tile >= level->rows)
    bottom_tile = level->rows - 1;

  for (int i = top_tile; i <= bottom_tile; i++) {
    for (int j = left_tile; j <= right_tile; j++) {
      if (is_blocked(level->data[i][j]))
        return true;
    }
  }
  return false;
}

void render_player(Player *player, char player_no, Level *level) {
  if (level->first_frame) {
    for (int y = 0; y < level->rows; y++) {
      for (int x = 0; x < level->columns; x++) {
        if (level->data[y][x] == player_no) {
          player->position.x = x * level->tile_size + level->offset_x +
                               (float)level->tile_size / 2;
          player->position.y = y * level->tile_size + level->offset_y +
                               (float)level->tile_size / 2;
          player->radius = (float)level->tile_size / 2;
          break;
        }
      }
    }
  }
  float name_size = MeasureText(player->name, 20);
  float middle_x = player->position.x - name_size / 2;
  DrawText(player->name, middle_x, player->position.y - player->radius - 25, 20,
           WHITE);
  DrawCircleV(player->position, player->radius, player->color);
}

bool check_level_completion(Player *player[], Level *level,
                            size_t player_count) {
  for (int i = 0; i < player_count; i++) {
    int player_tile_x =
        (int)((player[i]->position.x - level->offset_x) / level->tile_size);
    int player_tile_y =
        (int)((player[i]->position.y - level->offset_y) / level->tile_size);

    if (level->data[player_tile_y][player_tile_x] == 'O') {
      return true;
    }
  }
  return false;
}
