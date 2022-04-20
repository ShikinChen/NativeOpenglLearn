//
// Created by Shiki on 2021/4/8.
//

#include "texture_map.h"
#include <plog/Log.h>
#include <glm/gtc/type_ptr.hpp>

TextureMap::TextureMap(bool isDraw) : BaseShader(isDraw) {
}
TextureMap::~TextureMap() {
  BaseShader::~BaseShader();
  NativeImageUtil::FreeNativeImage(&img);
}
const char *TextureMap::GetVertex() {
  return "#version 300 es                          \n"
		 "layout(location = 0) in vec4 v_Position;  \n"
		 "layout(location = 1) in vec2 f_Position;  \n"
		 "out vec2 v_texCoord;  					\n"
		 "uniform mat4 u_Matrix;  				    \n"
		 "void main()                              \n"
		 "{                                        \n"
		 "   gl_Position = v_Position*u_Matrix;   \n"
		 "   v_texCoord = f_Position;              \n"
		 "}                                        \n";
}
const char *TextureMap::GetFragment() {
  return "#version 300 es                              \n"
		 "precision mediump float;                     \n"
		 "in vec2 v_texCoord;  							\n"
		 "layout(location = 0) out vec4 outColor;  		\n"
		 "uniform sampler2D s_TextureMap;  				\n"
		 "void main()                                  \n"
		 "{                                            \n"
		 "   outColor = texture(s_TextureMap, v_texCoord);  \n"
		 "}                                            \n";
}
bool TextureMap::OnCreate() {
  PLOGD << "OnCreate";
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

  glGenTextures(1, &textureId);
  glBindTexture(GL_TEXTURE_2D, textureId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);

  if (program > 0) {
	s_TextureMap = glGetUniformLocation(program, "s_TextureMap");
	u_Matrix = glGetUniformLocation(program, "u_Matrix");
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
  if (img.ppPlane[0] && program > GL_NONE && textureId > GL_NONE) {
	//激活GL_TEXTURE0纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.ppPlane[0]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glUseProgram(program);

	GLuint v_index = 0;//对应layout的索引
	glVertexAttribPointer(v_index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vertexs);
	glEnableVertexAttribArray(v_index);

	GLuint f_index = 1;//对应layout的索引
	glVertexAttribPointer(f_index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), fragments);
	glEnableVertexAttribArray(f_index);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);

	//v0对应纹理第一层 GL_TEXTURE0
	glUniform1i(s_TextureMap, 0);

	glUniformMatrix4fv(u_Matrix, 1, GL_FALSE, value_ptr(matrix));

	GLushort indices[] = {0, 1, 2, 0, 2, 3};
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
  }
}
void TextureMap::Destroy() {
  PLOGD << "Destroy";
  if (program > 0) {
	glDeleteTextures(1, &textureId);
  }
  BaseShader::Destroy();
  NativeImageUtil::FreeNativeImage(&img);
}
NativeImage *TextureMap::GetImg() {
  return &img;
}
void TextureMap::ResetMatrix() {
  if (img.width <= 0 || img.height <= 0 || width <= 0 || height <= 0) {
	return;
  }
  float sreen_r = width * 1.0 / height;
  float img_r = img.width * 1.0 / img.height;

  float lr = 1.0f;
  float tb = 1.0f;
  if (sreen_r > img_r) {//宽度缩放
	lr = width / (height * 1.0 / img.height * img.width);
	//这里top和bottom为0.5对应回顶点坐标0.5
	tb = 0.5f;

  } else {//高度缩放
	tb = height / (width * 1.0 / img.width * img.height);
	lr = 2.0f;
  }
  matrix = glm::ortho(-lr, lr, -tb, tb);
}


