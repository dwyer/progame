#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct Tilemap Tilemap;

Tilemap *Tilemap_load(const char *filename);
void Tilemap_free(Tilemap * tilemap);

SDL_Rect Tilemap_get_area(const Tilemap * tilemap);
int Tilemap_tile_is_occupied(const Tilemap * tilemap, int x, int y);
int Tilemap_pixel_is_occupied(const Tilemap * tilemap, int x, int y);
int Tilemap_region_is_occupied(const Tilemap * tilemap, int x, int y,
							   int w, int h);

int Tilemap_draw_background(const Tilemap * tilemap, SDL_Surface * screen,
							SDL_Rect camera);
int Tilemap_draw_foreground(const Tilemap * tilemap, SDL_Surface * screen,
							SDL_Rect camera);

#endif
