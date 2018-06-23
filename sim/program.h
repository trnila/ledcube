#pragma once

#include <glm/glm.hpp>


class Program {
public:
	Program(const char *vertex, const char *fragment);

	void setUniform(const char *attr, glm::mat4 coord);

	void setUniform(const char *attr, glm::vec3 vec);

	void activate();

	void configureCoord();

private:
	GLuint program;

	GLint getUniformHandle(const char *attribute_name);

	GLint getAttributeHandle(const char *attribute_name);

	GLuint createShader(const char *filename, GLenum type);
};

