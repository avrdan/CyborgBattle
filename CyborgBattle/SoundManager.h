#pragma once
#include "SDL_mixer.h"
#include <string>
#include <list>

struct SoundListing
{
	Mix_Chunk* sound = nullptr;
	std::string name;
};

class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void loadSound(const std::string& name, const std::string& file);
	void playSound(const std::string& name);
	void playMusic(const std::string& name, int loopNr = -1);

	static SoundManager& getInstance();
private:
	std::list<SoundListing> sounds;
	Mix_Music* music = nullptr;
};

