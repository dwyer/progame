#include <assert.h>
#include <stdbool.h>

#include <SDL/SDL.h>

#include "main.h"
#include "tilemap.h"
#include "input.h"
#include "event.h"
#include "entity.h"
#include "entities.h"
#include "script.h"
#include "game.h"

Input input = { 0, 0, 0, 0 };
InputCode input_codes[INPUT_CODE_NUM] = { {-1, -1} };

struct World {
	Entity *player;
	EntityList *entities;
} world;

/**
 * \return 0 on success, -1 on error.
 */
int Game_init(void) {
	world.player = NULL;
	world.entities = EntityList_new();
	return 0;
}

void Game_quit(void) {
	EntityList_free(world.entities);
	Tilemap_close();
}

/**
 * Play the game.
 * \param screen The screen.
 * \return 0 on success, -1 on error.
 */
int Game_play(SDL_Surface *screen) {
	while (Game_events()) {
		/* Draw. */
		SDL_FillRect(screen, NULL, 0);
		if (Game_draw(screen) || SDL_Flip(screen)) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
	};
	return 0;
}

/**
 * Polls for events. Manages the input queue and handles when input must be
 * passed to the World class. Returns false it the player signaled a quit event,
 * true otherwise.
 * \return false if a quit event was triggered, true otherwise.
 */
bool Game_events(void) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_USEREVENT) {
			Game_event(event.user);
		} else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			/* Instead of hardcoding keyboard events, we'll map them to action
			 * events so they can be configured in scripting. */
			InputCode *code = NULL;
			for (code = input_codes; code->sym != -1; code++) {
				if (code->sym == event.key.keysym.sym) {
					if (code->code == EVENT_INPUT_QUIT)
						return false;
					else
						Event_push(code->code,
								   (void *) (event.type == SDL_KEYDOWN),
								   NULL);
				}
			}
		} else if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
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
	else if (event.code == EVENT_CONFIG_BINDKEY) {
		/* Add code/sym pair to the end of list */
		int i;
		for (i = 0; input_codes[i].sym != -1; i++);
		input_codes[i++] = *(InputCode *) event.data1;
		input_codes[i].code = -1;
		input_codes[i].sym = -1;
	}
	return 0;
}

void Game_add_entity(Entity *entity) {
	assert(entity);
	if (world.entities->first == NULL)
		world.player = entity;
	EntityList_append(world.entities, entity);
}

void Game_set_tilemap(const char *filename) {
	Tilemap_close();
	Tilemap_open(filename);
}

SDL_Rect Game_collision_check(Entity *entity) {
	SDL_Rect ret = { 0, 0, 0, 0 };
	SDL_Rect pos;
	SDL_Rect vel;
	EntityNode *node;

	assert(entity);
	pos = Entity_get_pos(entity);
	vel = Entity_get_vel(entity);
	ret.x = Tilemap_is_region_occupied(pos.x + vel.x, pos.y, pos.w, pos.h);
	ret.y = Tilemap_is_region_occupied(pos.x, pos.y + vel.y, pos.w, pos.h);
	for (node = world.entities->first; node != NULL; node = node->next) {
		if (node->entity == entity)
			continue;
		else if (ret.x && ret.y)
			break;
		ret.x = ret.x || Entity_occupies_region(node->entity,
									   pos.x + vel.x, pos.y, pos.w, pos.h);
		ret.y = ret.y || Entity_occupies_region(node->entity,
									   pos.x, pos.y + vel.y, pos.w, pos.h);
	}
	return ret;
}

/**
 * Update the state of each entity.
 * \return 1
 */
int Game_update(void) {
	SDL_Rect vel = { 0, 0, 0, 0 };
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
	if (world.player)
		Entity_set_vel(world.player, vel.x, vel.y);
	/* Update each entity. */
	for (node = world.entities->first; node != NULL; node = node->next) {
		SDL_Rect pos, vel, col;

		Script_call(Entity_get_update_callback_ref(node->entity));
		pos = Entity_get_pos(node->entity);
		vel = Entity_get_vel(node->entity);
		col = Game_collision_check(node->entity);
		if (col.x)
			vel.x = 0;
		if (col.y)
			vel.y = 0;
		Entity_set_pos(node->entity, pos.x + vel.x, pos.y + vel.y);
	}
	return 1;
}

/**
 * \param focus The portion of the map to focus on.
 * \return A rectangular portion of the tilemap centered around the focus.
 */
SDL_Rect Game_get_camera(SDL_Rect focus) {
	SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
	SDL_Rect area = Tilemap_get_area();

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
int Game_draw(SDL_Surface *screen) {
	SDL_Rect center = Entity_get_pos(world.player);
	SDL_Rect camera = Game_get_camera(center);
	EntityNode *node;

	if (Tilemap_draw_background(screen, camera))
		return -1;
	/* This loop simply draws entities in the order they appear in the list.
	 * This is not ideal, as it could cause entities high on the screen to be
	 * draw over entities low on the screen.  This shall be solved by making
	 * sure the list is sorted by entity position, from top to bottom. */
	for (node = world.entities->first; node != NULL; node = node->next)
		if (Entity_draw(node->entity, screen, camera))
			return -1;
	if (Tilemap_draw_foreground(screen, camera))
		return -1;
	return 0;
}
