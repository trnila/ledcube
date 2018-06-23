#pragma once

#include <string>
#include <GL/glew.h>

#define GL_CHECK(cmd) do { \
  checkOpenglError("before " #cmd, __FILE__, __LINE__); \
  cmd; \
  checkOpenglError(#cmd, __FILE__, __LINE__); \
} while(0);


void checkOpenglError(const char *cmd, const char *file, int line);

std::string getGLLog(GLuint id);
