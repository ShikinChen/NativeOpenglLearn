//
// Created by Shiki on 2021/4/7.
//

#include "ShaderControl.h"
#include <android/native_window_jni.h>
#include <plog/Log.h>
#include <algorithm>
#include <list>

ShaderControl::ShaderControl() {}
ShaderControl::~ShaderControl() {
  if (eglThread) {
    delete eglThread;
    eglThread = nullptr;
  }
  while (!shaderlList.empty()) {
    auto item = shaderlList.back();
    shaderlList.pop_back();
    delete item;
    item = nullptr;
  }
}

void callbackSurfaceCreate(void *context) {
  ShaderControl *openglControl = static_cast<ShaderControl *>(context);
  if (openglControl && !openglControl->shaderlList.empty()) {
    auto list = openglControl->shaderlList;
    ShaderlList::iterator item;
    for (item = list.begin(); item != list.end(); ++item) {
      static_cast<BaseShader *>(*item)->OnCreate();
    }
  }
}

void callbackSurfaceChange(int width, int height, void *context) {
  ShaderControl *openglControl = static_cast<ShaderControl *>(context);
  if (openglControl && !openglControl->shaderlList.empty()) {
    auto list = openglControl->shaderlList;
    ShaderlList::iterator item;
    for (item = list.begin(); item != list.end(); ++item) {
      static_cast<BaseShader *>(*item)->OnChange(width, height);
    }
  }
}

void callbackSurfaceDraw(void *context) {
  ShaderControl *openglControl = static_cast<ShaderControl *>(context);
  if (openglControl && !openglControl->shaderlList.empty()) {
    auto list = openglControl->shaderlList;
    ShaderlList::iterator item;
    for (item = list.begin(); item != list.end(); ++item) {
      auto shader = static_cast<BaseShader *>(*item);
      if (shader->isDraw) {
        shader->OnDraw();
      }
    }
  }
}

void callbackSurfaceDestroy(void *context) {
  ShaderControl *openglControl = static_cast<ShaderControl *>(context);
  if (openglControl && !openglControl->shaderlList.empty()) {
    auto list = openglControl->shaderlList;
    ShaderlList::iterator item;
    for (item = list.begin(); item != list.end(); ++item) {
      static_cast<BaseShader *>(*item)->Destroy();
    }
  }
}

void ShaderControl::OnSurfaceCreate(JNIEnv *env, jobject surface) {
  nativeWindow = ANativeWindow_fromSurface(env, surface);
  eglThread = new EglThread();
  eglThread->set_render_type(HANDLE);
  eglThread->CallbackOnCreate(callbackSurfaceCreate, this);
  eglThread->CallbackOnChange(callbackSurfaceChange, this);
  eglThread->CallbackOnDraw(callbackSurfaceDraw, this);
  eglThread->CallbackOnDestroy(callbackSurfaceDestroy, this);

  eglThread->OnSurfaceCreate(nativeWindow);
}
void ShaderControl::OnSurfaceChange(int width, int height) {
  if (eglThread) {
    eglThread->OnSurfaceChange(width, height);
  }
}
void ShaderControl::OnSurfaceDestroy() {
  if (eglThread) {
    eglThread->Destroy();
    delete eglThread;
    eglThread = nullptr;
  }
  if (!shaderlList.empty()) {
    ShaderlList::iterator item;
    for (item = shaderlList.begin(); item != shaderlList.end(); ++item) {
      auto shader = static_cast<BaseShader *>(*item);
      shader->Destroy();
    }
  }
  if (nativeWindow) {
    ANativeWindow_release(nativeWindow);
    nativeWindow = nullptr;
  }
}
void ShaderControl::NotifyRender() {
  if (eglThread && eglThread->render_type() == HANDLE) {
    eglThread->NotifyRender();
  }
}
