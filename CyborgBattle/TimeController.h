#pragma once
#include <iostream>
#include "SDL.h"

class TimeController
{
public:
	static TimeController timeController;
	// reference values
	enum class State {PAUSE = 0, PLAY};

	State timeState = State::PLAY;
	// last time we checked  how many ticks we were up to
	Uint32 lastUpdate = 0;
	// delta time in seconds (time since last frame)
	float dT = 0; 

	TimeController() = default;
	~TimeController() = default;

	// update lastUpdate and dT
	void updateTime();
	void pause();
	void resume();
	void reset();
};

