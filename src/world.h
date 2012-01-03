#ifndef WORLD_H
#define WORLD_H



#include "tmx.h"

typedef struct {
	int nothing;
} World;

World *createWorld(const char *filename);
<<<<<<< HEAD
int  updateWorld(World *world, int CurrentFPS);
int  drawWorld(World *world, SDL_Surface *surf);
void freeWorld(World *world);
=======
int updateWorld(World * world);
int drawWorld(World * world, SDL_Surface * surf);
void freeWorld(World * world);
>>>>>>> dae736970a6a554d360699b98647d60d468dc1b1

#endif
