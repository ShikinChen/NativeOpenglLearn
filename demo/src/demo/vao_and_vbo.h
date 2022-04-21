//
// Created by Shiki on 2021/4/13.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_VAO_AND_VBO_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_VAO_AND_VBO_H_
#include "../opengl/base_shader.h"

class VaoAndVbo : public BaseShader {
 public:
  VaoAndVbo(bool isDraw = true);
  virtual ~VaoAndVbo();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;
 private:
  GLuint vaoId;
  GLuint vboIds[2];
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_VAO_AND_VBO_H_
