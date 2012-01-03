#include <stdio.h>
#include <stdbool.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "SDL/SDL.h"
#include "main.h"
#include "world.h"
#include "input.h"

/* Number of milliseconds between logic updates. */
#define UPDATE_INTERVAL 10

Uint32 pushUpdateEvent(Uint32 interval, void *param);
int playGame(SDL_Surface *screen);
bool handleEvents(World *world, Input *input);

/**
 * Initialize everything, run the game, deinitialize, quit.
 */
int main(int argc, char *argv[]) {
	SDL_Surface *screen = NULL;
	SDL_TimerID timer_id;
	lua_State *lua_state;

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
	
	if ((lua_state = luaL_newstate()) != NULL) {
		fprintf(stderr, "Error create Lua state.\n");
		SDL_Quit();
		return -1;
	}
	luaL_openlibs(lua_state);
	/* Play the fucking game. */
	playGame(screen);
	/* Deinitialization */
	lua_close(lua_state);
	SDL_FreeSurface(screen);
	SDL_RemoveTimer(timer_id);
	SDL_Quit();
	return 0;
}

/** 
 * Pushes a user event to the event queue which will indicate that it's time to
 * update world.
 */
Uint32 pushUpdateEvent(Uint32 interval, void *param) {
	SDL_Event event;
	SDL_UserEvent user;

	user.type = SDL_USEREVENT;
	user.code = 0;
	user.data1 = NULL;
	user.data2 = NULL;
	event.type = SDL_USEREVENT;
	event.user = user;
	SDL_PushEvent(&event);
	return interval;
}

/**
 * Play the game. Returns 0 on success, -1 on error.
 */
int playGame(SDL_Surface *screen) {
	const char filename[] = "res/untitled.tmx.bin";
	World *world = NULL;
	Input input = { 0, 0, 0, 0 };

	if ((world = createWorld(filename)) == NULL) {
		return -1;
	}
	do {
		/* Draw. */
		if (drawWorld(world, screen) || SDL_Flip(screen)) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
	} while (handleEvents(world, &input));
	freeWorld(world);
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
				input->left = 1;
			} else if (event.key.keysym.sym == SDLK_RIGHT) {
				input->right = 1;
			} else if (event.key.keysym.sym == SDLK_UP) {
				input->up = 1;
			} else if (event.key.keysym.sym == SDLK_DOWN) {
				input->down = 1;
			} else if (event.key.keysym.sym == SDLK_q) {
				return false;
			}
		} else if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				input->left = 0;
			} else if (event.key.keysym.sym == SDLK_RIGHT) {
				input->right = 0;
			} else if (event.key.keysym.sym == SDLK_UP) {
				input->up = 0;
			} else if (event.key.keysym.sym == SDLK_DOWN) {
				input->down = 0;
			}
		} else if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
