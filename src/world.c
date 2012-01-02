#include "main.h"
#include "world.h"

#define PLAYER_SPEED 2

World *createWorld(const char *filename) {
	World *world = NULL;
	
	world = malloc(sizeof(World));
	world->camera.x = 0;
	world->camera.y = 0;
	world->camera.w = SCREEN_W;
	world->camera.h = SCREEN_H;
	world->controller.up = 0;
	world->controller.down = 0;
	world->controller.left = 0;
	world->controller.right = 0;
	if ((world->tilemap = TMP_LoadTilemap(filename)) == NULL) {
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return NULL;
	}
	if ((world->player = createPlayer(0, 0)) == NULL) {
		fputs("Failed to load player.\n", stderr);
		return NULL;
	}
	return world;
}

void updateWorld(World *world) {
	int my = 0, mx = 0;

	/* Update player position. */
	mx = 0;
	my = 0;
	if (world->controller.left) {
		/*mx = Interpolate(SPEEDPPS, CurrentFPS) * -1;*/
		mx  = -PLAYER_SPEED;
	} else if (world->controller.right) {
		/*mx = Interpolate(SPEEDPPS, CurrentFPS);*/
		mx = PLAYER_SPEED;
	}
	if (world->controller.up) {
		/*my = Interpolate(SPEEDPPS, CurrentFPS) * -1;*/
		my = -PLAYER_SPEED;
	} else if (world->controller.down) {
		/*my = Interpolate(SPEEDPPS, CurrentFPS);*/
		my = PLAYER_SPEED;
	}
	
	if (mx || my)
	movePlayer(world->player, mx, my);

	/* Update camera position. */
	world->camera.x = world->player->pos.x - (SCREEN_W - 16) / 2;
	if (world->camera.x < 0)
		world->camera.x = 0;
	else if (world->camera.x >= world->tilemap->width * 16 - SCREEN_W)
		world->camera.x = world->tilemap->width * 16 - SCREEN_W - 1;
	world->camera.y = world->player->pos.y - (SCREEN_H - 16) / 2;
	if (world->camera.y < 0)
		world->camera.y = 0;
	else if (world->camera.y >= world->tilemap->height * 16 - SCREEN_H)
		world->camera.y = world->tilemap->height * 16 - SCREEN_H - 1;
}

int drawWorld(World *world, SDL_Surface *surf) {
	int i;

	for (i = 0; i < world->tilemap->depth; i++) {
		if (i == world->tilemap->depth - 1) {
			if (drawPlayer(world->player, surf, world->camera)) {
				return -1;
			}
			
		}
		if (SDL_BlitSurface(world->tilemap->layers[i], &world->camera, surf, NULL)) {
			return -1;
		}
	}
	return 0;
}

void freeWorld(World *world) {
	TMP_FreeTilemap(world->tilemap);
	freePlayer(world->player);
	free(world);
}
