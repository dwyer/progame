#include "player.h"

Player *Player_create(int x, int y) {
	Player *player = NULL;

	player = malloc(sizeof(Player));
	player->pos.x = x;
	player->pos.y = y;
	player->pos.w = 16;
	player->pos.h = 16;

	player->src.x = 32;
	player->src.y = 18;

	player->src.h = 16;
	player->src.w = 16;

	player->speed = 1;

	player->State.State = p_idle;
	player->State.StateTime = 0;

	if (!(player->sprite = SDL_LoadBMP("res/player1.bmp"))) {
		fprintf(stderr, "%s\n", SDL_GetError());
		free(player);
		return NULL;
	}
	SDL_SetColorKey(player->sprite, SDL_SRCCOLORKEY,
					SDL_MapRGB(player->sprite->format, 255, 0, 255));
	return player;
}

void Player_move(Player * player, int x, int y) {
	int Direction = 0;

	Player_update_state(player, p_walk);

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

	switch (Direction) {
	case D_norwest:
	case D_noreast:
	case D_north:
		player->src.y = 0;
		break;

	case D_east:
	case D_soueast:
		player->src.y = 18;
		break;
	case D_west:
	case D_souwest:
		player->src.y = 54;
		break;
	case D_south:
		player->src.y = 36;
		break;
	}

	player->pos.x += x;
	player->pos.y += y;
}

int drawPlayer(Player * player, SDL_Surface * surface, SDL_Rect camera) {
	SDL_Rect dst;

	dst.x = player->pos.x - camera.x;
	dst.y = player->pos.y - camera.y;
	return SDL_BlitSurface(player->sprite, &player->src, surface, &dst);
}

void Player_free(Player * player) {
	SDL_FreeSurface(player->sprite);
	free(player);
}

int Player_update_state(Player * player, int State) {
	if (!player)
		return -1;

	switch (State) {
	case p_walk:{
			if (player->State.State != p_walk) {
				player->State.State = p_walk;
				player->State.StateTime = SDL_GetTicks();
				player->State.TimeSwitch = 75;
				player->src.x = 32;
			} else {
				if (player->State.StateTime + player->State.TimeSwitch <=
					SDL_GetTicks()) {
					if (player->src.x == 32)
						player->src.x = 0;
					else
						player->src.x = 32;

					player->State.StateTime = SDL_GetTicks();
				}
			}
			break;
		}

	case p_idle:{
			player->State.State = p_idle;
			player->src.x = 32;

			if (player->src.x != 32
				&& player->State.StateTime + player->State.TimeSwitch <=
				SDL_GetTicks()) {
				player->src.x = 32;

				player->State.TimeSwitch = 0;
				player->State.StateTime = 0;
			}
		}
	}

}
