#ifndef EVENT_H
#define EVENT_H

/**
 * Event codes for SDL_UserEvent
 */
enum {
	EVENT_UPDATE
};

int pushUserEvent(int code, void *data1, void *data2);

#endif
