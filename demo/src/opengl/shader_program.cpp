//
// Created by Shiki on 2022/4/20.
//

#include "shader_program.h"
ShaderProgram::ShaderProgram() : program_(0) {
}
ShaderProgram::~ShaderProgram() {

}
GLuint ShaderProgram::program() const {
  return program_;
}
const std::map<std::string, GLuint> &ShaderProgram::uniform_map() const {
  return uniform_map_;
}
const std::map<std::string, GLuint> &ShaderProgram::attrib_map() const {
  return attrib_map_;
}
void ShaderProgram::set_program(GLuint program) {
  program_ = program;
}
