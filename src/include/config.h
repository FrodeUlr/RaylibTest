#include <stdbool.h>

typedef struct Config {
  int screenWidth;
  int screenHeight;
  int targetFPS;
  bool fullscreen;
} Config;

void LoadConfig(Config *config, const char *filename);
void SetDefaultConfig(Config *config);
