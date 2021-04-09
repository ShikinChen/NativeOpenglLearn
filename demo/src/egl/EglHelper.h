//
// Created by Shiki on 2021/4/4.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGLHELPER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGLHELPER_H_
#include <plog/Log.h>
#include <EGL/egl.h>

class EglHelper {
 public:
  EGLDisplay eglDisplay;
  EGLSurface eglSurface;
  EGLConfig eglConfig;
  EGLContext eglContext;

 public:
  EglHelper();
  virtual ~EglHelper();

  int InitEgl(EGLNativeWindowType win);

  int SwapBuffers();

  void Destroy();
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGLHELPER_H_
