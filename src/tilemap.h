#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct Tilemap Tilemap;

Tilemap *Tilemap_open(const char *filename);
void Tilemap_free(Tilemap * tilemap);

SDL_Rect Tilemap_get_area(const Tilemap * tilemap);
SDL_Rect Tilemap_get_size(const Tilemap * tilemap);
int Tilemap_is_tile_occupied(const Tilemap * tilemap, int x, int y);
int Tilemap_is_pixel_occupied(const Tilemap * tilemap, int x, int y);
int Tilemap_is_region_occupied(const Tilemap * tilemap, int x, int y,
							   int w, int h);

int Tilemap_draw_background(const Tilemap * tilemap, SDL_Surface * screen,
							SDL_Rect camera);
int Tilemap_draw_foreground(const Tilemap * tilemap, SDL_Surface * screen,
							SDL_Rect camera);

#endif
