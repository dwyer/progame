#include "player.h"

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
{
	player->pos.x += x;
	player->pos.y += y;
}

int drawPlayer(Player * player, SDL_Surface * surface)
{
	return SDL_BlitSurface(player->sprite, NULL, surface,
						   &player->rel_pos);
}

void freePlayer(Player * player)
{
	free(player);
}
