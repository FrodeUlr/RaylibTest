#include "game.h"
#include "external/raylib.h"
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
  struct Level *level = malloc(sizeof(Level));
  game->level = level;
  initialize_players(game, 2);
  printf("Entering main game loop\n");
  Menu *menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("Error menu");
    return;
  }
  new_menu(menu, 2);
  Rectangle game_viewport =
      (Rectangle){0, 40, (float)screen_width, (float)screen_height - 40};
  Rectangle ui_divider =
      (Rectangle){(float)screen_width / 2 - 5, 40, 10.0f, screen_height - 40};
  while (game->running) {
    BeginDrawing();
    if (game->gameState == MAIN_MENU) {
      ClearBackground(RAYWHITE);
      draw_main_menu(menu, game);
      if (game->gameState == EXIT) {
        printf("Exiting game from main menu\n");
        game->running = false;
      } else if (game->gameState != MAIN_MENU &&
                 menu->players[0]->nameLen != 0 &&
                 menu->players[1]->nameLen != 0) {
        generate_player(game->players[0], menu->players[0]->name, PLAYER_ONE,
                        screen_width * 0.9f, screen_height * 0.7f, RAYWHITE);
        generate_player(game->players[1], menu->players[1]->name, PLAYER_TWO,
                        screen_width * 0.1f, screen_height * 0.3f, BEIGE);
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
        if (level->number != 1) {
          for (int i = 0; i < game->playerCount; i++) {
            reset_player_movement(game->players[i]);
          }
          set_level(level, 1);
          level->firstFrame = true;
        }
        if (check_level_completion(game->players, game->level,
                                   game->playerCount)) {
          game->gameState = LEVEL_TWO;
        }
      }
      if (game->gameState == LEVEL_TWO) {
        if (level->number != 2) {
          for (int i = 0; i < game->playerCount; i++) {
            reset_player_movement(game->players[i]);
          }
          set_level(level, 2);
          level->firstFrame = true;
        }
        if (check_level_completion(game->players, game->level,
                                   game->playerCount)) {
          game->gameState = GAME_OVER;
        }
      }
      if (level->cameras == NULL) {
        set_camera(game, screen_width, screen_height);
      }
      level->cameras[0]->target =
          (Vector2){game->players[0]->position.x, game->players[0]->position.y};
      level->cameras[1]->target =
          (Vector2){game->players[1]->position.x, game->players[1]->position.y};
      update_position(game->players, game->playerCount, level);
      players_collision(game->players, game->playerCount, level);
      draw_viewports(&game_viewport, game, level);
      DrawRectangleRec(ui_divider, DARKGRAY);
      draw_ui(game->players, game->playerCount, screen_width, screen_height);
      if (level->firstFrame) {
        level->firstFrame = false;
      }
      check_inputs(menu, game, level);
    }
    EndDrawing();
  }
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

void initialize_players(Game *game, int player_count) {
  game->playerCount = player_count;
  game->players = malloc(sizeof(Player *) * game->playerCount);
  for (int i = 0; i < player_count; i++) {
    game->players[i] = malloc(sizeof(Player));
  }
}

void free_game(Game *game) {
  for (size_t i = 0; i < game->playerCount; i++) {
    if (game->level->cameras != NULL && game->level->cameras[i] != NULL) {
      free(game->level->cameras[i]);
    }
  }
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
  game->level->cameras = malloc(sizeof(Camera2D *) * game->playerCount);
  for (size_t i = 0; i < game->playerCount; i++) {
    game->level->cameras[i] = malloc(sizeof(Camera2D));
    game->level->cameras[i]->target =
        (Vector2){game->players[0]->position.x, game->players[0]->position.y};
    game->level->cameras[i]->offset = (Vector2){
        (float)screen_width * (i == 0 ? 1 : 3) / 4, (float)screen_height / 2};
    game->level->cameras[i]->rotation = 0.0f;
    game->level->cameras[i]->zoom = 1.0f;
  }
}

void draw_viewports(Rectangle *game_viewport, Game *game, Level *level) {
  for (size_t i = 0; i < game->playerCount; i++) {
    BeginScissorMode(
        game_viewport->x + i * (game_viewport->width / 2) + (10 + i * 5),
        game_viewport->y + 10, game_viewport->width / 2 - (25 - i * 5),
        game_viewport->height - 20);
    BeginMode2D(*level->cameras[i]);
    render_level(level);
    render_players(game->players, game->playerCount, level);
    EndMode2D();
    EndScissorMode();
  }
}
