#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>

SDL_Renderer *renderer;
SDL_Window *window;

SDL_Renderer *getRenderer() {
	return renderer;
}

SDL_Window *getWindow() {
	return window;
}

void initSDLMixer() {
	int flags = MIX_INIT_MP3;
	int initted = Mix_Init(flags);
	if ((initted & flags) != flags)
	{
		printf("Mix_Init: Failed to init SDL_Mixer\n");
		printf("Mix_Init: %s\n", Mix_GetError());
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
	}

	Mix_AllocateChannels(32);
}

void initSDLTTF() {
	if (TTF_Init() < 0)
	{
		printf("Failed init SDL TTF: %s\n", TTF_GetError());
		exit(1);
	}
}

void initSDL()
{

	window = SDL_CreateWindow("Madblocks", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED | SDL_INIT_TIMER, 768, 768, NULL);

	initSDLMixer();
	initSDLTTF();

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	
	SDL_ShowCursor(SDL_DISABLE);
}