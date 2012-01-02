#include <stdio.h>
#include "SDL/SDL.h"
#include "tmx.h"

#define TILE_SZ 16

TMP_Tilemap *TMP_LoadTilemap(const char *filename)
{
	TMP_Tilemap *result = NULL;
	SDL_Rect src = { 0, 0, TILE_SZ, TILE_SZ };
	SDL_Rect dst = { 0, 0, TILE_SZ, TILE_SZ };
	FILE *f = fopen(filename, "rb");
	int i, j, k;

	/* load data */
	result = malloc(sizeof(TMP_Tilemap));
	for  (i = 0; (result->source[i] = fgetc(f)) != '\0'; i++);
	
	fread(&result->width, sizeof(int), 1, f);
	fread(&result->height, sizeof(int), 1, f);
	fread(&result->depth, sizeof(int), 1, f);
	result->data = malloc(sizeof(Uint32 **) * result->depth);
	
	for (i = 0; i < result->depth; i++) {
		result->data[i] = malloc(sizeof(Uint32 *) * result->height);
		for (j = 0; j < result->height; j++) {
			result->data[i][j] = malloc(sizeof(Uint32) * result->width);
			for (k = 0; k < result->width; k++) {
				fread(&result->data[i][j][k], sizeof(Uint32), 1, f);
			}
		}
	}
	result->collision = result->data[result->depth-1];
	fclose(f);
	/* load tileset and tiles */
	/*TODO: get filename from the TMX file */
	result->tileset = SDL_LoadBMP("res/tilemap.bmp");
	result->tilesetw = result->tileset->w / TILE_SZ;
	result->tileseth = result->tileset->h / TILE_SZ;
	/* draw layers */
	result->layers = malloc(sizeof(SDL_Surface *) * result->depth);
	for (i = 0; i < result->depth; i++) {
		result->layers[i] =
			SDL_CreateRGBSurface(SDL_HWSURFACE,
								 result->width * TILE_SZ,
								 result->height * TILE_SZ, 32, 0, 0, 0, 0);
		SDL_SetColorKey(result->layers[i], SDL_SRCCOLORKEY,
						SDL_MapRGB(result->layers[i]->format, 255,
								   0, 255));
		SDL_FillRect(result->layers[i], NULL, 0xff00ff);
		for (j = 0; j < result->height; j++) {
			for (k = 0; k < result->width; k++) {
				int datum = result->data[i][j][k] - 1;
				src.x = datum % result->tilesetw * TILE_SZ;
				src.y = datum / result->tilesetw * TILE_SZ;
				dst.x = k * TILE_SZ;
				dst.y = j * TILE_SZ;
				SDL_BlitSurface(result->tileset, &src,
								result->layers[i], &dst);
			}
		}
	}
	return result;
}

void TMP_DrawTilemap(TMP_Tilemap * tilemap, SDL_Surface * surface)
{
	int i;

	for (i = 0; i < tilemap->depth; i++)
		SDL_BlitSurface(tilemap->layers[i], NULL, surface, NULL);
}

void TMP_FreeTilemap(TMP_Tilemap * tilemap)
{
	int i, j;

	for (i = 0; i < tilemap->depth; i++) {
		for (j = 0; j < tilemap->width; j++)
			free(tilemap->data[i][j]);
		free(tilemap->data[i]);
		SDL_FreeSurface(tilemap->layers[i]);
	}
	free(tilemap->data);
	free(tilemap->layers);
}

int TMP_TileIsOccupied(TMP_Tilemap *tilemap, int x, int y) {
	return x < 0 || y < 0 || x >= tilemap->width || y >= tilemap->height || tilemap->collision[y][x];
}
int TMP_PixelIsOccupied(TMP_Tilemap *tilemap, int x, int y) {
	return (TMP_TileIsOccupied(tilemap, x / 16, y / 16) ||
			TMP_TileIsOccupied(tilemap, (x + 15) / 16, y / 16) ||
			TMP_TileIsOccupied(tilemap, x / 16, (y + 15) / 16) ||
			TMP_TileIsOccupied(tilemap, (x + 15) / 16, (y + 15) / 16));
}
