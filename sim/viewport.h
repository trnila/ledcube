#pragma once

class Viewport {
public:
	Viewport(int screen_width, int screen_height);

	void resize(int screen_width, int screen_height);

	glm::mat4 getMatrix();

	int getWidth() {
		return screen_width;
	}

	int getHeight() {
		return screen_height;
	}

private:
	int screen_width, screen_height;
};
