//
// Created by Shiki on 2021/4/15.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_EGL_RENDER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_EGL_RENDER_H_
#include "../opengl/base_shader.h"

class EGLRender : public BaseShader {
 public:
  int index;
 public:
  EGLRender(bool is_draw = true);
  virtual ~EGLRender();
  void ResetMatrix() override;
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;

};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_EGL_RENDER_H_
