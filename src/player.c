#include "../include/player.h"
#include <math.h>
#include <stdio.h>

void generate_player(Player *player, char *name, PlayerType playerType, float x,
                     float y, Color color) {
  player->name = name;
  player->playerType = playerType;
  player->position = (Vector2){x, y};
  player->speed = PLAYER_START_SPEED;
  player->acceleration = PLAYER_START_ACCELERATION;
  player->velocity = (Vector2){10.0f, 10.0f};
  player->accelerationVector = (Vector2){0.0f, 0.0f};
  player->maxSpeed = PLAYER_MAX_SPEED;
  player->radius = PLAYER_RADIUS;
  player->color = color;
  player->mass = PLAYER_MASS;
  SetPlayerKeys(player);
  player->number = (playerType == PLAYER_ONE) ? '1' : '2';
}

void SetPlayerKeys(Player *player) {
  if (player->playerType == PLAYER_ONE) {
    player->keys.left = KEY_A;
    player->keys.right = KEY_D;
    player->keys.up = KEY_W;
    player->keys.down = KEY_S;
    player->keys.jump = KEY_LEFT_SHIFT;
  } else if (player->playerType == PLAYER_TWO) {
    player->keys.left = KEY_LEFT;
    player->keys.right = KEY_RIGHT;
    player->keys.up = KEY_UP;
    player->keys.down = KEY_DOWN;
    player->keys.jump = KEY_SPACE;
  }
}

