#ifndef SOUND
#define SOUND

#include "header.h"

Sounds getSound();
void playSound(Mix_Chunk *s);
void loadSounds();
void loadMusic(char *name);

#endif SOUND