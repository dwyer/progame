#ifndef TMX_H
#define TMX_H

/** For bounds-checking purposes
 * Length made a define instead of an integer literal.
 * Alternately the check could be against sizeof(tilemap->source),
 * however seeing the most obvious improvement is for 'source' to be
 * made a 'char *' and malloc'd, the sizeof method introduces latent bugs.
 */
#define TMP_TILEMAP_SOURCE_LEN 1024

typedef struct {
	char source[TMP_TILEMAP_SOURCE_LEN];
	int width;
	int height;
	int depth;
	int layerw;
	int layerh;
	int tilesetw;
	int tileseth;
	Uint32 ***data;
	Uint32 **collision;
	SDL_Surface **layers;
	SDL_Surface *tileset;
} Tilemap;

Tilemap *Tilemap_load(const char *filename);
void Tilemap_draw(Tilemap * tilemap, SDL_Surface * surface);
void Tilemap_free(Tilemap * tilemap);
int Tilemap_tile_is_occupied(Tilemap * tilemap, int x, int y);
int Tilemap_pixel_is_occupied(Tilemap * tilemap, int x, int y);

#endif
