#pragma once

#include "space.h"

class Animation {
public:
	virtual void update(int diff, Space *space) = 0;
};
