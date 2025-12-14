#include "../include/menu.h"

void draw_main_menu(Menu *menu) {
  int halfScreenWidth = menu->screenWidth / 2;
  int halfScreenHeight = menu->screenHeight / 2;
  DrawText("Player 1 name:", halfScreenWidth - 80, halfScreenHeight - 190, 20,
           BLACK);
  DrawRectangle(halfScreenWidth - 100, halfScreenHeight - 170, 200, 40,
                LIGHTGRAY);
  DrawText(menu->player1Name, halfScreenWidth - 90, halfScreenHeight - 160, 20,
           BLACK);
  DrawText("Player 2 name:", halfScreenWidth - 80, halfScreenHeight - 120, 20,
           BLACK);
  DrawRectangle(halfScreenWidth - 100, halfScreenHeight - 100, 200, 40,
                LIGHTGRAY);
  DrawText(menu->player2Name, halfScreenWidth - 90, halfScreenHeight - 90, 20,
           BLACK);
  DrawRectangle(halfScreenWidth - 100, halfScreenHeight, 200, 40, DARKGRAY);
  DrawText("START", halfScreenWidth - 30, halfScreenHeight + 10, 20, WHITE);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse,
                               (Rectangle){halfScreenWidth - 100,
                                           halfScreenHeight - 170, 200, 40}))
      menu->editingPlayer1 = true;
    else if (CheckCollisionPointRec(mouse, (Rectangle){halfScreenWidth - 100,
                                                       halfScreenHeight - 100,
                                                       200, 40}))
      menu->editingPlayer1 = false;
    else if (CheckCollisionPointRec(mouse,
                                    (Rectangle){halfScreenWidth - 100,
                                                halfScreenHeight, 200, 40})) {
      menu->currentScreen = LEVEL_ONE;
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
}
