#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include "SDL.h"
#include "drawing_functions.h"
#include "Globals.h"
#include "groupBuilder.h"
#include "dataGroupType.h"
#include "group.h"

class Frame
{
public:
	Frame() = default;
	~Frame() = default;

	void Draw(SDL_Texture* spriteSheet, float x, float y);
	void loadFrame(std::ifstream& file, list<DataGroupType>& groupTypes);

	int getFrameNumber() const;
	const std::list<Group*>& getFrameData() const;
	const SDL_Point getOffset() const;
	float getDuration() const;
private:
	// index
	int frameNumber = 0;
	// region on the spritesheet
	SDL_Rect clip;
	float duration = 0;
	// pivot point for aligning
	SDL_Point offset;

	std::list<Group*> frameData;
};

