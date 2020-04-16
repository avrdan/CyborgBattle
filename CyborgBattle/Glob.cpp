#include "Glob.h"
#include "EntityManager.h"
#include "SoundManager.h"

enum class State {
	GLOB_STATE_IDLE,
	GLOB_STATE_MOVE,
	GLOB_STATE_ATTACK,
	GLOB_STATE_TELEGRAPH,
	GLOB_STATE_DEAD
};

int Glob::globsKilled = 0;

Glob::Glob(AnimationSet* animSet)
{
	this->animSet = animSet;
	this->type = "enemy";

	// defaults
	this->x = Globals::screenWidth / 2.0f;
	this->y = Globals::screenHeight / 2.0f;
	this->moveSpeed = 0.0f;
	this->moveSpeedMax = 20.0f;
	this->hp = this->hpMax = 10 + (std::rand() % 20); // 10-29

	this->collisionBoxW  = 18;
	this->collisionBoxH  = 20;
	this->collisionBox.w = this->collisionBoxW;
	this->collisionBox.h = this->collisionBoxH;

	this->collisionBoxYOffset = -14;

	this->direction = Direction::DOWN;
	changeAnimation(static_cast<int>(State::GLOB_STATE_IDLE), true);
}


Glob::~Glob()
{
}

void Glob::update()
{
	// check if died
	if (this->hp < 1 && state != static_cast<int>(State::GLOB_STATE_DEAD))
	{
		changeAnimation(static_cast<int>(State::GLOB_STATE_DEAD), true);
		this->moving = false;
		die();
	}

	think();
	updateCollisionBox();
	updateMovement();
	updateCollisions();
	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}

void Glob::die()
{
	this->moving = false;
	this->state = static_cast<int>(State::GLOB_STATE_DEAD);
	changeAnimation(static_cast<int>(this->state), true);
	SoundManager::getInstance().playSound({ "enemyDie" });

	// add to our score count
	++Glob::globsKilled;
}

void Glob::updateDamages()
{
	if (this->active && this->hp > 0 && this->invincibleTimer <= 0)
	{
		for (const auto& entity : EntityManager::getInstance().getEntities())
		{
			if (!entity)
				continue;
			if (entity->isActive() && entity->getType() == "hero")
			{
				Agent* enemy = static_cast<Agent*>(entity);
				if (enemy->getDamage() > 0 
					&& Globals::checkCollision(
						this->collisionBox, enemy->getHitBox()))
				{
					this->hp -= enemy->getDamage();
					if (this->hp > 0)
					{
						SoundManager::getInstance().playSound({ "enemyHit" });
						this->invincibleTimer = 0.1f;
					}
					// angle from other entity, towards this entity
					this->slideAngle = Globals::angleBetweenTwoEntities(enemy, this);
					this->slideAmount = 300.0f;
				}
			}
		}
	}
}

void Glob::changeAnimation(int newState, bool resetFrameToBeggining)
{
	this->state = newState;
	if (this->state == static_cast<int>(State::GLOB_STATE_IDLE))
	{
		switch (this->direction)
		{
		case Direction::DOWN:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_DOWN);
			break;
		case Direction::UP:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_UP);
			break;
		case Direction::LEFT:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_LEFT);
			break;
		case Direction::RIGHT:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::GLOB_STATE_MOVE))
	{
		switch (this->direction)
		{
		case Direction::DOWN:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_DOWN);
			break;
		case Direction::UP:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_UP);
			break;
		case Direction::LEFT:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_LEFT);
			break;
		case Direction::RIGHT:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_IDLE_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::GLOB_STATE_ATTACK))
	{
		switch (this->direction)
		{
		case Direction::DOWN:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_ATTACK_DOWN);
			break;															   
		case Direction::UP:													   
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_ATTACK_UP);
			break;															   
		case Direction::LEFT:												   
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_ATTACK_LEFT);
			break;															   
		case Direction::RIGHT:												  
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_ATTACK_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::GLOB_STATE_TELEGRAPH))
	{
		switch (this->direction)
		{
		case Direction::DOWN:
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_TELEGRAPH_DOWN);
			break;															   
		case Direction::UP:													   
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_TELEGRAPH_UP);
			break;															   
		case Direction::LEFT:												   
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_TELEGRAPH_LEFT);
			break;															   
		case Direction::RIGHT:												   
			this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_TELEGRAPH_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::GLOB_STATE_DEAD))
	{
		this->currentAnim = this->animSet->getAnimation(animType.GLOB_ANIM_DIE);
	}

	if (this->currentAnim)
	{
		if (resetFrameToBeggining)
		{
			this->currentFrame = this->currentAnim->getFrame(0);
		}
		else if (this->currentFrame)
		{
			this->currentFrame = this->currentAnim->getFrame(this->currentFrame->getFrameNumber());
		}
	}
}

