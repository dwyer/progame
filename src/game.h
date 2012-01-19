#ifndef WORLD_H
#define WORLD_H

#include "input.h"

typedef struct World World;

World *Game_new(void);
void Game_free();

void Game_set_script(Script *script);

int Game_event(SDL_UserEvent event);
int Game_update(void);
int Game_draw(SDL_Surface * surface);

#endif
