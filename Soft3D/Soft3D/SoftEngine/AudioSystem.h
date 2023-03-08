#pragma once
#include <iostream>
#include <fmod.hpp>

class Sound;
class Channel;


class AudioSystem
{
public:
	AudioSystem();
	Sound* LoadSound(std::string path);
	Channel* PlaySound(Sound* sound);
	static AudioSystem* ThisAudio;

private:
	FMOD::System* system;

};

