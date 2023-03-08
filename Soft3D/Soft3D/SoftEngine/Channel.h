#pragma once

#include "fmod.hpp"

class Channel
{
public:

	Channel(FMOD::Channel* channel);
	void Stop();
	void SetVolume(float vol);
	void SetPitch(float vol);
	bool Playing();

private:

	FMOD::Channel* mChannel;

};

