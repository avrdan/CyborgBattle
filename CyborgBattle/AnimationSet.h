#pragma once
#include "Animation.h"
#include <list>
#include "dataGroupType.h"

class AnimationSet
{
public:
	AnimationSet() = default;
	~AnimationSet() {
		if (spriteSheet)
			cleanup(spriteSheet);
		if (whiteSpriteSheet)
			cleanup(whiteSpriteSheet);
	};

	Animation* getAnimation(const std::string& name);
	void loadAnimationSet(const std::string& fileName, 
		std::list<DataGroupType>& groupTypes, bool setColorKey = false, 
		int transparentPixelIndex = 0, bool createWhiteTexture = false);

	SDL_Texture* getSpriteSheet();
	SDL_Texture* getWhiteSpriteSheet();
private:
	std::string imageName;
	SDL_Texture* spriteSheet = nullptr;
	// used to show damage
	SDL_Texture* whiteSpriteSheet = nullptr;

	std::list<Animation> animations;
	std::string resPath = getResourcePath();

	void setSurfaceColorKey(SDL_Surface* surface, int transparentPixelIndex);
	void createWhiteTextureFromSurface(const std::string& whiteSurfaceFileName, SDL_Surface* surface, int transparentPixelIndex);
	void loadSpriteSheetTextures(bool setColorKey, bool createWhiteTexture, int transparentPixelIndex);
};

