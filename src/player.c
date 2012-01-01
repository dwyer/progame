#include "player.h"
#include "ai.h"

Player *createPlayer(int x, int y)
{
	Player *player = malloc(sizeof(Player));
	player->pos.x = x;
	player->pos.y = y;
	player->pos.w = 16;
	player->pos.h = 16;
	player->rel_pos = player->pos;
	player->sprite =
		SDL_CreateRGBSurface(SDL_HWSURFACE, 16, 16, 32, 0, 0, 0, 0);
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
	
	if (Walkable(x, y, Direction)){
		player->pos.x += x;
		player->pos.y += y;
	}
}

int drawPlayer(Player * player, SDL_Surface * surface, SDL_Rect camera)
{	
	player->rel_pos.x = player->pos.x - camera.x;
	player->rel_pos.y = player->pos.y - camera.y;
	
	return SDL_BlitSurface(player->sprite, NULL, surface,
						   &player->rel_pos);
}

void freePlayer(Player * player)
{
	free(player);
}
