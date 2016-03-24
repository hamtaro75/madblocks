#include "header.h"

void updateOption(Inputs *input) {
	if (input->up && getInfoGame()->choiceMenu > MIN_CHOICE_OPTION) {
		--getInfoGame()->choiceMenu;
		playSound(getSound().menuMove);
	}
	else if (input->down && getInfoGame()->choiceMenu < MAX_CHOICE_OPTION) {
		++getInfoGame()->choiceMenu;
		playSound(getSound().menuMove);
	}
	else if (input->right) {
		if (getInfoGame()->choiceMenu == OPTION_MUSIC && getInfoGame()->option->volumeMusic < MAX_VOLUME) {
			Mix_VolumeMusic(getInfoGame()->option->volumeMusic += 2);
			playSound(getSound().menuMove);
		}
		else if (getInfoGame()->choiceMenu == OPTION_SOUND && getInfoGame()->option->volumeSound < MAX_VOLUME) {
			Mix_Volume(-1, getInfoGame()->option->volumeSound += 2);
			playSound(getSound().menuMove);
		}

	}
	else if (input->left) {
		if (getInfoGame()->choiceMenu == OPTION_MUSIC && getInfoGame()->option->volumeMusic > MIN_VOLUME) {
			Mix_VolumeMusic(getInfoGame()->option->volumeMusic -= 2);
			playSound(getSound().menuMove);
		}
		else if (getInfoGame()->choiceMenu == OPTION_SOUND && getInfoGame()->option->volumeSound > MIN_VOLUME) {
			Mix_Volume(-1, getInfoGame()->option->volumeSound -= 2);
			playSound(getSound().menuMove);
		}
	}
	else if (input->enter) {
		if (getInfoGame()->choiceMenu == OPTION_CONTROLS) {
			playSound(getSound().menuChoose);
		}
		else if (getInfoGame()->choiceMenu == OPTION_RETURN) {
			getInfoGame()->isOnMenu = IS_IN_PRINCIPAL_MENU;
			playSound(getSound().menuChoose);
			saveOption();
		}
	}
}

void drawOption(Map *map) {
	drawTile(getInfoGame()->tileset, 0 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 1 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 2 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 3 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 4 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 5 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 6 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 7 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 8 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 9 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 10 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 11 * TILE_SIZE, 3 * TILE_SIZE, 0, 0);

	drawTile(getInfoGame()->option->slider, 6 * TILE_SIZE, 3 * TILE_SIZE, 0 * TILE_SIZE, 0);
	drawTile(getInfoGame()->option->slider, 7 * TILE_SIZE, 3 * TILE_SIZE, 1 * TILE_SIZE, 0);
	drawTile(getInfoGame()->option->slider, 8 * TILE_SIZE, 3 * TILE_SIZE, 2 * TILE_SIZE, 0);
	drawTile(getInfoGame()->option->slider, 6 * TILE_SIZE + getInfoGame()->option->volumeMusic + 27, 3 * TILE_SIZE, 3 * TILE_SIZE, 0);

	drawTile(getInfoGame()->tileset, 0 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 1 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 2 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 3 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 4 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 5 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 6 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 7 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 8 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 9 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 10 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 11 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);

	drawTile(getInfoGame()->option->slider, 6 * TILE_SIZE, 5 * TILE_SIZE, 0 * TILE_SIZE, 0);
	drawTile(getInfoGame()->option->slider, 7 * TILE_SIZE, 5 * TILE_SIZE, 1 * TILE_SIZE, 0);
	drawTile(getInfoGame()->option->slider, 8 * TILE_SIZE, 5 * TILE_SIZE, 2 * TILE_SIZE, 0);
	drawTile(getInfoGame()->option->slider, 6 * TILE_SIZE + getInfoGame()->option->volumeSound + 27, 5 * TILE_SIZE, 3 * TILE_SIZE, 0);

	drawTile(getInfoGame()->tileset, 0 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 1 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 2 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 3 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 4 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 5 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 6 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 7 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 8 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 9 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 10 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 11 * TILE_SIZE, 7 * TILE_SIZE, 0, 0);

	drawTile(getInfoGame()->tileset, 0 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 1 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 2 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 3 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 4 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 5 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 6 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 7 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 8 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 9 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 10 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 11 * TILE_SIZE, 9 * TILE_SIZE, 0, 0);

	int c0 = SELECT_MENU(getInfoGame()->choiceMenu, 0);
	int c1 = SELECT_MENU(getInfoGame()->choiceMenu, 1);
	int c2 = SELECT_MENU(getInfoGame()->choiceMenu, 2);
	int c3 = SELECT_MENU(getInfoGame()->choiceMenu, 3);


	drawString("Volume music", 0 * TILE_SIZE + TILE_SIZE / 4, 3 * TILE_SIZE + TILE_SIZE / 4, 255, c0, c0, 0);
	drawString("Volume sound", 0 * TILE_SIZE + TILE_SIZE / 4, 5 * TILE_SIZE + TILE_SIZE / 4, 255, c1, c1, 0);
	drawString("Controls", 0 * TILE_SIZE + TILE_SIZE / 4, 7 * TILE_SIZE + TILE_SIZE / 4, 255, c2, c2, 0);
	drawString("Back", 0 * TILE_SIZE + TILE_SIZE / 4, 9 * TILE_SIZE + TILE_SIZE / 4, 255, c3, c3, 0);
}

void saveOption() {
	FILE *f;

	f = fopen("config/option", "wb");
	if (f == NULL) {
		printf("Error on saving file in configuration\n");
	}
	else {
		fwrite(getInfoGame()->option, sizeof(*getInfoGame()->option), 1, f);
		fclose(f);
	}
}

void loadOption() {
	FILE *f;

	getInfoGame()->option = (s_option *)malloc(sizeof(*getInfoGame()->option));

	f = fopen("config/option", "rb");
	if (f != NULL) {
		fread(getInfoGame()->option, sizeof(*getInfoGame()->option), 1, f);
		Mix_VolumeMusic(getInfoGame()->option->volumeMusic);
		Mix_Volume(-1, getInfoGame()->option->volumeSound);
		fclose(f);
	}
	else {
		getInfoGame()->option->volumeMusic = 64;
		getInfoGame()->option->volumeSound = 64;
		f = fopen("config/option", "wb");
		if (f == NULL)
			printf("Error on creating the file of configuration for option\n");
		else {
			fwrite(getInfoGame()->option, sizeof(*getInfoGame()->option), 1, f);
		}
		fclose(f);
	}
	getInfoGame()->option->slider = loadImage("img/Blocks/slider.png");
}