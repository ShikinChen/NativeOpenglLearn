//
// Created by Shiki on 2021/4/3.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_

#include <EGL/eglplatform.h>
#include<mutex>
#include<thread>
#include<condition_variable>
#include "egl_lifecycle.h"

enum RenderType {
  AUTO,
  HANDLE
};

const long long kFps = 1000 / 60;

class EglThread {

 public:
  EglThread();
  virtual ~EglThread();

  void OnSurfaceCreate(EGLNativeWindowType window);
  void OnSurfaceChange(int width, int height);

  using OnDestroy = void (*)(void *context);
  OnDestroy onDestroy = nullptr;
  void *onDestroyContext;

  void CallbackOnDestroy(OnDestroy onDestroy, void *context);
  void set_render_type(RenderType render_type);
  RenderType render_type();

  void NotifyRender();
  void Destroy();
  void set_egl_lifecycle(const std::weak_ptr<EglLifecycle> &egl_lifecycle);

 public:
  std::unique_ptr<std::thread> egl_thread_;
  ANativeWindow *native_window_ = nullptr;

  bool is_create_ = false;
  bool is_change_ = false;
  bool is_exit_ = false;
  bool is_start_ = false;
  int surface_width_ = 0;
  int surface_height_ = 0;

 private:
  std::weak_ptr<EglLifecycle> egl_lifecycle_;

  std::mutex mutex_;
  std::condition_variable notify_render_;
  bool is_render_ = false;

  void Run();

 private:
  RenderType render_type_ = AUTO;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_EGLTHREAD_H_
