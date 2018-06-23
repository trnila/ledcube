#pragma once

#include <memory.h>
#include <assert.h>

class Space {
public:
	Space(unsigned int dim): dim(dim) {
		space = new char[dim * dim * dim];
		clear();
	}

	~Space() {
		delete []space;
	}

	void clear() {
		memset(space, 0, dim * dim * dim);
	}

	void set(int x, int y, int z, char val) {
		int i = index(x, y, z);
		assert(i < dim * dim * dim);

		space[i] = val;
	}

	char get(int x, int y, int z) {
		int i = index(x, y, z);
		assert(i < dim * dim * dim);

		return space[i];
	}

	int getDim() {
		return dim;
	}

	void copy(Space* another) {
		assert(dim == another->dim);

		memcpy(space, another->space, dim * dim * dim);
	}


private:
	unsigned int dim;
	char *space;

	int index(int x, int y, int z) {
		return z * (dim * dim) +
		       y * dim +
		       x;
	}
};
