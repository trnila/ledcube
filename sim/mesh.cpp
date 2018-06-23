#include <GL/glew.h>
#include "mesh.h"
#include "errors.h"
#include "program.h"

Mesh::Mesh(const GLfloat *vertices, int vertices_size, const GLushort *elements, int elements_size) : elements_size(
		elements_size) {
	// vertices
	GL_CHECK(glGenBuffers(1, &vbo));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW));

	// elements
	GL_CHECK(glGenBuffers(1, &ibo));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_size, elements, GL_STATIC_DRAW));
}

Mesh::~Mesh() {
	GL_CHECK(glDeleteBuffers(1, &vbo));
	GL_CHECK(glDeleteBuffers(1, &ibo));
}

void Mesh::draw(Program *program) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	program->configureCoord();

	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	glDrawElements(GL_TRIANGLES, elements_size, GL_UNSIGNED_SHORT, 0);
}
