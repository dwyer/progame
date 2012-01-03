#include <stdio.h>
#include "SDL/SDL.h"
#include "main.h"
#include "world.h"
#include "input.h"

#define SPEEDPPS 0.2

#define FPS_NO 20
#define FRAMETIME 16 /*ms*/

Uint32 inc_timer(Uint32 interval, void *param) {
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
int main(int argc, char *argv[])
{
	const char   filename[] = "res/untitled.tmx.bin";
	SDL_Surface* screen = NULL;
	SDL_TimerID	 timer_id;
	World*       world = NULL;
	Input input = { 0, 0, 0, 0 };

	SDL_Event    event;
	
	int          CurrentDelay = 10,
				 AverageDelay = 10;
	unsigned int StartTime = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	if ((timer_id = SDL_AddTimer(1, inc_timer, NULL)) == NULL) {
		fputs(SDL_GetError(), stderr);
		return -1;
	}
	if ((screen =
		 SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP,
						  SDL_HWSURFACE)) == NULL) {
		fprintf(stderr, "%s\n", SDL_GetError());
		return -1;
	}
	if ((world = createWorld(filename)) == NULL) {
		return -1;
	}
	/* So far the only entity is the player. Later this will be replaced by a
	 * linked-list of all entities (the player, npcs, enemies, items, etc.) */
	while (updateWorld(world, input, CurrentDelay)) {
		StartTime = SDL_GetTicks();

		/* Events */

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					input.left = 1;
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					input.right = 1;
				} else if (event.key.keysym.sym == SDLK_UP) {
					input.up = 1;
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					input.down = 1;
				} else if (event.key.keysym.sym == SDLK_q) {
					return 0;
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
				return 0;
			}
		}

		/* Draw. */
		if (drawWorld(world, screen)) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
		
		/*Delay to prevent CPU chewing*/
		if (CurrentDelay-StartTime < FRAMETIME)
		SDL_Delay(FRAMETIME - (CurrentDelay-StartTime));
		
		/* Update screen. */
		if (SDL_Flip(screen) == -1) {
			fprintf(stderr, "%s\n", SDL_GetError());
			return -1;
		}
				
		GetDelay(&CurrentDelay, &AverageDelay, StartTime);
	}
	freeWorld(world);
	SDL_FreeSurface(screen);
	SDL_RemoveTimer(timer_id);
	SDL_Quit();
	return 0;
}

void GetDelay(int *CurrentDelay, int *AverageDelay, int StartTime)
{
	*CurrentDelay = (SDL_GetTicks() - StartTime);

	*AverageDelay += *CurrentDelay;
	if (*AverageDelay != *CurrentDelay)
		*AverageDelay /= 2;
}

/* 
 * Floating point arithmetic may be CPU intensive
 * but this yields great benifits, and scales better than 
 * any fixed speed alternative.
 */
float Interpolate(float Speed, float Time){
	return (Speed * Time);
}
