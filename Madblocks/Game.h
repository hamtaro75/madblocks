#ifndef GAME
#define GAME

#include "header.h"

int checkMoveKey(Map *map, int destX, int destY);
void checkBoxMove(Map *map);
pressurePlate getPressurePlate(Map *map, int srcX, int srcY);
s_teleporter getTeleporter(Map *map, int posX, int posY);
void checkCollision(Map *map);
void checkInteractionPlate(Map *map);
void changePosCamera(Map *map);
void updateGame(Inputs *input, Map *map);
void drawGame(Map *map);

#endif GAME