#include "Game.h"
#include "EntityManager.h"
#include "Glob.h"

Game::Game(const std::string& gameMap, const std::string& texTitle, const std::string& texOverlay)
{
	const std::string& resPath = getResourcePath();
	this->backgroundImg = 
		loadTexture(resPath + gameMap, Globals::renderer);
	this->splashImage =
		loadTexture(resPath + texTitle, Globals::renderer);
	this->overlayImage =
		loadTexture(resPath + texOverlay, Globals::renderer);

	this->splashShowing = true;
	this->overlayTimer = 2.0f;

	// load up sounds
	SoundManager::getInstance().loadSound({ "hit" }, resPath + "Randomize2.wav");
	SoundManager::getInstance().loadSound({ "enemyHit" }, resPath + "Hit_Hurt_9.wav");
	SoundManager::getInstance().loadSound({ "swing" }, resPath + "Randomize21.wav");
	SoundManager::getInstance().loadSound({ "dash" }, resPath + "dash.wav");
	SoundManager::getInstance().loadSound({ "growl" }, resPath + "Randomize34.wav");
	SoundManager::getInstance().loadSound({ "enemyDie" }, resPath + "Randomize41.wav");

	// music
	SoundManager::getInstance().playMusic("Fatal Theory.wav");

	std::list<DataGroupType> dataGroupTypes = getFrameDataGroups();

	this->heroAnimSet = new AnimationSet();
	this->heroAnimSet->loadAnimationSet("udemyCyborg.fdset", 
		dataGroupTypes, true, 0, true);

	this->wallAnimSet = new AnimationSet();
	this->wallAnimSet->loadAnimationSet("wall.fdset", dataGroupTypes);

	this->globAnimSet = new AnimationSet();
	this->globAnimSet->loadAnimationSet("glob.fdset", 
		dataGroupTypes, true, 0, true);

	// build hero entity
	this->hero = new Hero(heroAnimSet);
	this->hero->setInvincibleTimer(0);
	this->hero->setX(Globals::screenWidth / 2.0f);
	this->hero->setX(Globals::screenHeight / 2.0f);
	// tells keyboard input to manage hero
	this->heroInput.setHero(hero);
	// add hero to the entity list
	EntityManager::getInstance().addEntity(hero);

	// build all the walls for this game
	// first, build walls on top and bottom of the screen
	int tileSize = 32;
	for (int i = 0; i < Globals::screenWidth / tileSize; ++i)
	{
		// fills in top row
		Wall* newWall = new Wall(wallAnimSet);
		newWall->setX(i * tileSize + tileSize / 2.0f);
		newWall->setY(tileSize / 2.0f);
		walls.push_back(newWall);
		EntityManager::getInstance().addEntity(newWall);

		// re-using pointer to create bottom row
		newWall = new Wall(wallAnimSet);
		newWall->setX(i * tileSize + tileSize / 2.0f);
		newWall->setY(Globals::screenHeight - tileSize / 2.0f);
		walls.push_back(newWall);
		EntityManager::getInstance().addEntity(newWall);
	}

	// now the sides
	for (int i = 1; i < Globals::screenHeight / tileSize - 1; ++i)
	{
		// fills in left column
		Wall* newWall = new Wall(wallAnimSet);
		newWall->setX(tileSize / 2.0f /*- tileSize / 2.0f*/);
		newWall->setY(i * tileSize + tileSize / 2.0f);
		walls.push_back(newWall);
		EntityManager::getInstance().addEntity(newWall);

		// re-using pointer for right column
		newWall = new Wall(wallAnimSet);
		newWall->setX(Globals::screenWidth - tileSize / 2.0f);
		newWall->setY(i * tileSize + tileSize / 2.0f);
		walls.push_back(newWall);
		EntityManager::getInstance().addEntity(newWall);
	}

}

Game::~Game()
{
	cleanup(this->backgroundImg);
	cleanup(this->splashImage);
	cleanup(this->overlayImage);
	cleanup(this->scoreTexture);

	EntityManager::getInstance().removeAll(false);

	if (this->heroAnimSet)
		delete this->heroAnimSet;
	if (this->wallAnimSet)
		delete this->wallAnimSet;
	if (this->hero)
		delete this->hero;

	// delete all of the wall entities
	EntityManager::getInstance().removeAll(this->walls, true);
	EntityManager::getInstance().removeAll(this->enemies, true);
}

