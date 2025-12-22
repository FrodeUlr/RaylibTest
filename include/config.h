#ifndef CONFIG_H
#define CONFIG_H
#include <stdbool.h>

extern const char *EXECUTABLE_PATH;

typedef struct Config {
  int screenWidth;
  int screenHeight;
  int targetFPS;
  bool fullscreen;
} Config;

void load(Config *config, const char *filename);
void set_default(Config *config);
char *get_absolute_path(const char *relativePath);

#endif
