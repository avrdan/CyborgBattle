#include "Wall.h"

void Wall::update()
{
	updateCollisionBox();

	if (!this->currentFrame || !this->currentAnim)
		return;

	this->frameTimer += TimeController::timeController.dT;

	if (this->frameTimer >= this->currentFrame->getDuration())
	{
		this->currentFrame = this->currentAnim->getNextFrame(this->currentFrame);
		this->frameTimer = 0;
	}
}

void Wall::changeAnimation(int newSate, bool resetFrameToBeginning)
{
	this->currentAnim  = this->animSet->getAnimation("wall");
	this->currentFrame = this->currentAnim->getFrame(0);
}

void Wall::updateCollisions()
{
	// dont' do anything
	// walls don't move or get pushed in this game
}
