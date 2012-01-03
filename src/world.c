#include "main.h"
#include "player.h"
#include "tmx.h"

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

World *createWorld(const char *filename)
{
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
	if ((world->tilemap = TMP_LoadTilemap(filename)) == NULL){
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return NULL;
	}
	if ((world->player = createPlayer(0, 0)) == NULL) {
		fputs("Failed to load player.\n", stderr);
		return NULL;
	}
	return world;
}

int updateWorld(World *world, int CurrentFPS) {
	SDL_Event event;
	int my = 0, mx = 0;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				world->controller.left = 1;
			} else if (event.key.keysym.sym == SDLK_RIGHT) {
				world->controller.right = 1;
			} else if (event.key.keysym.sym == SDLK_UP) {
				world->controller.up = 1;
			} else if (event.key.keysym.sym == SDLK_DOWN) {
				world->controller.down = 1;
			} else if (event.key.keysym.sym == SDLK_q) {
				return 0;
			}
		} else if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				world->controller.left = 0;
			} else if (event.key.keysym.sym == SDLK_RIGHT) {
				world->controller.right = 0;
			} else if (event.key.keysym.sym == SDLK_UP) {
				world->controller.up = 0;
			} else if (event.key.keysym.sym == SDLK_DOWN) {
				world->controller.down = 0;
			}
		} else if (event.type == SDL_QUIT) {
			return 0;
		}
	}

	/* Update player position. */
	mx = 0;
	my = 0;
	if (world->controller.left) {
		mx = Interpolate(PlayerSpeed, CurrentFPS) * -1;
	} else if (world->controller.right) {
		mx = Interpolate(PlayerSpeed, CurrentFPS);
	}
	if (world->controller.up) {
		my = Interpolate(PlayerSpeed, CurrentFPS) * -1; 
	} else if (world->controller.down) {
		my = Interpolate(PlayerSpeed, CurrentFPS);
	}

	if (mx
		&& !TMP_PixelIsOccupied(world->tilemap, world->player->pos.x + mx,
								world->player->pos.y))
		movePlayer(world->player, mx, 0);
	if (my
		&& !TMP_PixelIsOccupied(world->tilemap, world->player->pos.x,
								world->player->pos.y + my))
		movePlayer(world->player, 0, my);
	
	if (!mx && !my)
	switchPlayerstate(world->player, p_idle);

	/* Update camera position. */
	world->camera.x = world->player->pos.x - (SCREEN_W - world->player->src.w) / 2;
	if (world->camera.x < 0)
		world->camera.x = 0;
	else if (world->camera.x >= world->tilemap->width * world->player->src.w - SCREEN_W)
		world->camera.x = world->tilemap->width * world->player->src.w - SCREEN_W - 1;
	world->camera.y = world->player->pos.y - (SCREEN_H - world->player->src.h) / 2;
	if (world->camera.y < 0)
		world->camera.y = 0;
	else if (world->camera.y >= world->tilemap->height * world->player->src.h - SCREEN_H)
		world->camera.y = world->tilemap->height * world->player->src.h - SCREEN_H - 1;
	return 1;
}

int drawWorld(World * world, SDL_Surface * surf)
{
	int i;

	for (i = 0; i < world->tilemap->depth - 1; i++) {
		if (i == 2) {			/* no magic numbers, please!!! */
			if (drawPlayer(world->player, surf, world->camera)) {
				return -1;
			}

		}
		if (SDL_BlitSurface
			(world->tilemap->layers[i], &world->camera, surf, NULL)) {
			return -1;
		}
	}
	return 0;
}

void freeWorld(World * world)
{
	TMP_FreeTilemap(world->tilemap);
	freePlayer(world->player);
	free(world);
}
