#include <SDL/SDL.h>
#include "entity.h"
#include "main.h"

enum {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	NUM_DIRECTIONS
};

enum {
	ACTION_LOOKING,
	ACTION_WALKING,
	NUM_ACTIONS
};

typedef struct FrameSet FrameSet;

struct FrameSet {
	SDL_Rect *frames;
	int length;
};

void FrameSet_add_frame(FrameSet *frameset, SDL_Rect frame) {
	frameset->frames = realloc(frameset->frames, (frameset->length + 1) * sizeof(*frameset->frames));
	frameset->frames[frameset->length] = frame;
	frameset->length++;
}

struct Entity {
	/** Position of the entity relative to the current world map, NOT the
	 * screen. That will be computed when we draw it. */
	SDL_Rect pos;

	/** Velocity of the entity. */
	SDL_Rect vel;

	SDL_Surface *sprite;
	FrameSet frames[NUM_ACTIONS][NUM_DIRECTIONS];
	unsigned int frameset;
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
	Entity_set_sprite(entity, "res/player1.bmp");
	return entity;
}

int Entity_set_sprite(Entity *entity, const char *filename) {
	SDL_Rect *frames;
	int i, j, w, h;

	if ((entity->sprite = SDL_LoadBMP(filename)) == NULL)
		return -1;
	SDL_SetColorKey(entity->sprite, SDL_SRCCOLORKEY, 0xff00ff);
	w = entity->sprite->w / entity->pos.w;
	h = entity->sprite->h / entity->pos.h;
	frames = calloc(w * h, sizeof(*frames));
	for (i = 0; i < w; i++)
		for (j = 0; j < h; j++) {
			frames[i + j * w].x = i * entity->pos.w;
			frames[i + j * w].y = j * entity->pos.h;
			frames[i + j * w].w = entity->pos.w;
			frames[i + j * w].h = entity->pos.h;
		}
	/* Bind frames to states. Hardcoded for now. TODO: do this in lua. */
	entity->frame = 0;
	entity->action = ACTION_LOOKING;
	entity->direction = DIRECTION_DOWN;
	for (i = 0; i < NUM_ACTIONS; i++)
		for (j = 0; j < NUM_DIRECTIONS; j++) {
			entity->frames[i][j].frames = NULL;
			entity->frames[i][j].length = 0;
		}
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_UP], frames[0]);
	FrameSet_add_frame(&entity->frames[ACTION_LOOKING][DIRECTION_UP], frames[1]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_UP], frames[2]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_RIGHT], frames[3]);
	FrameSet_add_frame(&entity->frames[ACTION_LOOKING][DIRECTION_RIGHT], frames[4]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_RIGHT], frames[5]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_DOWN], frames[6]);
	FrameSet_add_frame(&entity->frames[ACTION_LOOKING][DIRECTION_DOWN], frames[7]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_DOWN], frames[8]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_LEFT], frames[9]);
	FrameSet_add_frame(&entity->frames[ACTION_LOOKING][DIRECTION_LEFT], frames[10]);
	FrameSet_add_frame(&entity->frames[ACTION_WALKING][DIRECTION_LEFT], frames[11]);
	return 0;
}

/**
 * Free's an entity from memory.
 */
void Entity_free(Entity * entity) {
	free(entity);
}

SDL_Rect Entity_get_pos(const Entity * entity) {
	return entity->pos;
}

SDL_Rect Entity_get_vel(const Entity * entity) {
	return entity->vel;
}

/**
 * Set the position of the given Entity to the given x and y.
 */
void Entity_set_pos(Entity * entity, int x, int y) {
	entity->pos.x = x;
	entity->pos.y = y;
}

void Entity_set_vel(Entity * entity, int x, int y) {
	if (x || y)
		entity->action = ACTION_WALKING;
	else
		entity->action = ACTION_LOOKING;
	if (x < 0)
		entity->direction = DIRECTION_LEFT;
	else if (x > 0)
		entity->direction = DIRECTION_RIGHT;
	if (y < 0)
		entity->direction = DIRECTION_UP;
	else if (y > 0)
		entity->direction = DIRECTION_DOWN;
	if (x == entity->vel.x && y == entity->vel.y)
		entity->frame++;
	else
		entity->frame = 0;
	entity->vel.x = x;
	entity->vel.y = y;
}

/**
 * Set the size of the entity.
 */
void Entity_set_size(Entity * entity, int w, int h) {
	entity->pos.w = w;
	entity->pos.h = h;
}

/**
 * Draw the entity if and only if the entity is within the bounds of the
 * camera.
 * \return 0 on success, non-zero on error.
 */
int Entity_draw(Entity * entity, SDL_Surface * screen, SDL_Rect camera) {
	FrameSet frameset = entity->frames[entity->action][entity->direction];
	SDL_Rect src = frameset.frames[entity->frame % frameset.length];
	SDL_Rect dst = entity->pos;
	dst.x -= camera.x;
	dst.y -= camera.y;
	if ((dst.x + dst.w >= 0 && dst.x < screen->w) &&
		(dst.y + dst.h >= 0 && dst.y < screen->h))
		return SDL_BlitSurface(entity->sprite, &src, screen, &dst);
	else
		return 0;
}
