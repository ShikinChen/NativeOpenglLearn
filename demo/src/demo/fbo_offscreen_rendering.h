//
// Created by Shiki on 2021/4/13.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_DEMO_FBO_OFFSCREEN_RENDERING_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_DEMO_FBO_OFFSCREEN_RENDERING_H_
#include "../opengl/base_shader.h"
#include "../include/ImageDef.h"

class FboOffscreenRendering : public BaseShader {
 public:
  bool isCreateFrameBufferObj;
 public:
  FboOffscreenRendering(bool isDraw = true);
  virtual ~FboOffscreenRendering();
  bool OnCreate() override;
  void OnChange(int width, int height) override;
  void OnDraw() override;
  void Destroy() override;
  void ResetMatrix() override;

  NativeImage *GetImg();


 protected:
  const char *GetVertex() override;
  const char *GetFragment() override;

 private:
  GLuint imgTextureId;
  GLuint fboTextureId;
  GLuint fboId;
  GLuint vaoIds[2];
  GLuint vboIds[4];
  GLint u_Matrix;
  GLint s_TextureMap;
  GLint fbo_s_TextureMap;
  GLint fbo_u_Matrix;
  GLuint fbo_program;
  GLuint fbo_vertexShader;
  GLuint fbo_fragmentShader;
  NativeImage img;
  bool CreateFrameBufferObj();

};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_DEMO_FBO_OFFSCREEN_RENDERING_H_
