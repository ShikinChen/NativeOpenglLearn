//
// Created by Shiki on 2021/4/7.
//

#include "BaseShader.h"
#include "../util/ShaderUtils.h"
BaseShader::BaseShader(bool isDraw) {
  this->isDraw = isDraw;
  program = GL_NONE;
  vertexs = nullptr;
  fragments = nullptr;
  vertexsSize = 0;
  fragmentsSize = 0;
}
BaseShader::~BaseShader() {
}
bool BaseShader::OnCreate() {
  program = ShaderUtils::CreateProgram(GetVertex(), GetFragment(), &vertexShader, &fragmentShader);
  return program > GL_NONE;
}
void BaseShader::OnChange(int width, int height) {
  this->height = height;
  this->width = width;
  glViewport(0, 0, width, height);
  ResetMatrix();
}
void BaseShader::OnDraw() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void BaseShader::Destroy() {
  if (vertexs) {
    delete[] vertexs;
    vertexs = nullptr;
  }
  if (fragments) {
    delete[] fragments;
    fragments = nullptr;
  }
  if (program > GL_NONE) {
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteProgram(program);
    program = GL_NONE;
  }
}
const char *BaseShader::GetVertex() {
  return nullptr;
}
const char *BaseShader::GetFragment() {
  return nullptr;
}
void BaseShader::ResetMatrix() {

}

