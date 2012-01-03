#include "main.h"
#include "player.h"
#include "tmx.h"
#include "input.h"

typedef struct {
	SDL_Rect camera;
	TMP_Tilemap *tilemap;
	Player *player;
} World;

World *createWorld(const char *filename)
{
	World *world = NULL;

	world = malloc(sizeof(World));
	world->camera.x = 0;
	world->camera.y = 0;
	world->camera.w = SCREEN_W;
	world->camera.h = SCREEN_H;
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

int updateWorld(World * world, Input input)
{
	int my = 0, mx = 0;

	/* Update player position. */
	mx = 0;
	my = 0;
	if (input.left) {
		mx -= world->player->speed;
	} else if (input.right) {
		mx += world->player->speed;
	}
	if (input.up) {
		my -= world->player->speed;
	} else if (input.down) {
		my += world->player->speed;
	}

	if (mx
		&& TMP_PixelIsOccupied(world->tilemap, world->player->pos.x + mx,
							   world->player->pos.y))
		mx = 0;					/* Allowing for soueast, etc */
	if (my && TMP_PixelIsOccupied(world->tilemap, world->player->pos.x,
								  world->player->pos.y + my))
		my = 0;

	if (!mx && !my)
		updatePlayerstate(world->player, p_idle);
	else
		movePlayer(world->player, mx, my);

	/* Update camera position. */
	world->camera.x =
		world->player->pos.x - (SCREEN_W - world->player->pos.w) / 2;
	if (world->camera.x < 0)
		world->camera.x = 0;
	else if (world->camera.x >=
			 world->tilemap->width * world->player->pos.w - SCREEN_W)
		world->camera.x =
			world->tilemap->width * world->player->pos.w - SCREEN_W - 1;
	world->camera.y =
		world->player->pos.y - (SCREEN_H - world->player->pos.h) / 2;
	if (world->camera.y < 0)
		world->camera.y = 0;
	else if (world->camera.y >=
			 world->tilemap->height * world->player->pos.h - SCREEN_H)
		world->camera.y =
			world->tilemap->height * world->player->pos.h - SCREEN_H - 1;
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
