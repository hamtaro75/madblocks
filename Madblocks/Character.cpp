#include "header.h"

int hasCharacterMoved(Character c) {
	if (c.prevPosX == c.posX && c.prevPosY == c.posY)
		return 1;
	else
		return 0;
}

void drawCharacter(Map *map) {
	drawTile(getInfoGame()->tilesetCharacter, map->character.posX * TILE_SIZE - map->camera.posX * TILE_SIZE, map->character.posY * TILE_SIZE - map->camera.posY * TILE_SIZE, TILE_SIZE * map->character.direction, 0);
}

void loadCharacter(Map *map, FILE *file) {
	map->character.nbKey = 0;
	fscanf(file, "%d %d %d", &map->character.posX, &map->character.posY, &map->character.direction);
	map->character.hasJustEnterTP = 0;
}