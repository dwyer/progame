#ifndef EVENT_H
#define EVENT_H

/**
 * Event codes for SDL_UserEvent
 */
enum {
	EVENT_BINDKEY,
	EVENT_UPDATE,
	EVENT_MOVEUP,
	EVENT_MOVEDOWN,
	EVENT_MOVELEFT,
	EVENT_MOVERIGHT,
	EVENT_QUIT
};

int Event_push(int code, void *data1, void *data2);

#endif
