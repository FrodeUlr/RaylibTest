#include "../include/textures.h"

TextureDef SetTextureDef(const char *name, int start_x, int end_x, int start_y,
                         int end_y, char *texture_path) {
  TextureDef tex_def;
  tex_def.name = name;
  tex_def.start_x = start_x;
  tex_def.end_x = end_x;
  tex_def.start_y = start_y;
  tex_def.end_y = end_y;
  tex_def.texture = LoadTexture(texture_path);
  return tex_def;
}
