#include <stdio.h>
#include "SDL/SDL.h"
#include "main.h"
#include "world.h"

#define SPEEDPPS 0.2

#define FPS_NO 20

int GetFPS();
float Interpolate(float Speed, int FPS);
int CurrentFPS = 10, AverageFPS = 10, StartTime = 0;

/*
 * So far all we're doing here is loading a tilemap and allowing the ``player''
 * (a 16x16 black square) to run around.
 * TODO:
 * * Collision detection.
 * * Player sprites with animations.
 * * Player moves (swing swords, shoot arrows, kick, punch, whatever).
 */
int main(int argc, char *argv[])
{
	const char filename[] = "res/untitled.tmx.bin";
	SDL_Surface *screen = NULL;
	World *world = NULL;
	int loop;
	
	LoadConfig(NULL);
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
	loop = 1;
	while (updateWorld(world)) {
		StartTime = SDL_GetTicks();
		/* Draw. */
		for (i = 0; i < tilemap->depth; i++) {
			if (i == tilemap->depth - 1) {
				if (drawPlayer(player, screen, camera)) {
					fputs(SDL_GetError(), stderr);
					return -1;
				}
				
			}
			if (SDL_BlitSurface(tilemap->layers[i], &camera, screen, NULL)){
				fputs(SDL_GetError(), stderr);
				return -1;
			}
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
	}
	
	freePlayer(player);
	TMP_FreeTilemap(tilemap);
	freeWorld(world);
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}

int GetFPS(int* CurrentFPS, int* AverageFPS, int StartTime)
{
	if (!CurrentFPS || !AverageFPS)
	return -1;
	
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
	return Speed * FPS;
}
