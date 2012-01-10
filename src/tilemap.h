#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct {
	int w;
	int h;
	int depth;
	int layer_w;
	int layer_h;
    int tile_w;
    int tile_h;
    int *collision;
	SDL_Surface *background;
	SDL_Surface *foreground;
} Tilemap;

Tilemap *Tilemap_load(const char *filename);
void Tilemap_draw(Tilemap * tilemap, SDL_Surface * surface);
void Tilemap_free(Tilemap * tilemap);
int Tilemap_tile_is_occupied(Tilemap * tilemap, int x, int y);
int Tilemap_pixel_is_occupied(Tilemap * tilemap, int x, int y);

#endif