void update_position(Player *players[], int playerCount, Level *level) {
  for (int i = 0; i < playerCount; i++) {
    float new_x = players[i]->position.x;
    float new_y = players[i]->position.y;
    float radius = players[i]->radius;
    float frame_time = GetFrameTime();
    bool x_movement_key_pressed = false;
    bool x_neg_movement_key_pressed = false;
    bool y_movement_key_pressed = false;
    bool y_neg_movement_key_pressed = false;

    if (IsKeyDown(players[i]->keys.right)) {
      if (players[i]->velocity.x > 0)
        new_x += players[i]->velocity.x * frame_time;
      if (players[i]->velocity.x < players[i]->maxSpeed &&
          players[i]->accelerationVector.x > -0.1f) {
        players[i]->accelerationVector.x += 0.1f;
        players[i]->velocity.x += players[i]->accelerationVector.x;
      }
      x_movement_key_pressed = true;
    } else if (IsKeyDown(players[i]->keys.left)) {
      if (players[i]->velocity.x < 0)
        new_x += players[i]->velocity.x * frame_time;
      if (players[i]->velocity.x > -players[i]->maxSpeed &&
          players[i]->accelerationVector.x < 0.1f) {
        players[i]->accelerationVector.x -= 0.1f;
        players[i]->velocity.x += players[i]->accelerationVector.x;
      }
      x_neg_movement_key_pressed = true;
    }
    if (IsKeyDown(players[i]->keys.down)) {
      if (players[i]->velocity.y > 0)
        new_y += players[i]->velocity.y * frame_time;
      if (players[i]->velocity.y < players[i]->maxSpeed &&
          players[i]->accelerationVector.y > -0.1f) {
        players[i]->accelerationVector.y += 0.1f;
        players[i]->velocity.y += players[i]->accelerationVector.y;
      }
      y_movement_key_pressed = true;
    } else if (IsKeyDown(players[i]->keys.up)) {
      if (players[i]->velocity.y < 0)
        new_y += players[i]->velocity.y * frame_time;
      if (players[i]->velocity.y > -players[i]->maxSpeed &&
          players[i]->accelerationVector.y < 0.1f) {
        players[i]->accelerationVector.y -= 0.1f;
        players[i]->velocity.y += players[i]->accelerationVector.y;
      }
      y_neg_movement_key_pressed = true;
    }

    if (!x_movement_key_pressed && players[i]->accelerationVector.x > 0) {
      if (players[i]->velocity.x < 10.0f) {
        players[i]->accelerationVector.x = 0.0f;
        players[i]->velocity.x = 0.0f;
      } else {
        players[i]->accelerationVector.x -= 0.1f;
        players[i]->velocity.x -= players[i]->accelerationVector.x;
        new_x += players[i]->velocity.x * frame_time;
      }
    }
    if (!x_neg_movement_key_pressed && players[i]->accelerationVector.x < 0) {
      if (players[i]->velocity.x > -10.0f) {
        players[i]->accelerationVector.x = 0.0f;
        players[i]->velocity.x = 0.0f;
      } else {
        players[i]->accelerationVector.x += 0.1f;
        players[i]->velocity.x -= players[i]->accelerationVector.x;
        new_x += players[i]->velocity.x * frame_time;
      }
    }
    if (!y_movement_key_pressed && players[i]->accelerationVector.y > 0) {
      if (players[i]->velocity.y < 10.0f) {
        players[i]->accelerationVector.y = 0.0f;
        players[i]->velocity.y = 0.0f;
      } else {
        players[i]->accelerationVector.y -= 0.1f;
        players[i]->velocity.y -= players[i]->accelerationVector.y;
        new_y += players[i]->velocity.y * frame_time;
      }
    }
    if (!y_neg_movement_key_pressed && players[i]->accelerationVector.y < 0) {
      if (players[i]->velocity.y > -10.0f) {
        players[i]->accelerationVector.y = 0.0f;
        players[i]->velocity.y = 0.0f;
      } else {
        players[i]->accelerationVector.y += 0.1f;
        players[i]->velocity.y -= players[i]->accelerationVector.y;
        new_y += players[i]->velocity.y * frame_time;
      }
    }
    /* if (IsKeyDown(players[i]->keys.right) || IsKeyDown(players[i]->keys.left)
     * || */
    /*     IsKeyDown(players[i]->keys.up) || IsKeyDown(players[i]->keys.down)) {
     */
    /*   if (players[i]->speed < players[i]->maxSpeed) { */
    /*     players[i]->acceleration += 0.1f * frame_time; */
    /*     players[i]->speed += players[i]->acceleration; */
    /*   } */
    /* } else { */
    /*   players[i]->acceleration = 0.0f; */
    /*   players[i]->speed = 10.0f; */
    /* } */

    // Convert position to tile coordinates, accounting for scaling and offset
    int left_tile = (int)((new_x - radius - level->offsetX) / level->tileSize);
    int right_tile = (int)((new_x + radius - level->offsetX) / level->tileSize);
    int top_tile = (int)((players[i]->position.y - radius - level->offsetY) /
                         level->tileSize);
    int bottom_tile = (int)((players[i]->position.y + radius - level->offsetY) /
                            level->tileSize);

    // Clamp tile indices
    left_tile =
        left_tile < 0
            ? 0
            : (left_tile >= level->columns ? level->columns - 1 : left_tile);
    right_tile =
        right_tile < 0
            ? 0
            : (right_tile >= level->columns ? level->columns - 1 : right_tile);
    top_tile = top_tile < 0
                   ? 0
                   : (top_tile >= level->rows ? level->rows - 1 : top_tile);
    bottom_tile =
        bottom_tile < 0
            ? 0
            : (bottom_tile >= level->rows ? level->rows - 1 : bottom_tile);

    if (!is_blocked(level->data[top_tile][left_tile]) &&
        !is_blocked(level->data[bottom_tile][left_tile]) &&
        !is_blocked(level->data[top_tile][right_tile]) &&
        !is_blocked(level->data[bottom_tile][right_tile])) {
      players[i]->position.x = new_x;
    }

    left_tile = (int)((players[i]->position.x - radius - level->offsetX) /
                      level->tileSize);
    right_tile = (int)((players[i]->position.x + radius - level->offsetX) /
                       level->tileSize);
    top_tile = (int)((new_y - radius - level->offsetY) / level->tileSize);
    bottom_tile = (int)((new_y + radius - level->offsetY) / level->tileSize);

    left_tile =
        left_tile < 0
            ? 0
            : (left_tile >= level->columns ? level->columns - 1 : left_tile);
    right_tile =
        right_tile < 0
            ? 0
            : (right_tile >= level->columns ? level->columns - 1 : right_tile);
    top_tile = top_tile < 0
                   ? 0
                   : (top_tile >= level->rows ? level->rows - 1 : top_tile);
    bottom_tile =
        bottom_tile < 0
            ? 0
            : (bottom_tile >= level->rows ? level->rows - 1 : bottom_tile);

    if (!is_blocked(level->data[top_tile][left_tile]) &&
        !is_blocked(level->data[bottom_tile][left_tile]) &&
        !is_blocked(level->data[top_tile][right_tile]) &&
        !is_blocked(level->data[bottom_tile][right_tile])) {
      players[i]->position.y = new_y;
    }
  }
}

