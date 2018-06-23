#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "viewport.h"

Viewport::Viewport(int screen_width, int screen_height) : screen_width(screen_width), screen_height(screen_height) {}

glm::mat4 Viewport::getMatrix() {
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 2.0, 0.0), glm::vec3(0.0, 0.0, -4.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f * screen_width / screen_height, 0.1f, 10.0f);

	return projection * view;
}

void Viewport::resize(int screen_width, int screen_height) {
	this->screen_width = screen_width;
	this->screen_height = screen_height;
}
