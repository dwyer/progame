#ifndef WORLD_H
#define WORLD_H

#include "input.h"

typedef struct _World World;

World *createWorld(const char *filename);
int updateWorld(World * world, Input input);
int drawWorld(World * world, SDL_Surface * surf);
void freeWorld(World * world);

#endif
