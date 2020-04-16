#pragma once
#include "Agent.h"
#include <cstdlib>
#include "AnimationSet.h"

struct GlobAnimType {
	const std::string& GLOB_ANIM_UP    = "up";
	const std::string& GLOB_ANIM_DOWN  = "down";
	const std::string& GLOB_ANIM_LEFT  = "left";
	const std::string& GLOB_ANIM_RIGHT = "right";

	const std::string& GLOB_ANIM_IDLE_UP    = "idleUp";
	const std::string& GLOB_ANIM_IDLE_DOWN  = "idleDown";
	const std::string& GLOB_ANIM_IDLE_LEFT  = "idleLeft";
	const std::string& GLOB_ANIM_IDLE_RIGHT = "idleRight";
					   
	const std::string& GLOB_ANIM_ATTACK_UP    = "attackUp";
	const std::string& GLOB_ANIM_ATTACK_DOWN  = "attackDown";
	const std::string& GLOB_ANIM_ATTACK_LEFT  = "attackLeft";
	const std::string& GLOB_ANIM_ATTACK_RIGHT = "attackRight";
					   
	const std::string& GLOB_ANIM_TELEGRAPH_UP    = "telegraphUp";
	const std::string& GLOB_ANIM_TELEGRAPH_DOWN  = "telegraphDown";
	const std::string& GLOB_ANIM_TELEGRAPH_LEFT  = "telegraphLeft";
	const std::string& GLOB_ANIM_TELEGRAPH_RIGHT = "telegraphRight";
					   
	const std::string& GLOB_ANIM_DIE = "die";
};

enum class StateAI
{
	GLOB_AI_NORMAL,
	GLOB_AI_CHASE
};

class Glob : public Agent
{
public:
	Glob(AnimationSet* animSet);
	virtual ~Glob();

	static int GetGlobsKilled() { return globsKilled; }
	static void ResetGlobsKilled() { globsKilled = 0; }

	void update() override;
	void die() override;
	void updateDamages() override;
	void changeAnimation(int newState, bool resetFrameToBeggining) override;
	void think();
	void telegraph();
	void attack();
	void findNearestTarget();
	void updateAnimation();
private:
	GlobAnimType animType;

	// when this is 0, choose to do an action
	float thinkTimer = 0;
	// who should I chase
	Agent* target = nullptr;

	StateAI aiState = StateAI::GLOB_AI_NORMAL;

	static int globsKilled;
};

