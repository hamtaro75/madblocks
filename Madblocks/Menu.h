#ifndef MENU
#define MENU

#include "header.h"

void updateMenu(Inputs *input, Map **map);
void drawMenu(Map *menu);

void updatePause(Inputs *input, Map **map);
void drawPause(Map *map);

void updateMenuChooseMap(Inputs *input, Map **map);
void drawMenuChooseMap(Map *map);

#endif MENU
