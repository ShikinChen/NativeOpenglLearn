#include <jni.h>
#include "headers.h"
#include <plog/Log.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <unordered_map>

#define NATIVE_EGL_CLASS_NAME "me/shiki/nativeopengllearn/native/NativeEgl"
#define MAIN_ACTIVITY_CLASS_NAME "me/shiki/nativeopengllearn/MySurfaceView"

JavaVM *javaVm = NULL;

#ifdef __cplusplus
extern "C" {
#endif
shared_ptr<ShaderControl> shaderlControl;

JNIEXPORT void JNICALL native_Init(JNIEnv *env, jobject thiz, jobject j_a_asset_manager) {
  AssetManagerUtils::Instance(env, &j_a_asset_manager);
}

JNIEXPORT void JNICALL native_Create(JNIEnv *env, jobject thiz, jobject surface) {
  if (!shaderlControl) {
	shaderlControl = make_shared<ShaderControl>();
	shaderlControl->AddShader(std::make_shared<Triangle>());
	shaderlControl->AddShader(std::make_shared<TextureMap>(false));
	shaderlControl->AddShader(std::make_shared<NV21TextureMap>(false));
	shaderlControl->AddShader(std::make_shared<VaoAndVbo>(false));
	shaderlControl->AddShader(std::make_shared<FboOffscreenRendering>(false));
//	shaderlControl->shader_list_.push_back(new VaoAndVbo(false));
	shaderlControl->AddShader(std::make_shared<EGLRender>(false));
  }
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

JNIEXPORT void JNICALL native_SwitchShader(JNIEnv *env, jobject thiz, jint unSelIndex, jint selIndex) {
  shaderlControl->SwitchShader(unSelIndex, selIndex);
}

JNIEXPORT void JNICALL native_NotifyRender(JNIEnv *env, jobject thiz) {
  if (shaderlControl) {
	shaderlControl->NotifyRender();
  }
}

JNIEXPORT void JNICALL native_SetImageData(JNIEnv *env,
										   jobject thiz,
										   jint index,
										   jint format,
										   jint width,
										   jint height,
										   jbyteArray imageData) {
  if (shaderlControl) {
	int len = env->GetArrayLength(imageData);
	uint8_t *buf = new uint8_t[len];
	env->GetByteArrayRegion(imageData, 0, len, reinterpret_cast<jbyte *>(buf));
	switch (index) {
	  case 1: {
		auto item = dynamic_pointer_cast<TextureMap>(shaderlControl->GetShader(index));
		NativeImageUtil::InitNativeImage(format, width, height, buf, item->img());
		item->ResetMatrix();
	  }
		break;
	  case 2: {
		auto item = dynamic_pointer_cast<NV21TextureMap>(shaderlControl->GetShader(index));
		NativeImageUtil::InitNativeImage(format, width, height, buf, item->img());
		item->ResetMatrix();
	  }
		break;
	  case 4: {
		auto item = dynamic_pointer_cast<FboOffscreenRendering>(shaderlControl->GetShader(index));
		NativeImageUtil::InitNativeImage(format, width, height, buf, item->GetImg());
		item->isCreateFrameBufferObj = true;
		item->ResetMatrix();
	  }
		break;
	}
	delete[] buf;
	env->DeleteLocalRef(imageData);
  }
}

static JNINativeMethod g_NativeEglMethods[] = {
	{"native_Init", "(Landroid/content/res/AssetManager;)V", (void *)(native_Init)},
	{"native_Create", "(Landroid/view/Surface;)V", (void *)(native_Create)},
	{"native_Change", "(II)V", (void *)(native_Change)},
	{"native_Destroy", "()V", (void *)(native_Destroy)},
	{"native_NotifyRender", "()V", (void *)(native_NotifyRender)},
};

static JNINativeMethod g_MainActivityMethods[] = {
	{"native_SwitchShader", "(II)V", (void *)(native_SwitchShader)},
	{"native_SetImageData", "(IIII[B)V", (void *)(native_SetImageData)},
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
  regRet = RegisterNativeMethods(env, MAIN_ACTIVITY_CLASS_NAME, g_MainActivityMethods,
								 sizeof(g_MainActivityMethods) /
									 sizeof(g_MainActivityMethods[0]));

  if (regRet != JNI_TRUE) {
	return JNI_ERR;
  }

  initLog("demo_app");
//  ShaderUtils::initPlog();
  return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM *jvm, void *p) {
  JNIEnv *env = nullptr;
  if (jvm->GetEnv((void **)(&env), JNI_VERSION_1_6) != JNI_OK) {
	return;
  }

  UnregisterNativeMethods(env, NATIVE_EGL_CLASS_NAME);
  UnregisterNativeMethods(env, MAIN_ACTIVITY_CLASS_NAME);
}
#ifdef __cplusplus
}
#endif