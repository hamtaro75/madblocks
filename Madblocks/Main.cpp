#define _CRT_SECURE_NO_DEPRECATE
#define TILE_SIZE 64
#define NB_TILE 16
#define MAX_SIZE_X 200
#define MAX_SIZE_Y 200
#define BACKGROUND 1
#define MIDGROUND 2
#define FOREGROUND 3
#define MIN_CHOICE_MENU 0
#define MAX_CHOICE_MENU 3
#define MIN_CHOICE_PAUSE 0
#define MAX_CHOICE_PAUSE 2
#define SELECT_MENU(x, y) x == y ? 255 : 0 
#define IS_IN_GAME 0
#define IS_IN_PRINCIPAL_MENU 1
#define IS_IN_PAUSE_MENU 2
#define IS_IN_OPTION_MENU 3
#define IS_IN_CHOOSEMAP_MENU 4
#define IS_IN_EDITOR 5
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define BLOCK 2
#define OPEN_DOOR 5
#define KEY 6
#define TELEPORTER_TWO_WAY 7
#define TELEPORTER_ONE_WAY 9
#define NOTHING 10
#define PRESSURE_PLATE_UP 11
#define PRESSURE_PLATE_DOWN 12
#define CLOSE_DOOR_PLATE 15
#define NB_MAX_TELEPORTER 10
#define MIN_VOLUME 0
#define MAX_VOLUME 128
#define MIN_CHOICE_OPTION 0
#define MAX_CHOICE_OPTION 3
#define OPTION_MUSIC 0
#define OPTION_SOUND 1
#define OPTION_CONTROLS 2
#define OPTION_RETURN 3
#include <SDL.h>
#include <stdlib.h>
#include <sys\types.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <dirent.h>
#include "header.h"


SDL_Renderer *getRenderer();
void getAllMapsInDirectory();
void printAllMapsName();

Mix_Music *music;

SDL_Texture *whiteSquare;
SDL_Texture *redSquare;
SDL_Texture *slider;
int editorPosX = 0;
int editorPosY = 0;
int selectedEditor = 10;

void initSDL();

typedef struct {
	int srcX;
	int srcY;
	int destX;
	int destY;
	int interaction;
	int isActive;
} pressurePlate;

typedef struct {
	int entryX1;
	int entryY1;
	int entryX2;
	int entryY2;
	int isTwoWay;
	int isEntry1; // to know if the player enter in entry 1 or not
} s_teleporter;

typedef struct {
	int posX;
	int posY;
	int prevPosX;
	int prevPosY;
	int direction;
	int nbKey;
	int hasJustEnterTP; // to don't loop on teleporter if it's two way
	SDL_Texture *tileset;
} Character;

typedef struct {
	int maxX;
	int maxY;
	int mapBackground[MAX_SIZE_Y][MAX_SIZE_X];
	int mapMiddle[MAX_SIZE_Y][MAX_SIZE_X];
	int mapForeground[MAX_SIZE_Y][MAX_SIZE_X];
//	SDL_Texture *tileset;
	Character character;
	int nbPlate;
	pressurePlate plate[10];
	int nbTeleporter;
	s_teleporter teleporter[10];
} Map;

typedef struct {
	int left;
	int right;
	int up;
	int down;
	int enter;
	int escape;
	int save;
	int erase;
} Inputs;

typedef struct {
	Mix_Chunk *getKey;
	Mix_Chunk *openDoor;
	Mix_Chunk *moveBlock;
	Mix_Chunk *teleport;
	Mix_Chunk *menuMove;
	Mix_Chunk *menuChoose;
} Sounds;

typedef struct {
	int isOnMenu;
	TTF_Font *fontMenu;
	int choiceMenu;
	int choicePause;
	SDL_Texture *tileset;
	int nbTotalMap;
	char *allMapsName[100];
	int mapChoosed;
	int volumeMusic;
	int volumeSound;
} infosGame;

infosGame infoGame;
Sounds sound;


Mix_Music *getMusic() {
	return music;
}

Sounds getSound() {
	return sound;
}

infosGame getInfoGame() {
	return infoGame;
}

