#ifndef WORLD_H
#define WORLD_H

#include "input.h"

typedef struct World World;

World *World_create(void);
void World_free(World * world);

int World_event(World * world, Input * input, SDL_UserEvent event);
int World_update(World * world, Input * input);
int World_draw(World * world, SDL_Surface * surface);

#endif
