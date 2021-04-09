//
// Created by Shiki on 2021/4/8.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TRIANGLE_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TRIANGLE_H_
#include "../opengl/BaseShader.h"
class Triangle : public BaseShader {
 public:
  Triangle();
  virtual ~Triangle();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;
 protected:
  const char *GetVertex() override;
  const char *GetFragment() override;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TRIANGLE_H_
