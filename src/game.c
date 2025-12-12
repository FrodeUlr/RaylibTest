#include "include/game.h"
#include "include/player.h"
#include "include/raylib.h"

void start_game(Config *config) {
  const char *title = "Test Game";
  if (config->fullscreen) {
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(GetScreenWidth(), GetScreenHeight(), title);
  } else {
    InitWindow(config->screenWidth, config->screenHeight, title);
  }
  const int screenWidth = GetScreenWidth();
  const int screenHeight = GetScreenHeight();
  Player player;
  player.position = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
  player.speed = 300.0f;
  player.radius = 20.0f;
  player.color = BLUE;

  Player player2;
  player2.position = (Vector2){100.0f, 100.0f};
  player2.speed = 200.0f;
  player2.radius = 15.0f;
  player2.color = GREEN;

  SetTargetFPS(config->targetFPS);

  while (!WindowShouldClose()) {

    // Player movement
    PlayerUpdatePosition(&player, false);
    PlayerUpdatePosition(&player2, true);

    // Player boundary checking
    TwoPlayerCollision(&player, &player2);
    PlayerCheckCollision(&player, screenWidth, screenHeight);
    PlayerCheckCollision(&player2, screenWidth, screenHeight);

    BeginDrawing();
    const char *fpsText = TextFormat("FPS: %d", GetFPS());
    const char *screenInfo =
        TextFormat("Screen: %dx%d", screenWidth, screenHeight);
    ClearBackground(RED);
    DrawCircleV(player.position, player.radius, player.color);
    DrawCircleV(player2.position, player2.radius, player2.color);
    DrawText(fpsText, 0, 0, 20, WHITE);
    DrawText(screenInfo, 0, 20, 20, WHITE);
    EndDrawing();
  }
  CloseWindow();
}
