#ifndef TMX_H
#define TMX_H

#include "SDL/SDL.h"

typedef struct {
	char source[32];
	int width;
	int height;
	int depth;
	int tilesetw;
	int tileseth;
	Uint32 ***data;
	SDL_Surface **layers;
	SDL_Surface *tileset;
} TMP_Tilemap;

TMP_Tilemap *TMP_LoadTilemap(const char *filename);
void TMP_DrawTilemap(TMP_Tilemap * tilemap, SDL_Surface * surface);
void TMP_FreeTilemap(TMP_Tilemap * tilemap);

#endif
