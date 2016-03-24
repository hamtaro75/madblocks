#include "header.h"
#include "Menu.h" // temporary
#include "Inputs.h" // temporary?
#include "Editor.h" // Temporary?

int isOnMenu() {
	return getInfoGame()->isOnMenu;
}

void drawMenuImage(char **tab, Inputs *input) {
	SDL_Rect dest;
	static int choice = 0;

	if (input->right && choice < 3)
		++choice;
	else if (input->left && choice > 0)
		--choice;

	dest.x = 0;
	dest.y = 0;
	dest.w = 768;
	dest.h = 768;

	SDL_Rect src;

	src.x = 0;
	src.y = 0;
	src.w = 517;
	src.h = 449;

	SDL_RenderCopy(getRenderer(), getInfoGame()->menu, &src, &dest);

	for (int x = 2; x < 10; ++x)
		drawTile(getInfoGame()->tileset, x * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
	drawString(tab[choice], 2 * TILE_SIZE + TILE_SIZE / 2, 5 * TILE_SIZE + TILE_SIZE / 4, 255, 255, 255, 0);
}

// to have 60 fps
void delay(unsigned int frameLimit)
{
	unsigned int ticks = SDL_GetTicks();

	if (frameLimit < ticks)
	{
		return;
	}

	if (frameLimit > ticks + 16)
	{
		SDL_Delay(16);
	}

	else
	{
		SDL_Delay(frameLimit - ticks);
	}
}

int	main(int ac, char **av) {
	Map *map;
	Map *menu;
	Map *editor;
	Inputs input;
	SDL_TimerID timer;

	initSDL();
	map = (Map *)malloc(sizeof(*map));
	initInfoGame();
	resetInputs(&input);
	menu = loadMap("maps/menu.txt");
	editor = loadMap("maps/editor.txt");
	loadSquareEditor();

	int frameLimit = SDL_GetTicks() + 16;
	Uint32 fps_lasttime = SDL_GetTicks(); //the last recorded time.
	Uint32 fps_current; //the current FPS.
	Uint32 fps_frames = 0; //frames passed since the last recorded fps.

	while (1)
	{	
		getKey(&input);
	
		if (!isOnMenu())
			loopGame(input, map);
		else if (isOnMenu() == IS_IN_PRINCIPAL_MENU)
			loopMainMenu(input, map, menu);
		else if (isOnMenu() == IS_IN_PAUSE_MENU) {
		//	loopPause(input, map);
			updatePause(&input, &map);
			if (isOnMenu() != IS_IN_PAUSE_MENU)
				clearWindow();
			else
				drawPause(map);
		}
		else if (isOnMenu() == IS_IN_OPTION_MENU)
			loopOption(input, map);
		else if (isOnMenu() == IS_IN_CHOOSEMAP_MENU) {
		//	loopMenuChooseMap(input, map);
			updateMenuChooseMap(&input, &map);
			drawMenuChooseMap(map);
		}
		else if (isOnMenu() == IS_IN_EDITOR)
			loopEditor(input, editor);

		if (input.enter)
			getInfoGame()->choiceMenu = 0;
		
		delay(frameLimit);
		frameLimit = SDL_GetTicks() + 16;

		if (getInfoGame()->joystick == NULL)
			resetInputs(&input);
			
		SDL_RenderPresent(getRenderer());


	/*	fps_frames++;
		
		if (fps_lasttime < SDL_GetTicks() - 1 * 2000)
		{
			fps_lasttime = SDL_GetTicks();
			fps_current = fps_frames;
			printf("fps ==> %d\n", fps_current);
			fps_frames = 0; 
		}*/
	}

	return 0;
}