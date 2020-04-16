#include "KeyboardInput.h"

void KeyboardInput::update(SDL_Event* e)
{
	if (!e || !this->hero)
		return;

	// button presses
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.scancode == DASH)
		{
			this->hero->dash();
		}
		if (e->key.keysym.scancode == SLASH)
		{
			this->hero->slash();
		}
	}

	// button holds
	// check for keys still being held
	const Uint8* keystates = SDL_GetKeyboardState(nullptr);
	// if hero not able to move or no direction buttons are
	// being held down, then stop moving (slide to a halt)
	if (this->hero->isNotAbleToMove() ||
		(!keystates[UP] && !keystates[DOWN] && !keystates[LEFT] && !keystates[RIGHT]))
	{
		this->hero->setMoving(false);
	}
	else
	{
		// ups
		if (keystates[UP])
		{
			if (keystates[RIGHT])
			{
				this->hero->move(270.0f + 45.0f);
			}
			else if (keystates[LEFT])
			{
				this->hero->move(270.0f - 45.0f);
			}
			else
			{
				this->hero->move(270.0f);
			}
		}

		// downs
		if (keystates[DOWN])
		{
			if (keystates[RIGHT])
			{
				this->hero->move(90.0f - 45.0f);
			}
			else if (keystates[LEFT])
			{
				this->hero->move(90.0f + 45.0f);
			}
			else
			{
				this->hero->move(90.0f);
			}
		}

		// left
		if (!keystates[UP] && !keystates[DOWN] && !keystates[RIGHT] && keystates[LEFT])
		{
			this->hero->move(180.0f);
		}

		// right
		if (!keystates[UP] && !keystates[DOWN] && keystates[RIGHT] && !keystates[LEFT])
		{
			this->hero->move(0.0f);
		}
	}
}

Hero* KeyboardInput::getHero() const
{
	return this->hero;
}

void KeyboardInput::setHero(Hero* hero)
{
	if (this->hero)
	{
		delete this->hero;
	}
	this->hero = hero;
}
