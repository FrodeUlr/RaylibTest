#include "../include/game.h"
#include "../include/player.h"
#include "../include/raylib.h"

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
  const Level level = get_level(1);

  char player1[32] = "";
  char player2[32] = "";
  int player1Len = 0, player2Len = 0;
  bool editingPlayer1 = true;
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (currentScreen == START) {
      DrawText("Player 1 name:", halfScreenWidth - 80, halfScreenHeight - 190,
               20, BLACK);
      DrawRectangle(halfScreenWidth - 100, halfScreenHeight - 170, 200, 40,
                    LIGHTGRAY);
      DrawText(player1, halfScreenWidth - 90, halfScreenHeight - 160, 20,
               BLACK);
      DrawText("Player 2 name:", halfScreenWidth - 80, halfScreenHeight - 120,
               20, BLACK);
      DrawRectangle(halfScreenWidth - 100, halfScreenHeight - 100, 200, 40,
                    LIGHTGRAY);
      DrawText(player2, halfScreenWidth - 90, halfScreenHeight - 90, 20, BLACK);
      DrawRectangle(halfScreenWidth - 100, halfScreenHeight, 200, 40, DARKGRAY);
      DrawText("START", halfScreenWidth - 30, halfScreenHeight + 10, 20, WHITE);

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();
        if (CheckCollisionPointRec(mouse, (Rectangle){halfScreenWidth - 100,
                                                      halfScreenHeight - 170,
                                                      200, 40}))
          editingPlayer1 = true;
        else if (CheckCollisionPointRec(
                     mouse, (Rectangle){halfScreenWidth - 100,
                                        halfScreenHeight - 100, 200, 40}))
          editingPlayer1 = false;
        else if (CheckCollisionPointRec(
                     mouse, (Rectangle){halfScreenWidth - 100, halfScreenHeight,
                                        200, 40})) {
          player_one.name = player1;
          player_two.name = player2;
          currentScreen = GAME;
        }
      }

      int key = GetCharPressed();
      while (key > 0) {
        if (editingPlayer1 && player1Len < 31) {
          player1[player1Len++] = (char)key;
          player1[player1Len] = '\0';
        } else if (!editingPlayer1 && player2Len < 31) {
          player2[player2Len++] = (char)key;
          player2[player2Len] = '\0';
        }
        key = GetCharPressed();
      }
      if (IsKeyPressed(KEY_BACKSPACE)) {
        if (editingPlayer1 && player1Len > 0)
          player1[--player1Len] = '\0';
        else if (!editingPlayer1 && player2Len > 0)
          player2[--player2Len] = '\0';
      }
    }
    if (currentScreen == GAME) {
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
    }
    EndDrawing();
  }
  CloseWindow();
}
