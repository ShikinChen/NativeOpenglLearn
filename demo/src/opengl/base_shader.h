//
// Created by Shiki on 2021/4/7.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASE_SHADER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASE_SHADER_H_
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <map>
#include "shader_program.h"
#include <vector>

class BaseShader {

 public:
  BaseShader(bool is_draw = true);

  virtual ~BaseShader();

  virtual void ResetMatrix();

  virtual bool OnCreate();

  virtual void OnChange(int width, int height);

  virtual void OnDraw();

  virtual void Destroy();

 public:
  int width;
  int height;
  bool is_draw_ = true;

//  GLuint vertex_shader_;
//  GLuint fragment_shader_;

  GLfloat *vertexs;
  GLfloat *fragments;

//  GLuint program;
  GLuint vbo;
  GLuint fbo;

  int vertexs_size_;
  int fragments_size_;

 protected:
  virtual const std::string GetVertexSh() const;
  virtual const std::string GetFragmentSh() const;

  virtual void CreateShaderProgram(const std::vector<std::string> &uniform_list = {},
								   const std::vector<std::string> &attrib_list = {});

 protected:
  glm::mat4 matrix;
  ShaderProgram shader_program_;
  std::string vsh_;
  std::string fsh_;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_BASE_SHADER_H_
