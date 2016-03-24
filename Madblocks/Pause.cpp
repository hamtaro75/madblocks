#include "header.h"

void updatePause(Inputs *input, Map **map) {
	if (input->up && getInfoGame()->choicePause != MIN_CHOICE_PAUSE)	{
		--getInfoGame()->choicePause;
		playSound(getSound().menuMove);
	}
	else if (input->down && getInfoGame()->choicePause != MAX_CHOICE_PAUSE) {
		++getInfoGame()->choicePause;
		playSound(getSound().menuMove);
	}
	else if (input->enter) {

		if (getInfoGame()->choicePause == 0)
			getInfoGame()->isOnMenu = 0;
		else if (getInfoGame()->choicePause == 1) {
			char *name = (char *)malloc(30);
			strcpy(name, "maps/");
			strcat(name, getInfoGame()->allMapsName[getInfoGame()->mapChoosed]);
			*map = loadMapBinary(name);
			free(name);
			getInfoGame()->isOnMenu = 0;
		}
		else if (getInfoGame()->choicePause == 2) {
			getInfoGame()->isOnMenu = 1;
		}
		playSound(getSound().menuChoose);
		getInfoGame()->choicePause = 0;
		getInfoGame()->choiceMenu = 0;
	}
}

void drawPause(Map *map) {
	drawTile(getInfoGame()->tileset, TILE_SIZE * 3, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 4, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 5, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 6, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 7, TILE_SIZE * 3, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 8, TILE_SIZE * 3, TILE_SIZE * 0, 0);

	drawTile(getInfoGame()->tileset, TILE_SIZE * 3, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 4, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 5, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 6, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 7, TILE_SIZE * 5, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 8, TILE_SIZE * 5, TILE_SIZE * 0, 0);

	drawTile(getInfoGame()->tileset, TILE_SIZE * 3, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 4, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 5, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 6, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 7, TILE_SIZE * 7, TILE_SIZE * 0, 0);
	drawTile(getInfoGame()->tileset, TILE_SIZE * 8, TILE_SIZE * 7, TILE_SIZE * 0, 0);

	int c0 = SELECT_MENU(getInfoGame()->choicePause, 0);
	int c1 = SELECT_MENU(getInfoGame()->choicePause, 1);
	int c2 = SELECT_MENU(getInfoGame()->choicePause, 2);

	drawString("Continue", TILE_SIZE * 5, TILE_SIZE * 3 + TILE_SIZE / 4, 255, c0, c0, 0);
	drawString("Restart", TILE_SIZE * 5, TILE_SIZE * 5 + TILE_SIZE / 4, 255, c1, c1, 0);
	drawString("Quit", TILE_SIZE * 5, TILE_SIZE * 7 + TILE_SIZE / 4, 255, c2, c2, 0);
}