//
// Created by Shiki on 2021/4/3.
//

#include "EglThread.h"
#include "EglHelper.h"
EglThread::EglThread() {
  pthread_mutex_init(&pthreadMutex, nullptr);
  pthread_cond_init(&pthreadCond, nullptr);
}
EglThread::~EglThread() {
  pthread_mutex_destroy(&pthreadMutex);
  pthread_cond_destroy(&pthreadCond);
}

void *eglThreadImpl(void *data) {
  EglThread *eglThread = static_cast<EglThread *>(data);
  EglHelper *eglHelper = new EglHelper();
  eglHelper->InitEgl(eglThread->nativeWindow);
  eglThread->isExit = false;
  while (!eglThread->isExit) {
	if (eglThread->isCreate) {
	  PLOGD << "eglThread create";
	  eglThread->isCreate = false;
	  if (eglThread->onCreate != nullptr) {
		eglThread->onCreate(eglThread->onCreateContext);
	  }
	}

	if (eglThread->isChange) {
	  PLOGD << "eglThread change";
	  eglThread->isChange = false;
	  if (eglThread->onChange) {
		eglThread->onChange(eglThread->surfaceWidth, eglThread->surfaceHeight, eglThread->onChangeContext);
	  }
	  eglThread->isStart = true;
	}
	if (eglThread->isStart) {
	  if (eglThread->onDraw) {
		eglThread->onDraw(eglThread->onDrawContext);
	  }
	  eglHelper->SwapBuffers();
	}
	switch (eglThread->GetRenderType()) {
	  case AUTO: {
		usleep(1000000 / 60);
	  }
		break;
	  case HANDLE: {
		pthread_mutex_lock(&eglThread->pthreadMutex);
		pthread_cond_wait(&eglThread->pthreadCond, &eglThread->pthreadMutex);
		pthread_mutex_unlock(&eglThread->pthreadMutex);
	  }
		break;
	}
  }
  if (eglThread->onDestroy != nullptr) {
	eglThread->onDestroy(eglThread->onDestroyContext);
  }
  PLOGD << "eglThread destroy";
  eglHelper->Destroy();
  delete eglHelper;
  eglHelper = nullptr;
  return 0;
}

void EglThread::OnSurfaceCreate(EGLNativeWindowType window) {
  if (eglThread == -1) {
	isCreate = true;
	nativeWindow = window;
	pthread_create(&eglThread, nullptr, eglThreadImpl, this);
  }
}

void EglThread::OnSurfaceChange(int width, int height) {
  isChange = true;
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
void EglThread::SetRenderType(RenderType renderType) {
  RenderType tmp = this->renderType;
  this->renderType = renderType;
  if (tmp == HANDLE) {
	NotifyRender();
  }
}
void EglThread::NotifyRender() {
  pthread_mutex_lock(&pthreadMutex);
  pthread_cond_signal(&pthreadCond);
  pthread_mutex_unlock(&pthreadMutex);
}
void EglThread::Destroy() {
  isExit = true;
  if (renderType == HANDLE) {
	NotifyRender();
  }
  pthread_join(eglThread, nullptr);
  isExit = false;
  nativeWindow = nullptr;
  eglThread = -1;
}
RenderType EglThread::GetRenderType() {
  return renderType;
}
