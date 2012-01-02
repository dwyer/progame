#include "player.h"

Player *createPlayer(int x, int y)
{
	Player *player = malloc(sizeof(Player));
	player->pos.x = x;
	player->pos.y = y;
	player->pos.w = 16; /* h and w in a rect used as dest are ignores. wut */
	player->pos.h = 16;
	player->rel_pos = player->pos;
	player->sprite =
		SDL_CreateRGBSurface(SDL_HWSURFACE, 16, 16, 32, 0, 0, 0, 0);
		
	player->src.x = 0;
	player->src.y = 0;
	
	player->src.h = 16;
	player->src.w = 16;
	
	return player;
}

void movePlayer(Player * player, int x, int y)
{	int Direction = 0;
	
	if (y != 0){
		if (x != 0){
			if (x > 0 && y < 0)
			Direction = D_noreast;
			
			else if (x > 0 && y > 0)
			Direction = D_soueast;
			
			else if (x < 0 && y < 0)
			Direction = D_norwest;
			
			else if (x < 0 && y > 0)
			Direction = D_souwest;
		}
		else {
			if (y < 0)
			Direction = D_north;
			else
			Direction = D_south;
		}
	}
	else {
		if (x > 0)
		Direction = D_east;
		else
		Direction = D_west;
	}
	
	if (1){
		player->pos.x += x;
		player->pos.y += y;
	}
}

int drawPlayer(Player * player, SDL_Surface * surface, SDL_Rect camera)
{	
	player->rel_pos.x = player->pos.x - camera.x;
	player->rel_pos.y = player->pos.y - camera.y;
	
	return SDL_BlitSurface(player->sprite, &player->src, surface,
						   &player->rel_pos);
}

void freePlayer(Player * player)
{
	SDL_FreeSurface(player->sprite);
	free(player);
}
