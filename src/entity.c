#include <assert.h>

#include <SDL/SDL.h>

#include "entity.h"
#include "main.h"

typedef struct FrameSet FrameSet;

struct FrameSet {
	SDL_Rect **frames;
	int length;
};

struct Entity {
	/** Position of the entity relative to the current world map, NOT the
	 * screen. That will be computed when we draw it. */
	SDL_Rect pos;

	/** Velocity of the entity. */
	SDL_Rect vel;

	SDL_Surface *sprite;
	SDL_Rect *frames;
	FrameSet framesets[NUM_ACTIONS][NUM_DIRECTIONS];
	unsigned int frame;
	unsigned int action;
	unsigned int direction;
};

/**
 * Create an entity.
 * \return A new Entity.
 */
Entity *Entity_new() {
	Entity *entity = NULL;

	entity = malloc(sizeof(*entity));
	entity->pos.x = 0;
	entity->pos.y = 0;
	entity->pos.w = 16;
	entity->pos.h = 18;
	return entity;
}

/**
 * Free an entity from memory.
 */
void Entity_free(Entity * entity) {
	int i, j;

	SDL_FreeSurface(entity->sprite);
	for (i = 0; i < NUM_ACTIONS; i++)
		for (j = 0; j < NUM_DIRECTIONS; j++)
			free(entity->framesets[i][j].frames);
	free(entity->frames);
	free(entity);
}

void Entity_set_sprite(Entity *entity, const char *filename) {
	int i, j, w, h;

	assert(entity->sprite = SDL_LoadBMP(filename));
	SDL_SetColorKey(entity->sprite, SDL_SRCCOLORKEY, 0xff00ff);
	w = entity->sprite->w / entity->pos.w;
	h = entity->sprite->h / entity->pos.h;
	entity->frames = calloc(w * h, sizeof(*entity->frames));
	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++) {
			entity->frames[i + j * w].x = i * entity->pos.w;
			entity->frames[i + j * w].y = j * entity->pos.h;
			entity->frames[i + j * w].w = entity->pos.w;
			entity->frames[i + j * w].h = entity->pos.h;
		}
	entity->frame = 0;
	entity->action = ACTION_STANDING;
	entity->direction = DIRECTION_DOWN;
	for (i = 0; i < NUM_ACTIONS; i++)
		for (j = 0; j < NUM_DIRECTIONS; j++) {
			entity->framesets[i][j].frames = NULL;
			entity->framesets[i][j].length = 0;
		}
}

void Entity_add_frame(Entity * entity, int action, int direction, int frame) {
	FrameSet *frameset = &entity->framesets[action][direction];

	frameset->frames = realloc(frameset->frames,
							   (frameset->length + 1) * sizeof(*frameset->frames));
	frameset->frames[frameset->length] = &entity->frames[frame];
	frameset->length++;
}

/**
 * Set the size of the entity.
 */
void Entity_set_size(Entity * entity, int w, int h) {
	entity->pos.w = w;
	entity->pos.h = h;
}

/**
 * Set the position of the given Entity to the given x and y.
 */
void Entity_set_pos(Entity * entity, int x, int y) {
	entity->pos.x = x;
	entity->pos.y = y;
}

/**
 * Set the given entity's velocity and adjust their animation frame
 * accordingly.
 */
void Entity_set_vel(Entity * entity, int x, int y) {
	/* set action */
	if (x || y)
		entity->action = ACTION_WALKING;
	else
		entity->action = ACTION_STANDING;
	/* set direction */
	if (x < 0)
		entity->direction = DIRECTION_LEFT;
	else if (x > 0)
		entity->direction = DIRECTION_RIGHT;
	if (y < 0)
		entity->direction = DIRECTION_UP;
	else if (y > 0)
		entity->direction = DIRECTION_DOWN;
	/* set frame */
	if (x == entity->vel.x && y == entity->vel.y)
		entity->frame++;
	else
		entity->frame = 0;
	/* set velocity */
	entity->vel.x = x;
	entity->vel.y = y;
}

SDL_Rect Entity_get_pos(const Entity * entity) {
	return entity->pos;
}

SDL_Rect Entity_get_vel(const Entity * entity) {
	return entity->vel;
}

/**
 * Draw the entity if and only if the entity is within the bounds of the
 * camera.
 * \return 0 on success, non-zero on error.
 */
int Entity_draw(Entity * entity, SDL_Surface * screen, SDL_Rect camera) {
	FrameSet frameset = entity->framesets[entity->action][entity->direction];
	SDL_Rect src = *frameset.frames[entity->frame % frameset.length];
	SDL_Rect dst = entity->pos;

	screen = SDL_GetVideoSurface();
	dst.x -= camera.x;
	dst.y -= camera.y;
	if ((dst.x + dst.w >= 0 && dst.x < screen->w) &&
		(dst.y + dst.h >= 0 && dst.y < screen->h))
		return SDL_BlitSurface(entity->sprite, &src, screen, &dst);
	else
		return 0;
}
