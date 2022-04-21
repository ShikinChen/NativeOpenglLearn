//
// Created by Shiki on 2021/4/13.
//

#include "vao_and_vbo.h"
#include <string>
#include <plog/Log.h>
#include "../util/asset_manager_utils.h"

#define VERTEX_POS_SIZE       3
#define VERTEX_COLOR_SIZE     4
#define VERTEX_STRIDE         (sizeof(GLfloat)*(VERTEX_POS_SIZE+VERTEX_COLOR_SIZE))

VaoAndVbo::VaoAndVbo(bool isDraw) : BaseShader(isDraw) {

}
VaoAndVbo::~VaoAndVbo() {

}

bool VaoAndVbo::OnCreate() {
  vsh_ = AssetManagerUtils::GetInstance()->read("vao_and_vbo/vert.glsl");
  fsh_ = AssetManagerUtils::GetInstance()->read("vao_and_vbo/frag.glsl");
//  PLOGD<<"vsh_:"<<vsh_;
//  PLOGD<<"fsh_:"<<fsh_;
  if (!BaseShader::OnCreate()) {
	return false;
  }
  GLfloat v[4 * (VERTEX_POS_SIZE + VERTEX_COLOR_SIZE)] = {
	  -0.5f, 0.5f, 0.0f,
	  1.0f, 0.0f, 0.0f, 1.0f,
	  -0.5f, -0.5f, 0.0f,
	  0.0f, 1.0f, 0.0f, 1.0f,
	  0.5f, -0.5f, 0.0f,
	  0.0f, 0.0f, 1.0f, 1.0f,
	  0.5f, 0.5f, 0.0f,
	  0.5f, 1.0f, 1.0f, 1.0f,
  };
  vertexs_size_ = sizeof(v) / sizeof(v[0]);
  vertexs = new float[vertexs_size_];
  memcpy(vertexs, v, sizeof(v));

  glGenBuffers(2, vboIds);
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  //vertexs只是指针不足有实际数组长度和数组内存大小
  glBufferData(GL_ARRAY_BUFFER, vertexs_size_ * sizeof(GLfloat), vertexs, GL_STATIC_DRAW);

  GLushort indices[] = {0, 1, 2, 0, 2, 3};
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vaoId);
  //绑定vao记录vbo相关操作
  glBindVertexArray(vaoId);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);

  int vertexPosIndex = 0;
  int vertexColorIndex = 1;

  glEnableVertexAttribArray(vertexPosIndex);
  glVertexAttribPointer(vertexPosIndex,
						VERTEX_POS_SIZE,
						GL_FLOAT,
						GL_FALSE,
						VERTEX_STRIDE,
						reinterpret_cast<const void *>(0));

  glEnableVertexAttribArray(vertexColorIndex);
  glVertexAttribPointer(vertexColorIndex,
						VERTEX_COLOR_SIZE,
						GL_FLOAT,
						GL_FALSE,
						VERTEX_STRIDE,
						reinterpret_cast<const void *>(VERTEX_POS_SIZE * sizeof(GLfloat)));

  glBindVertexArray(GL_NONE);

  //解除vbo缓存绑定
  glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
  return true;
}
void VaoAndVbo::OnChange(int width, int height) {
  BaseShader::OnChange(width, height);
}
void VaoAndVbo::OnDraw() {
  if (shader_program_.program() > GL_NONE) {
	glUseProgram(shader_program_.program());
	//使用vao
	glBindVertexArray(vaoId);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<const void *>(0));
	glBindVertexArray(GL_NONE);
  }
}
void VaoAndVbo::Destroy() {
  if (shader_program_.program() > GL_NONE) {
	glDeleteBuffers(2, vboIds);
	glDeleteVertexArrays(1, &vaoId);
	vaoId = GL_NONE;
  }
  BaseShader::Destroy();
}

