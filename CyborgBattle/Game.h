#pragma once
#include "Globals.h"
#include "Hero.h"
#include "Wall.h"
#include "KeyboardInput.h"
#include "drawing_functions.h"
#include "SoundManager.h"

class Game
{
public:
	Game(const std::string& gameMap, const std::string& texTitle, const std::string& texOverlay);
	~Game();

	void update();
	void draw();
private:
	std::list<DataGroupType> getFrameDataGroups();

	AnimationSet* heroAnimSet = nullptr;
	AnimationSet* wallAnimSet = nullptr;
	AnimationSet* globAnimSet = nullptr;
	
	SDL_Texture* backgroundImg = nullptr;
	SDL_Texture* splashImage   = nullptr;
	SDL_Texture* overlayImage  = nullptr;
	SDL_Texture* scoreTexture = nullptr;
	
	Hero* hero = nullptr;
	KeyboardInput heroInput;

	std::list<Entity*> walls;
	std::list<Entity*> enemies;

	bool splashShowing = false;
	float overlayTimer = 0.0f;
};

