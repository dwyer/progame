#include <SDL/SDL.h>
#include "event.h"

/**
 * Push a ``user'' (developer really) defined update to the update queue.
 */
int pushUserEvent(int code, void *data1, void *data2) {
	SDL_Event event;

	event.user.type = event.type = SDL_USEREVENT;
	event.user.code = code;
	event.user.data1 = data1;
	event.user.data2 = data2;
	SDL_PushEvent(&event);
	return 0;
}
