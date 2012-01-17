#ifndef PLAYER_H
#define PLAYER_H

#include <SDL/SDL.h>

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

typedef struct Player Player;

Player *Player_new(void);
void Player_free(Player * player);

SDL_Rect Player_get_pos(const Player * player);
SDL_Rect Player_get_vel(const Player * player);
int Player_get_speed(const Player * player);

void Player_set_pos(Player * player, int x, int y);
void Player_set_vel(Player * player, int x, int y);
void Player_move(Player * player, int x, int y);

int Player_draw(Player * player, SDL_Surface * surface, SDL_Rect camera);
int Player_update_state(Player * player, int State);

#endif
