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
      } else {
        DrawRectangle(x * tile_width, y * tile_height, tile_width, tile_height,
                      color);
      }
    }
  }
}
