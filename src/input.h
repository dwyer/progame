#ifndef INPUT_H
#define INPUT_H

#define key_down 1
#define key_up   2

#include <stdbool.h>

typedef struct {
	int sym;
	int code;
} InputCode;

typedef struct {
	bool up;
	bool down;
	bool left;
	bool right;
} Input;

#endif
