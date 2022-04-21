//
// Created by Shiki on 2021/4/13.
//

#include "nv21_texture_map.h"
#include <glm/gtc/type_ptr.hpp>
#include "../util/asset_manager_utils.h"

NV21TextureMap::NV21TextureMap(bool isDraw) : BaseShader(isDraw) {
}
NV21TextureMap::~NV21TextureMap() {
  BaseShader::~BaseShader();
  NativeImageUtil::FreeNativeImage(&img_);
}

bool NV21TextureMap::OnCreate() {
  PLOGD << "OnCreate";
  vsh_ = AssetManagerUtils::GetInstance()->read("nv21_texture_map/vert.glsl");
  fsh_ = AssetManagerUtils::GetInstance()->read("nv21_texture_map/frag.glsl");

  if (!BaseShader::OnCreate()) {
	return false;
  }
  GLfloat v[] = {
	  -1.0f, 1.0f, 0.0f,
	  -1.0f, -1.0f, 0.0f,
	  1.0f, -1.0f, 0.0f,
	  1.0f, 1.0f, 0.0f,
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

  GLuint textureIds[2] = {0};
  glGenTextures(2, textureIds);

  yTextureId = textureIds[0];
  uvTextureId = textureIds[1];

  glBindTexture(GL_TEXTURE_2D, yTextureId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);

  glBindTexture(GL_TEXTURE_2D, uvTextureId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);

  if (shader_program_.program() > 0) {
	y_Texture = glGetUniformLocation(shader_program_.program(), "y_Texture");
	uv_Texture = glGetUniformLocation(shader_program_.program(), "uv_Texture");
	u_Matrix = glGetUniformLocation(shader_program_.program(), "u_matrix");
  }
  return true;
}
void NV21TextureMap::OnChange(int width, int height) {
  BaseShader::OnChange(width, height);
}

void NV21TextureMap::OnDraw() {
  BaseShader::OnDraw();
  if (shader_program_.program() > GL_NONE && yTextureId > GL_NONE && uvTextureId > GL_NONE && img_.ppPlane[0]) {

	glBindTexture(GL_TEXTURE_2D, yTextureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_LUMINANCE,
				 img_.width,
				 img_.height,
				 0,
				 GL_LUMINANCE,
				 GL_UNSIGNED_BYTE,
				 img_.ppPlane[0]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glBindTexture(GL_TEXTURE_2D, uvTextureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_LUMINANCE_ALPHA,
				 img_.width >> 1,
				 img_.height >> 1,
				 0,
				 GL_LUMINANCE_ALPHA,
				 GL_UNSIGNED_BYTE,
				 img_.ppPlane[1]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glUseProgram(shader_program_.program());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, yTextureId);
	glUniform1i(y_Texture, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, uvTextureId);
	glUniform1i(uv_Texture, 1);

	GLuint v_index = 0;//对应layout的索引
	glVertexAttribPointer(v_index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertexs);
	glEnableVertexAttribArray(v_index);

	GLuint f_index = 1;//对应layout的索引
	glVertexAttribPointer(f_index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), fragments);
	glEnableVertexAttribArray(f_index);

	glUniformMatrix4fv(u_Matrix, 1, GL_FALSE, value_ptr(matrix));

	GLushort indices[] = {0, 1, 2, 0, 2, 3};
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
  }
}
void NV21TextureMap::Destroy() {
  PLOGD << "Destroy";
  if (shader_program_.program() > 0) {
	glDeleteTextures(1, &yTextureId);
	glDeleteTextures(1, &uvTextureId);
  }
  BaseShader::Destroy();
  NativeImageUtil::FreeNativeImage(&img_);
}
NativeImage *NV21TextureMap::img() {
  return &img_;
}
void NV21TextureMap::ResetMatrix() {
  if (img_.width <= 0 || img_.height <= 0 || width <= 0 || height <= 0) {
	return;
  }
  float sreen_r = width * 1.0 / height;
  float img_r = img_.width * 1.0 / img_.height;

  float lr = 1.0f;
  float tb = 1.0f;
  if (sreen_r > img_r) {//宽度缩放
	lr = width / (height * 1.0 / img_.height * img_.width);
	//这里top和bottom为1.0对应回顶点坐标1.0
	tb = 1.0f;

  } else {//高度缩放
	tb = height / (width * 1.0 / img_.width * img_.height);
	lr = 1.0f;
  }
  matrix = glm::ortho(-lr, lr, -tb, tb);
}

