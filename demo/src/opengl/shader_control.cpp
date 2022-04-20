//
// Created by Shiki on 2021/4/7.
//

#include "shader_control.h"
#include <android/native_window_jni.h>
#include <plog/Log.h>
#include <algorithm>
#include <list>

ShaderControl::ShaderControl() {}
ShaderControl::~ShaderControl() {
  OnSurfaceDestroy();
}

void ShaderControl::OnSurfaceCreate(JNIEnv *env, jobject surface) {
  native_window_ = ANativeWindow_fromSurface(env, surface);
  egl_thread_ = make_unique<EglThread>();
  egl_thread_->set_render_type(HANDLE);
  egl_thread_->set_egl_lifecycle(shared_from_this());
  OnDestroy();

  egl_thread_->OnSurfaceCreate(native_window_);
}
void ShaderControl::OnSurfaceChange(int width, int height) {
  if (egl_thread_) {
	egl_thread_->OnSurfaceChange(width, height);
  }
}
void ShaderControl::OnSurfaceDestroy() {
  if (egl_thread_) {
	egl_thread_->Destroy();
  }
  OnDestroy();
  if (native_window_) {
	ANativeWindow_release(native_window_);
	native_window_ = nullptr;
  }
}
void ShaderControl::NotifyRender() {
  if (egl_thread_ && egl_thread_->render_type()==HANDLE) {
	egl_thread_->NotifyRender();
  }
}
void ShaderControl::OnCreate() {
  if (!shader_list_.empty()) {
	for (auto item = shader_list_.begin(); item!=shader_list_.end(); ++item) {
	  item->get()->OnCreate();
	}
  }
}
void ShaderControl::OnDraw() {
  if (!shader_list_.empty()) {
	auto list = shader_list_;
	for (auto item = list.begin(); item!=list.end(); ++item) {
	  if (item->get()->is_draw_) {
		item->get()->OnDraw();
	  }
	}
  }
}
void ShaderControl::OnDestroy() {
  if (!shader_list_.empty()) {
	auto list = shader_list_;
	for (auto item = list.begin(); item!=list.end(); ++item) {
	  item->get()->Destroy();
	}
  }
}
void ShaderControl::OnChange(int width, int height) {
  if (!shader_list_.empty()) {
	auto list = shader_list_;
	for (auto item = list.begin(); item!=list.end(); ++item) {
	  item->get()->OnChange(width, height);
	}
  }
}
void ShaderControl::AddShader(const shared_ptr<BaseShader> &shader) {
  shader_list_.emplace_back(shader);
}
const shared_ptr<BaseShader> &ShaderControl::GetShader(int index) const {
  shared_ptr<BaseShader> item;
  if (!shader_list_.empty() && shader_list_.size() > index) {
	return shader_list_[index];
  }
  return item;
}
void ShaderControl::SwitchShader(int unSelIndex, int selIndex) {
  auto list = shader_list_;
  if (unSelIndex < list.size() && selIndex < list.size()) {
	shader_list_[unSelIndex]->is_draw_ = false;
	shader_list_[selIndex]->is_draw_ = true;
	NotifyRender();
  }
}

