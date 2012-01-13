#ifndef EntityH
#define EntityH

typedef struct Entity Entity;

Entity *Entity_new();
void Entity_free(Entity *entity);

SDL_Rect Entity_get_pos(const Entity *entity);
SDL_Rect Entity_get_vel(const Entity *entity);

void Entity_set_pos(Entity *entity, int x, int y);
void Entity_set_vel(Entity *entity, int x, int y);
void Entity_set_size(Entity *entity, int w, int h);

int Entity_draw(Entity *entity, SDL_Surface *screen, SDL_Rect camera);

#endif
