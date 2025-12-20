#ifndef LEVELS_H
#define LEVELS_H
#include "raylib.h"
#include "textures.h"
#include <stdbool.h>

typedef struct Level {
  int number;
  int columns;
  int rows;
  int offset_x;
  int offset_y;
  int tile_size;
  bool first_frame;
  const char **data;
  bool completed;
  TextureDef wall_texture;
  TextureDef ground_texture;
  TextureDef target_texture;
  TextureDef house_texture;
  TextureDef water_texture;
} Level;

typedef enum { WALL, TARGET, WATER, GROUND, HOUSE, PLAYER } TILE_TYPE;

void render_level(Level *level);
void set_offset(Level *level);
void set_level(Level *level, int number);

#endif
