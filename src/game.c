#include "game.h"
#include "external/raylib.h"
#include "level.h"
#include "menu.h"
#include <stdio.h>
#include <stdlib.h>

void start_game(Game *game, Config *config) {
  printf(
      "Starting game with resolution %dx%d, fullscreen: %d, target FPS: %d\n",
      config->screenWidth, config->screenHeight, config->fullscreen,
      config->targetFPS);
  const char *title = "Test Game";
  if (config->fullscreen) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(GetScreenWidth(), GetScreenHeight(), title);
  } else {
    InitWindow(config->screenWidth, config->screenHeight, title);
  }
  const int screen_width = GetScreenWidth();
  const int screen_height = GetScreenHeight();
  const int half_screen_width = screen_width / 2;
  const int half_screen_height = screen_height / 2;
  if (config->targetFPS >= 24 && config->targetFPS <= 240) {
    SetTargetFPS(config->targetFPS);
  } else {
    SetTargetFPS(60);
  }
  game->level = NULL;
  game->level = malloc(sizeof(Level));
  if (game->level == NULL) {
    printf("Error allocating memory for level\n");
    return;
  }
  game->players = NULL;
  game->playerCount = 0;
  game->firstPlayerSet = false;
  game->level->number = 0;
  game->level->cameras = NULL;
  printf("Entering main game loop\n");
  Menu *menu = NULL;
  menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("Error menu");
    return;
  }
  new_menu(menu, 2);
  game->frameCounter = 0;
  Rectangle game_viewport =
      (Rectangle){0, 40, (float)screen_width, (float)screen_height - 40};
  while (game->running) {
    if (game->frameCounter >= config->targetFPS * 60) {
      game->frameCounter = 0;
    } else {
      game->frameCounter += 1;
    }
    BeginDrawing();
    if (game->gameState == MAIN_MENU) {
      ClearBackground(RAYWHITE);
      draw_main_menu(menu, game);
      if (game->gameState == EXIT) {
        printf("Exiting game from main menu\n");
        game->running = false;
      } else if (game->gameState != MAIN_MENU) {
        initialize_players(game, menu);
      } else {
        game->gameState = MAIN_MENU;
      }
    } else if (game->gameState == GAME_OVER) {
      ClearBackground(RAYWHITE);
      DrawText("GAME OVER", half_screen_width - 100, half_screen_height - 20,
               40, BLACK);
      if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        game->gameState = MAIN_MENU;
      }
    } else if (game->gameState != MAIN_MENU || game->gameState != EXIT) {
      ClearBackground(BLACK);
      if (game->gameState == LEVEL_ONE) {
        if (game->level->number != 1) {
          for (int i = 0; i < game->playerCount; i++) {
            reset_player_movement(game->players[i]);
          }
          printf("Setting level to 1\n");
          set_level(game->level, 1);
          printf("Level set to %d\n", game->level->number);
          game->level->firstFrame = true;
        }
        if (check_level_completion(game->players, game->level,
                                   game->playerCount)) {
          game->gameState = LEVEL_TWO;
        }
        if (check_kill_zone(game->players, game->level, game->playerCount)) {
        }
      }
      if (game->gameState == LEVEL_TWO) {
        if (game->level->number != 2) {
          for (int i = 0; i < game->playerCount; i++) {
            reset_player_movement(game->players[i]);
          }
          set_level(game->level, 2);
          game->level->firstFrame = true;
        }
        if (check_level_completion(game->players, game->level,
                                   game->playerCount)) {
          game->gameState = GAME_OVER;
        }
      }
      set_camera(game, screen_width, screen_height);
      update_camera(game);
      update_position(game->players, game->playerCount, game->level);
      players_collision(game->players, game->playerCount, game->level);
      draw_viewports(&game_viewport, game, game->level);
      draw_ui(game->players, game->playerCount, screen_width, screen_height);
      if (game->level->firstFrame) {
        game->level->firstFrame = false;
      }
      check_inputs(menu, game, game->level);
    }
    EndDrawing();
  }
  free_level_data(game->level);
  free_menu(menu);
  free_game(game);
  CloseWindow();
}

void draw_ui(Player *players[], int playerCount, int screenWidth,
             int screenHeight) {
  Rectangle ui_viewport = (Rectangle){0, 0, (float)screenWidth, 40.0f};
  DrawRectangleRec(ui_viewport, BLACK);
  BeginScissorMode(ui_viewport.x, ui_viewport.y, ui_viewport.width,
                   ui_viewport.height);
  DrawLine(0, 40, screenWidth, 40, DARKGRAY);
  const char *fpsText = TextFormat("FPS: %d", GetFPS());
  const char *screenInfo =
      TextFormat("Screen: %dx%d", screenWidth, screenHeight);
  DrawText(fpsText, 0, 0, 20, WHITE);
  DrawText(screenInfo, 0, 20, 20, WHITE);
  for (size_t i = 0; i < playerCount; i++) {
    const char *position_text =
        TextFormat("%s\tPos: (%.2f, %.2f). ", players[i]->name,
                   players[i]->position.x, players[i]->position.y);
    const char *player_speeds = TextFormat(
        "Speed: x->%.2f y->%.2f Accel: x->%.2f y->%.2f", players[i]->velocity.x,
        players[i]->velocity.y, players[i]->accelerationVector.x,
        players[i]->accelerationVector.y);
    int text_width = MeasureText(position_text, 20);
    DrawText(position_text, screenWidth / 2, i * 20, 20, WHITE);
    DrawText(player_speeds, (screenWidth / 2) + text_width, i * 20, 20, WHITE);
  }
  EndScissorMode();
}

