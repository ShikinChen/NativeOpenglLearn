//
// Created by Shiki on 2022/4/20.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGL_LIFECYCLE_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGL_LIFECYCLE_H_

class EglLifecycle {
 public:
  virtual void OnCreate();
  virtual void OnChange(int width, int height);
  virtual void OnDraw();
  virtual void OnDestroy();
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_EGL_EGL_LIFECYCLE_H_
