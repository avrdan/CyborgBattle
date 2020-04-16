#include "Frame.h"

void Frame::Draw(SDL_Texture* spriteSheet, float x, float y)
{
	SDL_Rect dest;
	dest.x = static_cast<int>(x - offset.x);
	dest.y = static_cast<int>(y - offset.y);
	dest.w = clip.w;
	dest.h = clip.h;

	renderTexture(spriteSheet, Globals::renderer, dest, &clip);
}

void Frame::loadFrame(std::ifstream& file, list<DataGroupType>& groupTypes)
{
	// store them in frame data
	GroupBuilder::buildGroups(groupTypes, frameData);

	std::string buffer;
	// frame
	std::getline(file, buffer);
	// clip
	std::getline(file, buffer);
	std::stringstream ss;
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> clip.x >> clip.y >> clip.w >> clip.h;
	// offset
	std::getline(file, buffer);
	ss.str("");
	ss.clear(); // clear error state
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> offset.x >> offset.y;
	// duration
	std::getline(file, buffer);
	ss.str("");
	ss.clear(); // clear error state
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> duration;
	// index
	std::getline(file, buffer);
	ss.str("");
	ss.clear(); // clear error state
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	ss >> frameNumber;

	GroupBuilder::loadGroups(file, frameData);
};

int Frame::getFrameNumber() const
{
	return frameNumber;
}

const std::list<Group*>& Frame::getFrameData() const
{
	return this->frameData;
}

const SDL_Point Frame::getOffset() const
{
	return this->offset;
}

float Frame::getDuration() const
{
	return this->duration;
}
