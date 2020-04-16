#pragma once
#include "TimeController.h"
#include "AnimationSet.h"
#include "Animation.h"
#include "Frame.h"

enum class Direction { UP, DOWN, LEFT, RIGHT, NONE };

class Entity
{
public:
	Entity();
	~Entity();

	int         getState() const;
	float		getX() const;
	float       getY() const;
	Direction	getDirection() const;
	bool		isSolid() const;
	bool		isCollideWithSolids() const;
	bool		isActive() const;
	const		std::string& getType() const;
	bool		isMoving() const;
	float		getAngle() const;
	float		getMoveSpeed() const;
	float		getMoveSpeedMax() const;
	float		getSlideAngle() const;
	float		getSlideAmount() const;
	float		getMoveLerp() const;
	float		getTotalXMove() const;
	float		getTotalYMove() const;
	const Frame*		getCurrentFrame() const;

	const SDL_Rect& getCollisionBox() const;
	const SDL_Rect& getLastCollisionBox() const;
	
	int      getCollisionBoxW() const;
	int      getCollisionBoxH() const;
	float    getCollisionBoxYOffset() const;

	void setX(float x);
	void setY(float y);

	void setMoving(bool moving);
	void setActive(bool active);

	// virtual functions
	virtual void update() {};
	virtual void draw();

	virtual void move(float angle);
	virtual void updateMovement();
	virtual void updateCollisionBox();
	virtual void changeAnimation(int newState, bool resetToBeggining) = 0;
	virtual void updateCollisions();
protected:
	int state;

	float x, y;
	Direction direction;
	// can things pass through me
	bool solid = true;
	bool collideWithSolids = true;
	bool active = true;
	std::string type = "entity";
	bool moving;
	// 360 degree
	float angle = 0;
	float moveSpeed = 0;
	float moveSpeedMax = 0;
	// direction pushed in
	float slideAngle = 0;
	float slideAmount = 0;
	float moveLerp = 4;
	// keeps track of total x,y movement per movement turn
	float totalXMove = 0, totalYMove = 0;

	SDL_Rect collisionBox;
	SDL_Rect lastCollisionBox;
	// default sizes
	int collisionBoxW = 0, collisionBoxH = 0;
	// from the entity's Y value, where should we set the collision box
	float collisionBoxYOffset = 0;

	// set of all animations that it can have
	AnimationSet* animSet = nullptr;
	Animation*    currentAnim = nullptr;
	Frame* currentFrame = nullptr;
	float frameTimer;
};

