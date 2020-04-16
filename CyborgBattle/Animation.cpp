#include "Animation.h"
#include <algorithm>

Animation::Animation(const std::string& name /*= ""*/)
{
	this->name = name;
}

int Animation::getNextFrameNumber(int frameNumber) const
{
	return frameNumber + 1 < frames.size() ? 
		frameNumber + 1 : 0;
}

Frame* Animation::getNextFrame(Frame* frame)
{
	if (!frame)
		return nullptr;
	return getFrame(getNextFrameNumber(frame->getFrameNumber()));
}

int Animation::getEndFrameNumber() const
{
	return static_cast<int>(frames.size() - 1);
}

Frame* Animation::getFrame(int frameNumber)
{
	auto it = std::find_if(frames.begin(), frames.end(), 
		[frameNumber](const Frame& frame) -> bool {
		return frame.getFrameNumber() == frameNumber;
	});

	if (it != frames.end())
		return &(*it);

	return nullptr;
}

const std::string& Animation::getName() const
{
	return name;
}

void Animation::loadAnimation(std::ifstream& file, std::list<DataGroupType>& groupTypes)
{
	// animation name
	std::getline(file, name);
	std::string buffer;
	// frames: nr
	std::getline(file,buffer);
	std:stringstream ss;
	buffer = Globals::clipOffDataHeader(buffer);
	ss << buffer;
	int nrFrames;
	ss >> nrFrames;

	for (int i = 0; i < nrFrames; ++i)
	{
		Frame newFrame;
		newFrame.loadFrame(file, groupTypes);
		frames.push_back(newFrame);
	}
}