void initialize_players(Game *game, Menu *menu) {
  if (game->players != NULL && game->playerCount > 0) {
    for (size_t i = 0; i < game->playerCount; i++) {
      if (game->players[i] != NULL) {
        game->players[i] = NULL;
      }
      if (game->level->cameras != NULL && game->level->cameras[i] != NULL) {
        free(game->level->cameras[i]);
      }
    }
    free(game->players);
    game->players = NULL;
    free(game->level->cameras);
    game->level->cameras = NULL;
    game->playerCount = 0;
    game->firstPlayerSet = false;
  }
  for (int i = 0; i < 2; i++) {
    if (menu->players[i]->nameLen != 0) {
      if (i == 0) {
        game->firstPlayerSet = true;
      }
      game->playerCount += 1;
    }
  }
  if (game->players == NULL) {
    game->players = malloc(sizeof(Player *) * game->playerCount);
    if (game->players == NULL) {
      printf("Error allocating memory for players\n");
      return;
    }
  }
  for (int i = 0; i < game->playerCount; i++) {
    printf("Generating player %d...\n", i + 1);
    int ai = game->firstPlayerSet ? i : i + 1;
    game->players[i] = malloc(sizeof(Player));
    if (game->players[i] == NULL) {
      printf("Error allocating memory for player %d\n", i + 1);
      return;
    }
    generate_player(game->players[i], menu->players[ai]->name, ai,
                    GetScreenWidth() * 0.9f, GetScreenHeight() * 0.7f,
                    ai == 0 ? RAYWHITE : PINK);
  }
}

void free_camera(Game *game) {
  for (size_t i = 0; i < game->playerCount; i++) {
    if (game->level->cameras != NULL && game->level->cameras[i] != NULL) {
      free(game->level->cameras[i]);
    }
  }
  if (game->level->cameras != NULL) {
    free(game->level->cameras);
  }
}

void free_game(Game *game) {
  free_camera(game);
  for (size_t i = 0; i < game->playerCount; i++) {
    if (game->players[i] != NULL) {
      free(game->players[i]);
    }
  }
  if (game->players != NULL) {
    free(game->players);
  }
  if (game->level != NULL) {
    UnloadTexture(game->level->wallTexture.texture);
    UnloadTexture(game->level->groundTexture.texture);
    UnloadTexture(game->level->targetTexture.texture);
    UnloadTexture(game->level->houseTexture.texture);
    free(game->level);
  }
}

void set_camera(Game *game, int screen_width, int screen_height) {
  free_camera(game);
  game->level->cameras = malloc(sizeof(Camera2D *) * game->playerCount);
  if (game->level->cameras == NULL) {
    printf("Error allocating memory for cameras\n");
    return;
  }
  float screen_scale = GetScreenWidth() / 1920.0f;
  bool single_player_mode = game->playerCount == 1;
  for (size_t i = 0; i < game->playerCount; i++) {
    game->level->cameras[i] = malloc(sizeof(Camera2D));
    if (game->level->cameras[i] == NULL) {
      printf("Error allocating memory for camera %zu\n", i);
      return;
    }
    game->level->cameras[i]->target =
        (Vector2){game->players[i]->position.x, game->players[i]->position.y};
    float divisor = single_player_mode ? 2.0f : 4.0f;
    game->level->cameras[i]->offset =
        (Vector2){(float)screen_width * (i == 0 ? 1 : 3) / divisor,
                  (float)screen_height / 2};
    game->level->cameras[i]->rotation = 0.0f;
    game->level->cameras[i]->zoom = 1.6f / screen_scale;
  }
}

void update_camera(Game *game) {
  for (size_t i = 0; i < game->playerCount; i++) {
    game->level->cameras[i]->target =
        (Vector2){game->players[i]->position.x, game->players[i]->position.y};
  }
}

void draw_viewports(Rectangle *game_viewport, Game *game, Level *level) {
  bool single_player_mode = game->playerCount == 1;
  for (size_t i = 0; i < game->playerCount; i++) {
    float divisor = single_player_mode ? 1.0f : 2.0f;
    BeginScissorMode(
        game_viewport->x + i * (game_viewport->width / divisor) + (10 + i * 5),
        game_viewport->y + 10, game_viewport->width / divisor - (25 - i * 5),
        game_viewport->height - 20);
    BeginMode2D(*level->cameras[i]);
    render_level(level);
    render_players(game);
    EndMode2D();
    EndScissorMode();
  }
  if (single_player_mode) {
    return;
  }
  Rectangle ui_divider = (Rectangle){(float)GetScreenWidth() / 2 - 5, 40, 10.0f,
                                     GetScreenHeight() - 40};
  DrawRectangleRec(ui_divider, DARKGRAY);
}
