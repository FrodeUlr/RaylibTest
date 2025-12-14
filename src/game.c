#include "../include/game.h"
#include "../include/menu.h"
#include "../include/player.h"
#include "../include/raylib.h"
#include <stdio.h>
#include <stdlib.h>

void start_game(Config *config) {
  GameScreen currentScreen = START;
  const char *title = "Test Game";
  if (config->fullscreen) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(GetScreenWidth(), GetScreenHeight(), title);
  } else {
    InitWindow(config->screenWidth, config->screenHeight, title);
  }
  const int screenWidth = GetScreenWidth();
  const int screenHeight = GetScreenHeight();
  const int halfScreenWidth = screenWidth / 2;
  const int halfScreenHeight = screenHeight / 2;
  Player player_one =
      generate_player("Player1", screenWidth * 0.9f, screenHeight * 0.8f, PINK);
  Player player_two = generate_player("Player2", screenWidth * 0.1f,
                                      screenHeight * 0.2f, VIOLET);
  SetTargetFPS(config->targetFPS);
  Level level = get_level(1);
  Menu *menu = malloc(sizeof(Menu));
  if (menu == NULL) {
    printf("Error menu");
    return;
  }
  menu->player1Name[0] = '\0';
  menu->player2Name[0] = '\0';
  menu->player1NameLen = 0;
  menu->player2NameLen = 0;
  menu->editingPlayer1 = true;
  menu->currentScreen = START;
  menu->screenWidth = screenWidth;
  menu->screenHeight = screenHeight;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (currentScreen == START) {
      draw_main_menu(menu);
      if (menu->currentScreen != START) {
        player_one.name = menu->player1Name;
        player_two.name = menu->player2Name;
        currentScreen = menu->currentScreen;
      }
    }
    if (currentScreen == LEVEL_ONE) {
      update_position(&player_one, false, level, screenWidth, screenHeight);
      update_position(&player_two, true, level, screenWidth, screenHeight);

      two_player_collision(&player_one, &player_two, level, screenWidth,
                           screenHeight);
      check_collision(&player_one, screenWidth, screenHeight);
      check_collision(&player_two, screenWidth, screenHeight);

      const char *fpsText = TextFormat("FPS: %d", GetFPS());
      const char *screenInfo =
          TextFormat("Screen: %dx%d", screenWidth, screenHeight);
      const char *playerPosText =
          TextFormat("Player1 Pos: (%.2f, %.2f)", player_one.position.x,
                     player_one.position.y);
      const char *player_two_pos_text =
          TextFormat("Player2 Pos: (%.2f, %.2f)", player_two.position.x,
                     player_two.position.y);
      ClearBackground(RED);
      render_level(level, screenWidth, screenHeight);
      render_player(player_one);
      render_player(player_two);
      DrawText(fpsText, 0, 0, 20, WHITE);
      DrawText(screenInfo, 0, 20, 20, WHITE);
      DrawText(playerPosText, 0, 40, 20, WHITE);
      DrawText(player_two_pos_text, 0, 60, 20, WHITE);
      if (check_level_completion(player_one, level, screenWidth,
                                 screenHeight) ||
          check_level_completion(player_two, level, screenWidth,
                                 screenHeight)) {
        currentScreen = LEVEL_TWO;
      }
    }
    if (currentScreen == LEVEL_TWO) {
      if (level.level != 2) {
        player_one.position =
            (Vector2){screenWidth * 0.9f, screenHeight * 0.8f};
        player_two.position =
            (Vector2){screenWidth * 0.1f, screenHeight * 0.2f};
        level = get_level(2);
      }
      update_position(&player_one, false, level, screenWidth, screenHeight);
      update_position(&player_two, true, level, screenWidth, screenHeight);

      two_player_collision(&player_one, &player_two, level, screenWidth,
                           screenHeight);
      check_collision(&player_one, screenWidth, screenHeight);
      check_collision(&player_two, screenWidth, screenHeight);

      const char *fpsText = TextFormat("FPS: %d", GetFPS());
      const char *screenInfo =
          TextFormat("Screen: %dx%d", screenWidth, screenHeight);
      const char *playerPosText =
          TextFormat("Player1 Pos: (%.2f, %.2f)", player_one.position.x,
                     player_one.position.y);
      const char *player_two_pos_text =
          TextFormat("Player2 Pos: (%.2f, %.2f)", player_two.position.x,
                     player_two.position.y);
      ClearBackground(RED);
      render_level(level, screenWidth, screenHeight);
      render_player(player_one);
      render_player(player_two);
      DrawText(fpsText, 0, 0, 20, WHITE);
      DrawText(screenInfo, 0, 20, 20, WHITE);
      DrawText(playerPosText, 0, 40, 20, WHITE);
      DrawText(player_two_pos_text, 0, 60, 20, WHITE);
      if (check_level_completion(player_one, level, screenWidth,
                                 screenHeight) ||
          check_level_completion(player_two, level, screenWidth,
                                 screenHeight)) {
        currentScreen = GAME_OVER;
      }
    }
    if (currentScreen == GAME_OVER) {
      DrawText("GAME OVER", halfScreenWidth - 100, halfScreenHeight - 20, 40,
               BLACK);
    }
    EndDrawing();
  }
  CloseWindow();
}
