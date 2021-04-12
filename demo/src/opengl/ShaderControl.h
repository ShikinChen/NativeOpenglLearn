//
// Created by Shiki on 2021/4/7.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGLCONTROL_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGLCONTROL_H_
#include "../egl/EglThread.h"
#include "BaseShader.h"
#include <jni.h>
#include <vector>
using namespace std;

typedef vector<BaseShader *> ShaderlList;

class ShaderControl {
 public:
  EglThread *eglThread = nullptr;
  ANativeWindow *nativeWindow = nullptr;
  int width;
  int height;
  ShaderlList shaderlList;
 public:
  ShaderControl();
  virtual ~ShaderControl();

  void OnSurfaceCreate(JNIEnv *env, jobject surface);

  void OnSurfaceChange(int width, int height);

  void OnSurfaceDestroy();

  void NotifyRender();
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGLCONTROL_H_
