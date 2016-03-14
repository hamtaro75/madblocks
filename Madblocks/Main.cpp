#define _CRT_SECURE_NO_DEPRECATE
#define TILE_SIZE 64
#define MAX_SIZE_X 200
#define MAX_SIZE_Y 200
#define BACKGROUND 1
#define MIDGROUND 2
#define FOREGROUND 3
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#include <SDL.h>
#include <stdlib.h>
#include <sys\types.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <string.h>
#include "header.h"


SDL_Renderer *getRenderer();
Mix_Music *music;
void initSDL();

typedef struct {
	int posX;
	int posY;
	int prevPosX;
	int prevPosY;
	int direction;
	int gotKey;
	SDL_Texture *tileset;
} Character;

typedef struct {
	int maxX;
	int maxY;
	int mapBackground[MAX_SIZE_Y][MAX_SIZE_X];
	int mapMiddle[MAX_SIZE_Y][MAX_SIZE_X];
	int mapForeground[MAX_SIZE_Y][MAX_SIZE_X];
	SDL_Texture *tileset;
	Character character;
} Map;

typedef struct {
	int left;
	int right;
	int up;
	int down;
} Inputs;

Mix_Music *getMusic() {
	return music;
}

void getKey(Inputs *input)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			exit(0);
		else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_LEFT)
				input->left = 1;
			else if (event.key.keysym.sym == SDLK_RIGHT)
				input->right = 1;
			else if (event.key.keysym.sym == SDLK_UP)
				input->up = 1;
			else if (event.key.keysym.sym == SDLK_DOWN)
				input->down = 1;
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				exit(0);
		}
	}
}

SDL_Texture *loadImage(char *name)
{
	SDL_Surface *loadedImage = NULL;
	SDL_Texture *texture = NULL;
	loadedImage = IMG_Load(name);

	if (loadedImage != NULL)
	{
		texture = SDL_CreateTextureFromSurface(getRenderer(), loadedImage);

		SDL_FreeSurface(loadedImage);
		loadedImage = NULL;
		printf("image loaded !\n");
	}
	else
		printf("L'image n'a pas pu être chargée! SDL_Error :  %s\n", SDL_GetError());

	return texture;
}

void drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy) {
	SDL_Rect dest;

	dest.x = destx;
	dest.y = desty;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;

	SDL_Rect src;

	src.x = srcx;
	src.y = srcy;
	src.w = TILE_SIZE;
	src.h = TILE_SIZE;

	/* Dessine la tile choisie sur l'écran aux coordonnées x et y */
	SDL_RenderCopy(getRenderer(), image, &src, &dest);
}

void showMapConsole(Map map) {
	int x = 0;
	int y = 0;
	
	while (y == 0 || map.mapMiddle[y - 1][x] != -2) {
		x = 0;
		while (map.mapMiddle[y][x] > -1) {
			printf("%d ", map.mapMiddle[y][x]);
			++x;
		}
		printf("\n");
		++y;
	}
}

void drawMap(int layer, Map *map) {

	int x = 0;
	int y = 0;

	y = 0;
	while (y == 0 || map->mapMiddle[y - 1][x] != -2) {
		x = 0;
		while (map->mapMiddle[y][x] > -1) {
			// Draw background
			int nbTile = map->mapBackground[y][x];
			int ysource = nbTile / 9 * TILE_SIZE;
			int xsource = nbTile % 9 * TILE_SIZE;
			drawTile(map->tileset, TILE_SIZE * x, TILE_SIZE * y, xsource, ysource);

			// Draw midgdpiround
			nbTile = map->mapMiddle[y][x];
			ysource = nbTile / 9 * TILE_SIZE;
			xsource = nbTile % 9 * TILE_SIZE;
			drawTile(map->tileset, TILE_SIZE * x, TILE_SIZE * y, xsource, ysource);
			++x;
		}
		++y;
	}
}

void drawCharacter(Map *map) {
	drawTile(map->character.tileset, map->character.posX * TILE_SIZE, map->character.posY * TILE_SIZE, 0, 0);
}

void loadCharacter(Map *map, FILE *file) {
	map->character.tileset = loadImage("img/Magician/toto.png");
	map->character.gotKey = 0;
	fscanf(file, "%d %d", &map->character.posX, &map->character.posY);
}

void loadMapMid(Map *map, FILE *file) {
	int breakX = 0;
	int breakY = 0;

	for (int y = 0; breakY != -2; y++) {
		breakX = 0;
		for (int x = 0; breakX > -1; x++) {
			fscanf(file, "%d", &map->mapMiddle[y][x]);
			breakX = map->mapMiddle[y][x];
		}
		breakY = breakX;
	}
}

