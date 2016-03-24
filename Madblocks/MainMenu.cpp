#include "menu.h"

void updateMenu(Inputs *input, Map **map) {
	if (input->up && getInfoGame()->choiceMenu != MIN_CHOICE_MENU)	{
		--getInfoGame()->choiceMenu;
		playSound(getSound().menuMove);
	}
	else if (input->down && getInfoGame()->choiceMenu != MAX_CHOICE_MENU) {
		++getInfoGame()->choiceMenu;
		playSound(getSound().menuMove);
	}
	else if (input->enter) {
		if (getInfoGame()->choiceMenu == 0) {
			getInfoGame()->isOnMenu = IS_IN_CHOOSEMAP_MENU;
			getInfoGame()->choicePause = 0;
		}
		else if (getInfoGame()->choiceMenu == 1)
			getInfoGame()->isOnMenu = IS_IN_EDITOR;
		else if (getInfoGame()->choiceMenu == 2) {
			getInfoGame()->isOnMenu = IS_IN_OPTION_MENU;
		}
		else if (getInfoGame()->choiceMenu == 3)
			exit(0);
		playSound(getSound().menuChoose);
	}
}

void drawMenu(Map *menu) {
	drawMap(MIDGROUND, menu);

	int c0 = SELECT_MENU(getInfoGame()->choiceMenu, 0);
	int c1 = SELECT_MENU(getInfoGame()->choiceMenu, 1);
	int c2 = SELECT_MENU(getInfoGame()->choiceMenu, 2);
	int c3 = SELECT_MENU(getInfoGame()->choiceMenu, 3);

	drawString("Start game", TILE_SIZE * 4 + TILE_SIZE / 2, 3 * TILE_SIZE + TILE_SIZE / 4, 255, c0, c0, 0);
	drawString("Edit map", TILE_SIZE * 4 + TILE_SIZE / 2, 5 * TILE_SIZE + TILE_SIZE / 4, 255, c1, c1, 0);
	drawString("Option", TILE_SIZE * 4 + TILE_SIZE / 2, 7 * TILE_SIZE + TILE_SIZE / 4, 255, c2, c2, 0);
	drawString("Exit", TILE_SIZE * 4 + TILE_SIZE / 2, 9 * TILE_SIZE + TILE_SIZE / 4, 255, c3, c3, 0);
}

void loopMainMenu(Inputs input, Map *map, Map *menu) {
	updateMenu(&input, &map);

	if (isOnMenu() != IS_IN_PRINCIPAL_MENU)
		clearWindow();
	else
		drawMenu(menu);
}