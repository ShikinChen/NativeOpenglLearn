//
// Created by Shiki on 2021/4/13.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_NV21_TEXTURE_MAP_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_NV21_TEXTURE_MAP_H_
#include "../opengl/base_shader.h"
#include "../include/ImageDef.h"


class NV21TextureMap : public BaseShader {
 public:
  NV21TextureMap(bool isDraw = true);
  virtual ~NV21TextureMap();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;

  NativeImage *GetImg();

 private:
  GLuint yTextureId = 0;
  GLuint uvTextureId = 0;

  GLuint y_Texture;
  GLuint uv_Texture;
  GLuint u_Matrix;
  NativeImage img;

 protected:
  const char *GetVertex() override;
  const char *GetFragment() override;
 public:
  void ResetMatrix() override;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_NV21_TEXTURE_MAP_H_
