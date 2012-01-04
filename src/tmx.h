#ifndef TMX_H
#define TMX_H

typedef struct {
	char source[32];
	int width;
	int height;
	int depth;
	int tilesetw;
	int tileseth;
	Uint32 ***data;
	Uint32 **collision;
	SDL_Surface **layers;
	SDL_Surface *tileset;
} TMP_Tilemap;

TMP_Tilemap *TMP_LoadTilemap(const char *filename);
void TMP_DrawTilemap(TMP_Tilemap * tilemap, SDL_Surface * surface);
void TMP_FreeTilemap(TMP_Tilemap * tilemap);
int TMP_TileIsOccupied(TMP_Tilemap * tilemap, int x, int y);
int TMP_PixelIsOccupied(TMP_Tilemap * tilemap, int x, int y);

#endif
