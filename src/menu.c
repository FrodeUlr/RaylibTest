#include "../include/menu.h"
#include <stdio.h>
#include <stdlib.h>

void generate_player_field(Menu *menu) {
  for (int i = 0; i < menu->playerCount; i++) {
    int halfScreenWidth = menu->screenWidth / 2;
    int halfScreenHeight = menu->screenHeight / 2;
    menu->players[i] = malloc(sizeof(PlayerDetails));
    menu->players[i]->name[0] = '\0';
    menu->players[i]->nameLen = 0;
    menu->players[i]->color = (i == 0) ? LIME : BLACK;
    menu->players[i]->focusField = (i == 0) ? PLAYER1_NAME : PLAYER2_NAME;
    menu->players[i]->rec = (Rectangle){
        halfScreenWidth - 102, halfScreenHeight - (172 - (i * 70)), 204, 44};
  }
}

void new_menu(Menu *menu, int playerCount) {
  menu->focusField = PLAYER1_NAME;
  menu->playerCount = playerCount;
  menu->screenWidth = GetScreenWidth();
  menu->screenHeight = GetScreenHeight();
  menu->exitColor = DARKGRAY;
  menu->startColor = DARKGRAY;
  menu->players = malloc(sizeof(PlayerDetails *) * playerCount);
  generate_player_field(menu);
}

void draw_player_name_input(Rectangle rec, const char *label,
                            const char *playerName, Color focusColor,
                            Color textColor) {
  DrawText(label, rec.x + 20, rec.y - 20, 20, focusColor);
  DrawRectangleLinesEx(rec, 8.0f, focusColor);
  DrawRectangle(rec.x + 2, rec.y + 2, rec.width - 4, rec.height - 4, LIGHTGRAY);
  DrawText(playerName, rec.x + 10, rec.y + 10, 20, textColor);
}

bool handle_menu_button(Rectangle rect, Color *color, Color hoverColor,
                        Color normalColor, bool resetColor) {
  Vector2 mouse = GetMousePosition();

  if (CheckCollisionPointRec(mouse, rect)) {
    *color = hoverColor;
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      return true;
    }
  } else if (resetColor) {
    *color = normalColor;
  }
  return false;
}

void draw_main_menu(Menu *menu, Game *game) {
  int halfScreenWidth = menu->screenWidth / 2;
  int halfScreenHeight = menu->screenHeight / 2;
  for (int i = 0; i < menu->playerCount; i++) {
    char label[20];
    snprintf(label, 20, "Player %d name:", i + 1);
    draw_player_name_input(menu->players[i]->rec, label, menu->players[i]->name,
                           menu->players[i]->color, BLACK);
  }
  //
  // Buttons
  //
  Rectangle rec_start = {.height = 40,
                         .width = 200,
                         .x = halfScreenWidth - 100,
                         .y = halfScreenHeight};
  DrawRectanglePro(rec_start, (Vector2){0, 0}, 0.0f, menu->startColor);
  DrawText("START", halfScreenWidth - 37, halfScreenHeight + 10, 20, WHITE);

  Rectangle rec_exit = {.height = 40,
                        .width = 200,
                        .x = halfScreenWidth - 100,
                        .y = halfScreenHeight + 60};
  DrawRectanglePro(rec_exit, (Vector2){0, 0}, 0.0f, menu->exitColor);
  DrawText("EXIT", halfScreenWidth - 28, halfScreenHeight + 70, 20, WHITE);

  //
  // Check collisions
  //
  if (handle_menu_button(menu->players[0]->rec, &menu->players[0]->color, LIME,
                         BLACK, menu->focusField != PLAYER1_NAME)) {
    menu->focusField = PLAYER1_NAME;
  }
  if (handle_menu_button(menu->players[1]->rec, &menu->players[1]->color, LIME,
                         BLACK, menu->focusField != PLAYER2_NAME)) {
    menu->focusField = PLAYER2_NAME;
  }
  if (handle_menu_button(rec_start, &menu->startColor, LIME, DARKGRAY,
                         menu->focusField != START_BUTTON)) {
    game->gameState = LEVEL_ONE;
  }
  if (handle_menu_button(rec_exit, &menu->exitColor, RED, DARKGRAY,
                         menu->focusField != EXIT_BUTTON)) {
    game->gameState = EXIT;
  }
  //
  // Check inputs
  //
  if (IsKeyPressed(KEY_TAB)) {
    menu->focusField = (menu->focusField + 1) % FOCUS_FIELD_COUNT;
    if (menu->focusField == START_BUTTON) {
      menu->startColor = LIME;
    } else if (menu->focusField == EXIT_BUTTON) {
      menu->exitColor = RED;
    } else {
      for (int i = 0; i < menu->playerCount; i++) {
        if (menu->focusField == menu->players[i]->focusField) {
          menu->players[i]->color = LIME;
        } else {
          menu->players[i]->color = BLACK;
        }
      }
    }
  }
  int key = GetCharPressed();
  while (key > 0) {
    if (menu->focusField != START_BUTTON || menu->focusField != EXIT_BUTTON) {
      for (int i = 0; i < menu->playerCount; i++) {
        if (menu->focusField == menu->players[i]->focusField &&
            menu->players[i]->nameLen < 15) {
          menu->players[i]->name[menu->players[i]->nameLen++] = (char)key;
          menu->players[i]->name[menu->players[i]->nameLen] = '\0';
        }
      }
    }
    key = GetCharPressed();
  }
  if (IsKeyPressed(KEY_BACKSPACE)) {
    if (menu->focusField != START_BUTTON || menu->focusField != EXIT_BUTTON) {
      for (int i = 0; i < menu->playerCount; i++) {
        if (menu->focusField == menu->players[i]->focusField &&
            menu->players[i]->nameLen > 0) {
          menu->players[i]->name[--menu->players[i]->nameLen] = '\0';
        }
      }
    }
  }
  if (IsKeyPressed(KEY_ENTER)) {
    if (menu->focusField == START_BUTTON) {
      game->gameState = LEVEL_ONE;
    } else if (menu->focusField == EXIT_BUTTON) {
      game->gameState = EXIT;
    }
  }
}

void check_inputs(Menu *menu, Game *game, Level *level) {
  if (IsKeyPressed(KEY_ESCAPE)) {
    game->gameState = MAIN_MENU;
    level->firstFrame = true;
  }
}

void free_menu(Menu *menu) {
  if (menu->players != NULL) {
    for (int i = 0; i < menu->playerCount; i++) {
      free(menu->players[i]);
    }
    free(menu->players);
  }
}
