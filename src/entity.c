#include <SDL/SDL.h>
#include "entity.h"

struct EntityNode {
	Entity *entity;
	struct EntityNode *last, *next;
};

struct EntityList {
	EntityNode *first;
};

EntityList *EntityList_new(void) {
	EntityList *list = malloc(sizeof(EntityList));

	list->first = NULL;
	return list;
}

void EntityList_free(EntityList * entities) {
	EntityNode *node = NULL, *temp;

	while (entities->first) {
		node = entities->first;
		entities->first = node->next;
		free(node->entity);
		free(node);
	}
	free(entities);
}

void EntityList_append(EntityList * entities, Entity * entity) {
	EntityNode *node = malloc(sizeof(EntityNode));

	node->last = NULL;
	node->next = entities->first;
	entities->first->last = node;
}

int Entities_draw(EntityList * list, SDL_Surface * surface, int camx,
				  int camy) {
	EntityNode *node = NULL;

	for (node = list->first; node != NULL; node = node->next) {
		SDL_BlitSurface(node->entity->sprite, NULL, surface, NULL);
	}
	return 0;
}
