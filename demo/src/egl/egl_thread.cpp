//
// Created by Shiki on 2021/4/3.
//

#include "egl_thread.h"
#include "EglHelper.h"
#include <chrono>

EglThread::EglThread() {
}

EglThread::~EglThread() {
}

void EglThread::Run() {
  EglHelper *eglHelper = new EglHelper();
  eglHelper->InitEgl(nativeWindow);
  is_exit_ = false;
  while (!is_exit_) {
	if (is_create_) {
	  PLOGD << "egl_thread_ create";
	  is_create_ = false;
	  if (onCreate) {
		onCreate(onCreateContext);
	  }
	}

	if (is_change_) {
	  PLOGD << "egl_thread_ change";
	  is_change_ = false;
	  if (onChange) {
		onChange(surfaceWidth, surfaceHeight, onChangeContext);
	  }
	  isStart = true;
	}
	if (isStart) {
	  if (onDraw) {
		std::lock_guard<std::mutex> locker(mutex_);
		onDraw(onDrawContext);
		is_render_ = false;
	  }
	  eglHelper->SwapBuffers();
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
  eglHelper->Destroy();
  delete eglHelper;
  eglHelper = nullptr;
}

void EglThread::OnSurfaceCreate(EGLNativeWindowType window) {
  is_create_ = true;
  nativeWindow = window;
  egl_thread_ = std::make_unique<std::thread>(&EglThread::Run, this);
}

void EglThread::OnSurfaceChange(int width, int height) {
  is_change_ = true;
  surfaceHeight = height;
  surfaceWidth = width;
  NotifyRender();
}
void EglThread::CallbackOnCreate(EglThread::OnCreate onCreate, void *context) {
  this->onCreate = onCreate;
  this->onCreateContext = context;
}
void EglThread::CallbackOnChange(EglThread::OnChange onChange, void *context) {
  this->onChange = onChange;
  this->onChangeContext = context;
}
void EglThread::CallbackOnDraw(EglThread::OnDraw onDraw, void *context) {
  this->onDraw = onDraw;
  this->onDrawContext = context;
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
  delete nativeWindow;
  nativeWindow = nullptr;
}
RenderType EglThread::render_type() {
  std::lock_guard<std::mutex> locker(mutex_);
  return render_type_;
}
