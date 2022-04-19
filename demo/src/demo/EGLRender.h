//
// Created by Shiki on 2021/4/15.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_EGLRENDER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_EGLRENDER_H_
#include "../opengl/BaseShader.h"

class EGLRender : public BaseShader {
 public:
  int index;
 public:
  EGLRender(bool isDraw = true);
  virtual ~EGLRender();
  void ResetMatrix() override;
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;
 protected:
  const char *GetVertex() override;
  const char *GetFragment() override;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_EGLRENDER_H_