int isOnMenu() {
	return infoGame.isOnMenu;
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
			else if (event.key.keysym.sym == SDLK_RETURN)
				input->enter = 1;
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				input->escape = 1;
			else if (event.key.keysym.sym == SDLK_s)
				input->save = 1;
			else if (event.key.keysym.sym == SDLK_DELETE)
				input->erase = 1;
		}
	}
}

void clearWindow() {
	SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(getRenderer());
	SDL_RenderPresent(getRenderer());
	SDL_Delay(1);
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
			int ysource = nbTile / NB_TILE * TILE_SIZE;
			int xsource = nbTile % NB_TILE * TILE_SIZE;
			drawTile(infoGame.tileset, TILE_SIZE * x, TILE_SIZE * y, xsource, ysource);

			// Draw midgdround
			nbTile = map->mapMiddle[y][x];
			ysource = nbTile / NB_TILE * TILE_SIZE;
			xsource = nbTile % NB_TILE * TILE_SIZE;
			drawTile(infoGame.tileset, TILE_SIZE * x, TILE_SIZE * y, xsource, ysource);
			++x;
		}
		++y;
	}
}

int hasCharacterMoved(Character c) {
	if (c.prevPosX == c.posX && c.prevPosY == c.posY)
		return 1;
	else
		return 0;
}

void drawCharacter(Map *map) {
	drawTile(map->character.tileset, map->character.posX * TILE_SIZE, map->character.posY * TILE_SIZE, TILE_SIZE * map->character.direction, 0);
}

void loadCharacter(Map *map, FILE *file) {
	map->character.tileset = loadImage("img/Magician/dm.png");
	map->character.nbKey = 0;
	fscanf(file, "%d %d %d", &map->character.posX, &map->character.posY, &map->character.direction);
	map->character.hasJustEnterTP = 0;
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

void loadPressurePlate(Map **map, FILE *file) {
	pressurePlate *plate = &(*map)->plate[(*map)->nbPlate];

	fscanf(file, "%d %d %d %d %d", &plate->srcX, &plate->srcY, &plate->destX, &plate->destY, &plate->interaction);
	plate->isActive = 0;
	(*map)->nbPlate++;
}

void loadTeleporter(Map **map, FILE *file) {
	s_teleporter *t = &(*map)->teleporter[(*map)->nbTeleporter];

	fscanf(file, "%d %d %d %d %d", &t->entryX1, &t->entryY1, &t->entryX2, &t->entryY2, &t->isTwoWay);
	(*map)->nbTeleporter++;
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
		map->nbPlate = 0;
		map->nbTeleporter = 0;
		do {
			fgets(buf, 20, file);
			if (strstr(buf, "#character"))
				loadCharacter(map, file);
			else if (strstr(buf, "#background"))
				loadMapBack(map, file);
			else if (strstr(buf, "#midground"))
				loadMapMid(map, file);
			else if (strstr(buf, "#pressurePlate")) {
				loadPressurePlate(&map, file);
			}
			else if (strstr(buf, "#teleporter")) {
				loadTeleporter(&map, file);
			}
		} while (strstr(buf, "#end") == NULL);
		// have to make a function check pressureplate here
		fclose(file);
		printf("finish read\n");
	}
	return map;
}

void drawGame(Map *map) {
	drawMap(MIDGROUND, map);
	drawCharacter(map);

	SDL_RenderPresent(getRenderer());
	SDL_Delay(1);
}

void writeString(char *str, int posX, int posY, int red, int green, int blue, int alpha) {
	SDL_Rect dest;
	SDL_Surface *surface; // To write text
	SDL_Texture *texture; // To convert the surface to texture
	SDL_Color textColor;

	textColor.r = red;
	textColor.g = green;
	textColor.b = blue;
	textColor.a = alpha;

	// create surface
	surface = TTF_RenderUTF8_Blended(infoGame.fontMenu, str, textColor);

	if (surface != NULL)
	{
		// Convert surface to texture
		texture = SDL_CreateTextureFromSurface(getRenderer(), surface);
		SDL_FreeSurface(surface);
		surface = NULL;

		dest.x = posX;
		dest.y = posY;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(getRenderer(), texture, NULL, &dest);

		SDL_DestroyTexture(texture);

	}
	else
		printf("Error on writint text : %s\n", TTF_GetError());
}

