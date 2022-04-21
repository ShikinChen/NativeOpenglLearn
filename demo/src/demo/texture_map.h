//
// Created by Shiki on 2021/4/8.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TEXTURE_MAP_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TEXTURE_MAP_H_
#include "../opengl/base_shader.h"
#include "../include//ImageDef.h"
#include "ImageDef.h"
#include <glm/glm.hpp>

class TextureMap : public BaseShader {
 public:
  TextureMap(bool isDraw = true);
  virtual ~TextureMap();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;

  NativeImage *img();

 private:
  GLuint texture_id_ = 0;
  GLuint s_texture_map_;
  GLuint u_matrix;
  NativeImage img_;

 public:
  void ResetMatrix() override;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TEXTURE_MAP_H_
