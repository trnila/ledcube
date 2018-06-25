#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <functional>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include "program.h"
#include "mesh.h"
#include "led.h"
#include "viewport.h"
#include "data.h"
#include "space.h"
#include "animations.h"
#include "timer.h"
#include "scheduler.h"


std::vector<Led *> leds;
Viewport *viewport;
Mesh *cube;
Program *program;
int prev_time = 0;
Space *space;
Ticker ticker(250);


std::vector<Animation*> animations = {
	new Slide(Axis::X),
	new Slide(Axis::Y),
	new Slide(Axis::Z),
	new Frame(),
	new Random(3),
	new Increment(true),
	new Increment(false),
	new Rain(),
};

Scheduler scheduler(2000);


void init_resources() {
	scheduler.add(animations);

	cube = new Mesh(cube_vertices, sizeof(cube_vertices), cube_elements, sizeof(cube_elements));
	program = new Program("cube.v.glsl", "cube.f.glsl");

	space = new Space(3);
	for (int z = -1; z <= 1; z++) {
		for (int x = -1; x <= 1; x++) {
			for (int y = -1; y <= 1; y++) {
				leds.push_back(new Led(cube, program, glm::vec3(1.0f * x, 1.0f * y, -4.0f + z)));
			}
		}
	}
}

void free_resources() {
	for (auto &obj: leds) {
		delete obj;
	}

	delete cube;
	delete program;
	delete viewport;
}


void onIdle() {
	int now = glutGet(GLUT_ELAPSED_TIME);
	int diff = now - prev_time;
	prev_time = now;

	Animation *current = scheduler.schedule(diff);

	if(ticker.isTriggered(diff)) {
		space->clear();
		current->update(diff, space);

		int i = 0;
		for (int x = 0; x <= 2; x++) {
			for (int y = 0; y <= 2; y++) {
				for (int z = 0; z <= 2; z++) {
					leds[i]->setActive(space->get(x, y, z));
					i++;
				}
			}
		}
	}


	for (auto &obj: leds) {
		obj->update(viewport);
	}

	glutPostRedisplay();
}

void onDisplay() {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto &obj: leds) {
		obj->draw();
	}

	glutSwapBuffers();
}

void onReshape(int width, int height) {
	viewport->resize(width, height);
	glViewport(0, 0, width, height);
}

int main(int argc, char *argv[]) {
	viewport = new Viewport(800, 600);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(viewport->getWidth(), viewport->getHeight());
	glutCreateWindow("Sim");

	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK) {
		fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
		return 1;
	}

	if (!GLEW_VERSION_2_0) {
		fprintf(stderr, "Error: your graphic card does not support OpenGL 2.0\n");
		return 1;
	}

	init_resources();

	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);
	glutIdleFunc(onIdle);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutMainLoop();

	free_resources();

	return 0;
}
