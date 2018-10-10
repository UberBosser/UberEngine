#ifndef UE_AUDIO_H
#define UE_AUDIO_H
#include "UE_core.h"


class SoundManager {
    public:
        SoundManager(const char* d);
        SoundManager(const char* d, const int v);
        void play();
        void play(int i);
        ~SoundManager();

    private:
        Mix_Chunk* sound;
};


class MusicManager {
    public:
        MusicManager(const char* d);
        void play();
        void play(int t);
        ~MusicManager();

    private:
        Mix_Music *music;
};

#endif
