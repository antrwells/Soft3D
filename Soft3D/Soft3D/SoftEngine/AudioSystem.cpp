#include "AudioSystem.h"
#include "Sound.h"
#include "Channel.h"

AudioSystem* AudioSystem::ThisAudio = nullptr;

AudioSystem::AudioSystem() {

    ThisAudio = this;
    FMOD_RESULT result;
    unsigned int version;

    // Create the FMOD system object
    result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
        std::cout << "Failed to create FMOD system object: " << result << std::endl;
        return;
    }

    // Check the version of FMOD
    result = system->getVersion(&version);
    if (result != FMOD_OK) {
        std::cout << "Failed to get FMOD version: " << result << std::endl;
        return;
    }
    std::cout << "FMOD version: " << version << std::endl;

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK) {
        std::cout << "Failed to initialize FMOD system: " << result << std::endl;
        return;
    }
}

Sound* AudioSystem::LoadSound(std::string path) {

    FMOD::Sound* sound;
    FMOD_RESULT result;
    result = system->createSound(path.c_str(), FMOD_DEFAULT, 0, & sound);
    if (result != FMOD_OK) {
        std::cout << "Failed to load sound: " << result << std::endl;
        return nullptr;
    }

    return new Sound(sound);

}

Channel* AudioSystem::PlaySound(Sound* sound) {
    FMOD_RESULT result;
    // Play the sound
    FMOD::Channel* channel;
    result = system->playSound(sound->GetObj(), 0, false, &channel);
    if (result != FMOD_OK) {
        std::cout << "Failed to play sound: " << result << std::endl;
        return nullptr;
    }

    return new Channel(channel);

}