#include "Hero.h"
#include "EntityManager.h"
#include "SoundManager.h"

enum class State {
	HERO_STATE_IDLE,
	HERO_STATE_MOVE,
	HERO_STATE_SLASH,
	HERO_STATE_DASH,
	HERO_STATE_DEAD
};

Hero::Hero(AnimationSet* animSet)
{
	this->animSet = animSet;
	type = "hero";

	// setup default hero values
	this->x = Globals::screenWidth / 2.0f;
	this->y = Globals::screenHeight / 2.0f;
	this->moveSpeed = 0.0f;
	this->moveSpeedMax = 80.0f;
	this->hp = this->hpMax = 20;
	this->damage = 0;
	this->collisionBoxW = 20;
	this->collisionBoxH = 24;
	this->collisionBoxYOffset = -20.0f;

	this->direction = Direction::DOWN;

	changeAnimation(static_cast<int>(State::HERO_STATE_IDLE), true);
	updateCollisionBox();
}

void Hero::update()
{
	// check if dead
	if (this->hp < 1 && this->state != static_cast<int>(State::HERO_STATE_DEAD))
	{
		changeAnimation(static_cast<int>(State::HERO_STATE_DEAD), true);
		this->moving = false;
		die();
	}

	updateCollisionBox();
	updateMovement();
	updateCollisions();

	updateHitBox();
	updateDamages();

	updateAnimation();
	updateInvincibleTimer();
}

void Hero::slash()
{
	if (this->hp > 0 && (this->state == static_cast<int>(State::HERO_STATE_MOVE) ||
		this->state == static_cast<int>(State::HERO_STATE_IDLE)))
	{
		this->moving = false;
		this->frameTimer = 0.0f;
		changeAnimation(static_cast<int>(State::HERO_STATE_SLASH), true);
		SoundManager::getInstance().playSound({ "swing" });
	}
}

void Hero::dash()
{
	if (this->hp > 0 && (this->state == static_cast<int>(State::HERO_STATE_MOVE) ||
		this->state == static_cast<int>(State::HERO_STATE_IDLE)))
	{
		this->moving = false;
		this->frameTimer = 0.0f;

		// push the hero in the direction they are travelling
		this->slideAngle = this->angle;
		this->slideAmount = 300;
		this->invincibleTimer = 0.1f;

		changeAnimation(static_cast<int>(State::HERO_STATE_DASH), true);
		SoundManager::getInstance().playSound({ "dash" });
	}
}

void Hero::die()
{
	this->moving = false;
	changeAnimation(static_cast<int>(State::HERO_STATE_DEAD), true);
}

void Hero::revive()
{
	this->hp = this->hpMax;
	changeAnimation(static_cast<int>(State::HERO_STATE_IDLE), true);
	this->moving = false;
	this->x = Globals::screenWidth / 2.0f;
	this->y = Globals::screenHeight / 2.0f;
	this->slideAmount = 0.0f;
}

void Hero::changeAnimation(int newState, bool resetFrameToBeginning)
{
	//return;
	this->state = newState;
	
	if (this->state == static_cast<int>(State::HERO_STATE_IDLE))
	{
		switch (direction)
		{
		case Direction::UP:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_IDLE_UP);
			break;
		case Direction::DOWN:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_IDLE_DOWN);
			break;
		case Direction::LEFT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_IDLE_LEFT);
			break;
		case Direction::RIGHT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_IDLE_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::HERO_STATE_MOVE))
	{
		switch (direction)
		{
		case Direction::UP:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_UP);
			break;
		case Direction::DOWN:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_DOWN);
			break;
		case Direction::LEFT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_LEFT);
			break;
		case Direction::RIGHT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::HERO_STATE_SLASH))
	{
		switch (direction)
		{
		case Direction::UP:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_SLASH_UP);
			break;
		case Direction::DOWN:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_SLASH_DOWN);
			break;
		case Direction::LEFT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_SLASH_LEFT);
			break;
		case Direction::RIGHT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_SLASH_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::HERO_STATE_DASH))
	{
		switch (direction)
		{
		case Direction::UP:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_DASH_UP);
			break;
		case Direction::DOWN:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_DASH_DOWN);
			break;
		case Direction::LEFT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_DASH_LEFT);
			break;
		case Direction::RIGHT:
			this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_DASH_RIGHT);
			break;
		default:
			this->currentAnim = nullptr;
			break;
		}
	}
	else if (this->state == static_cast<int>(State::HERO_STATE_DEAD))
	{
		this->currentAnim = animSet->getAnimation(animType.HERO_ANIM_DIE);
	}

	if (this->currentAnim)
	{
		if (resetFrameToBeginning)
		{
			this->currentFrame = this->currentAnim->getFrame(0);
		}
		else
		{
			if (this->currentFrame)
			{
				this->currentFrame = this->currentAnim->getFrame(
					this->currentFrame->getFrameNumber());
			}
		}
	}
}

void Hero::updateAnimation()
{
	if (!this->currentFrame || !this->currentAnim)
		return;

	if (this->state == static_cast<int>(State::HERO_STATE_MOVE) && !moving)
	{
		changeAnimation(static_cast<int>(State::HERO_STATE_IDLE), true);
	}

	// if should be showing running animation, change state properly
	if (this->state != static_cast<int>(State::HERO_STATE_MOVE) && moving)
	{
		changeAnimation(static_cast<int>(State::HERO_STATE_MOVE), true);
	}

	this->frameTimer += TimeController::timeController.dT;

	if (this->frameTimer >= this->currentFrame->getDuration())
	{
		// if we are at the end of an animation
		if (this->currentFrame->getFrameNumber() ==
			this->currentAnim->getEndFrameNumber())
		{
			if (this->state == static_cast<int>(State::HERO_STATE_SLASH) ||
				this->state == static_cast<int>(State::HERO_STATE_DASH))
			{
				this->changeAnimation(static_cast<int>(State::HERO_STATE_MOVE), true);
			}
			else if (this->state == static_cast<int>(State::HERO_STATE_DEAD) && this->hp > 0)
			{
				this->changeAnimation(static_cast<int>(State::HERO_STATE_MOVE), true);
			}
			else
			{
				// reset animation (loops it back round)
				this->currentFrame = this->currentAnim->getFrame(0);
			}
		}
		else
		{
			this->currentFrame = this->currentAnim->getNextFrame(this->currentFrame);
		}
		this->frameTimer = 0;
	}
}

void Hero::updateDamages()
{
	if (this->active && this->hp > 0 && this->invincibleTimer <= 0)
	{
		for (const auto& entity : EntityManager::getInstance().getEntities())
		{
			if (!entity)
				continue;

			if (entity->isActive() && entity->getType() == "enemy")
			{
				// we know enemies are agents
				Agent* enemy = static_cast<Agent*>(entity);

				if (enemy->getDamage() > 0 &&
					Globals::checkCollision(this->collisionBox,
						enemy->getHitBox()))
				{
					this->hp -= enemy->getDamage();

					// still alive
					if (this->hp > 0)
					{
						this->invincibleTimer = 0.3f;
						SoundManager::getInstance().playSound({ "hit" });
					}

					this->slideAngle = Globals::angleBetweenTwoEntities(enemy, this);
					this->slideAmount = 200.0f;

				}
			}
		}
	}
}

bool Hero::isNotAbleToMove() const
{
	return getState() != static_cast<int>(State::HERO_STATE_MOVE) &&
		getState() != static_cast<int>(State::HERO_STATE_IDLE);
}
