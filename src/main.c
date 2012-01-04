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

/* Number of milliseconds between logic updates. */
#define UPDATE_INTERVAL 10

/**
 * Event codes for SDL_UserEvent
 */
enum {
	EVENT_UPDATE
};

Uint32 pushUpdateEvent(Uint32 interval, void *param);
int pushUserEvent(int code, void *data1, void *data2);
int playGame(SDL_Surface *screen);
bool handleEvents(World *world, Input *input);

int load_world(lua_State *L) {
	const char *filename = lua_tostring(L, -1);
	printf("%s\n", filename);
	return 1;
}

static luaL_Reg regs[] = {
	{ "load_world", load_world },
	{ NULL, NULL }
};

/**
 * Initialize everything, run the game, deinitialize, quit.
 */
int main(int argc, char *argv[]) {
	SDL_Surface *screen = NULL;
	SDL_TimerID  timer_id;
	lua_State   *lua_state;
	luaL_Reg *reg = regs;

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
	
	if ((lua_state = luaL_newstate()) == NULL) {
		fprintf(stderr, "Error creating Lua state.\n");
		return -1;
	}
	luaL_openlibs(lua_state);
	while (reg->name != NULL) {
		lua_register(lua_state, reg->name, reg->func);
		reg++;
	}
	luaL_dofile(lua_state, "res/scripts/init.lua");
	
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
	pushUserEvent(EVENT_UPDATE, NULL, NULL);
	return interval;
}

/**
 * Push a ``user'' (developer really) defined update to the update queue.
 */
int pushUserEvent(int code, void *data1, void *data2) {
	SDL_Event event;

	event.user.type = event.type = SDL_USEREVENT;
	event.user.code = code;
	event.user.data1 = data1;
	event.user.data2 = data2;
	SDL_PushEvent(&event);
	return 0;
}

/**
 * Play the game. Returns 0 on success, -1 on error.
 */
int playGame(SDL_Surface *screen) {
	const char filename[] = "res/maps/untitled.tmx.bin";
	World *world = NULL;
	Input input = { 0, 0, 0, 0 };
	settings* pref = malloc(sizeof(settings));
	memset(pref, 0, sizeof(settings));
	LoadConfig(pref);

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
