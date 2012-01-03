#ifndef INPUT_H
#define INPUT_H

typedef struct {
	int up;
	int down;
	int left;
	int right;
} Input;

void ProcessInput(SDL_Event * event);

#endif
