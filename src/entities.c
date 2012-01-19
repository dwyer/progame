#include <assert.h>

#include <SDL/SDL.h>

#include "entity.h"
#include "entities.h"

/**
 * Create an empty list.
 */
EntityList *EntityList_new(void) {
	EntityList *list;

	list = malloc(sizeof(*list));
	list->first = NULL;
	return list;
}

/**
 * Free the list, along with all nodes and entities contained therein.
 */
void EntityList_free(EntityList *list) {
	while (list->first)
		EntityList_delete(list, list->first);
	free(list);
}

/**
 * Delete the given node (and entity it contains) from the list.
 */
void EntityList_delete(EntityList *list, EntityNode *node) {
	assert(list && list->first && node);
	if (list->first == node)
		list->first = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	Entity_free(node->this);
	free(node);
}

/**
 * This is prepend, really. Proper append will be implemented once the entity
 * list class is complete.
 */
void EntityList_append(EntityList *list, Entity *entity) {
	EntityNode *node;

	node = malloc(sizeof(*node));
	node->this = entity;
	node->next = NULL;
	node->prev = NULL;
	if (list->first) {
		list->first->prev = node;
		node->next = list->first;
	}
	list->first = node;
}
