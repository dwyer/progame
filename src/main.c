#include <stdio.h>
#include "SDL/SDL.h"
#include "player.h"
#include "tmx.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define SCREEN_BPP 32

#define PLAYER_SPEED 2

#define SPEEDPPS 0.6

typedef struct {
	int up;
	int down;
	int left;
	int right;
} Controller;

#define FPS_NO 20

/* TODO: no globals, ``please'' */
int AverageFPS = 9;
int CurrentFPS = 9;
int StartTime = 0;

int GetFPS();
float MoveCamera();

int game(TMP_Tilemap * tilemap, SDL_Surface * screen)
{
	SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
	SDL_Event event;
	Player *player = createPlayer(0, 0);
	Controller controller = { 0, 0, 0, 0 };
	int i;

	while (1) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					controller.left = 1;
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					controller.right = 1;
				} else if (event.key.keysym.sym == SDLK_UP) {
					controller.up = 1;
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					controller.down = 1;
				} else if (event.key.keysym.sym == SDLK_q) {
					return 0;
				}
			} else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					controller.left = 0;
				} else if (event.key.keysym.sym == SDLK_RIGHT) {
					controller.right = 0;
				} else if (event.key.keysym.sym == SDLK_UP) {
					controller.up = 0;
				} else if (event.key.keysym.sym == SDLK_DOWN) {
					controller.down = 0;
				}
			} else if (event.type == SDL_QUIT) {
				return 0;
			}
		}
		/* Update player position. */
		if (controller.left) {
			player->pos.x -= PLAYER_SPEED;
		} else if (controller.right) {
			player->pos.x += PLAYER_SPEED;
		}
		if (controller.up) {
			player->pos.y -= PLAYER_SPEED;
		} else if (controller.down) {
			player->pos.y += PLAYER_SPEED;
		}
		/* Update camera position. */
		camera.x = player->pos.x - (SCREEN_W - 16) / 2;
		if (camera.x < 0)
			camera.x = 0;
		else if (camera.x >= tilemap->width * 16 - SCREEN_W)
			camera.x = tilemap->width * 16 - SCREEN_W - 1;
		camera.y = player->pos.y - (SCREEN_H - 16) / 2;
		if (camera.y < 0)
			camera.y = 0;
		else if (camera.y >= tilemap->height * 16 - SCREEN_H)
			camera.y = tilemap->height * 16 - SCREEN_H - 1;
		player->rel_pos.x = player->pos.x - camera.x;
		player->rel_pos.y = player->pos.y - camera.y;
		for (i = 0; i < tilemap->depth; i++) {
			if (i == tilemap->depth - 1) {
				if (drawPlayer(player, screen)) {
					fputs(SDL_GetError(), stderr);
					return -1;
				}
			}
			if (SDL_BlitSurface(tilemap->layers[i], &camera, screen, NULL)) {
				fputs(SDL_GetError(), stderr);
				return -1;
			}
		}
		if (SDL_Flip(screen) == -1) {
			fputs(SDL_GetError(), stderr);
			return -1;
		}
		SDL_Delay(1000 / 60);
	}
}

int main(int argc, char *argv[])
{
	const char filename[] = "res/untitled.tmx.bin";
	SDL_Surface *screen = NULL;
	TMP_Tilemap *tilemap = NULL;

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
	if ((tilemap = TMP_LoadTilemap(filename)) == NULL) {
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return -1;
	}
	game(tilemap, screen);
	TMP_FreeTilemap(tilemap);
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}

int GetFPS(){
	CurrentFPS = SDL_GetTicks() - StartTime;
	
	AverageFPS += CurrentFPS;
	AverageFPS /= 2;
	
	return 0;
}

float MoveCamera(){
	return SPEEDPPS * CurrentFPS;
}

