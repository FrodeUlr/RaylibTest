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
  SetTargetFPS(config->targetFPS);
  Level *level = malloc(sizeof(Level));
  game->level = level;
  initialize_players(game, 2);
  printf("Entering main game loop\n");
  Menu *menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("Error menu");
    return;
  }
  new_menu(menu);
  while (game->running) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (game->game_state == MAIN_MENU) {
      draw_main_menu(menu, game);
      if (game->game_state == EXIT) {
        printf("Exiting game from main menu\n");
        game->running = false;
      } else if (game->game_state != MAIN_MENU && menu->player1NameLen != 0 &&
                 menu->player2NameLen != 0) {
        generate_player(game->players[0], menu->player1Name, PLAYER_ONE,
                        screen_width * 0.9f, screen_height * 0.7f, PINK);
        generate_player(game->players[1], menu->player2Name, PLAYER_TWO,
                        screen_width * 0.1f, screen_height * 0.3f, VIOLET);
      } else {
        game->game_state = MAIN_MENU;
      }
    } else if (game->game_state == GAME_OVER) {
      DrawText("GAME OVER", half_screen_width - 100, half_screen_height - 20,
               40, BLACK);
      if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
        game->game_state = MAIN_MENU;
      }
    } else if (game->game_state != MAIN_MENU || game->game_state != EXIT) {
      if (game->game_state == LEVEL_ONE) {
        if (level->number != 1) {
          set_level(level, 1);
          level->first_frame = true;
        }
        if (check_level_completion(game->players, game->level,
                                   game->player_count)) {
          game->game_state = LEVEL_TWO;
        }
      }
      if (game->game_state == LEVEL_TWO) {
        if (level->number != 2) {
          set_level(level, 2);
          level->first_frame = true;
        }
        if (check_level_completion(game->players, game->level,
                                   game->player_count)) {
          game->game_state = GAME_OVER;
        }
      }
      ClearBackground(BLACK);
      render_level(level);
      update_position(game->players, game->player_count, level);
      two_player_collision(game->players[0], game->players[1], level);

      render_players(game->players, game->player_count, level);
      draw_ui(game->players, game->player_count, screen_width, screen_height);
      if (level->first_frame) {
        level->first_frame = false;
      }
      check_inputs(menu, game, level);
    }
    EndDrawing();
  }
  for (size_t i = 0; i < game->player_count; i++) {
    if (game->players[i] != NULL) {
      free(game->players[i]);
    }
  }
  if (game->players != NULL) {
    free(game->players);
  }
  if (game->level != NULL) {
    UnloadTexture(game->level->wall_texture.texture);
    UnloadTexture(game->level->ground_texture.texture);
    UnloadTexture(game->level->target_texture.texture);
    UnloadTexture(game->level->house_texture.texture);
    free(game->level);
  }
  if (menu != NULL) {
    free(menu);
  }
  CloseWindow();
}

void draw_ui(Player *players[], int player_count, int screen_width,
             int screen_height) {
  const char *fpsText = TextFormat("FPS: %d", GetFPS());
  const char *screenInfo =
      TextFormat("Screen: %dx%d", screen_width, screen_height);
  for (size_t i = 0; i < player_count; i++) {
    const char *position_text =
        TextFormat("%s Pos: (%.2f, %.2f)", players[i]->name,
                   players[i]->position.x, players[i]->position.y);
    DrawText(position_text, 0, 40 + i * 20, 20, WHITE);
  }
  DrawText(fpsText, 0, 0, 20, WHITE);
  DrawText(screenInfo, 0, 20, 20, WHITE);
}

void initialize_players(Game *game, int player_count) {
  game->player_count = player_count;
  game->players = malloc(sizeof(Player *) * game->player_count);
  for (int i = 0; i < player_count; i++) {
    game->players[i] = malloc(sizeof(Player));
  }
}
