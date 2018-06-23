#include <glm/glm.hpp>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include "program.h"
#include "errors.h"
#include "mapped_file.h"


Program::Program(const char *vertex, const char *fragment) {
	GL_CHECK(program = glCreateProgram());
	GLuint shader = createShader(vertex, GL_VERTEX_SHADER);
	GL_CHECK(glAttachShader(program, shader));

	shader = createShader(fragment, GL_FRAGMENT_SHADER);
	GL_CHECK(glAttachShader(program, shader));

	GLint link_ok = GL_FALSE;
	GL_CHECK(glLinkProgram(program));
	GL_CHECK(glGetProgramiv(program, GL_LINK_STATUS, &link_ok));
	if (!link_ok) {
		fprintf(stderr, "glLinkProgram:");
		fprintf(stderr, "%s", getGLLog(program).c_str());
		return;
	}

}

void Program::setUniform(const char *attr, glm::mat4 coord) {
	activate();
	GL_CHECK(glUniformMatrix4fv(getUniformHandle(attr), 1, GL_FALSE, glm::value_ptr(coord)));
}

void Program::setUniform(const char *attr, glm::vec3 vec) {
	activate();
	GL_CHECK(glUniform3fv(getUniformHandle(attr), 1, glm::value_ptr(vec)));
}

void Program::activate() {
	GL_CHECK(glUseProgram(program));
}

void Program::configureCoord() {
	GL_CHECK(glVertexAttribPointer(
			getAttributeHandle("coord3d"), // attribute
			3,                 // number of elements per vertex, here (x,y,z)
			GL_FLOAT,          // the type of each element
			GL_FALSE,          // take our values as-is
			0,                 // no extra data between each position
			0                  // offset of first element
	));

}

GLint Program::getUniformHandle(const char *attribute_name) {
	GLint handle = glGetUniformLocation(program, attribute_name);
	if (handle == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", attribute_name);
		return -1;
	}

	return handle;
}

GLint Program::getAttributeHandle(const char *attribute_name) {
	GLint handle = glGetAttribLocation(program, attribute_name);
	if (handle == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return -1;
	}
	GL_CHECK(glEnableVertexAttribArray((GLuint) handle));

	return handle;
}

GLuint Program::createShader(const char *filename, GLenum type) {
	MappedFile source(filename);

	GLuint res;
	GL_CHECK(res = glCreateShader(type));
	const GLchar* sources[] = {
			// Define GLSL version
#ifdef GL_ES_VERSION_2_0
			"#version 100\n"  // OpenGL ES 2.0
#else
			"#version 120\n"  // OpenGL 2.1
#endif
			,
			// GLES2 precision specifiers
#ifdef GL_ES_VERSION_2_0
	// Define default float precision for fragment shaders:
        (type == GL_FRAGMENT_SHADER) ?
          "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
          "precision highp float;           \n"
          "#else                            \n"
          "precision mediump float;         \n"
          "#endif                           \n"
          : ""
          // Note: OpenGL ES automatically defines this:
          // #define GL_ES
#else
			// Ignore GLES 2 precision specifiers:
			"#define lowp   \n"
			"#define mediump\n"
			"#define highp  \n"
#endif
			, source.get_content() };
	GL_CHECK(glShaderSource(res, sizeof(sources) / sizeof(*sources), sources, nullptr));

	GL_CHECK(glCompileShader(res));
	GLint compile_ok = GL_FALSE;
	GL_CHECK(glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok));
	if (compile_ok == GL_FALSE) {
		fprintf(stderr, "%s: %s", filename, getGLLog(res).c_str());
		GL_CHECK(glDeleteShader(res));
		return 0;
	}

	return res;
}
