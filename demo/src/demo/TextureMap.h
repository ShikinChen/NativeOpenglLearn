//
// Created by Shiki on 2021/4/8.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TEXTURE_MAP_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TEXTURE_MAP_H_
#include "../opengl/BaseShader.h"
#include "../include//ImageDef.h"
#include "ImageDef.h"

class TextureMap : public BaseShader {
 public:
  TextureMap(bool isDraw = true);
  virtual ~TextureMap();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;

  NativeImage *GetImg();

 private:
  GLuint textureId = 0;
  GLuint sampleLoc;
  NativeImage img;

 protected:
  const char *GetVertex() override;
  const char *GetFragment() override;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TEXTURE_MAP_H_
