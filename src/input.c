#include <SDL/SDL.h>
#include "input.h"

void ProcessInput(SDL_Event * event)
{
	/* Here we will call the callbacks to the script interpreter */
	switch (event->type) {
	case SDL_KEYDOWN:
		break;
	case SDL_KEYUP:
		break;
	default:
		break;
	}
}
