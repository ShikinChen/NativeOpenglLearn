//
// Created by Shiki on 2021/4/7.
//

#include "BaseShader.h"
#include "../util/ShaderUtils.h"
BaseShader::BaseShader() {

}
BaseShader::~BaseShader() {
  if (vertexs) {
	delete[] vertexs;
  }
  if (fragments) {
	delete[] fragments;
  }
}
bool BaseShader::OnCreate() {
  program = ShaderUtils::CreateProgram(GetVertex(), GetFragment(), &vertexShader, &fragmentShader);
  return program > 0;
}
void BaseShader::OnChange(int width, int height) {
  this->height = height;
  this->width = width;
  glViewport(0, 0, width, height);
}
void BaseShader::OnDraw() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void BaseShader::Destroy() {
  if (program > 0) {
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(program);
  }
}
const char *BaseShader::GetVertex() {
  return nullptr;
}
const char *BaseShader::GetFragment() {
  return nullptr;
}
