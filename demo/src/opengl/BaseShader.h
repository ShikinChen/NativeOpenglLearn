//
// Created by Shiki on 2021/4/7.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASESHADER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASESHADER_H_
#include <GLES3/gl3.h>

class BaseShader {
 public:
  int width;
  int height;

  GLuint vertexShader;
  GLuint fragmentShader;
  GLuint textureId;

  float *vertexs = nullptr;
  float *fragments = nullptr;

  GLuint program = -1;
  GLuint vbo;
  GLuint fbo;

  int vertexsSize = 0;
  int fragmentsSize = 0;

 public:
  BaseShader();

  virtual ~BaseShader();

  virtual bool OnCreate();

  virtual void OnChange(int width, int height);

  virtual void OnDraw();

  virtual void Destroy();

 protected:
  virtual const char *GetVertex();
  virtual const char *GetFragment();
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASESHADER_H_