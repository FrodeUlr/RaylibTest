#include "../include/menu.h"

void new_menu(Menu *menu) {
  menu->player1Name[0] = '\0';
  menu->player2Name[0] = '\0';
  menu->player1NameLen = 0;
  menu->player2NameLen = 0;
  menu->editingPlayer1 = true;
  menu->screenWidth = GetScreenWidth();
  menu->screenHeight = GetScreenHeight();
  menu->player1Color = LIME;
  menu->player2Color = BLACK;
}

void draw_main_menu(Menu *menu, Game *game) {
  int halfScreenWidth = menu->screenWidth / 2;
  int halfScreenHeight = menu->screenHeight / 2;
  DrawText("Player 1 name:", halfScreenWidth - 80, halfScreenHeight - 192, 20,
           BLACK);
  DrawRectangleLines(halfScreenWidth - 100, halfScreenHeight - 170, 201, 41,
                     menu->player1Color);
  DrawRectangle(halfScreenWidth - 100, halfScreenHeight - 170, 200, 40,
                LIGHTGRAY);
  DrawText(menu->player1Name, halfScreenWidth - 90, halfScreenHeight - 160, 20,
           BLACK);
  DrawText("Player 2 name:", halfScreenWidth - 80, halfScreenHeight - 122, 20,
           BLACK);
  DrawRectangleLines(halfScreenWidth - 100, halfScreenHeight - 100, 201, 41,
                     menu->player2Color);
  DrawRectangle(halfScreenWidth - 100, halfScreenHeight - 100, 200, 40,
                LIGHTGRAY);
  DrawText(menu->player2Name, halfScreenWidth - 90, halfScreenHeight - 90, 20,
           BLACK);
  DrawRectangle(halfScreenWidth - 100, halfScreenHeight, 200, 40, DARKGRAY);
  DrawText("START", halfScreenWidth - 30, halfScreenHeight + 10, 20, WHITE);

  DrawRectangle(halfScreenWidth - 100, halfScreenHeight + 60, 200, 40,
                DARKGRAY);
  DrawText("EXIT", halfScreenWidth - 30, halfScreenHeight + 70, 20, WHITE);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse,
                               (Rectangle){halfScreenWidth - 100,
                                           halfScreenHeight - 170, 200, 40})) {
      menu->editingPlayer1 = true;
      menu->player1Color = LIME;
      menu->player2Color = BLACK;
    } else if (CheckCollisionPointRec(mouse, (Rectangle){halfScreenWidth - 100,
                                                         halfScreenHeight - 100,
                                                         200, 40})) {
      menu->editingPlayer1 = false;
      menu->player1Color = BLACK;
      menu->player2Color = LIME;
    } else if (CheckCollisionPointRec(mouse,
                                      (Rectangle){halfScreenWidth - 100,
                                                  halfScreenHeight, 200, 40})) {
      game->game_state = LEVEL_ONE;
    } else if (CheckCollisionPointRec(mouse, (Rectangle){halfScreenWidth - 100,
                                                         halfScreenHeight + 60,
                                                         200, 40})) {
      game->game_state = EXIT;
    }
  }

  int key = GetCharPressed();
  while (key > 0) {
    if (menu->editingPlayer1 && menu->player1NameLen < 31) {
      menu->player1Name[menu->player1NameLen++] = (char)key;
      menu->player1Name[menu->player1NameLen] = '\0';
    } else if (!menu->editingPlayer1 && menu->player2NameLen < 31) {
      menu->player2Name[menu->player2NameLen++] = (char)key;
      menu->player2Name[menu->player2NameLen] = '\0';
    }
    key = GetCharPressed();
  }
  if (IsKeyPressed(KEY_BACKSPACE)) {
    if (menu->editingPlayer1 && menu->player1NameLen > 0)
      menu->player1Name[--menu->player1NameLen] = '\0';
    else if (!menu->editingPlayer1 && menu->player2NameLen > 0)
      menu->player2Name[--menu->player2NameLen] = '\0';
  }
  if (IsKeyPressed(KEY_ESCAPE)) {
    game->game_state = EXIT;
  }
}

void check_inputs(Menu *menu, Game *game, Level *level) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    game->game_state = MAIN_MENU;
    level->first_frame = true;
  }
}
