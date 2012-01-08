#ifndef EntityH
#define EntityH

typedef struct Entity Entity;
typedef struct EntityNode EntityNode;
typedef struct EntityList EntityList;

struct Entity {
	SDL_Rect pos;
	SDL_Surface *sprite;
};

int Entities_Draw(EntityList * list, SDL_Surface * surface, int camx,
				  int camy);

#endif
