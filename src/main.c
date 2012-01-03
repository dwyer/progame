#include <stdio.h>
#include <stdbool.h>
#include "SDL/SDL.h"
#include "main.h"
#include "world.h"
#include "input.h"

/* Number of milliseconds between logic updates. */
#define UPDATE_INTERVAL 10

/* 
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

/*
 * So far all we're doing here is loading a tilemap and allowing the ``player''
 * (a 16x18 dragon guy) to run around.
 * TODO:
 * * Player moves (swing swords, shoot arrows, kick, punch, whatever).
 */
int playGame(SDL_Surface *screen) {
	const char filename[] = "res/untitled.tmx.bin";
	SDL_Event event;
	World *world = NULL;
	Input input = { 0, 0, 0, 0 };
	bool play = true;

	if ((world = createWorld(filename)) == NULL) {
		return -1;
	}

	do {
		/* Events */
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_USEREVENT) {
				updateWorld(world, input);
			} else if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					input.left = 1;
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					input.right = 1;
				} else if (event.key.keysym.sym == SDLK_UP) {
					input.up = 1;
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					input.down = 1;
				} else if (event.key.keysym.sym == SDLK_q) {
					play = false;
				}
			} else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					input.left = 0;
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					input.right = 0;
				} else if (event.key.keysym.sym == SDLK_UP) {
					input.up = 0;
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					input.down = 0;
				}
			} else if (event.type == SDL_QUIT) {
				play = false;
			}
		}

		/* Draw. */
		if (drawWorld(world, screen)) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}

		/* Update screen. */
		if (SDL_Flip(screen) == -1) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
	} while (play);
	freeWorld(world);
	return 0;
}

int main(int argc, char *argv[]) {
	SDL_Surface *screen = NULL;
	SDL_TimerID timer_id;

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
	playGame(screen);
	/* Deinitialization */
	SDL_FreeSurface(screen);
	SDL_RemoveTimer(timer_id);
	SDL_Quit();
	return 0;
}
