//
// Created by Shiki on 2021/4/3.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_

#include <EGL/eglplatform.h>
#include<mutex>
#include<thread>
#include<condition_variable>

enum RenderType {
  AUTO,
  HANDLE
};

const long long kFps = 1000/60;

class EglThread {

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
  void set_render_type(RenderType render_type);
  RenderType render_type();

  void NotifyRender();
  void Destroy();

 public:
  std::unique_ptr<std::thread> egl_thread_;
  ANativeWindow *nativeWindow = nullptr;

  bool is_create_ = false;
  bool is_change_ = false;
  bool is_exit_ = false;
  bool isStart = false;
  int surfaceWidth = 0;
  int surfaceHeight = 0;

 private:
  std::mutex mutex_;
  std::condition_variable notify_render_;
  bool is_render_ = false;

  void Run();

 private:
  RenderType render_type_ = AUTO;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_
