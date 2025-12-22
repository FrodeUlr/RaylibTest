#include "../include/level.h"
#include "../include/config.h"
#include "../include/raylib.h"
#include <linux/limits.h>
#include <string.h>

void set_offset(Level *level) {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  int tile_w = screen_width / level->columns;
  int tile_h = screen_height / level->rows;
  level->tileSize = tile_w < tile_h ? tile_w : tile_h;

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
  int height = 0;
  switch (number) {
  case 1: {
    static const char *data[] = {
        "##########################################",
        "#........................................#",
        "#.....#..................................#",
        "#.....#..................................#",
        "#.....#...........................2......#",
        "#.....#..................................#",
        "#..1.....................................#",
        "#...............O........................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........@...............................#",
        "#........................................#",
        "#.....####...............................#",
        "#........................................#",
        "#........................................#",
        "##########################################",
    };

    height = sizeof(data) / sizeof(data[0]);
    level->data = data;
    level->wallTexture =
        SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                      get_absolute_path("../art/Tiles/FarmLand_Tile.png"));
    level->groundTexture =
        SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                      get_absolute_path("../art/Tiles/Grass_Middle.png"));
    break;
  }
  case 2: {
    static const char *data[] = {
        "##########################################",
        "#........................................#",
        "#........................................#",
        "#..................................2.....#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#........................................#",
        "#...................O..........#.........#",
        "#..............................##........#",
        "#.############.................#.........#",
        "#.....1........................#.........#",
        "#..............................#.........#",
        "#........................................#",
        "##########################################",
    };
    height = sizeof(data) / sizeof(data[0]);
    level->data = data;
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
  level->rows = height;
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

void render_level(Level *level) {
  TILE_TYPE tile_type;
  for (int y = 0; y < level->rows; y++) {
    for (int x = 0; x < level->columns; x++) {
      char tile = level->data[y][x];
      Color color;
      switch (tile) {
      case '#':
        color = DARKGRAY;
        tile_type = WALL;
        break;
      case '.':
        color = LIGHTGRAY;
        tile_type = GROUND;
        break;
      case '@':
        color = BLUE;
        tile_type = WATER;
        break;
      case 'O':
        color = GREEN;
        tile_type = TARGET;
        break;
      case 'H':
        color = YELLOW;
        tile_type = HOUSE;
        break;
      case '1':
      case '2':
        color = RED;
        tile_type = PLAYER;
        break;
      default:
        color = BLACK;
        tile_type = WALL;
        break;
      }
      if (tile_type == TARGET) {
        DrawTextureForGame(level, level->groundTexture, x, y, 1);
        DrawTextureForGame(level, level->targetTexture, x, y, 1);
      } else if (tile_type == WALL) {
        DrawTextureForGame(level, level->wallTexture, x, y, 1);
      } else if (tile_type == GROUND) {
        DrawTextureForGame(level, level->groundTexture, x, y, 1);
      } else if (tile_type == PLAYER) {
        DrawTextureForGame(level, level->groundTexture, x, y, 1);
      } else if (tile_type == WATER) {
        DrawTextureForGame(level, level->waterTexture, x, y, 1);
      } else if (tile_type == HOUSE) {
        DrawTextureForGame(level, level->groundTexture, x, y, 1);
        DrawTextureForGame(level, level->houseTexture, x, y, 1);
      } else {
        DrawTextureForGame(level, level->groundTexture, x, y, 1);
      }
    }
  }
}
