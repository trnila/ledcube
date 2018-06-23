#include "animation.h"

class Random: public Animation {
public:
	explicit Random(int dim): space(dim) {}

	void update(int delta, Space *fb) {
		const int D = space.getDim();
		space.set(rand() % D, rand() % D, rand() % D, rand() % 2);

		fb->copy(&space);
	}

private:
	Space space;
};

class Increment: public Animation {
public:
	Increment(bool fill): x(-1), y(0), z(0), fill(fill), space(3) {}

	void update(int delta, Space *dst) {
		if(!fill && x >= 0) {
			space.set(x, y, z, 0);
		}

		x++;
		if(x >= space.getDim()) {
			x = 0;
			y++;
			if(y >= space.getDim()) {
				y = 0;
				z++;
				if(z >= space.getDim()) {
					z = 0;
					space.clear();
				}
			}
		}
		space.set(x, y, z, 1);
		dst->copy(&space);
	}

private:
	int x, y, z;
	bool fill;
	Space space;
};


class Frame: public Animation {
public:
	void update(int delta, Space *space) {
		const int coords[][2] = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {2, 1}, {2, 0}, {1, 0}};

		for(int j = 0; j < 3; j++) {
			space->set(coords[ii][0], coords[ii][1], j, 1);
			space->set(2 - coords[ii][0], 2 - coords[ii][1], j, 1);
			space->set(1, 1, j, 1);
		}

		ii++;
		if(ii >= sizeof(coords) / sizeof(*coords)) {
			ii = 0;
		}
	}

private:
	int ii = 0;
};

enum class Axis {
	X,
	Y,
	Z
};

class Slide: public Animation {
public:
	explicit Slide(Axis axe): axe(axe) {}

	void update(int diff, Space *space) {

		for(int a = 0; a < space->getDim(); a++) {
			for(int b = 0; b < space->getDim(); b++) {
				switch(axe) {
					case Axis::X:
						space->set(cur, a, b, 1);
						break;
					case Axis::Y:
						space->set(a, cur, b, 1);
						break;
					case Axis::Z:
						space->set(a, b, cur, 1);
						break;
				}
			}
		}

		int next = cur + dir;
		if(next >= space->getDim() || next < 0) {
			dir *= -1;
		}
		cur += dir;
	}

private:
	int cur = 0;
	int dir = 1;
	Axis axe = Axis::Y;
};

class Rain: public Animation {
public:
	Rain(){
		memset(tears, -1, sizeof(tears));
	}

	void update(int diff, Space *space) {
		int choosen = rand() % 9;
		if(tears[choosen] <= 0) {
			tears[choosen] = 3;
		}

		for(int i = 0; i < sizeof(tears) / sizeof(*tears); i++) {
			if(tears[i] > 0) {
				tears[i]--;
				space->set(i / 3, i % 3, tears[i], 1);
			}
		}
	}
private:
	int tears[9];
};
