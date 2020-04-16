#pragma once
#include "Frame.h"

class Animation
{
public:
	Animation(const std::string& name = "");
	~Animation() {}

	int getNextFrameNumber(int frameNumber) const;
	Frame* getNextFrame(Frame* frame);
	int getEndFrameNumber() const;
	Frame* getFrame(int frameNumber);
	const std::string& getName() const;
	void loadAnimation(std::ifstream& file, std::list<DataGroupType>& groupTypes);
private:
	std::string name;
	std::list<Frame> frames;
};

