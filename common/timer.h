#pragma once

struct Ticker {
	int elapsed;
	int trigger_time;

	Ticker(int trigger_time): trigger_time(trigger_time) {}

	bool isTriggered(int delta) {
		elapsed += delta;

		if(elapsed > trigger_time) {
			elapsed -= trigger_time;
			return true;
		}

		return false;
	}
};