void drawMenu(Map *menu) {
	drawMap(MIDGROUND, menu);

	int c0 = SELECT_MENU(infoGame.choiceMenu, 0);
	int c1 = SELECT_MENU(infoGame.choiceMenu, 1);
	int c2 = SELECT_MENU(infoGame.choiceMenu, 2);
	int c3 = SELECT_MENU(infoGame.choiceMenu, 3);

	writeString("Start game", TILE_SIZE * 4 + TILE_SIZE / 2, 3 * TILE_SIZE + TILE_SIZE / 4, 255, c0, c0, 0);
	writeString("Edit map", TILE_SIZE * 4 + TILE_SIZE / 2, 5 * TILE_SIZE + TILE_SIZE / 4, 255, c1, c1, 0);
	writeString("Option", TILE_SIZE * 4 + TILE_SIZE / 2, 7 * TILE_SIZE + TILE_SIZE / 4, 255, c2, c2, 0);
	writeString("Exit", TILE_SIZE * 4 + TILE_SIZE / 2, 9 * TILE_SIZE + TILE_SIZE / 4, 255, c3, c3, 0);
}

// dest = give potential new pos of the key to check if the key can be at the dest pos
int checkMoveKey(Map *map, int destX, int destY) {
	if (map->mapMiddle[destY][destX] == NOTHING ||
		map->mapMiddle[destY][destX] == PRESSURE_PLATE_UP ||
		map->mapMiddle[destY][destX] == OPEN_DOOR) {
		map->mapMiddle[destY][destX] = KEY;
		return 1;
	}
	else {
		map->character.posX = map->character.prevPosX;
		map->character.posY = map->character.prevPosY;
		return 0;
	}
}

void checkBoxMove(Map *map) {
	int canMove = 1;
	if (map->character.direction == RIGHT) {
		if (map->mapMiddle[map->character.posY][map->character.posX + 1] != NOTHING &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY][map->character.posX + 1] == KEY) {
				canMove = checkMoveKey(map, map->character.posX + 2, map->character.posY);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY][map->character.posX + 1] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				if (Mix_PlayChannel(-1, sound.moveBlock, 0) == -1)
					printf("Can't play sound moveBlock");
			}
		}
	}
	else if (map->character.direction == LEFT) {
		if (map->mapMiddle[map->character.posY][map->character.posX - 1] != NOTHING &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY][map->character.posX - 1] == KEY) {
				canMove = checkMoveKey(map, map->character.posX - 2, map->character.posY);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY][map->character.posX - 1] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				if (Mix_PlayChannel(-1, sound.moveBlock, 0) == -1)
					printf("Can't play sound moveBlock");
			}
		}
	}
	else if (map->character.direction == UP) {
		if (map->mapMiddle[map->character.posY - 1][map->character.posX] != NOTHING &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY - 1][map->character.posX] == KEY) {
				canMove = checkMoveKey(map, map->character.posX, map->character.posY - 2);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY - 1][map->character.posX] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				if (Mix_PlayChannel(-1, sound.moveBlock, 0) == -1)
					printf("Can't play sound moveBlock");
			}
		}
	}
	else if (map->character.direction == DOWN) {
		if (map->mapMiddle[map->character.posY + 1][map->character.posX] != NOTHING &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY + 1][map->character.posX] == KEY) {
				canMove = checkMoveKey(map, map->character.posX, map->character.posY + 2);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY + 1][map->character.posX] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				if (Mix_PlayChannel(-1, sound.moveBlock, 0) == -1)
					printf("Can't play sound moveBlock");
			}
		}
	}
}

pressurePlate getPressurePlate(Map *map, int srcX, int srcY) {
	for (int x = 0; x < 10; x++) {
		if (map->plate[x].srcX == srcX && map->plate[x].srcY == srcY)
			return map->plate[x];
	}
}

