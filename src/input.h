#ifndef INPUT_H
#define INPUT_H

#define INPUT_CODE_NUM 100

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

extern Input input;
extern InputCode input_codes[INPUT_CODE_NUM];

#endif
