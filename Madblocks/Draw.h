#ifndef DRAW
#define DRAW

#include "header.h"

void clearWindow();
void drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy);
void drawString(char *str, int posX, int posY, int red, int green, int blue, int alpha);

#endif DRAW