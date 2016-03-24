#include "header.h"

// dest = give potential new pos of the key to check if the key can be at the dest pos
int checkMoveKey(Map *map, int destX, int destY) {
	if (map->mapMiddle[destY][destX] == NOTHING ||
		map->mapMiddle[destY][destX] == PRESSURE_PLATE_UP ||
		map->mapMiddle[destY][destX] == OPEN_DOOR) {
		map->mapMiddle[destY][destX] = KEY;
		return 1;
	}
	else {
		map->character.posX = map->character.prevPosX;
		map->character.posY = map->character.prevPosY;
		return 0;
	}
}

void checkBoxMove(Map *map) {
	int canMove = 1;
	if (map->character.direction == RIGHT) {
		if (map->mapMiddle[map->character.posY][map->character.posX + 1] != NOTHING &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY][map->character.posX + 1] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY][map->character.posX + 1] == KEY) {
				canMove = checkMoveKey(map, map->character.posX + 2, map->character.posY);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY][map->character.posX + 1] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				playSound(getSound().moveBlock);
			}
		}
	}
	else if (map->character.direction == LEFT) {
		if (map->mapMiddle[map->character.posY][map->character.posX - 1] != NOTHING &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY][map->character.posX - 1] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY][map->character.posX - 1] == KEY) {
				canMove = checkMoveKey(map, map->character.posX - 2, map->character.posY);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY][map->character.posX - 1] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				playSound(getSound().moveBlock);
			}
		}
	}
	else if (map->character.direction == UP) {
		if (map->mapMiddle[map->character.posY - 1][map->character.posX] != NOTHING &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY - 1][map->character.posX] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY - 1][map->character.posX] == KEY) {
				canMove = checkMoveKey(map, map->character.posX, map->character.posY - 2);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY - 1][map->character.posX] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				playSound(getSound().moveBlock);
			}
		}
	}
	else if (map->character.direction == DOWN) {
		if (map->mapMiddle[map->character.posY + 1][map->character.posX] != NOTHING &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != OPEN_DOOR &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != PRESSURE_PLATE_UP &&
			map->mapMiddle[map->character.posY + 1][map->character.posX] != KEY) {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
		else {
			if (map->mapMiddle[map->character.posY + 1][map->character.posX] == KEY) {
				canMove = checkMoveKey(map, map->character.posX, map->character.posY + 2);
			}
			if (canMove) {
				map->mapMiddle[map->character.posY + 1][map->character.posX] = 2;
				map->mapMiddle[map->character.posY][map->character.posX] = 10;
				playSound(getSound().moveBlock);
			}
		}
	}
}

pressurePlate getPressurePlate(Map *map, int srcX, int srcY) {
	for (int x = 0; x < 10; x++) {
		if (map->plate[x].srcX == srcX && map->plate[x].srcY == srcY)
			return map->plate[x];
	}
}

s_teleporter getTeleporter(Map *map, int posX, int posY) {
	for (int x = 0; x < NB_MAX_TELEPORTER; x++) {
		if (map->teleporter[x].entryX1 == posX && map->teleporter[x].entryY1 == posY) {
			map->teleporter[x].isEntry1 = 1;
			return map->teleporter[x];
		}
		else if (map->teleporter[x].entryX2 == posX && map->teleporter[x].entryY2 == posY) {
			map->teleporter[x].isEntry1 = 0;
			return map->teleporter[x];
		}
	}
}

void checkCollision(Map *map) {
	// if different from floor or open door or pressure plate
	if (map->mapMiddle[map->character.posY][map->character.posX] != 10 &&
		map->mapMiddle[map->character.posY][map->character.posX] != 5 &&
		map->mapMiddle[map->character.posY][map->character.posX] != 11)
	{
		// if a box
		if (map->mapMiddle[map->character.posY][map->character.posX] == 2) {
			checkBoxMove(map);
		}
		// if a key
		else if (map->mapMiddle[map->character.posY][map->character.posX] == 6) {
			++map->character.nbKey;
			map->mapMiddle[map->character.posY][map->character.posX] = 10;
			playSound(getSound().getKey);
		}
		// if close door & gotkey
		else if (map->mapMiddle[map->character.posY][map->character.posX] == 4 && map->character.nbKey) {
			--map->character.nbKey;
			map->mapMiddle[map->character.posY][map->character.posX] = 5;
			playSound(getSound().openDoor);
		}
		else if (map->mapMiddle[map->character.posY][map->character.posX] == TELEPORTER_TWO_WAY || map->mapMiddle[map->character.posY][map->character.posX] == TELEPORTER_ONE_WAY) {
			s_teleporter t = getTeleporter(map, map->character.posX, map->character.posY);
			if (t.isEntry1 && !map->character.hasJustEnterTP) {
				map->character.posX = t.entryX2;
				map->character.posY = t.entryY2;
				map->character.hasJustEnterTP = 1;
				playSound(getSound().teleport);
			}
			else if (t.isTwoWay && !map->character.hasJustEnterTP) {
				map->character.posX = t.entryX1;
				map->character.posY = t.entryY1;
				map->character.hasJustEnterTP = 1;
				playSound(getSound().teleport);
			}
		}
		else {
			map->character.posX = map->character.prevPosX;
			map->character.posY = map->character.prevPosY;
		}
	}
}

