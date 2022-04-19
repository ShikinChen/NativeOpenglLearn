//
// Created by Shiki on 2021/4/15.
//

#include "EGLRender.h"
EGLRender::~EGLRender() {
  index = 0;
}
EGLRender::EGLRender(bool isDraw) : BaseShader(isDraw) {

}

const char *EGLRender::GetVertex() {
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
const char *EGLRender::GetFragment() {
  const char *fShaderStr = nullptr;
  switch (index) {
	default: {
	  case 1: {
		fShaderStr = "#version 300 es                              \n"
					 "precision highp float;                     	\n"
					 "in vec2 v_texCoord;  							\n"
					 "layout(location = 0) out vec4 outColor;  		\n"
					 "uniform sampler2D s_TextureMap;  				\n"
					 "uniform vec2 u_texSize;  						\n"
					 "void main()                                  \n"
					 "{                                            \n"
					 "   outColor = texture(s_TextureMap, v_texCoord);  \n"
					 "}                                            \n";
	  }
	  break;
	  fShaderStr = "#version 300 es                              \n"
				   "precision mediump float;                     \n"
				   "in vec2 v_texCoord;  							\n"
				   "layout(location = 0) out vec4 outColor;  		\n"
				   "uniform sampler2D s_TextureMap;  				\n"
				   "void main()                                  \n"
				   "{                                            \n"
				   "   outColor = texture(s_TextureMap, v_texCoord);  \n"
				   "}                                            \n";
	}
	  break;
  }
  return fShaderStr;
}

void EGLRender::ResetMatrix() {
  BaseShader::ResetMatrix();
}
bool EGLRender::OnCreate() {
  return BaseShader::OnCreate();
}
void EGLRender::OnChange(int width, int height) {
  BaseShader::OnChange(width, height);
}
void EGLRender::OnDraw() {
  BaseShader::OnDraw();
}
void EGLRender::Destroy() {
  BaseShader::Destroy();
}

