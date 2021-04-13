//
// Created by Shiki on 2021/4/4.
//

#include "EglHelper.h"
#include "Constants.h"
#include <EGL/eglext.h>

EglHelper::EglHelper() {}

EglHelper::~EglHelper() {

}

int EglHelper::InitEgl(EGLNativeWindowType win) {
  //得到默认的显示设备（就是窗口）
  eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
  if (eglDisplay == NULL) {
	PLOGE << "eglDisplay 获取失败";
	return CODE_ERROR;
  }
  //初始化默认显示设备
  EGLint *version = new EGLint[2];
  if (!eglInitialize(eglDisplay, &version[0], &version[1])) {
	PLOGE << "egl 初始化失败";
	return CODE_ERROR;
  }
  //设置显示设备的属性
  const EGLint attribs[] = {
	  EGL_RED_SIZE, 8,
	  EGL_GREEN_SIZE, 8,
	  EGL_BLUE_SIZE, 8,
	  EGL_ALPHA_SIZE, 8,
	  EGL_DEPTH_SIZE, 16,
	  EGL_STENCIL_SIZE, 8,
	  EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT_KHR,
	  EGL_SURFACE_TYPE,EGL_PBUFFER_BIT,
	  EGL_NONE
  };
  EGLint numConfig;
  //从系统中获取对应属性的配置
  if (!eglChooseConfig(eglDisplay, attribs, NULL, 1, &numConfig)) {
	PLOGE << "egl 获取配置失败";
	return CODE_ERROR;
  }
  if (!eglChooseConfig(eglDisplay, attribs, &eglConfig, numConfig, &numConfig)) {
	PLOGE << "egl 配置失败";
	return CODE_ERROR;
  }
  //创建EglContext
  int attribList[] = {
	  EGL_CONTEXT_CLIENT_VERSION, 3,
	  EGL_NONE,
  };
  eglContext = eglCreateContext(eglDisplay, eglConfig, EGL_NO_CONTEXT, attribList);
  if (eglContext == EGL_NO_CONTEXT) {
	PLOGE << "egl 创建上下文失败";
	return CODE_ERROR;
  }
  //创建渲染的Surface
  eglSurface = eglCreateWindowSurface(eglDisplay, eglConfig, win, NULL);
  if (eglSurface == NULL) {
	PLOGE << "egl 创建Surface失败";
	return CODE_ERROR;
  }
  //绑定EglContext和Surface到显示设备中
  if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext)) {
	PLOGE << "egl 绑定EglContext和Surface失败";
	return CODE_ERROR;
  }
  PLOGD << "egl 初始化成功";
  return CODE_SUCCESS;
}
int EglHelper::SwapBuffers() {
  if (eglDisplay == EGL_NO_DISPLAY || eglSurface == EGL_NO_SURFACE) {
	PLOGE << "egl 还没初始化";
	return CODE_ERROR;
  }
  if (!eglSwapBuffers(eglDisplay, eglSurface)) {
	PLOGE << "egl 刷新数据失败";
	return CODE_ERROR;
  }
  return CODE_SUCCESS;
}
void EglHelper::Destroy() {
  if (eglDisplay != EGL_NO_DISPLAY) {
	eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  }
  if (eglDisplay != EGL_NO_DISPLAY && eglSurface != EGL_NO_SURFACE) {
	eglDestroySurface(eglDisplay, eglSurface);
	eglSurface = EGL_NO_SURFACE;
  }
  if (eglDisplay != EGL_NO_DISPLAY && eglContext != EGL_NO_CONTEXT) {
	eglDestroyContext(eglDisplay, eglContext);
	eglContext = EGL_NO_CONTEXT;
  }
  if (eglDisplay != EGL_NO_DISPLAY) {
	eglTerminate(eglDisplay);
	eglDisplay = EGL_NO_DISPLAY;
  }
}
