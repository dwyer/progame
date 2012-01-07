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
	key->Pushed = (which == key_down);
}
