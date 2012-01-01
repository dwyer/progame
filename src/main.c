#include <stdio.h>
#include "SDL/SDL.h"
#include "tmx.h"

#define SCREEN_W 320
#define SCREEN_H 240
#define SCREEN_BPP 32

#define CAMERA_SPEED 2

typedef struct {
    int up;
    int down;
    int left;
    int right;
} Controls;

int game(TMP_Tilemap * tilemap, SDL_Surface * screen)
{
    SDL_Rect camera = { 0, 0, SCREEN_W, SCREEN_H };
    SDL_Event event;
    Controls controller = { 0, 0, 0, 0 };
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
        if (controller.left) {
            camera.x -= CAMERA_SPEED;
        } else if (controller.right) {
            camera.x += CAMERA_SPEED;
        }
        if (controller.up) {
            camera.y -= CAMERA_SPEED;
        } else if (controller.down) {
            camera.y += CAMERA_SPEED;
        }
        for (i = 0; i < tilemap->depth; i++) {
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
