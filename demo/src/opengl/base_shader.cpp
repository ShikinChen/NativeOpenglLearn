//
// Created by Shiki on 2021/4/7.
//

#include "base_shader.h"
#include "../util/ShaderUtils.h"

BaseShader::BaseShader(bool is_draw) : is_draw_(is_draw) {
  vertexs = nullptr;
  fragments = nullptr;
  vertexs_size_ = 0;
  fragments_size_ = 0;
}
BaseShader::~BaseShader() {
  if (vertexs) {
	delete vertexs;
	vertexs = nullptr;
  }
  if (fragments) {
	delete fragments;
	fragments = nullptr;
  }
}
bool BaseShader::OnCreate() {
  CreateShaderProgram();
//	  ShaderUtils::CreateProgram(GetVertexSh().c_str(), GetFragmentSh().c_str(), &vertex_shader_, &fragment_shader_);
  return shader_program_.program() > GL_NONE;
}
void BaseShader::OnChange(int width, int height) {
  this->height = height;
  this->width = width;
  glViewport(0, 0, width, height);
  ResetMatrix();
}
void BaseShader::OnDraw() {
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void BaseShader::Destroy() {
  if (vertexs) {
	delete[] vertexs;
	vertexs = nullptr;
  }
  if (fragments) {
	delete[] fragments;
	fragments = nullptr;
  }
  if (shader_program_.program() > GL_NONE) {
	glDeleteProgram(shader_program_.program());
	shader_program_.set_program(GL_NONE);
  }
}
const std::string BaseShader::GetVertexSh() const {
  return vsh_;
}
const std::string BaseShader::GetFragmentSh() const {
  return fsh_;
}
void BaseShader::ResetMatrix() {

}
void BaseShader::CreateShaderProgram(const std::vector<std::string> &uniform_list,
									 const std::vector<std::string> &attrib_list
) {
  auto compileShader = [](GLenum type, const std::string &shaderSourceStr) -> GLuint {
	GLuint shader = glCreateShader(type);
	const char *shaderSource = shaderSourceStr.c_str();
	glShaderSource(shader, 1, const_cast<const char **>(&shaderSource), NULL);
	glCompileShader(shader);
	GLint isShaderCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isShaderCompiled);
	if (!isShaderCompiled) {
	  GLint infoLogLength = 0;
	  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	  std::vector<char> infoLog(infoLogLength + 1);
	  GLsizei charactersWritten = 0;
	  glGetShaderInfoLog(shader, infoLogLength, &charactersWritten, infoLog.data());
	  std::string msg(infoLog.begin(), infoLog.begin() + charactersWritten);
	  glDeleteShader(shader);
	  throw std::runtime_error("Shader compiling failed: " + msg);
	}
	return shader;
  };

  GLuint vertexShader = 0;
  GLuint fragmentShader = 0;
  GLuint program = 0;

  try {
	vertexShader = compileShader(GL_VERTEX_SHADER, GetVertexSh());
	fragmentShader = compileShader(GL_FRAGMENT_SHADER, GetFragmentSh());

	program = glCreateProgram();
	glAttachShader(program, fragmentShader
	);
	glAttachShader(program, vertexShader
	);
	glLinkProgram(program);
	GLint isLinked = 0;
	glGetProgramiv(program,
				   GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
	  GLint infoLogLength = 0;
	  glGetProgramiv(program,
					 GL_INFO_LOG_LENGTH, &infoLogLength);
	  std::vector<char> infoLog(infoLogLength + 1);
	  GLsizei charactersWritten = 0;
	  glGetProgramInfoLog(program, infoLogLength, &charactersWritten, infoLog
		  .
			  data()
	  );
	  std::string msg(infoLog.begin(), infoLog.begin() + charactersWritten);
	  throw std::runtime_error("Shader program linking failed: " + msg);
	}
  }
  catch (const std::exception &) {
	if (program!=0) {
	  glDeleteProgram(program);
	}
	if (vertexShader!=0) {
	  glDeleteShader(vertexShader);
	}
	if (fragmentShader!=0) {
	  glDeleteShader(fragmentShader);
	}
	throw;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  shader_program_.
	  set_program(program);

  std::for_each(uniform_list.begin(), uniform_list.end(), [this](std::string item) {
	shader_program_.PutUniform(item, glGetUniformLocation(shader_program_.program(), item.c_str()));
  });

  std::for_each(attrib_list.begin(), attrib_list.end(), [this](std::string item) {
	shader_program_.PutAttrib(item, glGetUniformLocation(shader_program_.program(), item.c_str()));
  });
}

