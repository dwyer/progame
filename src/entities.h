#ifndef ENTITIES_H
#define ENTITIES_H

typedef struct EntityList EntityList;
typedef struct EntityNode EntityNode;

EntityList *EntityList_new(void);
void EntityList_free(EntityList *list);
void EntityList_append(EntityList *list, Entity *entity);
void EntityList_delete(EntityList *list, EntityNode *node);

struct EntityList {
	EntityNode *first;
};

struct EntityNode {
	Entity *entity;
	EntityNode *prev;
	EntityNode *next;
};

#endif
