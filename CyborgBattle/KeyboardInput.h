#pragma once
#include "Hero.h"

class KeyboardInput
{
public:
	KeyboardInput()
		// hardcoded keyboard buttons
		:UP(SDL_SCANCODE_UP)
		,DOWN(SDL_SCANCODE_DOWN)
		,LEFT(SDL_SCANCODE_LEFT)
		,RIGHT(SDL_SCANCODE_RIGHT)
		,SLASH(SDL_SCANCODE_Z)
		,DASH(SDL_SCANCODE_X)
		// TODO: load scancodes up from a textfile (gives ability to customize)
	{}

	~KeyboardInput() {};

	void update(SDL_Event* e);

	Hero* getHero() const;
	void setHero(Hero* hero);
private:
	Hero* hero = nullptr;
	SDL_Scancode UP, DOWN, LEFT, RIGHT;
	SDL_Scancode SLASH, DASH;
};

