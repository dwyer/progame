#ifndef WORLD_H
#define WORLD_H

#include "input.h"

typedef struct _World World;

World *World_create(const char *filename);
int World_update(World * world, Input input);
int World_draw(World * world, SDL_Surface * surf);
void World_free(World * world);

#endif
