#ifndef INPUT_H
#define INPUT_H

#define key_down 1
#define key_up   2

struct Key{
	char* Name;
	int (*callback)(int Action, void* Pref);
	int Pushed;
};
typedef struct Key Key;

typedef struct {
	Key up;
	Key down;
	Key left;
	Key right;
} Input;

void ProcessInput(SDL_Event * event);
void toggle(int which, Key* key);


int move_up(int which, void* Pref);
int move_right(int which, void* Pref);
int move_left(int which, void* Pref);
int move_down(int which, void* Pref);

#endif
