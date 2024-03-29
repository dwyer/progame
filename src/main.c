#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include <SDL/SDL.h>

#include "config.h"
#include "script.h"
#include "main.h"
#include "event.h"
#include "entity.h"
#include "game.h"

/* Number of milliseconds between logic updates. */
#define UPDATE_INTERVAL 10

Uint32 push_update_event(Uint32 interval, void *param);

/**
 * Initialize everything, run the game, deinitialize, quit.
 */
int main(int argc, char *argv[]) {
    int result = 0;
    SDL_Surface *screen = NULL;
    SDL_TimerID timer_id;
    const char *script_file = "res/scripts/init.lua";
    const char *config_file = "res/scripts/config.lua";

    /* Initialization. */
    srand(time(NULL));
    if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO)) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }
    if (!(timer_id = SDL_AddTimer(UPDATE_INTERVAL, push_update_event, NULL))) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }
    if (!(screen = SDL_SetVideoMode(SCREEN_W, SCREEN_H, SCREEN_BPP,
                                    SCREEN_FLAGS))) {
        fprintf(stderr, "%s\n", SDL_GetError());
        return -1;
    }
    SDL_WM_SetCaption("/prog/ame", NULL);
    if (Game_init())
        return -1;
    if (Script_init())
        return -1;
    if (Script_run(script_file))
        return -1;
    if (Config_run(config_file))
        return -1;
    /* Play the fucking game. */
    result = Game_play(screen);
    /* Deinitialization */
    Game_quit();
    Script_quit();
    SDL_FreeSurface(screen);
    SDL_RemoveTimer(timer_id);
    SDL_Quit();
    return result;
}

/** 
 * Pushes a user event to the event queue which will indicate that it's time to
 * update world.
 * \param interval Milliseconds between updates.
 * \param param Ignored.
 * \return interval
 */
Uint32 push_update_event(Uint32 interval, void *param) {
    Event_push(EVENT_WORLD_UPDATE, NULL, NULL);
    return interval;
}
