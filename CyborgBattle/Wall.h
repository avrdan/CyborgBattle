#pragma once
#include "AnimationSet.h"
#include "Entity.h"

class Wall : public Entity
{
public:
	Wall(AnimationSet* animSet)
	{
		this->animSet = animSet;
		this->collisionBoxW = 32;
		this->collisionBoxH = 32;
		this->collisionBoxYOffset = -16.0f;
		updateCollisionBox();
		changeAnimation(0, false);
	}

	~Wall() {};

	void update() override;
	void changeAnimation(int newSate, bool resetFrameToBeginning) override;
	void updateCollisions() override;
private:

};

