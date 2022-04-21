//
// Created by Shiki on 2022/4/20.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_UTIL_ASSET_MANAGER_UTIL_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_UTIL_ASSET_MANAGER_UTIL_H_
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <memory>

class AssetManagerUtils {
 public:
  virtual ~AssetManagerUtils();

  std::string read(const char *path);

  static AssetManagerUtils *Instance(JNIEnv *env, jobject *j_a_asset_manager) {
	if (m_instance_ == nullptr) {
	  m_instance_ = new AssetManagerUtils(env, j_a_asset_manager);
	}
	return m_instance_;
  }

  static AssetManagerUtils *GetInstance() {
	if (m_instance_ == nullptr) {
	  throw std::logic_error("the instance is not init, please initialize the instance first");
	}
	return m_instance_;
  }

 private:
  AssetManagerUtils(JNIEnv *env, jobject *j_a_asset_manager);

 private:
  AAssetManager *a_asset_manager_ = nullptr;
  jobject j_a_asset_manager;
  static AssetManagerUtils *m_instance_;
};
#endif //NATIVEOPENGLLEARN_DEMO_SRC_UTIL_ASSET_MANAGER_UTIL_H_
