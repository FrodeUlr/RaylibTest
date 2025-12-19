#ifndef TEXTURES_H
#define TEXTURES_H
#include "raylib.h"

typedef struct TextureDef {
  const char *name;
  int start_x;
  int end_x;
  int start_y;
  int end_y;
  Texture2D texture;
} TextureDef;

TextureDef SetTextureDef(const char *name, int start_x, int end_x, int start_y,
                         int end_y, char *texture_path);

#endif // !DEBUG
