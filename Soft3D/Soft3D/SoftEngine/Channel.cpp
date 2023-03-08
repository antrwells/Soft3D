#include "Channel.h"

Channel::Channel(FMOD::Channel* channel) {

	mChannel = channel;

}

void Channel::Stop() {

	mChannel->stop();

}

void Channel::SetVolume(float vol) {
	
	mChannel->setVolume(vol);

}

void Channel::SetPitch(float pi)
{

	mChannel->setPitch(pi);
	

}

bool Channel::Playing() {
	bool playing = false;
	mChannel->isPlaying(&playing);
	return playing;

}