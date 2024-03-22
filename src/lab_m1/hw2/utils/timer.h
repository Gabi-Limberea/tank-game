#pragma once

class Timer {
	float ttl{};
	float maxTime{};

	Timer() {}
public:
	explicit Timer(int maxTime);
	Timer(float time) : ttl(time), maxTime(time) {}

	void Reset();
	void HardReset();
	bool IsDone();
	void Update(float deltaTimeSeconds);
};