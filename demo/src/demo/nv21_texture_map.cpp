//
// Created by Shiki on 2021/4/13.
//

#include "nv21_texture_map.h"
#include <glm/gtc/type_ptr.hpp>

NV21TextureMap::NV21TextureMap(bool isDraw) : BaseShader(isDraw) {
}
NV21TextureMap::~NV21TextureMap() {
  BaseShader::~BaseShader();
  NativeImageUtil::FreeNativeImage(&img);
}

const char *NV21TextureMap::GetVertex() {
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
const char *NV21TextureMap::GetFragment() {
  return "#version 300 es                              \n"
		 "precision mediump float;                     \n"
		 "in vec2 v_texCoord;  							\n"
		 "layout(location = 0) out vec4 outColor;  		\n"
		 "uniform sampler2D y_Texture;  				\n"
		 "uniform sampler2D uv_Texture;  				\n"
		 "void main()                                  \n"
		 "{                                            \n"
		 "  vec3 yuv;  									\n"
		 "  yuv.x=texture(y_Texture,v_texCoord).r;		\n"
		 "  yuv.y=texture(uv_Texture,v_texCoord).a-0.5;		\n"
		 "  yuv.z=texture(uv_Texture,v_texCoord).r-0.5;		\n"
		 "	highp vec3 rgb = mat3( 1,       1,       	1,	\n"
		 "               0, 		-0.344, 	1.770,		\n"
		 "               1.403,  -0.714,       0) * yuv; 	\n"
		 "   outColor = vec4(rgb, 1);  					\n"
		 "}                                            \n";
}

bool NV21TextureMap::OnCreate() {
  PLOGD << "OnCreate";
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

  if (program > 0) {
	y_Texture = glGetUniformLocation(program, "y_Texture");
	uv_Texture = glGetUniformLocation(program, "uv_Texture");
	u_Matrix = glGetUniformLocation(program, "u_Matrix");
  }
  return true;
}
void NV21TextureMap::OnChange(int width, int height) {
  BaseShader::OnChange(width, height);
}

void NV21TextureMap::OnDraw() {
  BaseShader::OnDraw();
  if (program > GL_NONE && yTextureId > GL_NONE && uvTextureId > GL_NONE && img.ppPlane[0]) {

	glBindTexture(GL_TEXTURE_2D, yTextureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_LUMINANCE,
				 img.width,
				 img.height,
				 0,
				 GL_LUMINANCE,
				 GL_UNSIGNED_BYTE,
				 img.ppPlane[0]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glBindTexture(GL_TEXTURE_2D, uvTextureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_LUMINANCE_ALPHA,
				 img.width >> 1,
				 img.height >> 1,
				 0,
				 GL_LUMINANCE_ALPHA,
				 GL_UNSIGNED_BYTE,
				 img.ppPlane[1]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glUseProgram(program);

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
  if (program > 0) {
	glDeleteTextures(1, &yTextureId);
	glDeleteTextures(1, &uvTextureId);
  }
  BaseShader::Destroy();
  NativeImageUtil::FreeNativeImage(&img);
}
NativeImage *NV21TextureMap::GetImg() {
  return &img;
}
void NV21TextureMap::ResetMatrix() {
  if (img.width <= 0 || img.height <= 0 || width <= 0 || height <= 0) {
	return;
  }
  float sreen_r = width * 1.0 / height;
  float img_r = img.width * 1.0 / img.height;

  float lr = 1.0f;
  float tb = 1.0f;
  if (sreen_r > img_r) {//宽度缩放
	lr = width / (height * 1.0 / img.height * img.width);
	//这里top和bottom为1.0对应回顶点坐标1.0
	tb = 1.0f;

  } else {//高度缩放
	tb = height / (width * 1.0 / img.width * img.height);
	lr = 1.0f;
  }
  matrix = glm::ortho(-lr, lr, -tb, tb);
}

