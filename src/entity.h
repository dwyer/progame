#ifndef ENTITY_H
#define ENTITY_H

enum {
	ACTION_STANDING,
	ACTION_WALKING,
	NUM_ACTIONS
};

enum {
	DIRECTION_UP,
	DIRECTION_DOWN,
	DIRECTION_LEFT,
	DIRECTION_RIGHT,
	NUM_DIRECTIONS
};

typedef struct Entity Entity;

Entity *Entity_new();
void Entity_free(Entity * entity);

void Entity_set_sprite(Entity *, const char *);
void Entity_add_frame(Entity * entity, int action, int direction, int frame);

void Entity_set_pos(Entity * entity, int x, int y);
void Entity_set_vel(Entity * entity, int x, int y);
void Entity_set_size(Entity * entity, int w, int h);
void Entity_set_update_callback_ref(Entity *entity, int ref);

SDL_Rect Entity_get_pos(const Entity * entity);
SDL_Rect Entity_get_vel(const Entity * entity);
int Entity_get_update_callback_ref(const Entity *entity);

int Entity_draw(Entity * entity, SDL_Surface * screen, SDL_Rect camera);

#endif
