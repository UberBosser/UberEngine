#include <SDL2/SDL_mixer.h>
#include "UE_audio.h"


SoundManager::SoundManager(const char *d) {
    sound = Mix_LoadWAV(d);
    if (!sound) {
        printf("Couldn't load sound: %s\n", Mix_GetError());
    }
    Mix_VolumeChunk(sound, 64);
}

SoundManager::SoundManager(const char* d, const int v) {
    sound = Mix_LoadWAV(d);
    if (!sound) {
        printf("Couldn't load sound: %s\n", Mix_GetError());
    }
    Mix_VolumeChunk(sound, v);
}

void SoundManager::play() {
    Mix_PlayChannel(-1, sound, 0);
}

void SoundManager::play(int l) {
    Mix_PlayChannel(-1, sound, l);
}

SoundManager::~SoundManager() {
    Mix_FreeChunk(sound);
}


MusicManager::MusicManager(const char* d) {
    music = Mix_LoadMUS(d);
    if (!music) {
        printf("Couldn't load music: %s\n", Mix_GetError());
    }
}

void MusicManager::play() {
    Mix_PlayMusic(music, -1);
}

void MusicManager::play(int l) {
    Mix_PlayMusic(music, l);
}

MusicManager::~MusicManager() {
    Mix_FreeMusic(music);
}