void Game::update()
{
	// enemy related
	int enemiesToBuild = 2;
	int enemiesBuilt = 0;
	float enemyBuildTimer = 1; // seconds
	int tileSize = 32;

	bool quit = false;

	SDL_Event e;
	// setup my time controller before the game starts
	TimeController::timeController.reset();
	// game loop
	while (!quit)
	{
		TimeController::timeController.updateTime();

		EntityManager::getInstance().removeInactiveEntities(false);
		// remove/delete enemies who are dead/inactive
		EntityManager::getInstance().removeInactiveEntities(this->enemies, true);

		// check for any events that might have happened
		while (SDL_PollEvent(&e))
		{
			// close the window
			if (e.type == SDL_QUIT)
				quit = true;

			// if keydown event
			if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE: // esc key
					quit = true;
					break;
				case SDL_SCANCODE_SPACE:
					if (this->splashShowing)
					{
						this->splashShowing = false;
					}

					if (this->overlayTimer <= 0 && this->hero->getHP() < 1)
					{
						// cleanup and restart the game
						enemiesToBuild = 2;
						enemiesBuilt = 0;
						enemyBuildTimer = 3;
						this->overlayTimer = 2;

						Glob::ResetGlobsKilled();
						cleanup(scoreTexture);
						scoreTexture = nullptr;

						for (auto entity : EntityManager::getInstance().getEntities())
						{
							entity->setActive(false);
						}

						hero->revive();
					}
					break;
				default:
					break;
				}
			}

			heroInput.update(&e);
		}

		// make our overlay timer tick down
		if (hero->getHP() < 1 && this->overlayTimer > 0)
		{
			this->overlayTimer -= TimeController::timeController.dT;
		}

		// update all entities
		for (const auto& entity : EntityManager::getInstance().getEntities())
		{
			if (!entity)
				continue;
			// remember how awesome polymorphism is?
			// update all entities in game world at once
			entity->update();
		}

		// spawn enemies
		if (this->hero->getHP() > 0 && !splashShowing)
		{
			if (enemiesToBuild == enemiesBuilt)
			{
				enemiesToBuild *= 2;
				enemiesBuilt = 0;
				enemyBuildTimer = 4;
			}

			enemyBuildTimer -= TimeController::timeController.dT;
			if (enemyBuildTimer <= 0 && enemiesBuilt < enemiesToBuild
				&& enemies.size() < 10)
			{
				Glob* enemy = new Glob(globAnimSet);
				// set enemies position to somewhere random within
				// the arena's open area
				enemy->setX(getRandomNumber(
					Globals::screenWidth - 
					(2*tileSize - tileSize) + tileSize + tileSize / 2));
				enemy->setY(getRandomNumber(
					Globals::screenHeight -
					(2 * tileSize - tileSize) + tileSize + tileSize / 2));
				
				enemy->setInvincibleTimer(0.1f);
				this->enemies.push_back(enemy);
				EntityManager::getInstance().addEntity(enemy);
			}
		}

		// draw all entities
		draw();
	}
}

void Game::draw()
{
	// clear the screen
	SDL_SetRenderDrawColor(Globals::renderer, 145, 133, 129, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(Globals::renderer);

	if (this->splashShowing)
	{
		renderTexture(this->splashImage, Globals::renderer, 0, 0);
	}
	else
	{
		// draw the background
		renderTexture(this->backgroundImg, Globals::renderer, 0, 0);

		// sort all entities based on y (depth)
		EntityManager::getInstance().sortEntities();

		// draw all of the entities
		for (const auto& entity : EntityManager::getInstance().getEntities())
		{
			if (!entity)
				continue;

			entity->draw();
		}

		if (this->overlayTimer <= 0 && hero->getHP() < 1)
		{
			renderTexture(overlayImage, Globals::renderer, 0, 0);
			if (!this->scoreTexture)
			{
				// generate score text
				SDL_Color color = {255, 255, 255, 255}; // white

				std::stringstream ss;
				ss << "Enemies dispatched: " << Glob::GetGlobsKilled();
				const std::string& resPath = getResourcePath();
				this->scoreTexture = renderText(ss.str(),
					resPath + "vermin_vibes_1989.ttf", color, 30, Globals::renderer);
			}

			renderTexture(scoreTexture, Globals::renderer, 20, 50);
		}
	}

	// after we're done drawing / rendering, show it to the screen
	SDL_RenderPresent(Globals::renderer);
}

std::list<DataGroupType> Game::getFrameDataGroups()
{
	// holds a list of group types
	// this list describes the types of groups of data our frames can have
	std::list<DataGroupType> dataGroupTypes;

	// so what data can a frame have?
	// collisionBoxes (although we have hardcoded the collision boxes)
	DataGroupType colBoxType;
	colBoxType.groupName = "collisionBox";
	colBoxType.dataType = DataGroupType::DATATYPE_BOX;

	// hitboxes
	DataGroupType hitBoxType;
	hitBoxType.groupName = "hitBox";
	hitBoxType.dataType = DataGroupType::DATATYPE_BOX;

	// damage
	DataGroupType dmgType;
	dmgType.groupName = "damage";
	dmgType.dataType = DataGroupType::DATATYPE_NUMBER;

	dataGroupTypes.push_back(colBoxType);
	dataGroupTypes.push_back(hitBoxType);
	dataGroupTypes.push_back(dmgType);

	return dataGroupTypes;
}
