#include <stdio.h>
#include "SDL/SDL.h"
#include "main.h"
#include "world.h"

#define SPEEDPPS 0.2

#define FPS_NO 20
#define FRAMETIME 16 /*ms*/

/*
 * So far all we're doing here is loading a tilemap and allowing the ``player''
 * (a 16x18 dragon guy) to run around.
 * TODO:
 * * Player moves (swing swords, shoot arrows, kick, punch, whatever).
 */
int main(int argc, char *argv[])
{
	const char filename[] = "res/untitled.tmx.bin";
	SDL_Surface *screen = NULL;
	World *world = NULL;
	int CurrentDelay = 10, AverageDelay = 10;
	unsigned int StartTime = 0, CurrentTime = 0;

	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fputs(SDL_GetError(), stderr);
		return -1;
	}
	if ((screen =
		 SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP,
						  SDL_HWSURFACE)) == NULL) {
		fputs(SDL_GetError(), stderr);
		return -1;
	}
	if ((world = createWorld(filename)) == NULL) {
		return -1;
	}
	/* So far the only entity is the player. Later this will be replaced by a
	 * linked-list of all entities (the player, npcs, enemies, items, etc.) */
	while (updateWorld(world, CurrentDelay)) {
		StartTime = SDL_GetTicks();
		/* Draw. */
		if (drawWorld(world, screen)) {
			fputs(SDL_GetError(), stderr);
			return -1;
		}
		/* Update screen. */
		if (SDL_Flip(screen) == -1) {
			fputs(SDL_GetError(), stderr);
			return -1;
		}
		GetDelay(&CurrentDelay, &AverageDelay, StartTime);

		CurrentTime = SDL_GetTicks();
		if (CurrentTime-StartTime < FRAMETIME) SDL_Delay(FRAMETIME-(CurrentTime-StartTime));
		/* Honestly, delaying for frames is not enterprise quality. 
		 * Using the framerate to compensate the animations so they
		 * match the actual speed is much more flexible, and is
		 * ENTERPRISE QUALITY. It allows for soeeds to not fluxuate
		 * despite the framerate, so it looks great, and as the engine
		 * grows, framerates will fluxuate. That is simply inevitable.
		 * I have written a working implementation for fps independant
		 * interpolation, and I think we should use it. */
	}
	freeWorld(world);
	SDL_FreeSurface(screen);
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
