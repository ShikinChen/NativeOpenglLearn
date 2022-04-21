//
// Created by Shiki on 2022/4/20.
//

#ifndef NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_SHADER_PROGRAM_H_
#define NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_SHADER_PROGRAM_H_
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

class ShaderProgram {

 public:
  ShaderProgram();
  virtual ~ShaderProgram();

  GLuint program() const;
  void set_program(GLuint program);

  const std::map<std::string, GLuint> &uniform_map() const;
  const std::map<std::string, GLuint> &attrib_map() const;

  void PutUniform(const std::string key, GLuint value) {
	uniform_map_.emplace(key, value);
  }

  void PutAttrib(const std::string key, GLuint value) {
	attrib_map_.emplace(key, value);
  }

 private:
  GLuint program_;
  std::map<std::string, GLuint> uniform_map_;
  std::map<std::string, GLuint> attrib_map_;
};

#endif //NATIVEOPENGLLEARN_DEMO_SRC_OPENGL_SHADER_PROGRAM_H_
