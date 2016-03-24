#include "header.h"

void clearWindow() {
	SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(getRenderer());
	SDL_RenderPresent(getRenderer());
	SDL_Delay(1);
}

void drawString(char *str, int posX, int posY, int red, int green, int blue, int alpha) {
	SDL_Rect dest;
	SDL_Surface *surface; // To write text
	SDL_Texture *texture; // To convert the surface to texture
	SDL_Color textColor;

	textColor.r = red;
	textColor.g = green;
	textColor.b = blue;
	textColor.a = alpha;

	// create surface
	surface = TTF_RenderUTF8_Blended(getInfoGame()->fontMenu, str, textColor);

	if (surface != NULL)
	{
		// Convert surface to texture
		texture = SDL_CreateTextureFromSurface(getRenderer(), surface);
		SDL_FreeSurface(surface);
		surface = NULL;

		dest.x = posX;
		dest.y = posY;

		SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
		SDL_RenderCopy(getRenderer(), texture, NULL, &dest);

		SDL_DestroyTexture(texture);

	}
	else
		printf("Error on writint text : %s\n", TTF_GetError());
}

void drawTile(SDL_Texture *image, int destx, int desty, int srcx, int srcy) {
	SDL_Rect dest;

	dest.x = destx;
	dest.y = desty;
	dest.w = TILE_SIZE;
	dest.h = TILE_SIZE;

	SDL_Rect src;

	src.x = srcx;
	src.y = srcy;
	src.w = TILE_SIZE;
	src.h = TILE_SIZE;

	/* Dessine la tile choisie sur l'écran aux coordonnées x et y */
	SDL_RenderCopy(getRenderer(), image, &src, &dest);
}