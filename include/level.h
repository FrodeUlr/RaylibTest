#ifndef LEVELS_H
#define LEVELS_H
#include "gametypes.h"
#include "raylib.h"
#include "textures.h"
#include <stdbool.h>

typedef struct Level {
  int number;
  int columns;
  int rows;
  int offsetX;
  int offsetY;
  int tileSize;
  int tileWidth;
  int tileHeight;
  bool firstFrame;
  char **data;
  bool completed;
  GameState nextState;
  TextureDef wallTexture;
  TextureDef groundTexture;
  TextureDef targetTexture;
  TextureDef houseTexture;
  TextureDef waterTexture;
  SpriteDef spritesheet;
  Camera2D **cameras;
} Level;

void render_level(Level *level);
void set_offset(Level *level);
void set_level(Level *level, int number);
void free_level_data(Level *level);

#endif
