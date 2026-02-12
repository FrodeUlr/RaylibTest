#ifndef TEXTURES_H
#define TEXTURES_H
#include "raylib.h"

typedef struct TextureDef {
  const char *name;
  int startX;
  int endX;
  int startY;
  int endY;
  Texture2D texture;
} TextureDef;

typedef struct AnimationFrame {
  int frameWidth;
  int frameHeight;
  int maxFrames;
  int currentFrame;
  int framesCount;
  int framesSpeed;
  Rectangle frameRect;
  Vector2 position;
  Texture2D texture;
} AnimationFrame;

typedef struct SpriteDef {
  int currentColumn;
  int rows;
  int columns;
  Vector2 frameSize;
  Texture2D texture;
} SpriteDef;

TextureDef SetTextureDef(const char *name, int startX, int endX, int startY,
                         int endY, char *texturePath);
AnimationFrame SetAnimationFrame(Texture2D texture, int maxFramesW,
                                 int maxFramesH, int startX, int startY);

#endif
