#ifndef LEVELS_H
#define LEVELS_H
#include "raylib.h"
#include "textures.h"
#include <stdbool.h>

typedef struct Level {
  int level;
  int width;
  int height;
  const char **data;
  bool completed;
  TextureDef wall_texture;
  TextureDef ground_texture;
} Level;

typedef enum { WALL, TARGET, WATER, GROUND } TILE_TYPE;

void render_level(Level level, int screen_width, int screen_height);
Level get_level(int number);

#endif
