//
// Created by Shiki on 2021/4/13.
//

#include "FboOffscreenRendering.h"
#include <glm/gtc/type_ptr.hpp>
#include "../util/ShaderUtils.h"
#include "../include/ImageDef.h"

FboOffscreenRendering::FboOffscreenRendering(bool isDraw) : BaseShader(isDraw) {
  fbo_program = GL_NONE;
}
FboOffscreenRendering::~FboOffscreenRendering() {
  NativeImageUtil::FreeNativeImage(&img);
}
const char *FboOffscreenRendering::GetVertex() {
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
const char *FboOffscreenRendering::GetFragment() {
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

bool FboOffscreenRendering::OnCreate() {
  if (!BaseShader::OnCreate()) {
	return false;
  }

  GLfloat v[] = {
	  -1.0f, -1.0f, 0.0f,
	  1.0f, -1.0f, 0.0f,
	  -1.0f, 1.0f, 0.0f,
	  1.0f, 1.0f, 0.0f,
  };
  vertexsSize = sizeof(v) / sizeof(v[0]);
  vertexs = new float[vertexsSize];
  memcpy(vertexs, v, sizeof(v));

  GLfloat f[] = {
	  0.0f, 1.0f,
	  1.0f, 1.0f,
	  0.0f, 0.0f,
	  1.0f, 0.0f,
  };
  fragmentsSize = sizeof(f) / sizeof(f[0]);
  fragments = new float[fragmentsSize];
  memcpy(fragments, f, sizeof(f));

//fbo 纹理坐标与正常纹理方向不同，原点位于左下角
  GLfloat fbo_f[] = {
	  0.0f, 0.0f,
	  1.0f, 0.0f,
	  0.0f, 1.0f,
	  1.0f, 1.0f,
  };

  const char *fboFragment = "#version 300 es                              \n"
							"precision mediump float;                     \n"
							"in vec2 v_texCoord;  							\n"
							"layout(location = 0) out vec4 outColor;  		\n"
							"uniform sampler2D s_TextureMap;  				\n"
							"void main()                                  \n"
							"{                                            \n"
							"  vec4 tmp = texture(s_TextureMap, v_texCoord);  \n"
							"  float luminance = tmp.r * 0.299 + tmp.g * 0.587 + tmp.b * 0.114;  \n"
							"   outColor = vec4(vec3(luminance),tmp.a);  \n"
							"}                                            \n";

  fbo_program = ShaderUtils::CreateProgram(GetVertex(), fboFragment, &fbo_vertexShader, &fbo_fragmentShader);
  if (fbo_program <= GL_NONE) {
	PLOGE << "离屏program创建失败";
	return false;
  }
  s_TextureMap = glGetUniformLocation(program, "s_TextureMap");
  fbo_s_TextureMap = glGetUniformLocation(fbo_program, "s_TextureMap");
  u_Matrix = glGetUniformLocation(program, "u_Matrix");
  fbo_u_Matrix = glGetUniformLocation(fbo_program, "u_Matrix");

  glGenBuffers(4, vboIds);
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glBufferData(GL_ARRAY_BUFFER, vertexsSize * sizeof(GLfloat), vertexs, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glBufferData(GL_ARRAY_BUFFER, fragmentsSize * sizeof(GLfloat), fragments, GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(fbo_f), fbo_f, GL_STATIC_DRAW);

  GLushort indices[] = {0, 1, 2, 1, 3, 2};
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[3]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glGenVertexArrays(2, vaoIds);

  glBindVertexArray(vaoIds[0]);

  int vertexPosIndex = 0;
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glEnableVertexAttribArray(vertexPosIndex);
  glVertexAttribPointer(vertexPosIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), reinterpret_cast<const void *>(0));
  glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

  int texturePosIndex = 1;
  glBindBuffer(GL_ARRAY_BUFFER, vboIds[1]);
  glEnableVertexAttribArray(texturePosIndex);
  glVertexAttribPointer(texturePosIndex,
						2,
						GL_FLOAT,
						GL_FALSE,
						2 * sizeof(GL_FLOAT),
						reinterpret_cast<const void *>(0));
  glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[3]);
  glBindVertexArray(GL_NONE);


  //离屏vao
  glBindVertexArray(vaoIds[1]);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
  glEnableVertexAttribArray(vertexPosIndex);
  glVertexAttribPointer(vertexPosIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), reinterpret_cast<const void *>(0));
  glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

  glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
  glEnableVertexAttribArray(texturePosIndex);
  glVertexAttribPointer(texturePosIndex,
						2,
						GL_FLOAT,
						GL_FALSE,
						2 * sizeof(GL_FLOAT),
						reinterpret_cast<const void *>(0));
  glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[3]);
  glBindVertexArray(GL_NONE);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);

  glGenTextures(1, &imgTextureId);
  glBindTexture(GL_TEXTURE_2D, imgTextureId);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, GL_NONE);

  if (!CreateFrameBufferObj()) {
	return false;
  }
  return true;
}
void FboOffscreenRendering::OnChange(int width, int height) {

  BaseShader::OnChange(width, height);
}
void FboOffscreenRendering::OnDraw() {
  BaseShader::OnDraw();
  if (program > GL_NONE && imgTextureId > GL_NONE && img.ppPlane[0]) {
	//屏幕渲染
	glViewport(0, 0, width, height);
	glUseProgram(program);
	glBindVertexArray(vaoIds[0]);
	glActiveTexture(GL_TEXTURE0);
	//绑定离屏纹理并且渲染到屏幕
	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glUniform1i(s_TextureMap, 0);
	glUniformMatrix4fv(u_Matrix, 1, GL_FALSE, value_ptr(matrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<const void *>(0));
	//等绘制完成才取消纹理绑定
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glBindVertexArray(GL_NONE);
  }
}
void FboOffscreenRendering::Destroy() {
  if (fbo_program) {
	glDeleteProgram(fbo_program);
  }
  if (imgTextureId) {
	glDeleteTextures(1, &imgTextureId);
  }
  if (fboTextureId) {
	glDeleteTextures(1, &fboTextureId);
  }
  if (vboIds[0]) {
	glDeleteBuffers(sizeof(vboIds) / sizeof(vboIds[0]), vboIds);
  }
  if (vaoIds[0]) {
	glDeleteBuffers(sizeof(vaoIds) / sizeof(vaoIds[0]), vaoIds);
  }
  if (fboId) {
	glDeleteFramebuffers(1, &fboId);
  }
  BaseShader::Destroy();
}
void FboOffscreenRendering::ResetMatrix() {
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
	tb = 1.0f;

  } else {//高度缩放
	tb = height / (width * 1.0 / img.width * img.height);
	lr = 1.0f;
  }
  matrix = glm::ortho(-lr, lr, -tb, tb);
}
NativeImage *FboOffscreenRendering::GetImg() {
  return &img;
}
bool FboOffscreenRendering::CreateFrameBufferObj() {
  if (program > GL_NONE) {
	glBindTexture(GL_TEXTURE_2D, imgTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.ppPlane[0]);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
  }
  if (img.width > 0 && img.height > 0 && fbo_program > GL_NONE) {
	glGenTextures(1, &fboTextureId);
	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);

	glGenFramebuffers(1, &fboId);
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	//将fbo绑定离屏纹理
	glBindTexture(GL_TEXTURE_2D, fboTextureId);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTextureId, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
	  PLOGE << "创建离屏纹理失败";
	  return false;
	}

	//离屏渲染
	//glPixelStorei(GL_UNPACK_ALIGNMENT,1)控制的是所读取数据的对齐方式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glViewport(0, 0, img.width, img.height);
	//绑定离屏缓存fbo 记录离屏操作
	glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	glUseProgram(fbo_program);
	glBindVertexArray(vaoIds[1]);
	glActiveTexture(GL_TEXTURE0);
	//将图片纹理绑定到离屏的program的纹理
	glBindTexture(GL_TEXTURE_2D, imgTextureId);
	glUniform1i(fbo_s_TextureMap, 0);
	glUniformMatrix4fv(u_Matrix, 1, GL_FALSE, value_ptr(glm::mat4(1)));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<const void *>(0));
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
  }
  return false;
}

