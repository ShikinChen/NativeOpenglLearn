//
// Created by Shiki on 2021/4/8.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TRIANGLE_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TRIANGLE_H_
#include "../opengl/base_shader.h"
class Triangle : public BaseShader {
 public:
  Triangle(bool is_draw = true);
  virtual ~Triangle();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_TRIANGLE_H_
