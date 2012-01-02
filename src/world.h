#ifndef WORLD_H
#define WORLD_H

#include "tmx.h"

typedef struct {
} World;

World *createWorld(const char *filename);
int updateWorld(World * world);
int drawWorld(World * world, SDL_Surface * surf);
void freeWorld(World * world);

#endif
