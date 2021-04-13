//
// Created by Shiki on 2021/4/7.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASESHADER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASESHADER_H_
#include <GLES3/gl3.h>
#include <glm/glm.hpp>

class BaseShader {

 protected:
  glm::mat4 matrix;

 protected:
  virtual const char *GetVertex();
  virtual const char *GetFragment();

 public:
  int width;
  int height;
  bool isDraw = true;

  GLuint vertexShader;
  GLuint fragmentShader;

  GLfloat *vertexs = nullptr;
  GLfloat *fragments = nullptr;

  GLuint program = -1;
  GLuint vbo;
  GLuint fbo;

  int vertexsSize = 0;
  int fragmentsSize = 0;

 public:
  BaseShader(bool isDraw = true);

  virtual ~BaseShader();

  virtual void ResetMatrix();

  virtual bool OnCreate();

  virtual void OnChange(int width, int height);

  virtual void OnDraw();

  virtual void Destroy();

};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASESHADER_H_
