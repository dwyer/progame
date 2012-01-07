#include <stdio.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "main.h"
#include "world.h"
#include "input.h"
#include "config.h"
#include "script.h"
#include "event.h"

/* Number of milliseconds between logic updates. */
#define UPDATE_INTERVAL 10

typedef struct InputCode InputCode;

/**
 * Here's an array that maps keyboard events to action events. Eventually this
 * will be loaded from a Lua file instead of hardcoded.
 */
struct InputCode {
	int sym;
	int code;
} input_codes[] = {
	{ SDLK_UP, EVENT_MOVEUP },
	{ SDLK_DOWN, EVENT_MOVEDOWN },
	{ SDLK_LEFT, EVENT_MOVELEFT },
	{ SDLK_RIGHT, EVENT_MOVERIGHT },
	{ SDLK_q, EVENT_QUIT },
	{ -1, -1 }
};

Uint32 pushUpdateEvent(Uint32 interval, void *param);
int playGame(SDL_Surface *screen);
bool handleEvents(World *world, Input *input);

/**
 * Initialize everything, run the game, deinitialize, quit.
 */
int main(int argc, char *argv[]) {
	int result = 0;
	SDL_Surface *screen = NULL;
	SDL_TimerID  timer_id;

	/* Initialization. */
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	if ((timer_id =
		SDL_AddTimer(UPDATE_INTERVAL, pushUpdateEvent, NULL)) == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	if ((screen =
		SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP,
						 SDL_HWSURFACE)) == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	SDL_WM_SetCaption("/prog/ame", NULL);
	
	/* Play the fucking game. */
	result = playGame(screen);
	
	/* Deinitialization */
	SDL_FreeSurface(screen);
	SDL_RemoveTimer(timer_id);
	SDL_Quit();
	return result;
}

/** 
 * Pushes a user event to the event queue which will indicate that it's time to
 * update world.
 */
Uint32 pushUpdateEvent(Uint32 interval, void *param) {
	pushUserEvent(EVENT_UPDATE, NULL, NULL);
	return interval;
}

/**
 * Play the game. Returns 0 on success, -1 on error.
 */
int playGame(SDL_Surface *screen) {
	const char filename[] = "res/maps/untitled.tmx.bin";
	lua_State *L = NULL;
	World *world = NULL;
	Input input = { 0, 0, 0, 0 };
	settings* pref = malloc(sizeof(settings));
	memset(pref, 0, sizeof(settings));
	LoadConfig(pref);
	
	if ((L = luaL_newstate()) == NULL) {
		fprintf(stderr, "Error creating Lua state.\n");
		return -1;
	}
	registerLuaRegs(L);
	luaL_dofile(L, "res/scripts/init.lua");
	if ((world = createWorld(filename)) == NULL) {
		return -1;
	}
	do {
		/* Draw. */
		SDL_FillRect(screen, NULL, 0);
		if (drawWorld(world, screen) || SDL_Flip(screen)) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
	} while (handleEvents(world, &pref->input));
	freeWorld(world);
	lua_close(L);
	return 0;
}

/**
 * Polls for events. Manages the input queue and handles when input must be
 * passed to the World class. Returns false it the player signaled a quit event,
 * true otherwise.
 */
bool handleEvents(World *world, Input *input) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_USEREVENT) {
			if (event.user.code == EVENT_UPDATE)
				updateWorld(world, *input);
			else if (event.user.code == EVENT_QUIT)
				return false;
			else if (event.user.code == EVENT_MOVEUP)
				input->up.callback(event.user.data1 != NULL, input);
			else if (event.user.code == EVENT_MOVEDOWN)
				input->down.callback(event.user.data1 != NULL, input);
			else if (event.user.code == EVENT_MOVELEFT)
				input->left.callback(event.user.data1 != NULL, input);
			else if (event.user.code == EVENT_MOVERIGHT)
				input->right.callback(event.user.data1 != NULL, input);
		} else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			/**
			 * Instead of hardcoding keyboard events, we'll map them to action
			 * events so they can be configured in scripting.
			 */
			InputCode *code = NULL;
			for (code = input_codes; code->sym != -1; code++) {
				if (code->sym == event.key.keysym.sym) {
					pushUserEvent(code->code, (void *)(event.type == SDL_KEYDOWN), NULL);
				}
			}
		} else if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
