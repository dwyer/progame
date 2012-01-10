#include "main.h"
#include "player.h"
#include "tilemap.h"
#include "input.h"
#include "event.h"
#include "entity.h"

typedef struct {
	Tilemap *tilemap;
	Player *player;
} World;

/**
 * Creates a new World object.
 * \param filename Path to this world's tilemap.
 * return A brave new world.
 */
World *World_create(const char *filename) {
	World *world = NULL;

	world = malloc(sizeof(World));
	if ((world->tilemap = Tilemap_load(filename)) == NULL) {
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return NULL;
	}
	if ((world->player = Player_create(27 * 16, 3 * 16)) == NULL) {
		fputs("Failed to create player.\n", stderr);
		return NULL;
	}
	return world;
}

/**
 * Frees the given World and all it's children.
 * \param world The World to be freed.
 */
void World_free(World * world) {
	Tilemap_free(world->tilemap);
	Player_free(world->player);
	free(world);
}

/**
 * Handles the given user event.
 */
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

/**
 * Update the state of each entity.
 * \param world The World.
 * \param input The player's Input.
 * \return 1
 */
int World_update(World * world, Input * input) {
	SDL_Rect vel = { 0, 0 };
    SDL_Rect center = Player_get_pos(world->player);
    int speed = Player_get_speed(world->player);

	/* Update player position. */
	if (input->left)
		vel.x -= speed;
	if (input->right)
		vel.x += speed;
	if (input->up)
		vel.y -= speed;
	if (input->down)
		vel.y += speed;

	if (vel.x &&
        Tilemap_pixel_is_occupied(world->tilemap, center.x + vel.x, center.y))
		vel.x = 0;
	if (vel.y &&
        Tilemap_pixel_is_occupied(world->tilemap, center.x, center.y + vel.y))
		vel.y = 0;

	if (!vel.x && !vel.y)
		Player_update_state(world->player, p_idle);
	else
		Player_move(world->player, vel.x, vel.y);
	return 1;
}

/**
 * \param world The world.
 * \param focus The portion of the map to focus on.
 * \return A rectangular portion of the tilemap centered around the focus.
 */
SDL_Rect World_get_camera(World *world, SDL_Rect focus) {
    SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
    SDL_Rect area = Tilemap_get_area(world->tilemap);

	camera.x = focus.x - (SCREEN_W - focus.w) / 2;
	if (area.w > SCREEN_W)
		if (camera.x < 0)
			camera.x = 0;
		else if (camera.x >= area.w - SCREEN_W)
			camera.x = area.w - SCREEN_W - 1;
	camera.y = focus.y - (SCREEN_H - focus.h) / 2;
	if (area.h > SCREEN_H)
		if (camera.y < 0)
			camera.y = 0;
		else if (camera.y >= area.h - SCREEN_H)
			camera.y = area.h - SCREEN_H - 1;
    return camera;
}

/**
 * Draw tilemap and entities.
 * \param world The world.
 * \param screen The screen.
 * return 0 on success, non-zero on failure.
 */
int World_draw(World * world, SDL_Surface * screen) {
    SDL_Rect center = Player_get_pos(world->player);
	SDL_Rect camera = World_get_camera(world, center);
    SDL_Surface *bg = NULL;
	int i;

    if (Tilemap_draw_background(world->tilemap, screen, camera))
        return -1;
    if (Player_draw(world->player, screen, camera))
        return -1;
    if (Tilemap_draw_foreground(world->tilemap, screen, camera))
        return -1;
	return 0;
}
