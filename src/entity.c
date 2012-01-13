#include <SDL/SDL.h>
#include "entity.h"

struct Entity {
    /**
     * Position of the entity relative to the current world map, NOT the screen.
     * That will be computed when we draw it.
     */
    SDL_Rect pos;

    /**
     * Velocity of the entity.
     */
    SDL_Rect vel;
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
    entity->pos.h = 16;
    return entity;
}

/**
 * Free's an entity from memory.
 */
void Entity_free(Entity *entity) {
    free(entity);
}

SDL_Rect Entity_get_pos(const Entity *entity) {
    return entity->pos;
}

SDL_Rect Entity_get_vel(const Entity *entity) {
    return entity->vel;
}

/**
 * Set the position of the given Entity to the given x and y.
 */
void Entity_set_pos(Entity *entity, int x, int y) {
    entity->pos.x = x;
    entity->pos.y = y;
}

void Entity_set_vel(Entity *entity, int x, int y) {
    entity->vel.x = x;
    entity->vel.y = y;
}

/**
 * Set the size of the entity.
 */
void Entity_set_size(Entity *entity, int w, int h) {
    entity->pos.w = w;
    entity->pos.h = h;
}

/**
 * Draw the entity if and only if the entity is within the bounds of the
 * camera.
 * \return 0 on success, non-zero on error.
 */
int Entity_draw(Entity *entity, SDL_Surface *screen, SDL_Rect camera) {
    SDL_Rect dst = entity->pos;
    dst.x -= camera.x;
    dst.y -= camera.y;
    if ((dst.x + dst.w >= 0 && dst.x < screen->w) && 
        (dst.y + dst.h >= 0 && dst.y < screen->h))
        return SDL_FillRect(screen, &dst, 0xff0000);
    else
        return 0;
}
