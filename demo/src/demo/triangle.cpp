//
// Created by Shiki on 2021/4/8.
//

#include "triangle.h"
#include <plog/Log.h>
Triangle::Triangle(bool isDraw) : BaseShader(isDraw) {

}
Triangle::~Triangle() {
  BaseShader::~BaseShader();
}
const char *Triangle::GetVertex() {
  return "#version 300 es                          \n"
		 "layout(location = 1) in vec4 vPosition;  \n"
		 "void main()                              \n"
		 "{                                        \n"
		 "   gl_Position = vPosition;              \n"
		 "}                                        \n";
}
const char *Triangle::GetFragment() {
  return "#version 300 es                              \n"
		 "precision mediump float;                     \n"
		 "out vec4 fragColor;                          \n"
		 "void main()                                  \n"
		 "{                                            \n"
		 "   fragColor = vec4 ( 1.0, 1.0, 0.0, 1.0 );  \n"
		 "}                                            \n";
}
bool Triangle::OnCreate() {
  PLOGD << "OnCreate";
  if (!BaseShader::OnCreate()) {
	return false;
  }
  GLfloat v[] = {
	  0.0f, 0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
  };
  vertexs_size_ = sizeof(v) / sizeof(v[0]);
  vertexs = new float[vertexs_size_];
  memcpy(vertexs, v, sizeof(v));
  return true;
}

void Triangle::OnChange(int width, int height) {
  PLOGD << "OnChange";
  BaseShader::OnChange(width, height);
}
void Triangle::OnDraw() {
  PLOGD << "OnDraw";
  BaseShader::OnDraw();
  glUseProgram(program);

  GLuint index = 1;//对应layout的索引
  glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, vertexs);
  glEnableVertexAttribArray(index);

  glDrawArrays(GL_TRIANGLES, 0, 3);
  glUseProgram(GL_NONE);
}
void Triangle::Destroy() {
  PLOGD << "Destroy";
  BaseShader::Destroy();
}

