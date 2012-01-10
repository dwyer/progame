#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include "tilemap.h"
#include "main.h"

#define TILE_SZ 16
#define TILEMAP_DIR "res/maps/"

Tilemap *Tilemap_load(const char *filename) {
    Tilemap *tilemap = NULL;
    SDL_Surface *tileset = NULL;
    SDL_Surface *layer = NULL;
    SDL_Rect src = { 0, 0, TILE_SZ, TILE_SZ };
    SDL_Rect dst = src;
    lua_State *L = NULL;
    char *str = NULL;
    int i, j, n;

    filename = "res/maps/untitled.lua"; /* override the filename for testing TODO:  delete this */
    L = luaL_newstate();
    luaL_dofile(L, filename);
    /*
     * Tiled maps exported to Lua have a version number of 1.1 and a luaversion
     * of 5.1. If these are in order, we'll just assume the rest of the map is
     * valid and not error check every little thing.
     */
    lua_getfield(L, -1, "version");
    if (strcmp("1.1", lua_tostring(L, -1))) {
        fprintf(stderr, "Not a valid tilemap: %s\n", filename);
        lua_close(L);
        return NULL;
    }
    lua_pop(L, 1);
    /* 
     * Create a tilemap and load its width and height. Tilesize and orientation
     * are ignored for now, but we should at least check them to determine the
     * validity of the map later on.
     */
    tilemap = malloc(sizeof(*tilemap));
    tilemap->collision = NULL;
    tilemap->background = NULL;
    tilemap->foreground = NULL;
    lua_getfield(L, 1, "width");
    lua_getfield(L, 1, "height");
    tilemap->w = luaL_checknumber(L, -2);
    tilemap->h = luaL_checknumber(L, -1);
    lua_pop(L, 2);
    tilemap->layer_w = tilemap->w * TILE_SZ;
    tilemap->layer_h = tilemap->h * TILE_SZ;
    /*
     * Get the first tileset for now. In the future we can always add support
     * for additional tilesets.
     */
    lua_getfield(L, 1, "tilesets");
    lua_pushnumber(L, 1);
    lua_gettable(L, -2);
    lua_getfield(L, -1, "image");
    str = calloc(strlen(TILEMAP_DIR) + strlen(lua_tostring(L, -1)) + 1, sizeof(char));
    strcpy(str, TILEMAP_DIR);
    strcat(str, lua_tostring(L, -1));
    if ((tileset = SDL_LoadBMP(str)) == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        lua_close(L);
        free(str);
        free(tilemap);
        return NULL;
    }
    free(str);
    lua_pop(L, 3); /* pop image, tileset 1, and tilesets */
    /*
     * Time to draw the layers.
     */
    tilemap->collision = calloc(tilemap->w * tilemap->h, sizeof(*tilemap->collision));
	tilemap->background = SDL_CreateRGBSurface(SDL_HWSURFACE, tilemap->layer_w, tilemap->layer_h, SCREEN_BPP, 0, 0, 0, 0);
	tilemap->foreground = SDL_CreateRGBSurface(SDL_HWSURFACE, tilemap->layer_w, tilemap->layer_h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_FillRect(tilemap->background, NULL, 0xFF00FF);
	SDL_FillRect(tilemap->foreground, NULL, 0xFF00FF);
	SDL_SetColorKey(tileset, SDL_SRCCOLORKEY, SDL_MapRGB(tileset->format, 255, 0, 255));
	SDL_SetColorKey(tilemap->background, SDL_SRCCOLORKEY, SDL_MapRGB(tileset->format, 255, 0, 255));
	SDL_SetColorKey(tilemap->foreground, SDL_SRCCOLORKEY, SDL_MapRGB(tileset->format, 255, 0, 255));
    lua_getfield(L, -1, "layers");
    tilemap->depth = lua_objlen(L, -1);
    layer = tilemap->background;
    for (i = 0; i < tilemap->depth; i++) {
        lua_rawgeti(L, -1, i + 1); /* get layer */
        lua_getfield(L, -1, "type"); /* get type */
        if (!strcmp(lua_tostring(L, -1), "tilelayer")) {
            lua_getfield(L, -2, "data");
            for (j = 0; j < lua_objlen(L, -1); j++) {
                int datum;
                lua_rawgeti(L, -1, j + 1);
                datum = lua_tointeger(L, -1);
                if (i == tilemap->depth - 1) { /* assume last layer is collision */
                    tilemap->collision[j] = datum;
                } else if (datum) {
                    src.x = (datum - 1) % (tileset->w / TILE_SZ) * TILE_SZ;
                    src.y = (datum - 1) / (tileset->w / TILE_SZ) * TILE_SZ; 
                    dst.x = (j % tilemap->h) * TILE_SZ;
                    dst.y = (j / tilemap->h) * TILE_SZ;
                    SDL_BlitSurface(tileset, &src, layer, &dst);
                }
                lua_pop(L, 1); /* pop data[j] */
            }
            lua_pop(L, 1); /* pop data */
        } else if (!strcmp(lua_tostring(L, -1), "objectgroup")) {
            layer = tilemap->foreground; /* swap layers */
        }
        lua_pop(L, 2); /* pop type and layer */
    }
    lua_pop(L, 1); /* pop layers */
    lua_close(L);
    return tilemap;
}

void Tilemap_draw(Tilemap * tilemap, SDL_Surface * surface) {
}

void Tilemap_free(Tilemap * tilemap) {
	int i, j;

    free(tilemap->collision);
	free(tilemap->background);
	free(tilemap->foreground);
	free(tilemap);
}

int Tilemap_tile_is_occupied(Tilemap * tilemap, int x, int y) {
	return x < 0 || y < 0 || x >= tilemap->w || y >= tilemap->h
		|| tilemap->collision[x + y * tilemap->w];
}

int Tilemap_pixel_is_occupied(Tilemap * tilemap, int x, int y) {
	return (Tilemap_tile_is_occupied(tilemap, x / TILE_SZ, y / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, (x + TILE_SZ - 1) / TILE_SZ, y / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, x / TILE_SZ, (y + TILE_SZ - 1) / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, (x + TILE_SZ - 1) / TILE_SZ, (y + TILE_SZ - 1) / TILE_SZ));
}
