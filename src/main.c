#include <stdio.h>
#include "SDL/SDL.h"
#include "player.h"
#include "tmx.h"
#include "motion.h"
#include "config.h"
#include "entity.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define SCREEN_BPP 32

#define PLAYER_SPEED 2

typedef struct {
	int up;
	int down;
	int left;
	int right;
} Controller;

int game(TMP_Tilemap * tilemap, SDL_Surface * screen, PrgConfig* Config)
{	
	EntityList* Entities = NULL;
	
	int mx = 0, my = 0;
	int AverageFPS = 9;
	int CurrentFPS = 9;
	int StartTime = 0;
	
	SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
	SDL_Event event;
	Player *player = createPlayer(0, 0);
	Controller controller = { 0, 0, 0, 0 };
	int i;
	
	Entities = malloc(sizeof(EntityList));
	memset(Entities, 0, sizeof(EntityList));
	
	Entities->Subject = malloc(sizeof(Entity));
	memset(Entities->Subject, 0, sizeof(Entity));
	
	Entities->Subject->Surf = SDL_CreateRGBSurface(SDL_HWSURFACE, 16, 16, 32, 0, 0, 0, 0);
	Entities->Subject->Source.h = 16;
	Entities->Subject->Source.w = 16;
	
	while (1) {
		StartTime = SDL_GetTicks();
		
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
			}
			else if (event.type == SDL_KEYUP) {
				if (event.key.keysym.sym == SDLK_LEFT) {
					controller.left = 0;
				}
				else if (event.key.keysym.sym == SDLK_RIGHT) {
					controller.right = 0;
				}
				else if (event.key.keysym.sym == SDLK_UP) {
					controller.up = 0;
				}
				else if (event.key.keysym.sym == SDLK_DOWN) {
					controller.down = 0;
				}
			}
			else if (event.type == SDL_QUIT) {
				return 0;
			}
		}
		/* Update player position. */
		mx = 0;
		my = 0;
		if (controller.left) {
			mx = Interpolate(Config->WalkSpeed, CurrentFPS) * -1;
		} else if (controller.right) {
			mx = Interpolate(Config->WalkSpeed, CurrentFPS);
		}
		if (controller.up) {
			my = Interpolate(Config->WalkSpeed, CurrentFPS) * -1;
		} else if (controller.down) {
			my = Interpolate(Config->WalkSpeed, CurrentFPS);
		}
		if (mx || my)
		movePlayer(player, mx, my);
		
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
		
		for (i = 0; i < tilemap->depth; i++) {
			if (i == tilemap->depth - 1) {
				if (drawPlayer(player, screen, camera)) {
					fputs(SDL_GetError(), stderr);
					return -1;
				}
				drawEntities(Entities, screen, camera.x, camera.y);
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
		/* SDL_Delay(1000 / 60); */
		GetFPS(&CurrentFPS, &AverageFPS, StartTime);
	}
}

int main(int argc, char *argv[])
{
	const char filename[] = "res/untitled.tmx.bin";
	SDL_Surface *screen = NULL;
	TMP_Tilemap *tilemap = NULL;
	PrgConfig Config;
	
	memset(&Config, 0, sizeof(PrgConfig));
	Config.WalkSpeed = 0.23;
	if (LoadConfig(&Config) != 0)
	return -1;
	
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		fputs(SDL_GetError(), stderr);
		return -1;
	}
	if ((screen =
		 SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP,
						  SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		fputs(SDL_GetError(), stderr);
		return -1;
	}
	if ((tilemap = TMP_LoadTilemap(filename)) == NULL) {
		fprintf(stderr, "Failed to open tilemap: %s!\n", filename);
		return -1;
	}
	
	game(tilemap, screen, &Config);
	
	TMP_FreeTilemap(tilemap);
	SDL_FreeSurface(screen);
	SDL_Quit();
	return 0;
}
