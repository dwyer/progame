#include <SDL/SDL.h>
#include "entity.h"

int drawEntities(EntityList * List, SDL_Surface * Surface, int camx,
				 int camy) {
	EntityList *Current = List;

	while (Current) {
		Current->Subject->rel_Dest.x = Current->Subject->Dest.x - camx;
		Current->Subject->rel_Dest.y = Current->Subject->Dest.y - camy;

		if (Current->Subject) {
			SDL_BlitSurface(Current->Subject->Surf,
							&Current->Subject->Source, Surface,
							&Current->Subject->rel_Dest);
		}

		Current = Current->Next;
	}

	return 0;
}
