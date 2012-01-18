#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL/SDL.h>

#include "main.h"
#include "input.h"
#include "script.h"
#include "world.h"
#include "event.h"
#include "config.h"

/* Number of milliseconds between logic updates. */
#define UPDATE_INTERVAL 10

Uint32 push_update_event(Uint32 interval, void *param);
int Game_play(SDL_Surface * screen);
bool Game_events(World * world, Input * input);

/**
 * Initialize everything, run the game, deinitialize, quit.
 */
int main(int argc, char *argv[]) {
	int result = 0;
	SDL_Surface *screen = NULL;
	SDL_TimerID timer_id;

	/* Initialization. */
	srand(time(NULL));
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	if ((timer_id =
		 SDL_AddTimer(UPDATE_INTERVAL, push_update_event, NULL)) == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	if ((screen =
		 SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP,
						  SCREEN_FLAGS)) == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	SDL_WM_SetCaption("/prog/ame", NULL);

	/* Play the fucking game. */
	result = Game_play(screen);

	/* Deinitialization */
	SDL_FreeSurface(screen);
	SDL_RemoveTimer(timer_id);
	SDL_Quit();
	return result;
}

/** 
 * Pushes a user event to the event queue which will indicate that it's time to
 * update world.
 * \param interval Milliseconds between updates.
 * \param param Ignored.
 * \return interval
 */
Uint32 push_update_event(Uint32 interval, void *param) {
	Event_push(EVENT_WORLD_UPDATE, NULL, NULL);
	return interval;
}

/**
 * Play the game.
 * \param screen The screen.
 * \return 0 on success, -1 on error.
 */
int Game_play(SDL_Surface * screen) {
	const char *config_file = "res/scripts/config.lua";
	const char *init_file = "res/scripts/init.lua";
	static InputCode input_codes[100] = { {-1, -1} };
	World *world = NULL;
	Script *script;
	Input input = { 0, 0, 0, 0 };

	Config_run(config_file);
	input.codes = input_codes;
	if ((script = Script_init()) == NULL)
		return -1;
	if ((world = World_new()) == NULL)
		return -1;
	if (Script_run(script, init_file))
		return -1;
	World_set_script(world, script);
	while (Game_events(world, &input)) {
		/* Draw. */
		SDL_FillRect(screen, NULL, 0);
		if (World_draw(world, screen) || SDL_Flip(screen)) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
	};
	Script_free(script);
	World_free(world);
	return 0;
}

/**
 * Polls for events. Manages the input queue and handles when input must be
 * passed to the World class. Returns false it the player signaled a quit event,
 * true otherwise.
 * \return false if a quit event was triggered, true otherwise.
 */
bool Game_events(World * world, Input * input) {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_USEREVENT) {
			World_event(world, input, event.user);
		} else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
			/* Instead of hardcoding keyboard events, we'll map them to action
			 * events so they can be configured in scripting. */
			InputCode *code = NULL;
			for (code = input->codes; code->sym != -1; code++) {
				if (code->sym == event.key.keysym.sym) {
					if (code->code == EVENT_INPUT_QUIT)
						return false;
					else
						Event_push(code->code,
								   (void *) (event.type == SDL_KEYDOWN),
								   NULL);
				}
			}
		} else if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
