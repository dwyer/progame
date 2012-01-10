#ifndef TILEMAP_H
#define TILEMAP_H

typedef struct Tilemap Tilemap;

Tilemap *Tilemap_load(const char *filename);
void Tilemap_free(Tilemap * tilemap);
SDL_Surface *Tilemap_background(const Tilemap *tilemap);
SDL_Surface *Tilemap_foreground(const Tilemap *tilemap);
int Tilemap_layer_w(const Tilemap *tilemap);
int Tilemap_layer_h(const Tilemap *tilemap);
int Tilemap_tile_is_occupied(Tilemap * tilemap, int x, int y);
int Tilemap_pixel_is_occupied(Tilemap * tilemap, int x, int y);

#endif
