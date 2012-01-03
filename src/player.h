#ifndef PLAYER_H
#define PLAYER_H

#include "SDL/SDL.h" /* Polluting the include stream D:*/

#define PLAYER_SPEED 2

#define D_north   1
#define D_noreast 2
#define D_east    3
#define D_soueast 4
#define D_south   5
#define D_souwest 6
#define D_west    7
#define D_norwest 8

#define p_idle 1
#define p_walk 2

#define PlayerSpeed 0.0001

typedef struct{
	int State;
	int StateTime;
	int TimeSwitch;
} PState;

typedef struct {
	PState State;
	SDL_Rect src;
	SDL_Rect pos;				/* Player's position relative to the map. */
	SDL_Rect rel_pos;
	SDL_Surface *sprite;
} Player;

Player *createPlayer(int x, int y);
void    movePlayer(Player * player, int x, int y);
int     drawPlayer(Player * player, SDL_Surface * surface, SDL_Rect camera);
void    freePlayer(Player * player);

#endif