s_teleporter getTeleporter(Map *map, int posX, int posY) {
	for (int x = 0; x < NB_MAX_TELEPORTER; x++) {
		if (map->teleporter[x].entryX1 == posX && map->teleporter[x].entryY1 == posY) {
			map->teleporter[x].isEntry1 = 1;
			return map->teleporter[x];
		}
		else if (map->teleporter[x].entryX2 == posX && map->teleporter[x].entryY2 == posY) {
			map->teleporter[x].isEntry1 = 0;
			return map->teleporter[x];
		}			
	}
}

void checkCollision(Map *map) {
	// if different from floor or open door or pressure plate
	if (map->mapMiddle[map->character.posY][map->character.posX] != 10 &&
		map->mapMiddle[map->character.posY][map->character.posX] != 5 &&
		map->mapMiddle[map->character.posY][map->character.posX] != 11)
	{
		// if a box
		if (map->mapMiddle[map->character.posY][map->character.posX] == 2) {
			checkBoxMove(map);
		}
		// if a key
		else if (map->mapMiddle[map->character.posY][map->character.posX] == 6) {
			++map->character.nbKey;
			map->mapMiddle[map->character.posY][map->character.posX] = 10;
			if (Mix_PlayChannel(-1, sound.getKey, 0) == -1)
				printf("Can't play sound getKey");	
		}
		// if close door & gotkey
		else if (map->mapMiddle[map->character.posY][map->character.posX] == 4 && map->character.nbKey) {
			--map->character.nbKey;
			map->mapMiddle[map->character.posY][map->character.posX] = 5;
			if (Mix_PlayChannel(-1, sound.openDoor, 0) == -1)
				printf("Can't play sound openDoor");
		}
		else if (map->mapMiddle[map->character.posY][map->character.posX] == TELEPORTER_TWO_WAY || map->mapMiddle[map->character.posY][map->character.posX] == TELEPORTER_ONE_WAY) {
			s_teleporter t = getTeleporter(map, map->character.posX, map->character.posY);
			if (t.isEntry1 && !map->character.hasJustEnterTP) {
				map->character.posX = t.entryX2;
				map->character.posY = t.entryY2;
				map->character.hasJustEnterTP = 1;
				if (Mix_PlayChannel(-1, sound.teleport, 0) == -1)
					printf("Can't play sound openDoor");
			} 
			else if (t.isTwoWay && !map->character.hasJustEnterTP) {
				map->character.posX = t.entryX1;
				map->character.posY = t.entryY1;
				map->character.hasJustEnterTP = 1;
				if (Mix_PlayChannel(-1, sound.teleport, 0) == -1)
					printf("Can't play sound openDoor");
			}
		}
		else {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
	}
}

void checkInteractionPlate(Map *map) {
	for (int x = 0; x < map->nbPlate; ++x) {
		pressurePlate plate = map->plate[x];
		if (map->mapMiddle[plate.srcY][plate.srcX] != PRESSURE_PLATE_UP && map->mapMiddle[plate.srcY][plate.srcX] != NOTHING && map->mapMiddle[plate.srcY][plate.srcX] != PRESSURE_PLATE_DOWN || (map->character.posX == plate.srcX && map->character.posY == plate.srcY)) {
			if (!map->plate[x].isActive) {
				if (Mix_PlayChannel(-1, sound.openDoor, 0) == -1)
					printf("Can't play sound openDoor");
				if (map->mapMiddle[plate.destY][plate.destX] != BLOCK)
					map->mapMiddle[plate.destY][plate.destX] = OPEN_DOOR;
			}	
			if (map->character.posX == plate.srcX && map->character.posY == plate.srcY)
				map->mapMiddle[plate.srcY][plate.srcX] = PRESSURE_PLATE_DOWN;
			if (map->mapMiddle[plate.destY][plate.destX] != BLOCK)
				map->mapMiddle[plate.destY][plate.destX] = OPEN_DOOR;


			map->plate[x].isActive = 1;
		}
		else {
			map->plate[x].isActive = 0;
			if (map->mapMiddle[plate.destY][plate.destX] != BLOCK && (map->character.posX != plate.destX || map->character.posY != plate.destY))
				map->mapMiddle[plate.destY][plate.destX] = CLOSE_DOOR_PLATE;
			if (map->character.posX == plate.destX && map->character.posY == plate.destY)
				map->mapMiddle[plate.destY][plate.destX] = OPEN_DOOR;
			map->mapMiddle[plate.srcY][plate.srcX] = PRESSURE_PLATE_UP;
			map->mapMiddle[plate.srcY][plate.srcX] = 11;
		}
	}
}

void updateGame(Inputs *input, Map *map) {
	map->character.prevPosX = map->character.posX;
	map->character.prevPosY = map->character.posY;
	if (input->left) {
		map->character.posX -= 1;
		map->character.direction = LEFT;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->right) {
		map->character.posX += 1;
		map->character.direction = RIGHT;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->up) {
		map->character.posY -= 1;
		map->character.direction = UP;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->down) {
		map->character.posY += 1;
		map->character.direction = DOWN;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->escape)
		infoGame.isOnMenu = 2;
	checkCollision(map);
	checkInteractionPlate(map);
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

void loadFont(char *name, int size) {
	infoGame.fontMenu = TTF_OpenFont(name, size);

	if (infoGame.fontMenu == NULL)
		printf("Failed to open Font %s: %s\n", name, TTF_GetError());
}

void initInfoGame() {
	infoGame.isOnMenu = IS_IN_PRINCIPAL_MENU;
	loadFont("font/GenBasB.ttf", 32);
	infoGame.choiceMenu = 0;
	infoGame.choicePause = 0;
	infoGame.tileset = loadImage("img/Blocks/all4.png");
	infoGame.nbTotalMap = 0;

	infoGame.volumeMusic = Mix_VolumeMusic(-1);
	infoGame.volumeSound = Mix_Volume(-1, -1);

	getAllMapsInDirectory();
	//printAllMapsName();
	

}

void updateMenu(Inputs *input, Map **map) {
	if (input->up && infoGame.choiceMenu != MIN_CHOICE_MENU)	{
		--infoGame.choiceMenu;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->down && infoGame.choiceMenu != MAX_CHOICE_MENU) {
		++infoGame.choiceMenu;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->enter) {
		if (infoGame.choiceMenu == 0) {
			infoGame.isOnMenu = IS_IN_CHOOSEMAP_MENU;
			infoGame.choicePause = 0;
		}
		else if (infoGame.choiceMenu == 1)
			infoGame.isOnMenu = IS_IN_EDITOR;
		else if (infoGame.choiceMenu == 2) {
			infoGame.isOnMenu = IS_IN_OPTION_MENU;
		}
		else if (infoGame.choiceMenu == 3)
			exit(0);
		if (Mix_PlayChannel(-1, sound.menuChoose, 0) == -1)
			printf("Can't play sound openDoor");
	}
}

void resetInputs(Inputs *input) {
	input->up = 0;
	input->right = 0;
	input->down = 0;
	input->left = 0;
	input->enter = 0;
	input->escape = 0;
	input->save = 0;
	input->erase = 0;
}

void updatePause(Inputs *input, Map **map) {
	if (input->up && infoGame.choicePause != MIN_CHOICE_PAUSE)	{
		--infoGame.choicePause;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->down && infoGame.choicePause != MAX_CHOICE_PAUSE) {
		++infoGame.choicePause;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->enter) {
		
		if (infoGame.choicePause == 0)
			infoGame.isOnMenu = 0;
		else if (infoGame.choicePause == 1) {
			char *name = (char *)malloc(30);
			strcpy(name, "maps/");
			strcat(name, infoGame.allMapsName[infoGame.mapChoosed]);
			*map = loadMap(name);
			free(name);
			infoGame.isOnMenu = 0;
		}
		else if (infoGame.choicePause == 2) {
			infoGame.isOnMenu = 1;
		}
		if (Mix_PlayChannel(-1, sound.menuChoose, 0) == -1)
			printf("Can't play sound openDoor");
		infoGame.choicePause = 0;
		infoGame.choiceMenu = 0;
	}
}

void drawPause(Map *map) {
	drawTile(infoGame.tileset, TILE_SIZE * 3, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 4, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 5, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 6, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 7, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 8, TILE_SIZE * 3, TILE_SIZE * 0, 0);

	drawTile(infoGame.tileset, TILE_SIZE * 3, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 4, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 5, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 6, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 7, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 8, TILE_SIZE * 5, TILE_SIZE * 0, 0);

	drawTile(infoGame.tileset, TILE_SIZE * 3, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 4, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 5, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 6, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 7, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(infoGame.tileset, TILE_SIZE * 8, TILE_SIZE * 7, TILE_SIZE * 0, 0);

	int c0 = SELECT_MENU(infoGame.choicePause, 0);
	int c1 = SELECT_MENU(infoGame.choicePause, 1);
	int c2 = SELECT_MENU(infoGame.choicePause, 2);

	writeString("Continue", TILE_SIZE * 5, TILE_SIZE * 3 + TILE_SIZE / 4, 255, c0, c0, 0);
	writeString("Restart", TILE_SIZE * 5, TILE_SIZE * 5 + TILE_SIZE / 4, 255, c1, c1, 0);
	writeString("Quit", TILE_SIZE * 5, TILE_SIZE * 7 + TILE_SIZE / 4, 255, c2, c2, 0);
}

int checkFileExist(char *name) {
	FILE *f;
	f = fopen(name, "r");
	if (f == NULL)
		return 0;
	fclose(f);
	return 1;
}

void saveMap(char *fileName, Map *editor) {
	FILE *file;

	file = fopen(fileName, "w");
	if (file == NULL)
		printf("Error on creating the file to save\n");
	else {
		fprintf(file, "#character\n");
		fprintf(file, "1 1 1\n");
		fprintf(file, "#background\n");
		for (int y = 0; y < 11; ++y) {
			for (int x = 0; x < 11; ++x) {
				fprintf(file, "%d ", editor->mapBackground[y][x]);
			}
			if (y < 10)
				fprintf(file, "-1\n");
			else
				fprintf(file, "-2\n");
		}
		fprintf(file, "#midground\n");
		for (int y = 0; y < 11; ++y) {
			for (int x = 0; x < 11; ++x) {
				fprintf(file, "%d ", editor->mapMiddle[y][x]);
			}
			if (y < 10)
				fprintf(file, "-1\n");
			else
				fprintf(file, "-2\n");
		}
		fprintf(file, "#end");
		fclose(file);
	}
}

void updateEditor(Inputs *input, Map *editor) {
	if (input->up && editorPosY > 0)
		editorPosY--;
	else if (input->right && editorPosX < 11)
		editorPosX++;
	else if (input->down && editorPosY < 11)
		editorPosY++;
	else if (input->left && editorPosX > 0)
		editorPosX--;
	else if (input->enter) {
		if (editorPosY == 11)
			selectedEditor = editor->mapMiddle[editorPosY][editorPosX];
		else
			editor->mapMiddle[editorPosY][editorPosX] = selectedEditor;
	}
	else if (input->save) {
		char nbCreationFile[4];
		int ret = 1;
		for (int i = 0; i < 100 && ret; ++i) {
			char *name = (char *)malloc(30);
			strcpy(name, "maps/creation");
			sprintf(nbCreationFile, "%d", i);
			strcat(name, nbCreationFile);
			strcat(name, ".txt");
			ret = checkFileExist(name);
			if (!ret) {
				saveMap(name, editor);
			}
			free(name);
		}
		if (ret)
			printf("Error occured on saving map\n");
	}
	else if (input->erase && editorPosY != 11)
		editor->mapMiddle[editorPosY][editorPosX] = NOTHING;
}


void drawForeground(Map *editor) {
	int x = 0;
	int y = 0;

	y = 0;
	while (y == 0 || editor->mapMiddle[y - 1][x] != -2) {
		x = 0;
		while (editor->mapMiddle[y][x] > -1) {
			drawTile(whiteSquare, TILE_SIZE * x, TILE_SIZE * y, 0, 0);
			++x;
		}
		++y;
	}
}

void drawEditor(Map *editor) {
	drawMap(BACKGROUND, editor);
	drawForeground(editor); // temporary
	drawTile(redSquare, TILE_SIZE * editorPosX, TILE_SIZE * editorPosY, 0, 0);
}

int isExtensionTxt(char *name) {
	char *ext = strrchr(name, '.');
	if (ext == NULL || strcmp(ext, ".txt") != 0)
		return 0;
	return 1;
}

void printAllMapsName() {
	for (int i = 0; i < infoGame.nbTotalMap; ++i) {
		printf("%s\n", infoGame.allMapsName[i]);
	}
}

void getAllMapsInDirectory() {
	DIR           *directoryMap;
	struct dirent *dir;
	
	directoryMap = opendir("maps/");
	if (directoryMap) {
		while ((dir = readdir(directoryMap)) != NULL) {
			if (isExtensionTxt(dir->d_name)) {
				infoGame.allMapsName[infoGame.nbTotalMap] = (char *)malloc(30);
				strcpy(infoGame.allMapsName[infoGame.nbTotalMap], dir->d_name);
				infoGame.nbTotalMap++;
			}
		}
		closedir(directoryMap);
	}
}

void updateMenuChooseMap(Inputs *input, Map **map) {
	if (input->left && infoGame.choiceMenu != 0)	{
		--infoGame.choiceMenu;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->right && infoGame.choiceMenu != infoGame.nbTotalMap - 1) {
		++infoGame.choiceMenu;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->enter) {
		char *name = (char *)malloc(30);
		strcpy(name, "maps/");
		strcat(name, infoGame.allMapsName[infoGame.choiceMenu]);
		*map = loadMap(name);
		free(name);
		infoGame.isOnMenu = IS_IN_GAME;
		if (Mix_PlayChannel(-1, sound.menuChoose, 0) == -1)
			printf("Can't play sound openDoor");
		infoGame.mapChoosed = infoGame.choiceMenu;
		infoGame.choiceMenu = 0;
	}
}

void drawMenuChooseMap(Map *map) {
	drawTile(infoGame.tileset, 3 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 4 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 5 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 6 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 7 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 8 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 9 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);

	writeString(infoGame.allMapsName[infoGame.choiceMenu], TILE_SIZE * 4, TILE_SIZE * 5 + TILE_SIZE / 4, 255, 255, 255, 0);
}

void updateOption(Inputs *input) {
	if (input->up && infoGame.choiceMenu > MIN_CHOICE_OPTION) {
		--infoGame.choiceMenu;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->down && infoGame.choiceMenu < MAX_CHOICE_OPTION) {
		++infoGame.choiceMenu;
		if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
			printf("Can't play sound openDoor");
	}
	else if (input->right) {
		if (infoGame.choiceMenu == OPTION_MUSIC && infoGame.volumeMusic < MAX_VOLUME) {
			Mix_VolumeMusic(++infoGame.volumeMusic);
			if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
				printf("Can't play sound openDoor");
		}
		else if (infoGame.choiceMenu == OPTION_SOUND && infoGame.volumeSound < MAX_VOLUME) {
			Mix_Volume(-1, ++infoGame.volumeSound);
			if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
				printf("Can't play sound openDoor");
		}
		
	}
	else if (input->left) {
		if (infoGame.choiceMenu == OPTION_MUSIC && infoGame.volumeMusic > MIN_VOLUME) {
			Mix_VolumeMusic(--infoGame.volumeMusic);
			if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
				printf("Can't play sound openDoor");
		}
		else if (infoGame.choiceMenu == OPTION_SOUND && infoGame.volumeSound > MIN_VOLUME) {
			Mix_Volume(-1, --infoGame.volumeSound);
			if (Mix_PlayChannel(-1, sound.menuMove, 0) == -1)
				printf("Can't play sound openDoor");
		}
		
	}
}

void drawOption(Map *map) {
	drawTile(infoGame.tileset, 0 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 1 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 2 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 3 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 4 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 5 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 6 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 7 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 8 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 9 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 10 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 11 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);

	drawTile(slider, 6 * TILE_SIZE, 3 * TILE_SIZE, 0 * TILE_SIZE, 0);
	drawTile(slider, 7 * TILE_SIZE, 3 * TILE_SIZE, 1 * TILE_SIZE, 0);
	drawTile(slider, 8 * TILE_SIZE, 3 * TILE_SIZE, 2 * TILE_SIZE, 0);
	drawTile(slider, 6 * TILE_SIZE + infoGame.volumeMusic + 27, 3 * TILE_SIZE, 3 * TILE_SIZE, 0);

	drawTile(infoGame.tileset, 0 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 1 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 2 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 3 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 4 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 5 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 6 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 7 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 8 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 9 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 10 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 11 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);

	drawTile(slider, 6 * TILE_SIZE, 5 * TILE_SIZE, 0 * TILE_SIZE, 0);
	drawTile(slider, 7 * TILE_SIZE, 5 * TILE_SIZE, 1 * TILE_SIZE, 0);
	drawTile(slider, 8 * TILE_SIZE, 5 * TILE_SIZE, 2 * TILE_SIZE, 0);
	drawTile(slider, 6 * TILE_SIZE + infoGame.volumeSound + 27, 5 * TILE_SIZE, 3 * TILE_SIZE, 0);

	drawTile(infoGame.tileset, 0 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 1 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 2 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 3 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 4 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 5 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 6 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 7 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 8 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 9 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 10 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 11 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);

	drawTile(infoGame.tileset, 0 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 1 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 2 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 3 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 4 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 5 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 6 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 7 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 8 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 9 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 10 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(infoGame.tileset, 11 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);

	int c0 = SELECT_MENU(infoGame.choiceMenu, 0);
	int c1 = SELECT_MENU(infoGame.choiceMenu, 1);
	int c2 = SELECT_MENU(infoGame.choiceMenu, 2);
	int c3 = SELECT_MENU(infoGame.choiceMenu, 3);


	writeString("Volume music", 0 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE + TILE_SIZE / 4, 255, c0, c0, 0);
	writeString("Volume sound", 0 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE + TILE_SIZE / 4, 255, c1, c1, 0);
	writeString("Controls", 0 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE + TILE_SIZE / 4, 255, c2, c2, 0);
	writeString("Back", 0 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE + TILE_SIZE / 4, 255, c3, c3, 0);
}

int	main(int ac, char **av) {
	Map *map;
	Map *menu;
	Map *editor;
	Inputs input;

	initSDL();
	map = (Map *)malloc(sizeof(*map));
	initInfoGame();
	resetInputs(&input);
	menu = loadMap("maps/menu.txt");
	editor = loadMap("maps/editor.txt");
	whiteSquare = loadImage("img/Blocks/white\ square.png");
	redSquare = loadImage("img/Blocks/red\ square.png");
	slider = loadImage("img/Blocks/slider.png");
	loadMusic("sound/Caviator.mp3");
	loadSounds();

	while (1)
	{
		getKey(&input);
		if (!isOnMenu()) {
			updateGame(&input, map);
			if (isOnMenu() != IS_IN_GAME)
				clearWindow();
			else
				drawGame(map);
		}
		else if (isOnMenu() == IS_IN_PRINCIPAL_MENU) {
			updateMenu(&input, &map);
			if (isOnMenu() != IS_IN_PRINCIPAL_MENU)
				clearWindow();
			else
				drawMenu(menu);
		}
		else if (isOnMenu() == IS_IN_PAUSE_MENU) {
			updatePause(&input, &map);
			if (isOnMenu() != IS_IN_PAUSE_MENU)
				clearWindow();
			else
				drawPause(map);
		}
		else if (isOnMenu() == IS_IN_OPTION_MENU) {
			updateOption(&input);
			drawOption(map);
		}
		else if (isOnMenu() == IS_IN_CHOOSEMAP_MENU) {
			updateMenuChooseMap(&input, &map);
			drawMenuChooseMap(map);
		}
		else if (isOnMenu() == IS_IN_EDITOR) {
			updateEditor(&input, editor);
			drawEditor(editor);
		}

		if (input.enter)
			infoGame.choiceMenu = 0;
		SDL_RenderPresent(getRenderer());
		SDL_Delay(1);
		resetInputs(&input);
	}

	return 0;
}