void loadMapBack(Map *map, FILE *file) {
	int breakX = 0;
	int breakY = 0;

	for (int y = 0; breakY != -2; y++) {
		breakX = 0;
		for (int x = 0; breakX > -1; x++) {
			fscanf(file, "%d", &map->mapBackground[y][x]);
			breakX = map->mapBackground[y][x];
		}
		breakY = breakX;
	}
}

Map *loadMap(char *nameMap) {
	FILE *file;
	Map *map;
	int test = 42;

	file = fopen(nameMap, "r");

	if (file == NULL) {
		printf("erreur ouverture fichier\n");
		map = NULL;
	}
	else {
		printf("ouverture du fichier\n");

		char buf[20];
		map = (Map *)malloc(sizeof(Map));
		map->tileset = loadImage("img/Blocks/all2.png");

		do {
			fgets(buf, 20, file);
			if (strstr(buf, "#character"))
				loadCharacter(map, file);
			else if (strstr(buf, "#background"))
				loadMapBack(map, file);
			else if (strstr(buf, "#midground"))
				loadMapMid(map, file);
		} while (strstr(buf, "#end") == NULL);
	}
	printf("finish read\n");
	return map;
}

void drawGame(Map *map) {
	drawMap(MIDGROUND, map);
	drawCharacter(map);

	SDL_RenderPresent(getRenderer());
	SDL_Delay(1);
}

void checkBoxMove(Map *map) {
	int *actualBlock = &map->mapMiddle[map->character.posY][map->character.posX];

	if (map->character.direction == RIGHT) {
		if (map->mapMiddle[map->character.posY][map->character.posX + 1] != 1 &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != 5) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			map->mapMiddle[map->character.posY][map->character.posX + 1] = 2;
			map->mapMiddle[map->character.posY][map->character.posX] = 1;
		}
	}
	else if (map->character.direction == LEFT) {
		if (map->mapMiddle[map->character.posY][map->character.posX - 1] != 1 &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != 5) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			map->mapMiddle[map->character.posY][map->character.posX - 1] = 2;
			map->mapMiddle[map->character.posY][map->character.posX] = 1;
		}
	}
	else if (map->character.direction == UP) {
		if (map->mapMiddle[map->character.posY - 1][map->character.posX] != 1 &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != 5) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			map->mapMiddle[map->character.posY - 1][map->character.posX] = 2;
			map->mapMiddle[map->character.posY][map->character.posX] = 1;
		}
	}
	else if (map->character.direction == DOWN) {
		if (map->mapMiddle[map->character.posY + 1][map->character.posX] != 1 &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != 5) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			map->mapMiddle[map->character.posY + 1][map->character.posX] = 2;
			map->mapMiddle[map->character.posY][map->character.posX] = 1;
		}
	}
}

void checkCollision(Map *map) {
	// if different from floor or open door
	if (map->mapMiddle[map->character.posY][map->character.posX] != 1 &&
		map->mapMiddle[map->character.posY][map->character.posX] != 5)
	{
		// if a box
		if (map->mapMiddle[map->character.posY][map->character.posX] == 2) {
			checkBoxMove(map);
		}
		// if a key
		else if (map->mapMiddle[map->character.posY][map->character.posX] == 6) {
			map->character.gotKey = 1;
			map->mapMiddle[map->character.posY][map->character.posX] = 1;
		}
		// if close door & gotkey
		else if (map->mapMiddle[map->character.posY][map->character.posX] == 4 && map->character.gotKey) {
			map->character.gotKey = 0;
			map->mapMiddle[map->character.posY][map->character.posX] = 5;
		}
		else {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
	}
}

void updateGame(Inputs *input, Map *map) {
	map->character.prevPosX = map->character.posX;
	map->character.prevPosY = map->character.posY;
	if (input->left) {
		input->left = 0;
		map->character.posX -= 1;
		map->character.direction = LEFT;
	}
	else if (input->right) {
		input->right = 0;
		map->character.posX += 1;
		map->character.direction = RIGHT;
	}
	else if (input->up) {
		input->up = 0;
		map->character.posY -= 1;
		map->character.direction = UP;
	}
	else if (input->down) {
		input->down = 0;
		map->character.posY += 1;
		map->character.direction = DOWN;
	}

	checkCollision(map);
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

	// repeat music
	if (Mix_PlayMusic(music, -1) == -1)
		printf("Mix_PlayMusic: %s\n", Mix_GetError());
}

int	main(int ac, char **av) {
	Map *map;
	Inputs input;

	initSDL();
	map = loadMap("maps/real.txt");
	loadMusic("sound/Caviator.mp3");

	while (1)
	{
		getKey(&input);
		updateGame(&input, map);
		drawGame(map);
	}

	return 0;
}