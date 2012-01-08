#include <stdio.h>
#include <assert.h>
#include <SDL/SDL.h>
#include "tilemap.h"
#include "main.h"

#define TILE_SZ 16

/** For bounds-checking purposes
 * Length made a define instead of an integer literal.
 * Alternately the check could be against sizeof(source),
 * however seeing the most obvious improvement is for 'source' to be
 * made a 'char *' and malloc'd, the sizeof method introduces latent bugs.
 */
#define TMP_TILEMAP_SOURCE_LEN 1024

Tilemap *Tilemap_load(const char *filename) {
	Tilemap *tilemap = NULL;
	SDL_Surface *tileset;
	SDL_Rect src = { 0, 0, TILE_SZ, TILE_SZ };
	SDL_Rect dst = { 0, 0, TILE_SZ, TILE_SZ };
	char source[TMP_TILEMAP_SOURCE_LEN];
	int tileset_w, tileset_h;
	FILE *f = NULL;
	int i, j, k;

	/* load data */
	if (!filename)
		return NULL;
	if ((f = fopen(filename, "rb")) == NULL)
	    return NULL;
	tilemap = malloc(sizeof(Tilemap));
	for (i = 0; (source[i] = fgetc(f)) != '\0'; i++);
	fread(&tilemap->w, sizeof(int), 1, f);
	fread(&tilemap->h, sizeof(int), 1, f);
	fread(&tilemap->depth, sizeof(int), 1, f);
	tilemap->data = malloc(sizeof(Uint32 **) * tilemap->depth);
	for (i = 0; i < tilemap->depth; i++) {
		tilemap->data[i] = malloc(sizeof(Uint32 *) * tilemap->h);
		for (j = 0; j < tilemap->h; j++) {
			tilemap->data[i][j] = malloc(sizeof(Uint32) * tilemap->w);
			for (k = 0; k < tilemap->w; k++) {
				fread(&tilemap->data[i][j][k], sizeof(Uint32), 1, f);
			}
		}
	}
	tilemap->collision = tilemap->data[tilemap->depth - 1];
	tilemap->layer_w = tilemap->w * TILE_SZ;
	tilemap->layer_h = tilemap->h * TILE_SZ;
	fclose(f);
	/* load tileset and tiles */
	/*TODO: get filename from the TMX file */
	tileset = SDL_LoadBMP("res/tilemap.bmp");
	tileset_w = tileset->w / TILE_SZ;
	tileset_h = tileset->h / TILE_SZ;
	/* draw layers */
	tilemap->background = SDL_CreateRGBSurface(SDL_HWSURFACE, tilemap->layer_w, tilemap->layer_h, SCREEN_BPP, 0, 0, 0, 0);
	tilemap->foreground = SDL_CreateRGBSurface(SDL_HWSURFACE, tilemap->layer_w, tilemap->layer_h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_FillRect(tilemap->background, NULL, 0xff00ff);
	SDL_FillRect(tilemap->foreground, NULL, 0xff00ff);
	SDL_SetColorKey(tilemap->background, SDL_SRCCOLORKEY, SDL_MapRGB(tileset->format, 255, 0, 255));
	SDL_SetColorKey(tilemap->foreground, SDL_SRCCOLORKEY, SDL_MapRGB(tileset->format, 255, 0, 255));
	for (i = 0; i < tilemap->depth - 1; i++) {
		SDL_Surface *layer = SDL_CreateRGBSurface(SDL_HWSURFACE, tilemap->layer_w, tilemap->layer_h, SCREEN_BPP, 0, 0, 0, 0);
		SDL_SetColorKey(layer, SDL_SRCCOLORKEY, SDL_MapRGB(layer->format, 255, 0, 255));
		SDL_FillRect(layer, NULL, 0xff00ff);
		for (j = 0; j < tilemap->h; j++) {
			for (k = 0; k < tilemap->w; k++) {
				int datum = tilemap->data[i][j][k] - 1;
				src.x = datum % tileset_w * TILE_SZ;
				src.y = datum / tileset_w * TILE_SZ;
				dst.x = k * TILE_SZ;
				dst.y = j * TILE_SZ;
				SDL_BlitSurface(tileset, &src, layer, &dst);
			}
		}
		SDL_BlitSurface(layer, NULL, i < 2 ? tilemap->background : tilemap->foreground, NULL);
	}
	return tilemap;
}

void Tilemap_draw(Tilemap * tilemap, SDL_Surface * surface) {
}

void Tilemap_free(Tilemap * tilemap) {
	int i, j;

	for (i = 0; i < tilemap->depth; i++) {
		for (j = 0; j < tilemap->w; j++)
			free(tilemap->data[i][j]);
		free(tilemap->data[i]);
	}
	free(tilemap->data);
	free(tilemap->background);
	free(tilemap->foreground);
	free(tilemap);
}

int Tilemap_tile_is_occupied(Tilemap * tilemap, int x, int y) {
	return x < 0 || y < 0 || x >= tilemap->w || y >= tilemap->h
		|| tilemap->collision[y][x];
}

int Tilemap_pixel_is_occupied(Tilemap * tilemap, int x, int y) {
	return (Tilemap_tile_is_occupied(tilemap, x / TILE_SZ, y / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, (x + TILE_SZ - 1) / TILE_SZ, y / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, x / TILE_SZ, (y + TILE_SZ - 1) / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, (x + TILE_SZ - 1) / TILE_SZ, (y + TILE_SZ - 1) / TILE_SZ));
}
