#include <SDL/SDL.h>
#include "input.h"

void ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				break;
			default:
				break;
		}
	}
}
