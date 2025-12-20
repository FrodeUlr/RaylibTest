#include "../include/level.h"
#include "../include/raylib.h"
#include <string.h>

void set_offset(Level *level) {
  int screen_width = GetScreenWidth();
  int screen_height = GetScreenHeight();
  int tile_w = screen_width / level->columns;
  int tile_h = screen_height / level->rows;
  level->tile_size = tile_w < tile_h ? tile_w : tile_h;

  int level_pixel_width = level->tile_size * level->columns;
  int level_pixel_height = level->tile_size * level->rows;
  level->offset_x = (screen_width - level_pixel_width) / 2;
  level->offset_y = (screen_height - level_pixel_height) / 2;
}

void set_level(Level *level, int number) {
  const char **level_data = NULL;
  level->target_texture = SetTextureDef(
      "Target_Tile", 0, 16, 0, 16, "../sprites/Outdoor decoration/Chest.png");
  level->house_texture =
      SetTextureDef("House_Tile", 0, 96, 0, 128,
                    "../sprites/Outdoor decoration/House_1_Wood_Base_Blue.png");
  level->water_texture = SetTextureDef("Water_Tile", 0, 48, 0, 48,
                                       "../sprites/Tiles/Water_Middle.png");
  int height = 0;
  switch (number) {
  case 1: {
    static const char *data[] = {
        "######################################",
        "#....................................#",
        "#.....#..............................#",
        "#.....#..............................#",
        "#.....#.......................2......#",
        "#.....#..............................#",
        "#..1.................................#",
        "#...............O....................#",
        "#....................................#",
        "#........@...........................#",
        "#....................................#",
        "#.....####...........................#",
        "#....................................#",
        "#....................................#",
        "######################################",
    };

    height = sizeof(data) / sizeof(data[0]);
    level->data = data;
    level->wall_texture = SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                                        "../sprites/Tiles/FarmLand_Tile.png");
    level->ground_texture = SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                                          "../sprites/Tiles/Grass_Middle.png");
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
        "#...................O....................#",
        "#........................................#",
        "#........................................#",
        "#.....1..................................#",
        "#........................................#",
        "#........................................#",
        "##########################################",
    };
    height = sizeof(data) / sizeof(data[0]);
    level->data = data;
    level->wall_texture = SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                                        "../sprites/Tiles/Cliff_Tile.png");
    level->ground_texture = SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                                          "../sprites/Tiles/Path_Middle.png");
    level->first_frame = true;
    break;
  }
  }
  level->number = number;
  level->completed = false;
  level->columns = level->data ? strlen(level->data[0]) : 0;
  level->rows = height;
  set_offset(level);
}

void DrawTextureForGame(Level *level, TextureDef tdef, int x_dest, int y_dest,
                        float custom_scale) {
  float scale = level->tile_size / (float)tdef.end_x * custom_scale;
  Rectangle source = {tdef.start_x, tdef.start_y, tdef.end_x, tdef.end_y};
  Rectangle dest = {x_dest * level->tile_size + level->offset_x,
                    y_dest * level->tile_size + level->offset_y,
                    tdef.end_x * scale, tdef.end_y * scale};
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
        DrawTextureForGame(level, level->ground_texture, x, y, 1);
        DrawTextureForGame(level, level->target_texture, x, y, 1);
      } else if (tile_type == WALL) {
        DrawTextureForGame(level, level->wall_texture, x, y, 1);
      } else if (tile_type == GROUND) {
        DrawTextureForGame(level, level->ground_texture, x, y, 1);
      } else if (tile_type == PLAYER) {
        DrawTextureForGame(level, level->ground_texture, x, y, 1);
      } else if (tile_type == WATER) {
        DrawTextureForGame(level, level->water_texture, x, y, 1);
      } else if (tile_type == HOUSE) {
        DrawTextureForGame(level, level->ground_texture, x, y, 1);
        DrawTextureForGame(level, level->house_texture, x, y, 1);
      } else {
        DrawTextureForGame(level, level->ground_texture, x, y, 1);
      }
    }
  }
}
