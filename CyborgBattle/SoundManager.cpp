#include "SoundManager.h"
#include <algorithm>
#include "res_path.h"

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
	for (const auto& soundListing : this->sounds)
	{
		Mix_FreeChunk(soundListing.sound);
	}

	Mix_PausedMusic();
	Mix_FreeMusic(this->music);
	this->music = nullptr;
}

void SoundManager::loadSound(const std::string& name, const std::string& file)
{
	SoundListing listing;
	listing.name = name;
	listing.sound = Mix_LoadWAV(file.c_str());
	this->sounds.push_back(listing);
}

void SoundManager::playSound(const std::string& name)
{
	auto it = std::find_if(this->sounds.begin(), this->sounds.end(), 
		[name](const SoundListing& listing) {
			return listing.name == name;
	});

	if (it != this->sounds.end())
	{
		Mix_PlayChannel(-1, it->sound, 0);
	}
}

void SoundManager::playMusic(const std::string& name, int loopNr /*= -1*/)
{
	const std::string& resPath = getResourcePath();
	this->music = 
		Mix_LoadMUS(std::string(resPath + name).c_str());

	if (this->music)
	{
		Mix_PlayMusic(this->music, loopNr);
	}
}

SoundManager& SoundManager::getInstance()
{
	static SoundManager soundManager;
	return soundManager;
}
