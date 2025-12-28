#ifndef MENU_H
#define MENU_H
#include "../include/game.h"
#include <stdbool.h>

typedef struct {
  const char *label;
  FocusField focusField;
  GameState action;
} MenuButton;

typedef struct PlayerDetails {
  char name[16];
  int nameLen;
  Color color;
  FocusField focusField;
  Rectangle rec;
} PlayerDetails;

typedef struct Button {
  const char *text;
  Color color;
  Rectangle rec;
  FocusField focusField;
  GameState action;
} Button;

typedef struct Menu {
  FocusField focusField;
  int playerCount;
  int buttonCount;
  PlayerDetails **players;
  Button **buttons;
} Menu;

void new_menu(Menu *menu, int playerCount);
void draw_main_menu(Menu *menu, Game *game);
void check_inputs(Menu *menu, Game *game, struct Level *level);
void free_menu(Menu *menu);

#endif
