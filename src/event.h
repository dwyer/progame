#ifndef EVENT_H
#define EVENT_H

/**
 * Event codes for SDL_UserEvent
 */
enum {
	EVENT_CONFIG_BINDKEY,
	EVENT_INPUT_MOVE_UP,
	EVENT_INPUT_MOVE_DOWN,
	EVENT_INPUT_MOVE_LEFT,
	EVENT_INPUT_MOVE_RIGHT,
	EVENT_INPUT_QUIT,
	EVENT_WORLD_UPDATE,
	EVENT_LAST					/* end of the line */
};

int Event_push(int code, void *data1, void *data2);

#endif
