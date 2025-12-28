#ifndef GAMETYPES_H
#define GAMETYPES_H

typedef enum { MAIN_MENU, EXIT, LEVEL_ONE, LEVEL_TWO, GAME_OVER } GameState;
typedef enum { WALL, TARGET, WATER, GROUND, HOUSE, PLAYER } TILE_TYPE;
typedef enum { PLAYER_ONE, PLAYER_TWO } PlayerType;
typedef enum FocusField {
  PLAYER1_NAME,
  PLAYER2_NAME,
  START_BUTTON,
  EXIT_BUTTON,
  FOCUS_FIELD_COUNT
} FocusField;

#endif
