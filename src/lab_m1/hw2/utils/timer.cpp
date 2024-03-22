#include "timer.h"
#include <cstdlib>
#include <random>

Timer::Timer(int maxTime) {
	this->maxTime = float(maxTime);

	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_real_distribution<> distr(0, this->maxTime);

	ttl = distr(generator);
}

void Timer::Reset() {
	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_real_distribution<float> distr(0, maxTime);

	ttl = distr(generator);
}

bool Timer::IsDone() {
	return ttl <= 0;
}

void Timer::Update(float deltaTimeSeconds) {
	ttl -= deltaTimeSeconds;
}

void Timer::HardReset() {
	ttl = maxTime;
}