void two_player_collision(Player *p1, Player *p2, Level *level) {
  float dx = p1->position.x - p2->position.x;
  float dy = p1->position.y - p2->position.y;

  float distance = sqrtf(dx * dx + dy * dy);
  float minDist = p1->radius + p2->radius;

  // No overlap or invalid state
  if (distance >= minDist || distance < 0.001f)
    return;

  float overlap = minDist - distance;

  // Normalize direction
  float nx = dx / distance;
  float ny = dy / distance;

  Vector2 old1 = p1->position;
  Vector2 old2 = p2->position;

  /* ─────────────── X AXIS ─────────────── */

  float moveX = nx * overlap * 0.5f;

  Vector2 p1_x = {old1.x + moveX, old1.y};
  Vector2 p2_x = {old2.x - moveX, old2.y};

  bool p1_x_blocked = collides_with_level(p1_x.x, p1_x.y, p1->radius, level);

  bool p2_x_blocked = collides_with_level(p2_x.x, p2_x.y, p2->radius, level);

  if (!p1_x_blocked)
    p1->position.x = p1_x.x;
  if (!p2_x_blocked)
    p2->position.x = p2_x.x;

  /* ─────────────── Y AXIS ─────────────── */

  float moveY = ny * overlap * 0.5f;

  Vector2 p1_y = {p1->position.x, old1.y + moveY};
  Vector2 p2_y = {p2->position.x, old2.y - moveY};

  bool p1_y_blocked = collides_with_level(p1_y.x, p1_y.y, p1->radius, level);

  bool p2_y_blocked = collides_with_level(p2_y.x, p2_y.y, p2->radius, level);

  if (!p1_y_blocked)
    p1->position.y = p1_y.y;
  if (!p2_y_blocked)
    p2->position.y = p2_y.y;
}

void players_collision(Player *players[], int player_count, Level *level) {
  for (int i = 0; i < player_count; i++) {
    for (int j = i + 1; j < player_count; j++) {
      two_player_collision(players[i], players[j], level);
    }
  }
}

bool is_blocked(char c) { return c == '#' || c == '@'; }

bool collides_with_level(float x, float y, float radius, Level *level) {
  int left_tile = (int)((x - radius - level->offsetX) / level->tileSize);
  int right_tile = (int)((x + radius - level->offsetX) / level->tileSize);
  int top_tile = (int)((y - radius - level->offsetY) / level->tileSize);
  int bottom_tile = (int)((y + radius - level->offsetY) / level->tileSize);

  // Clamp
  if (left_tile < 0)
    left_tile = 0;
  if (right_tile >= level->columns)
    right_tile = level->columns - 1;
  if (top_tile < 0)
    top_tile = 0;
  if (bottom_tile >= level->rows)
    bottom_tile = level->rows - 1;

  for (int y = top_tile; y <= bottom_tile; y++) {
    for (int x = left_tile; x <= right_tile; x++) {
      if (is_blocked(level->data[y][x]))
        return true;
    }
  }

  return false;
}

void render_players(Player *players[], size_t playerCount, Level *level) {
  for (size_t i = 0; i < playerCount; i++) {
    if (level->firstFrame) {
      for (int y = 0; y < level->rows; y++) {
        for (int x = 0; x < level->columns; x++) {
          if (level->data[y][x] == players[i]->number) {
            players[i]->position.x = x * level->tileSize + level->offsetX +
                                     (float)level->tileSize / 2;
            players[i]->position.y = y * level->tileSize + level->offsetY +
                                     (float)level->tileSize / 2;
            players[i]->radius = (float)level->tileSize / 3;
            break;
          }
        }
      }
    }
    float name_size = MeasureText(players[i]->name, 20);
    float middle_x = players[i]->position.x - name_size / 2;
    DrawText(players[i]->name, middle_x,
             players[i]->position.y - players[i]->radius - 25, 20, WHITE);
    DrawCircleV(players[i]->position, players[i]->radius, players[i]->color);
  }
}

bool check_level_completion(Player *players[], Level *level,
                            size_t playerCount) {
  for (int i = 0; i < playerCount; i++) {
    int player_tile_x =
        (int)((players[i]->position.x - level->offsetX) / level->tileSize);
    int player_tile_y =
        (int)((players[i]->position.y - level->offsetY) / level->tileSize);

    if (level->data[player_tile_y][player_tile_x] == 'O') {
      return true;
    }
  }
  return false;
}
