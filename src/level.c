#include "level.h"
#include "config.h"
#include "constants.h"
#include "raylib.h"
#include "textures.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void free_level_data(Level *level) {
  if (level->data != NULL) {
    for (int i = 0; i < level->rows; i++) {
      if (level->data[i] != NULL)
        free(level->data[i]);
    }
    free(level->data);
  }
}

void load_level(Level *level, const char *filename) {
  char buffer[MAX_LEVEL_WIDTH];

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("Failed to open level file");
    return;
  }
  level->rows = 0;
  level->data = malloc(sizeof(char *) * MAX_LEVEL_HEIGHT);
  if (level->data == NULL) {
    perror("Failed to allocate memory for level data");
    fclose(file);
    return;
  }
  while (fgets(buffer, sizeof(buffer), file)) {
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
    level->data[level->rows] = strdup(buffer);
    level->rows++;
  }
}

void set_offset(Level *level) {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  int tile_w = screen_width / level->columns;
  int tile_h = screen_height / level->rows;
  printf("Calculated tile size: %d (tile_w: %d, tile_h: %d)\n",
         tile_w < tile_h ? tile_w : tile_h, tile_w, tile_h);
  level->tileSize = tile_w < tile_h ? tile_w : tile_h;
  level->tileWidth = tile_w;
  level->tileHeight = tile_h;

  int level_pixel_width = level->tileSize * level->columns;
  int level_pixel_height = level->tileSize * level->rows;
  level->offsetX = (screen_width - level_pixel_width) / 2;
  level->offsetY = (screen_height - level_pixel_height) / 2;
}

void set_level(Level *level, int number) {
  const char **level_data = NULL;
  level->targetTexture =
      SetTextureDef("Target_Tile", 0, 16, 0, 16,
                    get_absolute_path("../art/Outdoor decoration/Chest.png"));
  level->houseTexture = SetTextureDef(
      "House_Tile", 0, 96, 0, 128,
      get_absolute_path(
          "../art/Outdoor decoration/House_1_Wood_Base_Blue.png"));
  level->waterTexture =
      SetTextureDef("Water_Tile", 0, 48, 0, 48,
                    get_absolute_path("../art/Tiles/Water_Middle.png"));
  level->spritesheet.texture =
      LoadTexture(get_absolute_path("../art/SpriteSheet.png"));
  level->spritesheet.currentColumn = 0;
  level->spritesheet.rows = 10;
  level->spritesheet.columns = 10;
  level->spritesheet.frameSize = (Vector2){
      ((float)level->spritesheet.texture.width / level->spritesheet.columns),
      ((float)level->spritesheet.texture.height / level->spritesheet.rows)};
  int height = 0;
  printf("Setting up level %d...\n", number);
  switch (number) {
  case 1: {
    printf("Setting level to 1\n");
    load_level(level, get_absolute_path("../levels/01"));
    // level->data = data;
    level->wallTexture =
        SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                      get_absolute_path("../art/Tiles/FarmLand_Tile.png"));
    level->groundTexture =
        SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                      get_absolute_path("../art/Tiles/Grass_Middle.png"));
    break;
  }
  case 2: {
    load_level(level, get_absolute_path("../levels/02"));
    level->wallTexture =
        SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                      get_absolute_path("../art/Tiles/Cliff_Tile.png"));
    level->groundTexture =
        SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                      get_absolute_path("../art/Tiles/Path_Middle.png"));
    level->firstFrame = true;
    break;
  }
  }
  level->number = number;
  level->completed = false;
  level->columns = level->data ? strlen(level->data[0]) : 0;
  set_offset(level);
}

void DrawTextureForGame(Level *level, TextureDef tdef, int xDest, int yDest,
                        float customScale) {
  float scale = level->tileSize / (float)tdef.endX * customScale;
  Rectangle source = {tdef.startX, tdef.startY, tdef.endX, tdef.endY};
  Rectangle dest = {xDest * level->tileSize + level->offsetX,
                    yDest * level->tileSize + level->offsetY, tdef.endX * scale,
                    tdef.endY * scale};
  Vector2 origin = {0, 0}; // Top-left corner
  float rotation = 0.0f;
  DrawTexturePro(tdef.texture, source, dest, origin, rotation, WHITE);
}

void draw_level_texture(Level *level, float customScale, bool flip, int row,
                        int column, Color color, int x, int y, float rotation) {
  float scale =
      level->tileSize / ((float)level->spritesheet.frameSize.x) * customScale;
  Rectangle source =
      (Rectangle){.x = 0 + level->spritesheet.frameSize.x * column,
                  .y = 0 + level->spritesheet.frameSize.y * row,
                  .width = level->spritesheet.frameSize.x,
                  .height = level->spritesheet.frameSize.y};
  if (flip) {
    source.width = -(level->spritesheet.frameSize.x);
  }
  int offset_rot_x = rotation == 90.0f || rotation == 180.0f
                         ? level->spritesheet.frameSize.x * scale
                         : 0;
  int offset_rot_y = rotation == 180.0f || rotation == 270.0f
                         ? level->spritesheet.frameSize.y * scale
                         : 0;
  Rectangle dest = {x * level->tileSize + level->offsetX + offset_rot_x,
                    y * level->tileSize + level->offsetY + offset_rot_y,
                    level->spritesheet.frameSize.x * scale,
                    level->spritesheet.frameSize.y * scale};
  Vector2 origin = {0, 0};
  DrawTexturePro(level->spritesheet.texture, source, dest, origin, rotation,
                 color);
}

void render_level(Level *level) {
  float options[] = {0.0f, 90.0f, 180.0f, 270.0f};
  for (int y = 0; y < level->rows; y++) {
    for (int x = 0; x < level->columns; x++) {
      int idx = (x + y) % 4;
      float positional_rotation = options[idx];
      char tile = level->data[y][x];
      Color color = WHITE;
      switch (tile) {
      case '1':
      case '2':
      case '.':
        draw_level_texture(level, 1.0f, false, 1, 0, color, x, y, 0.0f);
        break;
      case '@':
      case 'D':
        DrawTextureForGame(level, level->waterTexture, x, y, 1);
        break;
      case 'O':
        draw_level_texture(level, 1.0f, false, 1, 0, color, x, y, 0.0f);
        DrawTextureForGame(level, level->targetTexture, x, y, 1);
        break;
      case 'H':
        draw_level_texture(level, 1.0f, false, 1, 0, color, x, y, 0.0f);
        DrawTextureForGame(level, level->houseTexture, x, y, 1);
        break;
      case 't':
        draw_level_texture(level, 1.0f, false, 1, 0, color, x, y, 0.0f);
        draw_level_texture(level, 1.0f, false, 4, 0, color, x, y, 0.0f);
        break;
      case 'm':
        draw_level_texture(level, 1.0f, false, 1, 0, color, x, y, 0.0f);
        draw_level_texture(level, 1.0f, false, 5, 0, color, x, y, 0.0f);
        break;
      case 'b':
        draw_level_texture(level, 1.0f, false, 1, 0, color, x, y, 0.0f);
        draw_level_texture(level, 1.0f, false, 6, 0, color, x, y, 0.0f);
        break;
      case '#':
      default:
        draw_level_texture(level, 1.0f, false, 1, 1, color, x, y, 0.0f);
        break;
      }
    }
  }
}
