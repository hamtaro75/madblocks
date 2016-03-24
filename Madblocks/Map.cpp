#include "header.h"

void showMapConsole(Map map) {
	int x = 0;
	int y = 0;

	while (y == 0 || map.mapMiddle[y - 1][x] != -2) {
		x = 0;
		while (map.mapMiddle[y][x] > -1) {
			printf("%d ", map.mapMiddle[y][x]);
			++x;
		}
		printf("\n");
		++y;
	}
}

void loadPressurePlate(Map **map, FILE *file) {
	pressurePlate *plate = &(*map)->plate[(*map)->nbPlate];

	fscanf(file, "%d %d %d %d %d", &plate->srcX, &plate->srcY, &plate->destX, &plate->destY, &plate->interaction);
	plate->isActive = 0;
	(*map)->nbPlate++;
}

void loadTeleporter(Map **map, FILE *file) {
	s_teleporter *t = &(*map)->teleporter[(*map)->nbTeleporter];

	fscanf(file, "%d %d %d %d %d", &t->entryX1, &t->entryY1, &t->entryX2, &t->entryY2, &t->isTwoWay);
	(*map)->nbTeleporter++;
}

void drawMap(int layer, Map *map) {
	int x = 0;
	int y = 0;

	while (y == 0 || map->mapMiddle[y - 1 + map->camera.posY][x + map->camera.posX] != -2) {
		x = 0;
		while (map->mapMiddle[y + map->camera.posY][x + map->camera.posX] > -1) {
			// Draw background
			int nbTile = map->mapBackground[y + map->camera.posY][x + map->camera.posX];
			int ysource = nbTile / NB_TILE * TILE_SIZE;
			int xsource = nbTile % NB_TILE * TILE_SIZE;
			drawTile(getInfoGame()->tileset, TILE_SIZE * x, TILE_SIZE * y, xsource, ysource);

			// Draw midgdround
			nbTile = map->mapMiddle[y + map->camera.posY][x + map->camera.posX];
			ysource = nbTile / NB_TILE * TILE_SIZE;
			xsource = nbTile % NB_TILE * TILE_SIZE;
			drawTile(getInfoGame()->tileset, TILE_SIZE * x, TILE_SIZE * y, xsource, ysource);
			++x;
		}
		++y;
	}
}

void loadMapMid(Map *map, FILE *file) {
	int breakX = 0;
	int breakY = 0;

	map->maxX = 0;
	map->maxY = 0;

	for (int y = 0; breakY != -2; y++) {
		breakX = 0;
		for (int x = 0; breakX > -1; x++) {
			fscanf(file, "%d", &map->mapMiddle[y][x]);
			breakX = map->mapMiddle[y][x];
			map->maxX = map->maxX > x ? map->maxX : x;
		}
		breakY = breakX;
		map->maxY = y + 1;
	}
}

void loadMapBack(Map *map, FILE *file) {
	int breakX = 0;
	int breakY = 0;

	for (int y = 0; breakY != -2; y++) {
		breakX = 0;
		for (int x = 0; breakX > -1; x++) {
			fscanf(file, "%d", &map->mapBackground[y][x]);
			breakX = map->mapBackground[y][x];
		}
		breakY = breakX;
	}
}

Map *loadMap(char *nameMap) {
	FILE *file;
	Map *map;

	file = fopen(nameMap, "r");

	if (file == NULL) {
		printf("erreur ouverture fichier\n");
		map = NULL;
	}
	else {
		printf("ouverture du fichier\n");

		char buf[20];
		map = (Map *)malloc(sizeof(Map));
		map->nbPlate = 0;
		map->nbTeleporter = 0;
		do {
			fgets(buf, 20, file);
			if (strstr(buf, "#character"))
				loadCharacter(map, file);
			else if (strstr(buf, "#background"))
				loadMapBack(map, file);
			else if (strstr(buf, "#midground"))
				loadMapMid(map, file);
			else if (strstr(buf, "#pressurePlate")) {
				loadPressurePlate(&map, file);
			}
			else if (strstr(buf, "#teleporter")) {
				loadTeleporter(&map, file);
			}
		} while (strstr(buf, "#end") == NULL);
		// have to make a function check pressureplate here
		fclose(file);
		printf("finish read\n");
	}
	return map;
}

Map *loadMapBinary(char *pathName) {
	FILE *f;
	Map *map;

	f = fopen(pathName, "rb");
	if (f == NULL) {
		printf("Error on loading the map\n");
		map = NULL;
	}
	else {
		map = (Map *)malloc(sizeof(*map));
		fread(map, sizeof(*map), 1, f);
		/* TEMPORAIRE !!!!*/
		if (map->character.posX == 0) {
			map->character.posX = map->character.posY = 1;
		}
		fclose(f);
	}
	return map;
}