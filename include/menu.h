#ifndef MENU_H
#define MENU_H
#include "../include/game.h"
#include "../include/player.h"
#include "../include/raylib.h"
#include <stdbool.h>

typedef enum FocusField {
  PLAYER1_NAME,
  PLAYER2_NAME,
  START_BUTTON,
  EXIT_BUTTON,
  FOCUS_FIELD_COUNT
} FocusField;

typedef struct PlayerDetails {
  char name[16];
  int nameLen;
  Color color;
  FocusField focusField;
  Rectangle rec;
} PlayerDetails;

typedef struct Menu {
  FocusField focusField;
  int screenWidth;
  int screenHeight;
  int playerCount;
  Color exitColor;
  Color startColor;
  PlayerDetails **players;
} Menu;

void new_menu(Menu *menu, int playerCount);
void draw_main_menu(Menu *menu, Game *game);
void check_inputs(Menu *menu, Game *game, Level *level);
void free_menu(Menu *menu);

#endif
