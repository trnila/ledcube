#pragma once

#include "viewport.h"
#include "mesh.h"
#include "program.h"


class Led {
public:
	Led(Mesh *mesh, Program *program, glm::vec3 pos);

	void update(Viewport *viewport);

	void draw();
	void setColor(uint8_t r, uint8_t g, uint8_t b);
	void setActive(bool active) {
		this->active = active;
	}

private:
	Mesh *mesh;
	Program *program;
	glm::vec3 pos;
	glm::mat4 mvp;
	glm::vec3 color;
	bool active;
};


