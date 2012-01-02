#ifndef WORLD_H
#define WORLD_H

#include "tmx.h"
#include "player.h"

typedef struct {
	SDL_Rect camera;
	TMP_Tilemap *tilemap;
	Player *player;
	struct {
		int up;
		int down;
		int left;
		int right;
	} controller;
} World;

World *createWorld(const char *filename);
void updateWorld(World *world);
int drawWorld(World *world, SDL_Surface *surf);
void freeWorld(World *world);

#endif
