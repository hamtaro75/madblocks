#ifndef MAP
#define MAP

#include "header.h"

void showMapConsole(Map map);
void drawMap(int layer, Map *map);
void loadMapMid(Map *map, FILE *file);
void loadMapBack(Map *map, FILE *file);
Map *loadMap(char *nameMap);
Map *loadMapBinary(char *pathName);

#endif MAP