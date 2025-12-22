#include "../include/game.h"
#include "../include/menu.h"
#include "../include/raylib.h"
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
  Level *level = malloc(sizeof(Level));
  game->level = level;
  initialize_players(game, 2);
  printf("Entering main game loop\n");
  Menu *menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("Error menu");
    return;
  }
  new_menu(menu, 2);
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
      render_level(level);
      update_position(game->players, game->playerCount, level);
      players_collision(game->players, game->playerCount, level);

      render_players(game->players, game->playerCount, level);
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
  const char *fpsText = TextFormat("FPS: %d", GetFPS());
  const char *screenInfo =
      TextFormat("Screen: %dx%d", screenWidth, screenHeight);
  DrawText(fpsText, 0, 0, 20, WHITE);
  DrawText(screenInfo, 0, 20, 20, WHITE);
  for (size_t i = 0; i < playerCount; i++) {
    const char *position_text =
        TextFormat("%s Pos: (%.2f, %.2f)", players[i]->name,
                   players[i]->position.x, players[i]->position.y);
    const char *player_speeds = TextFormat(
        "Speed: x->%.2f y->%.2f Accel: x->%.2f y->%.2f", players[i]->velocity.x,
        players[i]->velocity.y, players[i]->accelerationVector.x,
        players[i]->accelerationVector.y);
    DrawText(position_text, 0, 40 + i * 20, 20, WHITE);
    DrawText(player_speeds, 300, 40 + i * 20, 20, WHITE);
  }
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
