#ifndef TILEMAP_H
#define TILEMAP_H

int Tilemap_open(const char *filename);
void Tilemap_close(void);

SDL_Rect Tilemap_get_area(void);
SDL_Rect Tilemap_get_size(void);
int Tilemap_is_tile_occupied(int x, int y);
int Tilemap_is_pixel_occupied(int x, int y);
int Tilemap_is_region_occupied(int x, int y, int w, int h);
int Tilemap_draw_background(SDL_Surface *screen, SDL_Rect camera);
int Tilemap_draw_foreground(SDL_Surface *screen, SDL_Rect camera);

#endif
