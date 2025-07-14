#include "music.h"

extern Music currentMusic;

void PlayMusic(Music music) 
{
    currentMusic = music;
    PlayMusicStream(music);
}

void StopMusic() 
{
    StopMusicStream(currentMusic);
}
