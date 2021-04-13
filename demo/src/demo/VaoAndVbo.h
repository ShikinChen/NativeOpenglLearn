//
// Created by Shiki on 2021/4/13.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_VAOANDVBO_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_VAOANDVBO_H_
#include "../opengl/BaseShader.h"

class VaoAndVbo : public BaseShader {
 public:
  VaoAndVbo(bool isDraw = true);
  virtual ~VaoAndVbo();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;
 protected:
  const char *GetVertex() override;
  const char *GetFragment() override;
 private:
  GLuint vaoId;
  GLuint vboIds[2];
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_VAOANDVBO_H_
