#ifndef PLAYER_H
#define PLAYER_H

#include "SDL/SDL.h" /* Polluting the include stream D:*/

#define PLAYER_SPEED 2

typedef struct {
	SDL_Rect pos;				/* Player's position relative to the map. */
	SDL_Rect rel_pos;
	SDL_Surface *sprite;
} Player;

Player *createPlayer(int x, int y);
void    movePlayer(Player * player, int x, int y);
int     drawPlayer(Player * player, SDL_Surface * surface, SDL_Rect camera);
void    freePlayer(Player * player);

#endif
