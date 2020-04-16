#include "AnimationSet.h"
#include <algorithm>

Animation* AnimationSet::getAnimation(const std::string& name)
{
	auto it = std::find_if(animations.begin(), animations.end(), 
		[name](const Animation& anim) -> bool{
		if (anim.getName() == name)
			return true;
		return false;
	});

	if (it != animations.end())
	{
		return &(*it);
	}

	return nullptr;
}

void AnimationSet::loadAnimationSet(const std::string& fileName, 
	std::list<DataGroupType>& groupTypes, bool setColorKey /*= false*/, 
	int transparentPixelIndex /*= 0*/, bool createWhiteTexture /*= false*/)
{
	std::ifstream file;
	file.open(resPath + fileName);
	if (!file.good())
	{
		file.close();
		return;
	}
		
	std::getline(file, this->imageName);
	loadSpriteSheetTextures(setColorKey, createWhiteTexture, transparentPixelIndex);

	std::string buffer;
	std::getline(file, buffer);
	std::stringstream ss;
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	int nrOfAnimations;
	ss >> nrOfAnimations;

	for (int i = 0; i < nrOfAnimations; ++i)
	{
		Animation newAnimation;
		newAnimation.loadAnimation(file, groupTypes);
		this->animations.push_back(newAnimation);
	}
	file.close();
}

SDL_Texture* AnimationSet::getSpriteSheet()
{
	return this->spriteSheet;
}

SDL_Texture* AnimationSet::getWhiteSpriteSheet()
{
	return this->whiteSpriteSheet;
}

void AnimationSet::setSurfaceColorKey(SDL_Surface* surface, int transparentPixelIndex)
{
	// for transparency, we will grab the [transparentPixelIndex] from the surface we just made
	SDL_Color* transparentPixel = &surface->format->palette->colors[transparentPixelIndex];
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format,
		transparentPixel->r, transparentPixel->g, transparentPixel->b));
	this->spriteSheet = convertSurfaceToTexture(surface, Globals::renderer);
}

void AnimationSet::createWhiteTextureFromSurface(const std::string& whiteSurfaceFileName, SDL_Surface* surface, int transparentPixelIndex)
{
	SDL_Surface* whiteSurface = loadSurface(whiteSurfaceFileName, Globals::renderer);
	surfacePaletteSwap(surface, whiteSurface);
	// for transparency, we will grab the [transparentPixelIndex] from the surface we just made
	SDL_Color* transparentPixel = &surface->format->palette->colors[transparentPixelIndex];
	SDL_SetColorKey(surface, 1, SDL_MapRGB(surface->format,
		transparentPixel->r, transparentPixel->g, transparentPixel->b));
	this->whiteSpriteSheet = convertSurfaceToTexture(whiteSurface, Globals::renderer);

	cleanup(whiteSurface);
}

void AnimationSet::loadSpriteSheetTextures(bool setColorKey, bool createWhiteTexture, int transparentPixelIndex)
{
	if (setColorKey)
	{
		SDL_Surface* spriteSurface = loadSurface(this->resPath + this->imageName, Globals::renderer);
		setSurfaceColorKey(spriteSurface, transparentPixelIndex);

		if (createWhiteTexture)
		{
			createWhiteTextureFromSurface(this->resPath + "allwhite.png", spriteSurface, transparentPixelIndex);;
		}
		else
		{
			this->whiteSpriteSheet = nullptr;
		}

		cleanup(spriteSurface);
	}
	else
	{
		spriteSheet = loadTexture(this->resPath + imageName, Globals::renderer);
	}
}
