#include "../include/textures.h"

TextureDef SetTextureDef(const char *name, int startX, int endX, int startY,
                         int endY, char *texturePath) {
  TextureDef tex_def = {0};
  tex_def.name = name;
  tex_def.startX = startX;
  tex_def.endX = endX;
  tex_def.startY = startY;
  tex_def.endY = endY;
  tex_def.texture = LoadTexture(texturePath);
  return tex_def;
}

AnimationFrame SetAnimationFrame(Texture2D texture, int maxFramesW,
                                 int maxFramesH, int startX, int startY) {
  AnimationFrame anim_frame = {0};
  anim_frame.texture = texture;
  anim_frame.frameWidth = texture.width / maxFramesW;
  anim_frame.frameHeight = texture.height / maxFramesH;
  anim_frame.maxFrames = maxFramesW;
  anim_frame.currentFrame = 0;
  anim_frame.framesCount = 0;
  anim_frame.framesSpeed = 8;
  anim_frame.frameRect =
      (Rectangle){startX, startY, (float)anim_frame.frameWidth,
                  (float)anim_frame.frameHeight};
  anim_frame.position = (Vector2){0.0f, 0.0f};
  return anim_frame;
}
