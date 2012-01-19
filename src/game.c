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

Tilemap *tilemap;
Entity *player;
EntityList *entities;
Script *script;

void Game_new(void) {
	if ((entities = EntityList_new()) == NULL) {
		Game_free();
	}
	player = NULL;
	tilemap = NULL;
}

void Game_free(void) {
	Tilemap_free(tilemap);
	EntityList_free(entities);
}

void Game_set_script(Script *script) {
	script = script;
}

void Game_set_tilemap(Tilemap *new) {
	if (tilemap)
		Tilemap_free(tilemap);
	tilemap = new;
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
		if (entities->first == NULL)
			player = event.data1;
		EntityList_append(entities, event.data1);
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

	if (player)
		Entity_set_vel(player, vel.x, vel.y);

	/* Update each entity. */
	for (node = entities->first; node != NULL; node = node->next) {
		SDL_Rect pos, vel;

		Script_call(script, Entity_get_update_callback_ref(node->this));
		pos = Entity_get_pos(node->this);
		vel = Entity_get_vel(node->this);
		if (Tilemap_is_region_occupied(tilemap, pos.x + vel.x, pos.y, pos.w, pos.h))
			vel.x = 0;
		if (Tilemap_is_region_occupied(tilemap, pos.x, pos.y + vel.y, pos.w, pos.h))
			vel.y = 0;
		Entity_set_pos(node->this, pos.x + vel.x, pos.y + vel.y);
	}
	return 1;
}

/**
 * \param focus The portion of the map to focus on.
 * \return A rectangular portion of the tilemap centered around the focus.
 */
SDL_Rect Game_get_camera(SDL_Rect focus) {
	SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
	SDL_Rect area = Tilemap_get_area(tilemap);

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
 * \param screen The screen.
 * \return 0 on success, non-zero on failure.
 */
int Game_draw(SDL_Surface * screen) {
	SDL_Rect center = Entity_get_pos(player);
	SDL_Rect camera = Game_get_camera(center);
	EntityNode *node;

	if (Tilemap_draw_background(tilemap, screen, camera))
		return -1;
	/* This loop simply draws entities in the order they appear in the list.
	 * This is not ideal, as it could cause entities high on the screen to be
	 * draw over entities low on the screen.  This shall be solved by making
	 * sure the list is sorted by entity position, from top to bottom. */
	for (node = entities->first; node != NULL; node = node->next)
		if (Entity_draw(node->this, screen, camera))
			return -1;
	if (Tilemap_draw_foreground(tilemap, screen, camera))
		return -1;
	return 0;
}