void checkInteractionPlate(Map *map) {
	for (int x = 0; x < map->nbPlate; ++x) {
		pressurePlate plate = map->plate[x];
		if (map->mapMiddle[plate.srcY][plate.srcX] != PRESSURE_PLATE_UP && map->mapMiddle[plate.srcY][plate.srcX] != NOTHING && map->mapMiddle[plate.srcY][plate.srcX] != PRESSURE_PLATE_DOWN || (map->character.posX == plate.srcX && map->character.posY == plate.srcY)) {
			if (!map->plate[x].isActive) {
				playSound(getSound().openDoor);
				if (map->mapMiddle[plate.destY][plate.destX] != BLOCK)
					map->mapMiddle[plate.destY][plate.destX] = OPEN_DOOR;
			}
			if (map->character.posX == plate.srcX && map->character.posY == plate.srcY)
				map->mapMiddle[plate.srcY][plate.srcX] = PRESSURE_PLATE_DOWN;
			if (map->mapMiddle[plate.destY][plate.destX] != BLOCK)
				map->mapMiddle[plate.destY][plate.destX] = OPEN_DOOR;


			map->plate[x].isActive = 1;
		}
		else {
			map->plate[x].isActive = 0;
			if (map->mapMiddle[plate.destY][plate.destX] != BLOCK && (map->character.posX != plate.destX || map->character.posY != plate.destY))
				map->mapMiddle[plate.destY][plate.destX] = CLOSE_DOOR_PLATE;
			if (map->character.posX == plate.destX && map->character.posY == plate.destY)
				map->mapMiddle[plate.destY][plate.destX] = OPEN_DOOR;
			map->mapMiddle[plate.srcY][plate.srcX] = PRESSURE_PLATE_UP;
			map->mapMiddle[plate.srcY][plate.srcX] = 11;
		}
	}
}

void changePosCamera(Map *map) {
	int nbBlockXdraw = SIZE_WIN_Y / TILE_SIZE;

	if (map->camera.posX + nbBlockXdraw / 2 < map->character.posX && map->camera.posX + nbBlockXdraw < map->maxX)
		++map->camera.posX;
	if ((map->camera.posX + nbBlockXdraw / 2 > map->character.posX) && (map->camera.posX > 0))
		--map->camera.posX;
	if (map->camera.posY + nbBlockXdraw / 2 < map->character.posY && map->camera.posY + nbBlockXdraw < map->maxY)
		++map->camera.posY;
	if (map->camera.posY + nbBlockXdraw / 2 > map->character.posY && map->camera.posY > 0)
		--map->camera.posY;
}

void updateGame(Inputs *input, Map *map) {
	map->character.prevPosX = map->character.posX;
	map->character.prevPosY = map->character.posY;
	if (input->left) {
		map->character.posX -= 1;
		map->character.direction = LEFT;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->right) {
		map->character.posX += 1;
		map->character.direction = RIGHT;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->up) {
		map->character.posY -= 1;
		map->character.direction = UP;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->down) {
		map->character.posY += 1;
		map->character.direction = DOWN;
		map->character.hasJustEnterTP = 0;
	}
	else if (input->escape)
		getInfoGame()->isOnMenu = 2;
	checkCollision(map);
	checkInteractionPlate(map);
	changePosCamera(map);
}

void drawGame(Map *map) {
	drawMap(MIDGROUND, map);
	drawCharacter(map);

	SDL_RenderPresent(getRenderer());
	SDL_Delay(1);
}