#pragma once
#include "fmod.hpp"

class Channel;

class Sound
{
public:

	Sound(FMOD::Sound* sound);
	Channel* Play();
	FMOD::Sound* GetObj();

private:
	FMOD::Sound* mSound;
};

