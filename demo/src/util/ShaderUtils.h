//
// Created by Shiki on 2021/4/8.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_UTIL_SHADERUTILS_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_UTIL_SHADERUTILS_H_
#include <GLES2/gl2.h>

class ShaderUtils {
 public:
  static void initPlog();

  static GLuint LoadShaders(int shaderType, const char *code);

  static GLuint CreateProgram(const char *vertex, const char *fragment, GLuint *vShader, GLuint *fShader);

  static void CheckGLError(const char *pGLOperation);
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_UTIL_SHADERUTILS_H_
