#ifndef TMX_H
#define TMX_H

/** For bounds-checking purposes
 * Length made a define instead of an integer literal.
 * Alternately the check could be against sizeof(tilemap->source),
 * however seeing the most obvious improvement is for 'source' to be
 * made a 'char *' and malloc'd, the sizeof method introduces latent bugs.
 */
#define TMP_TILEMAP_SOURCE_LEN 32

typedef struct {
	char source[TMP_TILEMAP_SOURCE_LEN];
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
