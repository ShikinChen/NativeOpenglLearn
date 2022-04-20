//
// Created by Shiki on 2021/4/4.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGL_HELPER_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGL_HELPER_H_
#include <plog/Log.h>
#include <EGL/egl.h>
#include "egl_lifecycle.h"

class EglHelper {

 public:
  EglHelper();
  virtual ~EglHelper();

  int InitEgl(EGLNativeWindowType win);

 public:

  int SwapBuffers();

  void Destroy();

 private:
  EGLDisplay egl_display_;
  EGLSurface egl_surface_;
  EGLConfig egl_config_;
  EGLContext egl_context_;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGL_HELPER_H_
