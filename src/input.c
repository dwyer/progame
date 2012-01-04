#include <SDL/SDL.h>
#include "input.h"
#include "config.h"

void ProcessInput(SDL_Event * event) {
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

int move_up(int which, void* Pref){
	toggle(which, &((settings*)Pref)->input.up);
	return 0;
}

int move_down(int which, void* Pref){
	toggle(which, &((settings*)Pref)->input.down);
	return 0;
}

int move_left(int which, void* Pref){
	toggle(which, &((settings*)Pref)->input.left);
	return 0;
}

int move_right(int which, void* Pref){
	toggle(which, &((settings*)Pref)->input.right);
	return 0;
}

void toggle(int which, Key* key){
	if (which == key_up)
	key->Pushed = 0;
	else if (which == key_down)
	key->Pushed = 1;
}
