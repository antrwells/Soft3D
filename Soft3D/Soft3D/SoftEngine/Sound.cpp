#include "Sound.h"
#include "AudioSystem.h"
#include "Channel.h"

Sound::Sound(FMOD::Sound* s) {
	
	mSound = s;

}

Channel* Sound::Play() {

	return AudioSystem::ThisAudio->PlaySound(this);

}

FMOD::Sound* Sound::GetObj() {
	return mSound;
}