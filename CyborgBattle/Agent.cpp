#include "Agent.h"
#include "groupBuilder.h"

Agent::Agent()
{
}


Agent::~Agent()
{
}

void Agent::updateHitBox()
{
	// assume damage is 0 for now
	this->damage = 0;

	if (!this->getCurrentFrame())
		return;

	GroupBox* hitBoxes = static_cast<GroupBox*>(GroupBuilder::findGroupByName("hitBox",
		getCurrentFrame()->getFrameData()));

	if (hitBoxes && hitBoxes->numberOfDataInGroup() > 0)
	{
		// update hitbox
		SDL_Rect hb = hitBoxes->data.front();
		this->hitBox.x = static_cast<int>(x - getCurrentFrame()->getOffset().x + hb.x);
		this->hitBox.y = static_cast<int>(y - getCurrentFrame()->getOffset().y + hb.y);
		this->hitBox.w = hb.w;
		this->hitBox.h = hb.h;
		
		// update damage
		GroupNumber* damages = 
			static_cast<GroupNumber*>(GroupBuilder::findGroupByName("damage", 
				getCurrentFrame()->getFrameData()));

		if (damages && damages->numberOfDataInGroup() > 0)
		{
			this->damage = damages->data.front();
		}
	}
}

void Agent::setInvincibleTimer(float invincileTimer)
{
	this->invincibleTimer = invincibleTimer;
}

void Agent::updateInvincibleTimer()
{
	if (this->invincibleTimer > 0)
	{
		this->invincibleTimer -= TimeController::timeController.dT;
	}
}

void Agent::draw()
{
	if (this->currentFrame && this->active)
	{
		if (this->invincibleTimer > 0 && this->animSet->getWhiteSpriteSheet())
		{
			this->currentFrame->Draw(this->animSet->getWhiteSpriteSheet(), x, y);
		}
		else
		{
			this->currentFrame->Draw(this->animSet->getSpriteSheet(), x, y);
		}
	}

	if (isSolid() && Globals::debugging)
	{
		// last collision box
		SDL_SetRenderDrawColor(Globals::renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &getLastCollisionBox());

		// hit box
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &this->hitBox);
	}
}

int Agent::getHP() const
{
	return this->hp;
}

int Agent::getDamage() const
{
	return this->damage;
}

const SDL_Rect& Agent::getHitBox() const
{
	return this->hitBox;
}
