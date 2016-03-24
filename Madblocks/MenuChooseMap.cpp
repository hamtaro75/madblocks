#include "header.h"

void updateMenuChooseMap(Inputs *input, Map **map) {
	if (input->left && getInfoGame()->choiceMenu != 0)	{
		--getInfoGame()->choiceMenu;
		playSound(getSound().menuMove);
	}
	else if (input->right && getInfoGame()->choiceMenu != getInfoGame()->nbTotalMap - 1) {
		++getInfoGame()->choiceMenu;
		playSound(getSound().menuMove);
	}
	else if (input->enter) {
		char *name = (char *)malloc(30);
		strcpy(name, "maps/");
		strcat(name, getInfoGame()->allMapsName[getInfoGame()->choiceMenu]);
		*map = loadMapBinary(name);
		free(name);
		getInfoGame()->isOnMenu = IS_IN_GAME;
		playSound(getSound().menuChoose);
		getInfoGame()->mapChoosed = getInfoGame()->choiceMenu;
		getInfoGame()->choiceMenu = 0;
	}
}

void drawMenuChooseMap(Map *map) {
	drawTile(getInfoGame()->tileset, 3 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 4 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 5 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 6 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 7 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 8 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawTile(getInfoGame()->tileset, 9 * TILE_SIZE, 5 * TILE_SIZE, 0, 0);

	drawString(getInfoGame()->allMapsName[getInfoGame()->choiceMenu], TILE_SIZE * 4, TILE_SIZE * 5 + TILE_SIZE / 4, 255, 255, 255, 0);
}