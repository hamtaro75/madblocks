#include "Sound.h"

Mix_Music *music;
Sounds sound;

Sounds getSound() {
	return sound;
}

Mix_Music *getMusic() {
	return music;
}

void playSound(Mix_Chunk *s) {
	if (Mix_PlayChannel(-1, s, 0) == -1)
		printf("Can't play sound moveBlock");
}

void loadMusic(char *name) {
	// if there's already a music, stop it
	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
	}

	// load music
	music = Mix_LoadMUS(name);
	if (music == NULL)
		fprintf(stderr, "Can't read the music \n");

	// play music in repeat
	if (Mix_PlayMusic(music, -1) == -1)
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
}

void loadSounds() {
	sound.getKey = Mix_LoadWAV("sound/getKey.wav");
	if (sound.getKey == NULL)
		printf("Can't read sound getKey : %s", Mix_GetError());
	sound.moveBlock = Mix_LoadWAV("sound/moveBlock.wav");
	if (sound.moveBlock == NULL)
		printf("Can't read sound moveBlock : %s", Mix_GetError());
	sound.openDoor = Mix_LoadWAV("sound/openDoor.wav");
	if (sound.openDoor == NULL)
		printf("Can't read sound openDoor : %s", Mix_GetError());
	sound.teleport = Mix_LoadWAV("sound/teleport.wav");
	if (sound.teleport == NULL)
		printf("Can't read sound teleport : %s", Mix_GetError());
	sound.menuMove = Mix_LoadWAV("sound/menuMove.wav");
	if (sound.menuMove == NULL)
		printf("Can't read sound menuMove : %s", Mix_GetError());
	sound.menuChoose = Mix_LoadWAV("sound/menuChoose.wav");
	if (sound.menuChoose == NULL)
		printf("Can't read sound menuChoose : %s", Mix_GetError());
}