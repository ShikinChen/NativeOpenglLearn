//
// Created by Shiki on 2021/4/7.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGLCONTROL_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGLCONTROL_H_
#include "../egl/egl_thread.h"
#include "base_shader.h"
#include <jni.h>
#include <vector>
using namespace std;

using ShaderlList = vector<shared_ptr<BaseShader>>;

class ShaderControl : public EglLifecycle, public std::enable_shared_from_this<ShaderControl> {
 public:
  ShaderControl();
  virtual ~ShaderControl();

  void OnSurfaceCreate(JNIEnv *env, jobject surface);

  void OnSurfaceChange(int width, int height);

  void OnSurfaceDestroy();

  void NotifyRender();

  void OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void OnDestroy() override;

  void AddShader(const shared_ptr<BaseShader> &shader);

  const shared_ptr<BaseShader> &GetShader(int index) const;

  void SwitchShader(int unSelIndex, int selIndex);

 private:
  std::unique_ptr<EglThread> egl_thread_;
  ANativeWindow *native_window_ = nullptr;
  int width;
  int height;
  ShaderlList shader_list_;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGLCONTROL_H_
