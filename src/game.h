#ifndef WORLD_H
#define WORLD_H

int Game_init(void);
void Game_quit(void);

int Game_play(SDL_Surface *screen);

void Game_add_entity(Entity *entity);
void Game_set_tilemap(const char *filename);

bool Game_events(void);

int Game_event(SDL_UserEvent event);
int Game_update(void);
int Game_draw(SDL_Surface *surface);

#endif
