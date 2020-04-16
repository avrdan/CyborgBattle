#pragma once
#include "Agent.h"

struct AnimType {
	const std::string& HERO_ANIM_UP				= "up";
	const std::string& HERO_ANIM_DOWN			= "down";
	const std::string& HERO_ANIM_LEFT			= "left";
	const std::string& HERO_ANIM_RIGHT			= "right";
	const std::string& HERO_ANIM_IDLE_UP		= "idleUp";
	const std::string& HERO_ANIM_IDLE_DOWN		= "idleDown";
	const std::string& HERO_ANIM_IDLE_LEFT		= "idleLeft";
	const std::string& HERO_ANIM_IDLE_RIGHT		= "idleRight";

	const std::string& HERO_ANIM_SLASH_UP		= "slashUp";
	const std::string& HERO_ANIM_SLASH_DOWN		= "slashDown";
	const std::string& HERO_ANIM_SLASH_LEFT		= "slashLeft";
	const std::string& HERO_ANIM_SLASH_RIGHT	= "slashRight";

	const std::string& HERO_ANIM_DASH_UP		= "dashUp";
	const std::string& HERO_ANIM_DASH_DOWN		= "dashDown";
	const std::string& HERO_ANIM_DASH_LEFT		= "dashLeft";
	const std::string& HERO_ANIM_DASH_RIGHT		= "dashRight";

	const std::string& HERO_ANIM_DIE		= "die";
};

class Hero :
	public Agent
{
public:
	Hero(AnimationSet* animSet);

	virtual ~Hero() {};

	void update() override;
	void slash();
	void dash();
	void die(); 
	void revive();
	void changeAnimation(int newState, bool resetFrameToBeginning) override;
	void updateAnimation();
	void updateDamages() override;
	bool isNotAbleToMove() const;
private:
	AnimType animType;
};

