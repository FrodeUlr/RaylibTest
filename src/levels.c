#include "../include/levels.h"
#include "../include/raylib.h"
#include <stdio.h>
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
    level_data = data;
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
    level_data = data;
    break;
  }
  }
  level.level = number;
  level.data = level_data;
  level.completed = false;
  level.width = level_data ? strlen(level_data[0]) : 0;
  level.height = height;
  level.wall_texture = LoadTexture("../sprites/Tiles/FarmLand_Tile.png");
  printf("Loaded wall texture with dimensions: %d x %d\n",
         level.wall_texture.width, level.wall_texture.height);
  return level;
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
        float scale_factor_x = (float)tile_width / level.wall_texture.width;
        float scale_factor_y = (float)tile_height / level.wall_texture.height;
        Rectangle source = {0, 0, level.wall_texture.width,
                            level.wall_texture.height};
        Rectangle dest = {x * tile_width, y * tile_height,
                          level.wall_texture.width * scale_factor_x,
                          level.wall_texture.height * scale_factor_y};
        Vector2 origin = {0, 0}; // Top-left corner
        float rotation = 0.0f;
        DrawTexturePro(level.wall_texture, source, dest, origin, rotation,
                       WHITE);
      } else {
        DrawRectangle(x * tile_width, y * tile_height, tile_width, tile_height,
                      color);
      }
    }
  }
}
