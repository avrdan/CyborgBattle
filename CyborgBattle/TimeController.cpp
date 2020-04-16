#include "TimeController.h"

// builds a global timeController object
TimeController TimeController::timeController;

void TimeController::updateTime()
{
	if (timeState == State::PAUSE)
	{
		dT = 0;
		return;
	}

	Uint32 timeDiff = SDL_GetTicks() - lastUpdate;
	// keep dT in seconds
	dT = timeDiff / 1000.0f;
	lastUpdate = SDL_GetTicks();
}

void TimeController::pause()
{
	timeState = State::PAUSE;
}

void TimeController::resume()
{
	timeState = State::PLAY;
}

void TimeController::reset()
{
	dT = 0;
	lastUpdate = SDL_GetTicks();
	timeState = State::PLAY;
}
