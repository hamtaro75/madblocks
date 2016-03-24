#include "header.h"

SDL_Texture *whiteSquare;
SDL_Texture *redSquare;

int editorPosX = 0;
int editorPosY = 0;
int selectedEditor = 10;

void saveMap(char *fileName, Map *editor) {
	FILE *file;

	file = fopen(fileName, "w");
	if (file == NULL)
		printf("Error on creating the file to save\n");
	else {
		fprintf(file, "#character\n");
		fprintf(file, "1 1 1\n");
		fprintf(file, "#background\n");
		for (int y = 0; y < 11; ++y) {
			for (int x = 0; x < 11; ++x) {
				fprintf(file, "%d ", editor->mapBackground[y][x]);
			}
			if (y < 10)
				fprintf(file, "-1\n");
			else
				fprintf(file, "-2\n");
		}
		fprintf(file, "#midground\n");
		for (int y = 0; y < 11; ++y) {
			for (int x = 0; x < 11; ++x) {
				fprintf(file, "%d ", editor->mapMiddle[y][x]);
			}
			if (y < 10)
				fprintf(file, "-1\n");
			else
				fprintf(file, "-2\n");
		}
		fprintf(file, "#end");
		fclose(file);
	}
}

void saveMapBinary(char *filename, Map *editor) {
	FILE *f;

	f = fopen(filename, "wb");
	if (f == NULL)
		printf("Error on creating the file to save\n");
	else {
		editor->character.posX = 1;
		editor->character.posY = 1;
		editor->character.direction = 1;
		fwrite(editor, sizeof(*editor), 1, f);
		fclose(f);
	}
}


int testSave = 0;

void updateEditor(Inputs *input, Map *editor) {
	if (input->up && editorPosY > 0)
		editorPosY--;
	else if (input->right && editorPosX < 11)
		editorPosX++;
	else if (input->down && editorPosY < 11)
		editorPosY++;
	else if (input->left && editorPosX > 0)
		editorPosX--;
	else if (input->enter) {
		if (testSave) {
			strcat(getInfoGame()->pathSave, ".map");
			if (!checkFileExist(getInfoGame()->pathSave)) {
				saveMapBinary(getInfoGame()->pathSave, editor);
				getInfoGame()->isOnMenu = IS_IN_PRINCIPAL_MENU;
			}
			else
				printf("this name already exist\n");
			free(getInfoGame()->pathSave);
			testSave = 0;
		}
		else if (editorPosY == 11)
			selectedEditor = editor->mapMiddle[editorPosY][editorPosX];
		else
			editor->mapMiddle[editorPosY][editorPosX] = selectedEditor;
	}
	else if (input->letter == 's' || testSave) {

		if (!testSave) {
			testSave = 1;
			getInfoGame()->pathSave = (char *)malloc(30);
			strcpy(getInfoGame()->pathSave, "maps/");
		}
		else {
			char c = input->letter;
			char letter[2] = { c, '\0' };
			strcat(getInfoGame()->pathSave, letter);
		}
	}
	else if (input->erase && editorPosY != 11)
		editor->mapMiddle[editorPosY][editorPosX] = NOTHING;
}

void drawForeground(Map *editor) {
	int x = 0;
	int y = 0;

	y = 0;
	while (y == 0 || editor->mapMiddle[y - 1][x] != -2) {
		x = 0;
		while (editor->mapMiddle[y][x] > -1) {
			drawTile(whiteSquare, TILE_SIZE * x, TILE_SIZE * y, 0, 0);
			++x;
		}
		++y;
	}
}

void drawEditor(Map *editor) {

	drawMap(BACKGROUND, editor);
	drawForeground(editor); // temporary
	drawTile(redSquare, TILE_SIZE * editorPosX, TILE_SIZE * editorPosY, 0, 0);

	if (testSave) {
		for (int x = 0; x != 11; ++x)
			drawTile(getInfoGame()->tileset, x * TILE_SIZE, 5 * TILE_SIZE, 0, 0);
		if (strcmp(getInfoGame()->pathSave, "maps/") != 0)
			drawString(getInfoGame()->pathSave + SIZE_PATH, TILE_SIZE / 4, TILE_SIZE * 5, 255, 255, 255, 0);
	}
}

void loadSquareEditor() {
	whiteSquare = loadImage("img/Blocks/white\ square.png");
	redSquare = loadImage("img/Blocks/red\ square.png");
}