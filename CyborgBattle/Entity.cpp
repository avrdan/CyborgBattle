#include "Entity.h"
#include <algorithm>
#include "Globals.h"
#include "EntityManager.h"


Entity::Entity()
{
}


Entity::~Entity()
{
}

int Entity::getState() const
{
	return this->state;
}

float Entity::getX() const
{
	return this->x;
}

float Entity::getY() const
{
	return this->y;
}

Direction Entity::getDirection() const
{
	return this->direction;
}

bool Entity::isSolid() const
{
	return this->solid;
}

bool Entity::isCollideWithSolids() const
{
	return this->collideWithSolids;
}

bool Entity::isActive() const
{
	return this->active;
}

const std::string& Entity::getType() const
{
	return this->type;
}

bool Entity::isMoving() const
{
	return this->moving;
}

float Entity::getAngle() const
{
	return this->angle;
}

float Entity::getMoveSpeed() const
{
	return this->moveSpeed;
}

float Entity::getMoveSpeedMax() const
{
	return this->moveSpeedMax;
}

float Entity::getSlideAngle() const
{
	return this->slideAngle;
}

float Entity::getSlideAmount() const
{
	return this->slideAmount;
}

float Entity::getMoveLerp() const
{
	return this->moveLerp;
}

float Entity::getTotalXMove() const
{
	return this->totalXMove;
}

float Entity::getTotalYMove() const
{
	return this->totalYMove;
}

const Frame* Entity::getCurrentFrame() const
{
	return this->currentFrame;
}

const SDL_Rect& Entity::getCollisionBox() const
{
	return this->collisionBox;
}

const SDL_Rect& Entity::getLastCollisionBox() const
{
	return this->lastCollisionBox;
}

int Entity::getCollisionBoxW() const
{
	return this->collisionBoxW;
}

int Entity::getCollisionBoxH() const
{
	return this->collisionBoxH;
}

float Entity::getCollisionBoxYOffset() const
{
	return this->collisionBoxYOffset;
}

void Entity::setX(float x)
{
	this->x = x;
}

void Entity::setY(float y)
{
	this->y = y;
}

void Entity::setMoving(bool moving)
{
	this->moving = moving;
}

void Entity::setActive(bool active)
{
	this->active = active;
}

