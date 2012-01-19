#ifndef INPUT_H
#define INPUT_H

typedef struct {
	int sym;
	int code;
} InputCode;

typedef struct {
	bool up;
	bool down;
	bool left;
	bool right;
	InputCode *codes;
} Input;

Input input;

#endif
