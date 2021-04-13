//
// Created by Shiki on 2021/4/8.
//

#include "ShaderUtils.h"
#include "../../include/log.h"

GLuint ShaderUtils::CreateProgram(const char *vertex, const char *fragment, GLuint *vShader, GLuint *fShader) {
  GLuint program = 0;

  GLuint vertexShader = LoadShaders(GL_VERTEX_SHADER, vertex);
  if (!vertexShader) {
    return program;
  }
  GLuint fragmentShader = LoadShaders(GL_FRAGMENT_SHADER, fragment);
  if (!fragmentShader) {
    return program;
  }
  program = glCreateProgram();
  if (program) {
    //将着色器程序添加到渲染程序中
    glAttachShader(program, vertexShader);
    CheckGLError("glAttachShader");
    glAttachShader(program, fragmentShader);
    CheckGLError("glAttachShader");
    //链接源程序
    glLinkProgram(program);
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    vertexShader = 0;

    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);
    fragmentShader = 0;
    if (linkStatus != GL_TRUE) {
      GLint infoLen;
      glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
      if (infoLen) {
        char *msg = static_cast<char *>(malloc(infoLen));
        if (msg) {
          glGetProgramInfoLog(program, infoLen, nullptr, msg);
          PLOGE << "shader编译错误:" << msg;
          free(msg);
        }
      }
      glDeleteProgram(program);
      program = 0;
    }
  }
  *vShader = vertexShader;
  *fShader = fragmentShader;
  return program;
}
GLuint ShaderUtils::LoadShaders(int shaderType, const char *code) {
  //创建shader（着色器：顶点或片元
  int shader = glCreateShader(shaderType);
  if (shader&&code) {
    //加载shader源码并编译shader
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    GLint compiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
      GLint infoLen = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
      if (infoLen) {
        char *msg = static_cast<char *>(malloc(infoLen));
        if (msg) {
          glGetShaderInfoLog(shader, infoLen, nullptr, msg);
          PLOGE << "shader编译错误:" << msg;
          free(msg);
        }
        glDeleteShader(shader);
        shader = 0;
      }
    }
  }
  return shader;
}
void ShaderUtils::initPlog() {
  initLog();
}
void ShaderUtils::CheckGLError(const char *pGLOperation) {
  for (GLint error = glGetError(); error; error = glGetError()) {
    PLOGE << "GLUtils::CheckGLError GL Operation " << pGLOperation << "() glError (0x" << error << ")\n";
  }
}
