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
			updateWorld(world, *input);
		} else if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				if (input->left.callback)
				input->left.callback(key_down, input);
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (input->right.callback)
				input->right.callback(key_down, input);
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				if (input->up.callback)
				input->up.callback(key_down, input);
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				if (input->down.callback)
				input->down.callback(key_down, input);
			}
			else if (event.key.keysym.sym == SDLK_q) {
				return false;
			}
		} else if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				if (input->left.callback)
				input->left.callback(key_up, input);
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				if (input->right.callback)
				input->right.callback(key_up, input);
			}
			else if (event.key.keysym.sym == SDLK_UP) {
				if (input->up.callback)
				input->up.callback(key_up, input);
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				if (input->left.callback)
				input->down.callback(key_up, input);
			}
		}
		else if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
