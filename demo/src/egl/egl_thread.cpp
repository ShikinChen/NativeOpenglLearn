//
// Created by Shiki on 2021/4/3.
//

#include "egl_thread.h"
#include "egl_helper.h"
#include <chrono>

EglThread::EglThread() {
}

EglThread::~EglThread() {
}

void EglThread::Run() {
  EglHelper eglHelper;
  eglHelper.InitEgl(native_window_);
  is_exit_ = false;
  while (!is_exit_) {
	auto egl_lifecycle = egl_lifecycle_.lock();
	if (is_create_) {
	  PLOGD << "egl_thread_ create";
	  is_create_ = false;
	  if (egl_lifecycle) {
		egl_lifecycle->OnCreate();
	  }
	}

	if (is_change_) {
	  PLOGD << "egl_thread_ change";
	  is_change_ = false;
	  if (egl_lifecycle) {
		egl_lifecycle->OnChange(surface_width_, surface_height_);
	  }
	  is_start_ = true;
	}
	if (is_start_) {
	  if (egl_lifecycle) {
		std::lock_guard<std::mutex> locker(mutex_);
		egl_lifecycle->OnDraw();
		is_render_ = false;
	  }
	  eglHelper.SwapBuffers();
	}
	switch (render_type()) {
	  case AUTO: {
		std::this_thread::sleep_for(std::chrono::milliseconds(kFps));
	  }
		break;
	  case HANDLE: {
		std::unique_lock<std::mutex> locker(mutex_);
		notify_render_.wait(locker, [this]() {
		  return is_render_;
		});

	  }
		break;
	}
  }
  if (onDestroy) {
	onDestroy(onDestroyContext);
  }
  PLOGD << "egl_thread_ destroy";
  eglHelper.Destroy();
//  delete eglHelper;
//  eglHelper = nullptr;
}

void EglThread::OnSurfaceCreate(EGLNativeWindowType window) {
  is_create_ = true;
  native_window_ = window;
  egl_thread_ = std::make_unique<std::thread>(&EglThread::Run, this);
}

void EglThread::OnSurfaceChange(int width, int height) {
  is_change_ = true;
  surface_height_ = height;
  surface_width_ = width;
  NotifyRender();
}

void EglThread::CallbackOnDestroy(EglThread::OnDestroy onDestroy, void *context) {
  this->onDestroy = onDestroy;
  onDestroyContext = context;
}
void EglThread::set_render_type(RenderType render_type) {
  {
	std::lock_guard<std::mutex> locker(mutex_);
	this->render_type_ = render_type;
  }
  if (render_type_==HANDLE) {
	NotifyRender();
  }
}
void EglThread::NotifyRender() {
  std::lock_guard<std::mutex> locker(mutex_);
  is_render_ = true;
  notify_render_.notify_one();
}
void EglThread::Destroy() {
  is_exit_ = true;
  if (render_type_==HANDLE) {
	NotifyRender();
  }
  if (egl_thread_) {
	egl_thread_->join();
  }
  native_window_ = nullptr;
}
RenderType EglThread::render_type() {
  std::lock_guard<std::mutex> locker(mutex_);
  return render_type_;
}
void EglThread::set_egl_lifecycle(const std::weak_ptr<EglLifecycle> &egl_lifecycle) {
  egl_lifecycle_ = egl_lifecycle;
}
