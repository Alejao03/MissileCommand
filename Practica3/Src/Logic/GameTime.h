#include <cstdint>

#ifndef GAME_TIME_H
#define GAME_TIME_H

struct GameTime
{
private:
	float _deltaTime = 0;
	uint64_t _previousTime = 0;
public:
	float getDeltaTime() { return _deltaTime; }

	float updateTimeRegistry(uint64_t currentTime, uint64_t timeF) {
		_deltaTime = (currentTime - _previousTime) / (float)(timeF);
		_previousTime = currentTime; // Actualiza el tiempo de inicio del frame
		return _deltaTime;
	}
};
# endif