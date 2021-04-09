#include <jni.h>
#include "headers.h"
#include <plog/Log.h>
#include <plog/Init.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <unordered_map>
#define NATIVE_EGL_CLASS_NAME "me/shiki/nativeopengllearn/native/NativeEgl"

JavaVM *javaVm = NULL;

#ifdef __cplusplus
extern "C" {
#endif
ShaderControl *shaderlControl = nullptr;
JNIEXPORT void JNICALL native_Create(JNIEnv *env, jobject thiz, jobject surface) {
  if (!shaderlControl) {
	shaderlControl = new ShaderControl();
	shaderlControl->shaderlList.push_back(new Triangle());
  }
  PLOGD << "native_Create";
  shaderlControl->OnSurfaceCreate(env, surface);
}

JNIEXPORT void JNICALL native_Change(JNIEnv *env, jobject thiz, jint width, jint height) {
  if (shaderlControl) {
	shaderlControl->OnSurfaceChange(width, height);
  }
}

JNIEXPORT void JNICALL native_Destroy(JNIEnv *env, jobject thiz) {
  if (shaderlControl) {
	shaderlControl->OnSurfaceDestroy();
  }
}

#ifdef __cplusplus
}
#endif

static JNINativeMethod g_NativeEglMethods[] = {
	{"native_Create", "(Landroid/view/Surface;)V", (void *)(native_Create)},
	{"native_Change", "(II)V", (void *)(native_Change)},
	{"native_Destroy", "()V", (void *)(native_Destroy)},
};

static int RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum) {
  PLOGD << "RegisterNativeMethods";
  jclass clazz = env->FindClass(className);
  if (!clazz) {
	PLOGD << "RegisterNativeMethods fail. clazz == NULL";
	return JNI_FALSE;
  }
  if (env->RegisterNatives(clazz, methods, methodNum) < 0) {
	PLOGD << "RegisterNativeMethods fail";
	return JNI_FALSE;
  }
  return JNI_TRUE;
}

static void UnregisterNativeMethods(JNIEnv *env, const char *className) {
  PLOGD << "UnregisterNativeMethods";
  jclass clazz = env->FindClass(className);
  if (!clazz) {
	PLOGD << "UnregisterNativeMethods fail. clazz == NULL";
	return;
  }
  if (env) {
	env->UnregisterNatives(clazz);
  }
}

extern "C" {
JNIEXPORT JNICALL jint JNI_OnLoad(JavaVM *jvm, void *reserved) {
  jint jniRet = JNI_ERR;
  JNIEnv *env = nullptr;
  if (jvm->GetEnv((void **)(&env), JNI_VERSION_1_6) != JNI_OK) {
	return jniRet;
  }
  jint regRet = RegisterNativeMethods(env, NATIVE_EGL_CLASS_NAME, g_NativeEglMethods,
									  sizeof(g_NativeEglMethods) /
										  sizeof(g_NativeEglMethods[0]));

  if (regRet != JNI_TRUE) {
	return JNI_ERR;
  }
  initLog("demo_app");
//  ShaderUtils::initPlog();
  return JNI_VERSION_1_6;
}

extern "C" void JNI_OnUnload(JavaVM *jvm, void *p) {
  JNIEnv *env = nullptr;
  if (jvm->GetEnv((void **)(&env), JNI_VERSION_1_6) != JNI_OK) {
	return;
  }

  UnregisterNativeMethods(env, NATIVE_EGL_CLASS_NAME);
}
}
