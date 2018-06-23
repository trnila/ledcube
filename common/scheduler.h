#pragma once

#include <vector>
#include "animation.h"
#include "timer.h"


class Scheduler {
public:
	Scheduler(int quantum): curr(0), changeAnimation(quantum) {}

	void add(Animation *animation) {
		animations.emplace_back(animation);
	}

	void add(std::vector<Animation*> src) {
		for(auto &anim: src) {
			add(anim);
		}
	}

	Animation* schedule(int delta) {
		assert(animations.size() > 0);

		if(changeAnimation.isTriggered(delta)) {
			curr = (curr + 1) % animations.size();
		}

		return animations[curr];
	}

private:
	std::vector<Animation*> animations;
	int curr;

	Ticker changeAnimation;
};