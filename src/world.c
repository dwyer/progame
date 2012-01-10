#include "main.h"
#include "player.h"
#include "tilemap.h"
#include "input.h"
#include "event.h"

typedef struct {
	Tilemap *tilemap;
	Player *player;
} World;

World *World_create(const char *filename) {
	World *world = NULL;

	world = malloc(sizeof(World));
	if ((world->tilemap = Tilemap_load(filename)) == NULL) {
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return NULL;
	}
	if ((world->player = Player_create(27 * 16, 3 * 16)) == NULL) {
		fputs("Failed to load player.\n", stderr);
		return NULL;
	}
	return world;
}

void World_free(World * world) {
	Tilemap_free(world->tilemap);
	Player_free(world->player);
	free(world);
}

int World_event(World * world, Input * input, SDL_UserEvent event) {
	if (event.code == EVENT_WORLD_UPDATE)
		World_update(world, input);
	else if (event.code == EVENT_INPUT_MOVE_UP)
		input->up = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_DOWN)
		input->down = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_LEFT)
		input->left = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_RIGHT)
		input->right = (event.data1 != NULL);
	else if (event.code == EVENT_CONFIG_BINDKEY) {
		/* Add code/sym pair to the end of list */
		int i;
		for (i = 0; input->codes[i].sym != -1; i++);
		input->codes[i++] = *(InputCode *) event.data1;
		input->codes[i].code = -1;
		input->codes[i].sym = -1;
	}
	return 0;
}

int World_update(World * world, Input * input) {
	SDL_Rect vel = { 0, 0 };

	/* Update player position. */
	if (input->left) {
		vel.x -= world->player->speed;
	}
	if (input->right) {
		vel.x += world->player->speed;
	}
	if (input->up) {
		vel.y -= world->player->speed;
	}
	if (input->down) {
		vel.y += world->player->speed;
	}

	if (vel.x
		&& Tilemap_pixel_is_occupied(world->tilemap,
									 world->player->pos.x + vel.x,
									 world->player->pos.y))
		vel.x = 0;
	if (vel.y
		&& Tilemap_pixel_is_occupied(world->tilemap, world->player->pos.x,
									 world->player->pos.y + vel.y))
		vel.y = 0;

	if (!vel.x && !vel.y)
		Player_update_state(world->player, p_idle);
	else
		Player_move(world->player, vel.x, vel.y);
	return 1;
}

int World_draw(World * world, SDL_Surface * surface) {
    SDL_Surface *layer = NULL;
	SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
	int i;

    layer = Tilemap_background(world->tilemap);
	/**
	 * Update camera position.
	 */
	camera.x =
		(world->player->pos.x - (SCREEN_W - world->player->pos.w) / 2);
	if (layer->w > SCREEN_W)
		if (camera.x < 0)
			camera.x = 0;
		else if (camera.x >= layer->w - SCREEN_W)
			camera.x = layer->w - SCREEN_W - 1;
	camera.y =
		(world->player->pos.y - (SCREEN_H - world->player->pos.h) / 2);
	if (layer->h > SCREEN_H)
		if (camera.y < 0)
			camera.y = 0;
		else if (camera.y >= layer->h - SCREEN_H)
			camera.y = layer->h - SCREEN_H - 1;

	/**
	 * Draw tilemap and entities.
	 */
    if (SDL_BlitSurface(layer, &camera, surface, NULL)) {
        return -1;
    }
    if (drawPlayer(world->player, surface, camera)) {
        return -1;
    }
    layer = Tilemap_foreground(world->tilemap);
    if (SDL_BlitSurface(layer, &camera, surface, NULL)) {
        return -1;
    }
	return 0;
}