void Glob::think()
{
	if (this->state == static_cast<int>(State::GLOB_STATE_IDLE) ||
		this->state == static_cast<int>(State::GLOB_STATE_MOVE))
	{
		this->thinkTimer -= TimeController::timeController.dT;
		// time to choose an action
		if (this->thinkTimer <= 0)
		{
			// reset the timer
			this->thinkTimer = std::rand() % 5; // 0-4 seconds
			int action = std::rand() % 10; // 0-9

			if (action < 3)
			{
				this->moving = false;
				//aiState = GLOB_AI_NORMAL;
				changeAnimation(static_cast<int>(State::GLOB_STATE_IDLE), true);
			}
			else
			{
				findNearestTarget();

				// found a living target
				if (this->target && this->target->getHP() > 0)
				{
					float dist = Globals::distanceBetweenTwoEntities(this, target);
					// if in range, ATTACK!
					if (dist < 100)
					{
						// telegraph our attack first so that
						// players have a chance to dodge
						telegraph();
						this->aiState = StateAI::GLOB_AI_NORMAL;
					}
					else
					{
						// otherwise move up to the player / target
						this->aiState = StateAI::GLOB_AI_CHASE;
						this->moving = true;
						changeAnimation(static_cast<int>(State::GLOB_STATE_MOVE), 
							this->state != static_cast<int>(State::GLOB_STATE_MOVE));
					}
				}
				else
				{
					// no targets, go idle
					this->moving = false;
					this->aiState = StateAI::GLOB_AI_NORMAL;
					changeAnimation(static_cast<int>(State::GLOB_STATE_IDLE), true);
				}
			}

			// if chasing a target
			if (this->aiState == StateAI::GLOB_AI_CHASE && 
				this->hp > 0 && this->active && this->target)
			{
				angle = Globals::angleBetweenTwoEntities(this, this->target);
				move(angle);
			}
		}
	}
}

void Glob::telegraph()
{
	this->moving = false;
	this->frameTimer = 0;
	changeAnimation(static_cast<int>(State::GLOB_STATE_TELEGRAPH), true);
}

void Glob::attack()
{
	this->moving = false;
	this->frameTimer = 0;
	this->slideAmount = 100;
	this->slideAngle = this->angle;
	changeAnimation(static_cast<int>(State::GLOB_STATE_ATTACK), true);
}

void Glob::findNearestTarget()
{
	this->target = nullptr;
	// find closest target
	for (const auto& entity : EntityManager::getInstance().getEntities())
	{
		if (!entity)
			continue;
		if (entity->getType() == "hero" && entity->isActive())
		{
			// if found first hero in list, just set them as a target
			// for now
			if (!target)
			{
				target = static_cast<Agent*>(entity);
			}
			else
			{
				// is the other hero closer to the previous target?
				float distNewEntity  = Globals::distanceBetweenTwoEntities(this, entity);
				float distCurrTarget = Globals::distanceBetweenTwoEntities(this, target);
				if (distNewEntity < distCurrTarget)
				{
					target = static_cast<Agent*>(entity);
				}
			}
		}
	}
}

void Glob::updateAnimation()
{
	if (!this->currentFrame || !this->currentAnim)
	{
		return;
	}

	// if in move state, but not actually going anywhere
	if (this->state == static_cast<int>(State::GLOB_STATE_MOVE) &&
		!this->moving)
	{
		changeAnimation(static_cast<int>(State::GLOB_STATE_IDLE), true);
	}

	// if in idle state, but actually moving
	if (this->state != static_cast<int>(State::GLOB_STATE_MOVE) &&
		this->moving)
	{
		changeAnimation(static_cast<int>(State::GLOB_STATE_IDLE), true);
	}

	this->frameTimer += TimeController::timeController.dT;

	// time to change frames
	if (this->frameTimer > this->currentFrame->getDuration())
	{
		// if at the end of the animation
		if (this->currentFrame->getFrameNumber() ==
			this->currentAnim->getEndFrameNumber())
		{
			if (this->state == static_cast<int>(State::GLOB_STATE_TELEGRAPH))
			{
				// done telegraphing, now attack
				attack();
			}
			else if (this->state == static_cast<int>(State::GLOB_STATE_ATTACK))
			{
				changeAnimation(static_cast<int>(State::GLOB_STATE_MOVE), true);
			}
			else if (this->state == static_cast<int>(State::GLOB_STATE_DEAD))
			{
				this->frameTimer = 0;
				// if alive again, change back to moving
				if (this->hp > 0)
				{
					changeAnimation(static_cast<int>(State::GLOB_STATE_MOVE), true);
				}
				else
				{
					this->active = false;
				}
			}
			else
			{
				// loop animation
				this->currentFrame = this->currentAnim->getFrame(0);
			}
		}
		else
		{
			// move to the next frame
			this->currentFrame = 
				this->currentAnim->getNextFrame(this->currentFrame);
		}

		this->frameTimer = 0;
	}
}
