//
// Created by Shiki on 2021/4/3.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_

#include <EGL/eglplatform.h>
#include <pthread.h>

enum RenderType {
  AUTO,
  HANDLE
};

class EglThread {
 public:
  pthread_t eglThread = -1;
  ANativeWindow *nativeWindow = NULL;

  bool isCreate = false;
  bool isChange = false;
  bool isExit = false;
  bool isStart = false;
  int surfaceWidth = 0;
  int surfaceHeight = 0;

  pthread_mutex_t pthreadMutex;
  pthread_cond_t pthreadCond;
 private:
  RenderType renderType = AUTO;

 public:
  EglThread();
  virtual ~EglThread();

  void OnSurfaceCreate(EGLNativeWindowType window);
  void OnSurfaceChange(int width, int height);

  typedef void (*OnCreate)(void *context);
  OnCreate onCreate = nullptr;
  void *onCreateContext;

  typedef void (*OnChange)(int width, int height, void *context);
  OnChange onChange = nullptr;
  void *onChangeContext;

  typedef void (*OnDraw)(void *context);
  OnDraw onDraw = nullptr;
  void *onDrawContext;

  typedef void (*OnDestroy)(void *context);
  OnDestroy onDestroy = nullptr;
  void *onDestroyContext;

  void CallbackOnCreate(OnCreate onCreate, void *context);
  void CallbackOnChange(OnChange onChange, void *context);
  void CallbackOnDraw(OnDraw onDraw, void *context);

  void CallbackOnDestroy(OnDestroy onDestroy, void *context);
  void SetRenderType(RenderType renderType);
  RenderType GetRenderType();

  void NotifyRender();
  void Destroy();
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_
