#pragma once
#include "Entity.h"
class Agent : public Entity
{
public:
	Agent();
	virtual ~Agent();

	virtual void updateHitBox();
	// how we get damaged
	virtual void updateDamages() = 0;
	virtual void die() = 0;

	virtual void setInvincibleTimer(float invincileTimer);
	virtual void updateInvincibleTimer();

	void draw();

	int getHP() const;
	int getDamage() const;
	const SDL_Rect& getHitBox() const;
protected:
	int hp = 0;
	int hpMax = 0;
	int damage = 0;
	// describes damaging area
	SDL_Rect hitBox;

	// if > 0, can't be hurt
	float invincibleTimer = 0;
private:
};

