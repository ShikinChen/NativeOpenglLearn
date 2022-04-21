//
// Created by Shiki on 2022/4/20.
//

#include "asset_manager_utils.h"
#include <jni.h>
#include <string>

AssetManagerUtils::AssetManagerUtils(JNIEnv *env, jobject *j_a_asset_manager) {
  a_asset_manager_ = AAssetManager_fromJava(env, *j_a_asset_manager);
}

AssetManagerUtils::~AssetManagerUtils() {
  a_asset_manager_ = nullptr;
}

std::string AssetManagerUtils::read(const char *path) {
  std::string result;
  if (a_asset_manager_) {
	AAsset *asset_file = AAssetManager_open(a_asset_manager_, path, AASSET_MODE_STREAMING);
	if (asset_file) {
	  size_t file_length = AAsset_getLength(asset_file);
	  char *data_buffer = (char *)malloc(file_length);
	  AAsset_read(asset_file, data_buffer, file_length);
	  AAsset_close(asset_file);
	  result = std::string(data_buffer, data_buffer + file_length);
	  free(data_buffer);
	}
  }
  return result;
}

AssetManagerUtils *AssetManagerUtils::m_instance_ = nullptr;
