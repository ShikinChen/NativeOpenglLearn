//
// Created by Shiki on 2021/4/7.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASE_SHADER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASE_SHADER_H_
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
  bool is_draw_ = true;

  GLuint vertex_shader_;
  GLuint fragment_shader_;

  GLfloat *vertexs;
  GLfloat *fragments;

  GLuint program;
  GLuint vbo;
  GLuint fbo;

  int vertexs_size_;
  int fragments_size_;

 public:
  BaseShader(bool is_draw = true);

  virtual ~BaseShader();

  virtual void ResetMatrix();

  virtual bool OnCreate();

  virtual void OnChange(int width, int height);

  virtual void OnDraw();

  virtual void Destroy();

};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASE_SHADER_H_
