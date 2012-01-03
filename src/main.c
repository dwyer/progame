#include <stdio.h>
#include "SDL/SDL.h"
#include "main.h"
#include "world.h"

#define SPEEDPPS 0.2

#define FPS_NO 20
#define FRAMETIME 16 /*ms*/

int GetFPS();
float Interpolate(float Speed, int FPS);

/*
 * So far all we're doing here is loading a tilemap and allowing the ``player''
 * (a 16x16 black square) to run around.
 * TODO:
 * * Player sprites with animations.
 * * Player moves (swing swords, shoot arrows, kick, punch, whatever).
 */
int main(int argc, char *argv[])
{
	const char filename[] = "res/untitled.tmx.bin";
	SDL_Surface *screen = NULL;
	World *world = NULL;
	int CurrentFPS = 10, AverageFPS = 10;
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
	while (updateWorld(world, CurrentFPS)) {
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
		GetFPS(&CurrentFPS, &AverageFPS, StartTime);

		CurrentTime = SDL_GetTicks();
		if (CurrentTime-StartTime < FRAMETIME) SDL_Delay(FRAMETIME-(CurrentTime-StartTime));
	}
	freeWorld(world);
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}

int GetFPS(int *CurrentFPS, int *AverageFPS, int StartTime)
{
	*CurrentFPS = SDL_GetTicks() - StartTime;

	*AverageFPS += *CurrentFPS;
	if (*AverageFPS != *CurrentFPS)
		*AverageFPS /= 2;

	return 0;
}

/* 
 * Floating point arithmetic may be CPU intensive
 * but this yields great benifits, and scales better than 
 * any fixed speed alternative.
 */
float Interpolate(float Speed, int FPS)
{
	return (Speed / 2) * FPS;
}