void Entity::draw()
{
	// frame
	if (this->currentFrame && this->active)
	{
		this->currentFrame->Draw(this->animSet->getSpriteSheet(), x, y);
	}

	// collision box
	if (isSolid() && Globals::debugging)
	{
		SDL_SetRenderDrawColor(Globals::renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(Globals::renderer, &(this->collisionBox));
	}
}

void Entity::move(float angle)
{
	this->moving = true;
	this->moveSpeed = this->moveSpeedMax;
	this->angle = angle;

	Direction newDirection = Globals::angleToDirection(this->angle);
	
	// direction changed? update anim
	if (this->direction != newDirection)
	{
		direction = newDirection;
		changeAnimation(this->state, false);
	}
}

void Entity::updateMovement()
{
	updateCollisionBox();
	// store collision box
	this->lastCollisionBox = this->collisionBox;

	// reset total moves
	totalXMove = 0;
	totalYMove = 0;

	if (this->moveSpeed > 0)
	{
		float moveDist = moveSpeed * 
			(TimeController::timeController.dT) * this->moveLerp;

		if (moveDist > 0)
		{
			// dist * cos/sin (angle in radians)
			float xMove = moveDist * std::cosf(this->angle * Globals::PI / 180);
			float yMove = moveDist * std::sinf(this->angle * Globals::PI / 180);
		
			this->x += xMove;
			this->y += yMove;

			this->totalXMove = xMove;
			this->totalYMove = yMove;

			if (!moving)
				moveSpeed -= moveDist;
		}
	}

	// sliding
	if (slideAmount > 0)
	{
		float slideDist = slideAmount * TimeController::timeController.dT * moveLerp;
		if (slideDist > 0)
		{
			float xMove = slideDist * std::cos(slideAngle * Globals::PI / 180);
			float yMove = slideDist * std::sin(slideAngle * Globals::PI / 180);

			x += xMove;
			y += yMove;

			totalXMove += xMove;
			totalYMove += yMove;

			slideAmount -= slideDist;
		}
		else
		{
			slideAmount = 0;
		}
	}

	// move the collision box up to the new position
	updateCollisionBox();
	SDL_UnionRect(&collisionBox, &lastCollisionBox, &collisionBox);
}

void Entity::updateCollisionBox()
{
 	this->collisionBox.x = static_cast<int>(this->x - this->collisionBox.w / 2);
 	this->collisionBox.y = static_cast<int>(this->y + this->collisionBoxYOffset);
	this->collisionBox.w = this->collisionBoxW;
	this->collisionBox.h = this->collisionBoxH;
}

void Entity::updateCollisions()
{
	// only check for collisions when moving
	if (isActive() && isCollideWithSolids() && 
		(this->moveSpeed > 0 || slideAmount > 0))
	{
		// list of potential collisions
		std::list<Entity*> collisions;

		for (const auto& entity : EntityManager::getInstance().getEntities())
		{
			if (!entity)
				continue;
			// if entity collides with current union collision box
			if (entity->isActive() && entity->getType() != getType() &&
				entity->isSolid() &&
				Globals::checkCollision(collisionBox, entity->getCollisionBox()))
			{
				collisions.push_back(entity);
			}
		}
		
		if (collisions.empty())
			return;

		updateCollisionBox();
		// multisample check for collisions between source and dest

		// find sample distance between checks (use quarter distance)
		float boxTravelSize = 0;
		if (collisionBox.w < collisionBox.h)
		{
			boxTravelSize = collisionBox.w / 4;
		}
		else
		{
			boxTravelSize = collisionBox.h / 4;
		}

		// use sampleBox to check for collisions from start point to end point
		// moving at boxTravelSize each sample
		SDL_Rect sampleBox = this->lastCollisionBox;
		float collisionMoveX = sampleBox.x;
		float collisionMoveY = sampleBox.y;
		float movementAngle = 
			Globals::angleBetweenTwoRects(this->lastCollisionBox, this->collisionBox);

		bool foundCollision = false;
		while (!foundCollision)
		{
			// check for collisions in current position
			SDL_Rect intersection;
			for (const auto& entity : collisions)
			{
				if (!entity)
					continue;
				if (SDL_IntersectRect(&sampleBox, &entity->getCollisionBox(), &intersection))
				{
					foundCollision = true;
					this->moveSpeed = 0;
					this->moving = false;
					this->slideAngle = Globals::angleBetweenTwoEntities(entity, this);

					// currently intersecting entity - need to do collision resolution
					if (intersection.w < intersection.h)
					{
						if (this->lastCollisionBox.x + this->lastCollisionBox.w / 2 < 
							entity->getCollisionBox().x + entity->getCollisionBox().w/2)
						{ // started on left, move left out of collision
							sampleBox.x -= intersection.w;
						}
						else
						{ // started on right
							sampleBox.x += intersection.w;
						}
					}
					else
					{
						if (this->lastCollisionBox.y + this->lastCollisionBox.h / 2 <
							entity->getCollisionBox().y + entity->getCollisionBox().h / 2)
						{   // started above, move up out of collision
							sampleBox.y -= intersection.h;
						}
						else
						{   // otherwise, started below
							sampleBox.y += intersection.y;
						}
					}
				}
			}

			// if collision found or sampleBox box is at the same place as collision box, exit loop
			if (foundCollision || 
				(sampleBox.x >= this->collisionBox.x - 1 && sampleBox.x <= this->collisionBox.x + 1) && 
				(sampleBox.y >= this->collisionBox.y - 1 && sampleBox.y <= this->collisionBox.y + 1))
			{
				break;
			}

			// move to next spot
			if (Globals::distanceBetweenTwoRects(sampleBox, collisionBox) > boxTravelSize)
			{
				movementAngle = Globals::angleBetweenTwoRects(sampleBox, this->collisionBox);
				float xMove = boxTravelSize * std::cosf(movementAngle* Globals::PI / 180);
				float yMove = boxTravelSize * std::sinf(movementAngle * Globals::PI / 180);

				collisionMoveX += xMove;
				collisionMoveY += yMove;
				sampleBox.x = static_cast<int>(collisionMoveX);
				sampleBox.y = static_cast<int>(collisionMoveY);
			}
			else
			{
				sampleBox = this->collisionBox;
			}
		}

		if (foundCollision)
		{
			// move out entity to where the sample box ended up
			slideAmount /= 2;
			this->x = sampleBox.x + sampleBox.w / 2;
			this->y = sampleBox.y - collisionBoxYOffset;
		}

		updateCollisionBox();
	}
}
