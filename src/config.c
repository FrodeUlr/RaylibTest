#include "../include/config.h"
#include <libgen.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char executable_dir_buffer[PATH_MAX];
const char *EXECUTABLE_PATH = NULL;

char *get_absolute_path(const char *relativePath) {
  if (relativePath == NULL) {
    return NULL;
  }
  if (strcmp(relativePath, "") == 0) {
    return "";
  }
  if (strncmp(relativePath, "../", 3) == 0) {
    relativePath += 3;
  }
  static char absolute_path[PATH_MAX];
  if (EXECUTABLE_PATH != NULL) {
    snprintf(absolute_path, sizeof(absolute_path), "%s/%s", EXECUTABLE_PATH,
             relativePath);
  } else {
    snprintf(absolute_path, sizeof(absolute_path), "%s", relativePath);
  }
  printf("Resolved absolute path: %s\n", absolute_path);
  return absolute_path;
}

void get(const char *fileName, char *buffer, size_t bufferSize,
         Config *config) {
  char exe_path[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
  if (len == -1) {
    perror("readlink");
    buffer[0] = '\0';
    return;
  }
  exe_path[len] = '\0';

  char exe_dir[PATH_MAX];
  strncpy(exe_dir, exe_path, sizeof(exe_dir));
  exe_dir[sizeof(exe_dir) - 1] = '\0';

  char *dir = dirname(exe_dir);
  snprintf(buffer, bufferSize, "%s/%s", dir, fileName);
  strncpy(executable_dir_buffer, dir, sizeof(executable_dir_buffer) - 1);
  executable_dir_buffer[sizeof(executable_dir_buffer) - 1] = '\0';
  EXECUTABLE_PATH = executable_dir_buffer;
}

void load(Config *config, const char *fileName) {
  char configPath[PATH_MAX];
  get(fileName, configPath, PATH_MAX, config);
  printf("Executable directory: %s\n", EXECUTABLE_PATH);
  printf("Loading config from: %s\n", configPath);
  FILE *file = fopen(configPath, "r");
  if (!file) {
    printf("Failed to open default config.cfg\n");
    printf("Creating default config at: %s\n", configPath);
    set_default(config);
    file = fopen(configPath, "w");
    if (file) {
      fprintf(file, "screenWidth=%d\n", config->screenWidth);
      fprintf(file, "screenHeight=%d\n", config->screenHeight);
      fprintf(file, "targetFPS=%d\n", config->targetFPS);
      fprintf(file, "fullscreen=%s\n", config->fullscreen ? "true" : "false");
      fclose(file);
    }
    return;
  }
  char line[256];
  while (fgets(line, sizeof(line), file)) {
    char key[50];
    char value[50];
    if (sscanf(line, "%49[^=]=%49s", key, value) == 2) {
      if (strcmp(key, "screenWidth") == 0) {
        config->screenWidth = atoi(value);
      } else if (strcmp(key, "screenHeight") == 0) {
        config->screenHeight = atoi(value);
      } else if (strcmp(key, "targetFPS") == 0) {
        config->targetFPS = atoi(value);
      } else if (strcmp(key, "fullscreen") == 0) {
        config->fullscreen = (strcmp(value, "true") == 0);
      }
    }
  }
}

void set_default(Config *config) {
  config->screenWidth = 800;
  config->screenHeight = 600;
  config->targetFPS = 60;
  config->fullscreen = false;
}
