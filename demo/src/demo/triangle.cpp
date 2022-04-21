//
// Created by Shiki on 2021/4/8.
//

#include "triangle.h"
#include <plog/Log.h>
#include "../util/asset_manager_utils.h"

Triangle::Triangle(bool is_draw) : BaseShader(is_draw) {

}
Triangle::~Triangle() {
  BaseShader::~BaseShader();
}

bool Triangle::OnCreate() {
  PLOGD << "OnCreate";
  GLfloat v[] = {
	  0.0f, 0.5f, 0.0f,
	  -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
  };
  vertexs_size_ = sizeof(v)/sizeof(v[0]);
  vertexs = new float[vertexs_size_];
  memcpy(vertexs, v, sizeof(v));
  vsh_ = AssetManagerUtils::GetInstance()->read("triangle/vert.glsl");
  fsh_ = AssetManagerUtils::GetInstance()->read("triangle/frag.glsl");
  if (!BaseShader::OnCreate()) {
	return false;
  }
  return true;
}

void Triangle::OnChange(int width, int height) {
  PLOGD << "OnChange";
  BaseShader::OnChange(width, height);
}
void Triangle::OnDraw() {
  PLOGD << "OnDraw";
  BaseShader::OnDraw();
  glUseProgram(shader_program_.program());

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

