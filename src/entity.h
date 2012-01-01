#ifndef EntityH
#define EntityH

typedef struct{
	int Type;
	SDL_Rect Source;
	SDL_Rect Dest;
	SDL_Rect rel_Dest;
	SDL_Surface* Surf;
} Entity;

struct EntityList{
	Entity* Subject;
	struct EntityList* Next;
};
typedef struct EntityList EntityList;

int drawEntities(EntityList* List, SDL_Surface* Surface, int camx, int camy);

#endif
