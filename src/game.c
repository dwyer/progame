#include <assert.h>

#include <SDL/SDL.h>

#include "main.h"
#include "tilemap.h"
#include "input.h"
#include "event.h"
#include "entity.h"
#include "entities.h"
#include "script.h"
#include "game.h"

World *world;

struct World {
	Tilemap *tilemap;
	Entity *player;
	EntityList *entities;
	Script *script;
};

/**
 * Creates a new World object.
 * \param filename Path to this world's tilemap.
 * \return A brave new world.
 */
World *Game_new(void) {
	World *world = malloc(sizeof(*world));

	if (world == NULL)
		return NULL;

	if ((world->entities = EntityList_new()) == NULL) {
		Game_free(world);
		return NULL;
	}

	world->player = NULL;
	world->tilemap = NULL;
	return world;
}

/**
 * Frees the given World and all it's children.
 * \param world The World to be freed.
 */
void Game_free(World *world) {
	Tilemap_free(world->tilemap);
	EntityList_free(world->entities);
	free(world);
}

void Game_set_script(Script *script) {
	world->script = script;
}

void Game_set_tilemap(Tilemap *tilemap) {
	if (world->tilemap)
		Tilemap_free(world->tilemap);
	world->tilemap = tilemap;
}

/**
 * Handles the given user event.
 * \return 0
 */
int Game_event(SDL_UserEvent event) {
	if (event.code == EVENT_WORLD_UPDATE)
		Game_update();
	else if (event.code == EVENT_INPUT_MOVE_UP)
		input.up = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_DOWN)
		input.down = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_LEFT)
		input.left = (event.data1 != NULL);
	else if (event.code == EVENT_INPUT_MOVE_RIGHT)
		input.right = (event.data1 != NULL);
	else if (event.code == EVENT_ENTITY_NEW) {
		if (world->entities->first == NULL)
			world->player = event.data1;
		EntityList_append(world->entities, event.data1);
	} else if (event.code == EVENT_TILEMAP_OPEN)
		Game_set_tilemap(event.data1);
	else if (event.code == EVENT_CONFIG_BINDKEY) {
		/* Add code/sym pair to the end of list */
		int i;
		for (i = 0; input.codes[i].sym != -1; i++);
		input.codes[i++] = *(InputCode *) event.data1;
		input.codes[i].code = -1;
		input.codes[i].sym = -1;
	}
	return 0;
}

/**
 * Update the state of each entity.
 * \param world The World.
 * \param input The player's Input.
 * \return 1
 */
int Game_update() {
	SDL_Rect vel = { 0, 0 };
	EntityNode *node;
	int speed = 1;

	/* Update player position. */
	if (input.left)
		vel.x -= speed;
	if (input.right)
		vel.x += speed;
	if (input.up)
		vel.y -= speed;
	if (input.down)
		vel.y += speed;

	if (world->player)
		Entity_set_vel(world->player, vel.x, vel.y);

	/* Update each entity. */
	for (node = world->entities->first; node != NULL; node = node->next) {
		SDL_Rect pos, vel;

		Script_call(world->script, Entity_get_update_callback_ref(node->this));
		pos = Entity_get_pos(node->this);
		vel = Entity_get_vel(node->this);
		if (Tilemap_is_region_occupied(world->tilemap, pos.x + vel.x, pos.y, pos.w, pos.h))
			vel.x = 0;
		if (Tilemap_is_region_occupied(world->tilemap, pos.x, pos.y + vel.y, pos.w, pos.h))
			vel.y = 0;
		Entity_set_pos(node->this, pos.x + vel.x, pos.y + vel.y);
	}
	return 1;
}

/**
 * \param world The world.
 * \param focus The portion of the map to focus on.
 * \return A rectangular portion of the tilemap centered around the focus.
 */
SDL_Rect Game_get_camera(SDL_Rect focus) {
	SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
	SDL_Rect area = Tilemap_get_area(world->tilemap);

	camera.x = focus.x - (SCREEN_W - focus.w) / 2;
	if (area.w > SCREEN_W) {
		if (camera.x < 0)
			camera.x = 0;
		else if (camera.x >= area.w - SCREEN_W)
			camera.x = area.w - SCREEN_W - 1;
	}
	camera.y = focus.y - (SCREEN_H - focus.h) / 2;
	if (area.h > SCREEN_H) {
		if (camera.y < 0)
			camera.y = 0;
		else if (camera.y >= area.h - SCREEN_H)
			camera.y = area.h - SCREEN_H - 1;
	}
	return camera;
}

/**
 * Draw tilemap and entities.
 * \param world The world.
 * \param screen The screen.
 * \return 0 on success, non-zero on failure.
 */
int Game_draw(SDL_Surface * screen) {
	SDL_Rect center = Entity_get_pos(world->player);
	SDL_Rect camera = Game_get_camera(center);
	EntityNode *node;

	if (Tilemap_draw_background(world->tilemap, screen, camera))
		return -1;
	/* This loop simply draws entities in the order they appear in the list.
	 * This is not ideal, as it could cause entities high on the screen to be
	 * draw over entities low on the screen.  This shall be solved by making
	 * sure the list is sorted by entity position, from top to bottom. */
	for (node = world->entities->first; node != NULL; node = node->next)
		if (Entity_draw(node->this, screen, camera))
			return -1;
	if (Tilemap_draw_foreground(world->tilemap, screen, camera))
		return -1;
	return 0;
}
