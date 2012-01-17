#include "player.h"

typedef struct {
	int State;
	int StateTime;
	int TimeSwitch;
} PState;

struct Player {
	PState State;
	SDL_Rect src;
	SDL_Rect pos;				/* Player's position relative to the map. */
	SDL_Rect vel;				/* Player's velocity */
	SDL_Surface *sprite;
	Uint32 speed;
};

Player *Player_create(void) {
	Player *player = NULL;

	player = malloc(sizeof(Player));
	player->pos.x = 0;
	player->pos.y = 0;
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
	SDL_SetColorKey(player->sprite, SDL_SRCCOLORKEY, 0xff00ff);
	return player;
}

void Player_free(Player * player) {
	SDL_FreeSurface(player->sprite);
	free(player);
}

void Player_set_pos(Player * player, int x, int y) {
	player->pos.x = x;
	player->pos.y = y;
}

void Player_set_vel(Player * player, int x, int y) {
	player->vel.x = x;
	player->vel.y = y;
}

SDL_Rect Player_get_pos(const Player * player) {
	return player->pos;
}

SDL_Rect Player_get_vel(const Player * player) {
	return player->vel;
}

int Player_get_speed(const Player * player) {
	return player->speed;
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

int Player_draw(Player * player, SDL_Surface * surface, SDL_Rect camera) {
	SDL_Rect dst;

	dst.x = player->pos.x - camera.x;
	dst.y = player->pos.y - camera.y;
	return SDL_BlitSurface(player->sprite, &player->src, surface, &dst);
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

			if (player->src.x != 32 &&
				player->State.StateTime + player->State.TimeSwitch <=
				SDL_GetTicks()) {
				player->src.x = 32;

				player->State.TimeSwitch = 0;
				player->State.StateTime = 0;
			}
		}
	}
	return 0;
}
