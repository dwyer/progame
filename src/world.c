#include "main.h"
#include "player.h"
#include "tilemap.h"
#include "input.h"
#include "event.h"

typedef struct {
	SDL_Rect camera;
	Tilemap *tilemap;
	Player *player;
} World;

World *World_create(const char *filename) {
	World *world = NULL;

	world = malloc(sizeof(World));
	world->camera.x = 0;
	world->camera.y = 0;
	world->camera.w = SCREEN_W;
	world->camera.h = SCREEN_H;
	if ((world->tilemap = Tilemap_load(filename)) == NULL) {
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return NULL;
	}
	if ((world->player = Player_create(27*16, 3*16)) == NULL) {
		fputs("Failed to load player.\n", stderr);
		return NULL;
	}
	return world;
}

void World_free(World *world) {
	Tilemap_free(world->tilemap);
	Player_free(world->player);
	free(world);
}

int World_event(World *world, Input *input, SDL_UserEvent event) {

	if (event.code == EVENT_WORLD_UPDATE)
		World_update(world, input);
	else if (event.code == EVENT_INPUT_QUIT) {
		SDL_Event e;
		e.type = SDL_QUIT;
		SDL_PushEvent(&e);
	} else if (event.code == EVENT_INPUT_MOVE_UP)
		input->up = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_DOWN)
		input->down = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_LEFT)
		input->left = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_RIGHT)
		input->right = (event.data1 != NULL);
	else if (event.code == EVENT_CONFIG_BINDKEY) {
		InputCode *code = NULL;
		InputCode *ic = (InputCode *)event.data1;
		InputCode nil = { -1, -1 };

		/* traverse to the end of the input codes */
		for (code = input->input_codes; code->sym != -1; code++);
		*code = *ic;
		code++;
		*code = nil; 
	}
	return 0;
}

int World_update(World * world, Input *input) {
	int my = 0, mx = 0;

	/* Update player position. */
	mx = 0;
	my = 0;
	if (input->left) {
		mx -= world->player->speed;
	}
	if (input->right) {
		mx += world->player->speed;
	}
	if (input->up) {
		my -= world->player->speed;
	}
	if (input->down) {
		my += world->player->speed;
	}

	if (mx
		&& Tilemap_pixel_is_occupied(world->tilemap, world->player->pos.x + mx,
							   world->player->pos.y))
		mx = 0;					/* Allowing for soueast, etc */
	if (my && Tilemap_pixel_is_occupied(world->tilemap, world->player->pos.x,
								  world->player->pos.y + my))
		my = 0;

	if (!mx && !my)
		Player_update_state(world->player, p_idle);
	else
		Player_move(world->player, mx, my);

	/* Update camera position. */
	world->camera.x = (world->player->pos.x - (SCREEN_W - world->player->pos.w) / 2);
	if (world->tilemap->layerw > SCREEN_W)
		if (world->camera.x < 0)
			world->camera.x = 0;
		else if (world->camera.x >= world->tilemap->layerw - SCREEN_W)
			world->camera.x = world->tilemap->layerw - SCREEN_W - 1;
	world->camera.y = (world->player->pos.y - (SCREEN_H - world->player->pos.h) / 2);
	if (world->tilemap->layerh > SCREEN_H)
		if (world->camera.y < 0)
			world->camera.y = 0;
		else if (world->camera.y >= world->tilemap->layerh - SCREEN_H)
			world->camera.y = world->tilemap->layerh - SCREEN_H - 1;
	return 1;
}

int World_draw(World * world, SDL_Surface * surf) {
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
