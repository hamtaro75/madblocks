#include "utils.h"
#include "Option.h"

infosGame *infoGame;

infosGame *getInfoGame() {
	return infoGame;
}

void printAllMapsName() {
	for (int i = 0; i < getInfoGame()->nbTotalMap; ++i) {
		printf("%s\n", getInfoGame()->allMapsName[i]);
	}
}

int isExtensionTxt(char *name) {
	char *ext = strrchr(name, '.');
	if (ext == NULL || strcmp(ext, ".txt") != 0)
		return 0;
	return 1;
}

int isExtensionMap(char *name) {
	char *ext = strrchr(name, '.');
	if (ext == NULL || strcmp(ext, ".map") != 0)
		return 0;
	return 1;
}

int checkFileExist(char *name) {
	FILE *f;
	f = fopen(name, "r");
	if (f == NULL)
		return 0;
	fclose(f);
	return 1;
}

void loadFont(char *name, int size) {
	infoGame->fontMenu = TTF_OpenFont(name, size);

	if (infoGame->fontMenu == NULL)
		printf("Failed to open Font %s: %s\n", name, TTF_GetError());
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
	}
	else
		printf("L'image n'a pas pu être chargée! SDL_Error :  %s\n", SDL_GetError());

	return texture;
}

void getAllMapsInDirectory() {
	DIR           *directoryMap;
	struct dirent *dir;

	directoryMap = opendir("maps/");
	if (directoryMap) {
		while ((dir = readdir(directoryMap)) != NULL) {
			if (isExtensionMap(dir->d_name)) {
				infoGame->allMapsName[infoGame->nbTotalMap] = (char *)malloc(30);
				strcpy(infoGame->allMapsName[infoGame->nbTotalMap], dir->d_name);
				infoGame->nbTotalMap++;
			}
		}
		closedir(directoryMap);
	}
}

void initInfoGame() {
	infoGame = (infosGame *)malloc(sizeof(*infoGame));

	infoGame->isOnMenu = IS_IN_PRINCIPAL_MENU;
	loadFont("font/GenBasB.ttf", 32);
	infoGame->choiceMenu = 0;
	infoGame->choicePause = 0;
	infoGame->tileset = loadImage("img/Blocks/all4.png");
	infoGame->tilesetCharacter = loadImage("img/Magician/dm.png");
	infoGame->nbTotalMap = 0;

	loadOption();

	loadSounds();
	loadMusic("sound/Caviator.mp3");

	infoGame->menu = loadImage("img/menu/test.jpg");

	infoGame->joystick = NULL;

	getAllMapsInDirectory();
	//printAllMapsName();
}