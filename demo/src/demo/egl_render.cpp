//
// Created by Shiki on 2021/4/15.
//
#include <plog/Log.h>
#include "egl_render.h"
#include "../util/asset_manager_utils.h"

EGLRender::~EGLRender() {
  index = 0;
}
EGLRender::EGLRender(bool is_draw) : BaseShader(is_draw) {

}

void EGLRender::ResetMatrix() {
  BaseShader::ResetMatrix();
}
bool EGLRender::OnCreate() {
  PLOGD << "OnCreate";
  vsh_ = AssetManagerUtils::GetInstance()->read("egl/vert.glsl");
  fsh_ = AssetManagerUtils::GetInstance()->read("egl/normal.frag.glsl");

  if (!BaseShader::OnCreate()) {
	return false;
  }
  return true;
}
void EGLRender::OnChange(int width, int height) {
  BaseShader::OnChange(width, height);
}
void EGLRender::OnDraw() {
  BaseShader::OnDraw();
}
void EGLRender::Destroy() {
  BaseShader::Destroy();
}

