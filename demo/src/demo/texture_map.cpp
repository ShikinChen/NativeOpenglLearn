//
// Created by Shiki on 2021/4/8.
//

#include "texture_map.h"
#include <plog/Log.h>
#include <glm/gtc/type_ptr.hpp>
#include "../util/asset_manager_utils.h"

TextureMap::TextureMap(bool isDraw) : BaseShader(isDraw) {
}
TextureMap::~TextureMap() {
  BaseShader::~BaseShader();
}

bool TextureMap::OnCreate() {
  PLOGD << "OnCreate";
  vsh_ = AssetManagerUtils::GetInstance()->read("texture_map/vert.glsl");
  fsh_ = AssetManagerUtils::GetInstance()->read("texture_map/frag.glsl");

  if (!BaseShader::OnCreate()) {
	return false;
  }

  GLfloat v[] = {
	  -1.0f, 0.5f, 0.0f,
	  -1.0f, -0.5f, 0.0f,
	  1.0f, -0.5f, 0.0f,
	  1.0f, 0.5f, 0.0f,
  };
  vertexs_size_ = sizeof(v) / sizeof(v[0]);
  vertexs = new float[vertexs_size_];
  memcpy(vertexs, v, sizeof(v));

  GLfloat f[] = {
	  0.0f, 0.0f,
	  0.0f, 1.0f,
	  1.0f, 1.0f,
	  1.0f, 0.0f,
  };
  fragments_size_ = sizeof(f) / sizeof(f[0]);
  fragments = new float[fragments_size_];
  memcpy(fragments, f, sizeof(f));

  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);

  if (shader_program_.program() > 0) {
	s_texture_map_ = glGetUniformLocation(shader_program_.program(), "s_TextureMap");
	u_matrix = glGetUniformLocation(shader_program_.program(), "u_Matrix");
  }

  return true;
}

void TextureMap::OnChange(int width, int height) {
  PLOGD << "OnChange";
  BaseShader::OnChange(width, height);
}
void TextureMap::OnDraw() {
  PLOGD << "OnDraw";
  BaseShader::OnDraw();
  if (img_.ppPlane[0] && shader_program_.program() > GL_NONE && texture_id_ > GL_NONE) {
	//激活GL_TEXTURE0纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_.width, img_.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img_.ppPlane[0]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glUseProgram(shader_program_.program());

	GLuint v_index = 0;//对应layout的索引
	glVertexAttribPointer(v_index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertexs);
	glEnableVertexAttribArray(v_index);

	GLuint f_index = 1;//对应layout的索引
	glVertexAttribPointer(f_index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), fragments);
	glEnableVertexAttribArray(f_index);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_);

	//v0对应纹理第一层 GL_TEXTURE0
	glUniform1i(s_texture_map_, 0);

	glUniformMatrix4fv(u_matrix, 1, GL_FALSE, value_ptr(matrix));

	GLushort indices[] = {0, 1, 2, 0, 2, 3};
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
  }
}
void TextureMap::Destroy() {
  PLOGD << "Destroy";
  if (shader_program_.program() > 0) {
	glDeleteTextures(1, &texture_id_);
  }
  BaseShader::Destroy();
  NativeImageUtil::FreeNativeImage(&img_);
}
NativeImage *TextureMap::img() {
  return &img_;
}
void TextureMap::ResetMatrix() {
  if (img_.width <= 0 || img_.height <= 0 || width <= 0 || height <= 0) {
	return;
  }
  float sreen_r = width * 1.0 / height;
  float img_r = img_.width * 1.0 / img_.height;

  float lr = 1.0f;
  float tb = 1.0f;
  if (sreen_r > img_r) {//宽度缩放
	lr = width / (height * 1.0 / img_.height * img_.width);
	//这里top和bottom为0.5对应回顶点坐标0.5
	tb = 0.5f;

  } else {//高度缩放
	tb = height / (width * 1.0 / img_.width * img_.height);
	lr = 2.0f;
  }
  matrix = glm::ortho(-lr, lr, -tb, tb);
}


