#pragma once

#include "program.h"


class Mesh {
public:
	Mesh(const GLfloat *vertices, int vertices_size, const GLushort *elements, int elements_size);

	~Mesh();

	void draw(Program *program);

private:
	GLuint vbo, ibo;
	int elements_size;
};



