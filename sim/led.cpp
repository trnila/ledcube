#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/freeglut.h>
#include "led.h"
#include "mesh.h"
#include "program.h"


Led::Led(Mesh *mesh, Program *program, glm::vec3 pos) : mesh(mesh), program(program), pos(pos), color(0, 0, 0), active(false) {}

void Led::update(Viewport *viewport) {
	if(!active) {
//		return;
	}

	float angle = glutGet(GLUT_ELAPSED_TIME) / 1000.0 * 45;  // 45° per second
	glm::vec3 axis_y(0, 1, 0);
	glm::mat4 anim = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis_y);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.0f));
	model *= anim;
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 4.0f));

	model = glm::translate(model, pos);
	model = glm::scale(model, glm::vec3(0.1f));

	mvp = viewport->getMatrix() * model;
}

void Led::draw() {
	if(!active) {
//		return;
	}

	color = active ? glm::vec3(1.0f, 0, 0) : glm::vec3(0, 1.0f, 0);

	program->setUniform("mvp", mvp);
	program->setUniform("color", color);

	mesh->draw(program);
}

void Led::setColor(uint8_t r, uint8_t g, uint8_t b) {
	color = glm::vec3(r / 255.0, g / 255.0, b / 255.0);
}
