#include "player.h"

Player *createPlayer(int x, int y)
{
	Player *player = NULL;

	player = malloc(sizeof(Player));
	player->pos.x = x;
	player->pos.y = y;
	player->pos.w = 16;			/* h and w in a rect used as dest are ignores. wut */
	player->pos.h = 16;
	player->rel_pos = player->pos;

	player->src.x = 0;
	player->src.y = 0;

	player->src.h = 16;
	player->src.w = 16;

	player->State.State = p_idle;
	player->State.StateTime = 0;

	if (!(player->sprite = SDL_LoadBMP("res/player1.bmp"))) {
		fputs(SDL_GetError(), stderr);
		free(player);
		return NULL;
	}
	SDL_SetColorKey(player->sprite, SDL_SRCCOLORKEY,
					SDL_MapRGB(player->sprite->format, 255, 0, 255));
	return player;
}

void movePlayer(Player * player, int x, int y)
{
	if (player->State.State != p_walk) {
		player->State.State = p_walk;
		player->State.StateTime = SDL_GetTicks();
		player->State.TimeSwitch = 50;
		player->src.y = 0;
		player->src.x = 16;
	} else {
		if (player->State.StateTime + player->State.TimeSwitch <=
			SDL_GetTicks()) {
			if (player->src.y == 16)
				player->src.y = 0;
			else
				player->src.y = 16;

			player->State.StateTime = SDL_GetTicks();
		}
	}

	int Direction = 0;

	if (y != 0) {
		if (x != 0) {
			if (x > 0 && y < 0)
				Direction = D_noreast;

			else if (x > 0 && y > 0)
				Direction = D_soueast;

			else if (x < 0 && y < 0)
				Direction = D_norwest;

			else if (x < 0 && y > 0)
				Direction = D_souwest;
		} else {
			if (y < 0)
				Direction = D_north;
			else
				Direction = D_south;
		}
	} else {
		if (x > 0)
			Direction = D_east;
		else
			Direction = D_west;
	}

	if (1) {
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
