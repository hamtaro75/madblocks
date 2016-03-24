#ifndef HEADER
#define HEADER

#define _CRT_SECURE_NO_DEPRECATE
#define TILE_SIZE 64
#define SIZE_WIN_X 768
#define SIZE_WIN_Y 768
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
#define SIZE_PATH 5
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
#define EXIT 8
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
#define DEAD_ZONE 8000
#define JOYSTICK_UP 0
#define JOYSTICK_DOWN 1
#define JOYSTICK_LEFT 2
#define JOYSTICK_RIGHT 3
#define JOYSTICK_START 4
#define JOYSTICK_A 10

#include <SDL.h>
#include <stdlib.h>
#include <sys\types.h>
#include <SDL_image.h>
#include <stdio.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string.h>
#include <dirent.h>

typedef struct{
	int posX;
	int posY;
} s_camera;

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
	s_camera camera;
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
	int letter;
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
	Inputs controls;
	unsigned int volumeMusic;
	unsigned int volumeSound;
	SDL_Texture *slider;
} s_option;

typedef struct {
	int isOnMenu;
	TTF_Font *fontMenu;
	int choiceMenu;
	int choicePause;
	SDL_Texture *tileset;
	int nbTotalMap;
	char *allMapsName[100];
	int mapChoosed;
	char *pathSave;
	SDL_Joystick *joystick;
	s_option *option;
	SDL_Texture *tilesetCharacter;
	SDL_Texture *menu;
} infosGame;


infosGame *getInfoGame();
void getAllMapsInDirectory();
void initInfoGame();
int checkFileExist(char *name);

void inputKeyboard(Inputs *input);
void resetInputs(Inputs *input);
void getKey(Inputs *input);


SDL_Renderer *getRenderer();
SDL_Texture *loadImage(char *path);

Sounds getSound();
void playSound(Mix_Chunk *s);
void loadSounds();
void loadMusic(char *name);

void updateOption(Inputs *input);
void drawOption(Map *map);
void saveOption();
void loadOption();
SDL_Texture *getSlider();

void initSDL();

void clearWindow();
void drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy);
void drawString(char *str, int posX, int posY, int red, int green, int blue, int alpha);

void loadCharacter(Map *map, FILE *file);
void drawCharacter(Map *map);

void showMapConsole(Map map);
void drawMap(int layer, Map *map);
void loadMapMid(Map *map, FILE *file);
void loadMapBack(Map *map, FILE *file);
Map *loadMap(char *nameMap);
Map *loadMapBinary(char *pathName);

void loadPressurePlate(Map **map, FILE *file);
void loadTeleporter(Map **map, FILE *file);

void updateMenu(Inputs *input, Map **map);
void drawMenu(Map *menu);

void updatePause(Inputs *input, Map **map);
void drawPause(Map *map);

void loadSquareEditor();
void updateEditor(Inputs *input, Map *editor);
void drawEditor(Map *editor);

void updateMenuChooseMap(Inputs *input, Map **map);
void drawMenuChooseMap(Map *map);

int checkMoveKey(Map *map, int destX, int destY);
void checkBoxMove(Map *map);
pressurePlate getPressurePlate(Map *map, int srcX, int srcY);
s_teleporter getTeleporter(Map *map, int posX, int posY);
void checkCollision(Map *map);
void checkInteractionPlate(Map *map);
void changePosCamera(Map *map);
void updateGame(Inputs *input, Map *map);
void drawGame(Map *map);

#endif HEADER