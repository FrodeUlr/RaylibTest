#include "../include/game.h"
#include "../include/menu.h"
#include "../include/player.h"
#include "../include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

void start_game(Config *config) {
  printf(
      "Starting game with resolution %dx%d, fullscreen: %d, target FPS: %d\n",
      config->screenWidth, config->screenHeight, config->fullscreen,
      config->targetFPS);
  GameScreen currentScreen = START;
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
  Player player_one = {0};
  Player player_two = {0};
  Player *players[] = {&player_one, &player_two};
  size_t player_count = sizeof(players) / sizeof(players[0]);
  SetTargetFPS(config->targetFPS);
  Level *level;
  level = malloc(sizeof(Level));
  printf("Entering main game loop\n");
  Menu *menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("Error menu");
    return;
  }
  new_menu(menu);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (currentScreen == START) {
      draw_main_menu(menu);
      if (menu->currentScreen != START) {
        *players[0] =
            generate_player(menu->player1Name, PLAYER_ONE, screen_width * 0.9f,
                            screen_height * 0.7f, PINK);
        *players[1] =
            generate_player(menu->player2Name, PLAYER_TWO, screen_width * 0.1f,
                            screen_height * 0.3f, VIOLET);
        currentScreen = menu->currentScreen;
      }
    } else if (currentScreen == GAME_OVER) {
      DrawText("GAME OVER", half_screen_width - 100, half_screen_height - 20,
               40, BLACK);
      if (IsKeyPressed(KEY_ENTER)) {
        currentScreen = START;
        menu->currentScreen = START;
      }
    } else {
      if (currentScreen == LEVEL_ONE) {
        if (level->level != 1) {
          *level = get_level(1);
          level->first_frame = true;
        }
        if (check_level_completion(players, level, player_count)) {
          currentScreen = LEVEL_TWO;
        }
      }
      if (currentScreen == LEVEL_TWO) {
        if (level->level != 2) {
          *level = get_level(2);
          level->first_frame = true;
        }
        if (check_level_completion(players, level, player_count)) {
          currentScreen = GAME_OVER;
        }
      }
      ClearBackground(BLACK);
      render_level(level, screen_width, screen_height);
      render_player(&player_one, '1', level);
      render_player(&player_two, '2', level);
      update_position(&player_one, level);
      update_position(&player_two, level);

      two_player_collision(&player_one, &player_two, level, screen_width,
                           screen_height);
      check_collision(&player_one, screen_width, screen_height);
      check_collision(&player_two, screen_width, screen_height);
      const char *fpsText = TextFormat("FPS: %d", GetFPS());
      const char *screenInfo =
          TextFormat("Screen: %dx%d", screen_width, screen_height);
      const char *playerPosText =
          TextFormat("Player1 Pos: (%.2f, %.2f)", player_one.position.x,
                     player_one.position.y);
      const char *player_two_pos_text =
          TextFormat("Player2 Pos: (%.2f, %.2f)", player_two.position.x,
                     player_two.position.y);
      if (level->first_frame) {
        level->first_frame = false;
      }
      DrawText(fpsText, 0, 0, 20, WHITE);
      DrawText(screenInfo, 0, 20, 20, WHITE);
      DrawText(playerPosText, 0, 40, 20, WHITE);
      DrawText(player_two_pos_text, 0, 60, 20, WHITE);
    }
    EndDrawing();
  }
  UnloadTexture(level->wall_texture.texture);
  UnloadTexture(level->ground_texture.texture);
  UnloadTexture(level->target_texture.texture);
  UnloadTexture(level->house_texture.texture);
  CloseWindow();
}
