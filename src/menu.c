#include "menu.h"
#include "level.h"
#include <stdio.h>
#include <stdlib.h>

const MenuButton menu_buttons[] = {
    {"Start Game", START_BUTTON, LEVEL_ONE},
    {"Exit", EXIT_BUTTON, EXIT},
};

void generate_player_field(Menu *menu) {
  for (int i = 0; i < menu->playerCount; i++) {
    int halfScreenWidth = GetScreenWidth() / 2;
    int halfScreenHeight = GetScreenHeight() / 2;
    menu->players[i] = malloc(sizeof(PlayerDetails));
    menu->players[i]->name[0] = '\0';
    menu->players[i]->nameLen = 0;
    menu->players[i]->color = (i == 0) ? LIME : BLACK;
    menu->players[i]->focusField = (i == 0) ? PLAYER1_NAME : PLAYER2_NAME;
    menu->players[i]->rec = (Rectangle){
        halfScreenWidth - 102, halfScreenHeight - (172 - (i * 70)), 204, 44};
  }
}

void generate_button_fields(Menu *menu) {
  for (int i = 0; i < menu->buttonCount; i++) {
    menu->buttons[i] = malloc(sizeof(Button));
    menu->buttons[i]->text = menu_buttons[i].label;
    menu->buttons[i]->rec =
        (Rectangle){((float)GetScreenWidth() / 2) - 100,
                    ((float)GetScreenHeight() / 2) + (i * 60), 200, 40};
    menu->buttons[i]->color = DARKGRAY;
    menu->buttons[i]->focusField = menu_buttons[i].focusField;
    menu->buttons[i]->action = menu_buttons[i].action;
  }
}

void new_menu(Menu *menu, int playerCount) {
  menu->focusField = PLAYER1_NAME;
  menu->playerCount = playerCount;
  menu->buttonCount = sizeof(menu_buttons) / sizeof(menu_buttons[0]);
  menu->players = malloc(sizeof(PlayerDetails *) * playerCount);
  menu->buttons = malloc(sizeof(Button *) * menu->buttonCount);
  generate_player_field(menu);
  generate_button_fields(menu);
}

void draw_player_name_input(Rectangle rec, const char *label,
                            const char *playerName, Color focusColor,
                            Color textColor) {
  DrawText(label, rec.x + (rec.width / 2) - ((float)MeasureText(label, 20) / 2),
           rec.y - 20, 20, focusColor);
  DrawRectangleLinesEx(rec, 8.0f, focusColor);
  DrawRectangle(rec.x + 2, rec.y + 2, rec.width - 4, rec.height - 4, LIGHTGRAY);
  DrawText(playerName,
           rec.x + (rec.width / 2) - ((float)MeasureText(playerName, 20) / 2),
           rec.y + 10, 20, textColor);
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
  int halfScreenWidth = GetScreenWidth() / 2;
  int halfScreenHeight = GetScreenHeight() / 2;
  for (int i = 0; i < menu->playerCount; i++) {
    char label[20];
    snprintf(label, 20, "Player %d name:", i + 1);
    draw_player_name_input(menu->players[i]->rec, label, menu->players[i]->name,
                           menu->players[i]->color, BLACK);
  }
  //
  // Buttons
  //
  for (int i = 0; i < menu->buttonCount; i++) {
    DrawRectanglePro(menu->buttons[i]->rec, (Vector2){0, 0}, 0.0f,
                     menu->buttons[i]->color);
    DrawText(menu->buttons[i]->text,
             menu->buttons[i]->rec.x + (menu->buttons[i]->rec.width / 2) -
                 ((float)MeasureText(menu->buttons[i]->text, 20) / 2),
             menu->buttons[i]->rec.y + 10, 20, WHITE);
  }
  //
  // Check collisions
  //
  for (int i = 0; i < menu->playerCount; i++) {
    if (handle_menu_button(menu->players[i]->rec, &menu->players[i]->color,
                           LIME, BLACK,
                           menu->focusField != menu->players[i]->focusField)) {
      menu->focusField = menu->players[i]->focusField;
    }
  }
  for (int i = 0; i < menu->buttonCount; i++) {
    if (handle_menu_button(menu->buttons[i]->rec, &menu->buttons[i]->color,
                           LIME, DARKGRAY,
                           menu->focusField != menu->buttons[i]->focusField)) {
      game->gameState = menu->buttons[i]->action;
    }
  }
  //
  // Check inputs
  //
  if (IsKeyPressed(KEY_TAB)) {
    menu->focusField = (menu->focusField + 1) % FOCUS_FIELD_COUNT;
    for (int i = 0; i < menu->buttonCount; i++) {
      if (menu->focusField == menu->buttons[i]->focusField) {
        menu->buttons[i]->color =
            (menu->buttons[i]->focusField == START_BUTTON) ? LIME : RED;
      } else {
        menu->buttons[i]->color = DARKGRAY;
      }
    }
    for (int i = 0; i < menu->playerCount; i++) {
      if (menu->focusField == menu->players[i]->focusField) {
        menu->players[i]->color = LIME;
      } else {
        menu->players[i]->color = BLACK;
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

void check_inputs(Menu *menu, Game *game, struct Level *level) {
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
  if (menu->buttons != NULL) {
    for (int i = 0; i < menu->buttonCount; i++) {
      free(menu->buttons[i]);
    }
    free(menu->buttons);
  }
}
