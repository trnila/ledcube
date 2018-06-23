#include "errors.h"

void checkOpenglError(const char *cmd, const char *file, int line) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		printf("OpenGL error: %d %s:%d %s\n", err, file, line, cmd);
		abort();
	}
}

std::string getGLLog(GLuint id) {
	GLint log_length = 0;
	if (glIsShader(id)) {
		GL_CHECK(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length));
	} else if (glIsProgram(id)) {
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &log_length);
	}

	char *log = new char[log_length];

	if (glIsShader(id)) {
		glGetShaderInfoLog(id, log_length, nullptr, log);
	} else if (glIsProgram(id)) {
		glGetProgramInfoLog(id, log_length, nullptr, log);
	} else {
		// TODO: unknown error?
	}

	std::string result = std::string(log);
	delete[] log;
	return result;
}
