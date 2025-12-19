#include "../include/levels.h"
#include "../include/raylib.h"
#include <string.h>

Level get_level(int number) {
  Level level;
  const char **level_data = NULL;
  int height = 0;
  switch (number) {
  case 1: {
    static const char *data[] = {
        "####################", "#..............@...#", "#..####............#",
        "#..#..#............#", "#..#..#..O.####....#", "#..####....#..#....#",
        "#..........#..#....#", "#..........####....#", "#..................#",
        "####################",
    };
    height = sizeof(data) / sizeof(data[0]);
    level.data = data;
    level.wall_texture = SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                                       "../sprites/Tiles/FarmLand_Tile.png");
    level.ground_texture = SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                                         "../sprites/Tiles/Grass_Middle.png");
    break;
  }
  case 2: {
    static const char *data[] = {
        "#######################", "#...#.................#",
        "#...#..........###....#", "#...####@@.........####",
        "#........@.O...#...####", "#........@.....#...####",
        "#..###......@.....#...#", "#.................#...#",
        "#..###................#", "#######################",
    };
    height = sizeof(data) / sizeof(data[0]);
    level.data = data;
    level.wall_texture = SetTextureDef("Stone_Tile", 0, 48, 0, 48,
                                       "../sprites/Tiles/Cliff_Tile.png");
    level.ground_texture = SetTextureDef("Ground_Tile", 0, 48, 0, 48,
                                         "../sprites/Tiles/Grass_Middle.png");
    break;
  }
  }
  level.level = number;
  level.completed = false;
  level.width = level.data ? strlen(level.data[0]) : 0;
  level.height = height;
  return level;
}

void DrawTextureForGame(TextureDef tdef, int tile_width, int tile_height,
                        int x_dest, int y_dest) {
  float scale_factor_x = (float)tile_width / tdef.end_x;
  float scale_factor_y = (float)tile_height / tdef.end_y;
  Rectangle source = {tdef.start_x, tdef.start_y, tdef.end_x, tdef.end_y};
  Rectangle dest = {x_dest * tile_width, y_dest * tile_height,
                    tdef.end_x * scale_factor_x, tdef.end_y * scale_factor_y};
  Vector2 origin = {0, 0}; // Top-left corner
  float rotation = 0.0f;
  DrawTexturePro(tdef.texture, source, dest, origin, rotation, WHITE);
}

void render_level(Level level, int screen_width, int screen_height) {
  int tile_width = screen_width / level.width;
  int tile_height = screen_height / level.height;
  TILE_TYPE tile_type;
  for (int y = 0; y < level.height; y++) {
    for (int x = 0; x < level.width; x++) {
      char tile = level.data[y][x];
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
        color = RED;
        tile_type = TARGET;
        break;
      default:
        color = BLACK;
        tile_type = WALL;
        break;
      }
      if (tile_type == TARGET) {
        DrawCircle(
            x * tile_width + tile_width / 2, y * tile_height + tile_height / 2,
            (float)(tile_width < tile_height ? tile_width : tile_height) / 4,
            color);
      } else if (tile_type == WALL) {
        DrawTextureForGame(level.wall_texture, tile_width, tile_height, x, y);
      } else if (tile_type == GROUND) {
        DrawTextureForGame(level.ground_texture, tile_width, tile_height, x, y);
      } else {
        DrawRectangle(x * tile_width, y * tile_height, tile_width, tile_height,
                      color);
      }
    }
  }
}
