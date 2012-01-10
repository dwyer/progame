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
#define TILEMAP_VERSION "1.1"
#define TILEMAP_LUAVERSION "5.1"

struct Tilemap {
	int w;
	int h;
    int tile_w;
    int tile_h;
    int *collision;
	SDL_Surface *background;
	SDL_Surface *foreground;
};

/*
 * Helper function to assist in creating a surface and setting its transparency.
 */
SDL_Surface *create_surface(int w, int h, Uint32 color_key) {
    SDL_Surface *surface = NULL;

	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, color_key);
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, color_key);
    return surface;
}

/*
 * Loads and returns a tilemap. It must be a Tiled tilemap exported to Lua.
 */
Tilemap *Tilemap_load(const char *filename) {
    Tilemap *tilemap = NULL;
    SDL_Surface *tileset = NULL;
    SDL_Surface *layer = NULL;
    SDL_Rect src = { 0, 0, TILE_SZ, TILE_SZ };
    SDL_Rect dst = src;
    Uint32 color_key = 0;
    lua_State *L = NULL;
    char *str = NULL;
    int i, j, n;

    L = luaL_newstate();
    luaL_dofile(L, filename);
    /*
     * Tiled maps exported to Lua have a version number luaversion number.
     * If these are in order, we'll just assume the format is valid and resist
     * error checking every little thing.
     */
    lua_getfield(L, -1, "version");
    lua_getfield(L, -2, "luaversion");
    if (strcmp(lua_tostring(L, -2), TILEMAP_VERSION) ||
        strcmp(lua_tostring(L, -1), TILEMAP_LUAVERSION)) {
        fprintf(stderr, "Not a valid tilemap: %s\n", filename);
        lua_close(L);
        return NULL;
    }
    lua_pop(L, 2); /* pop version and luaversion */
    /* 
     * Create a tilemap and load its width and height.
     * TODO: check to make sure the level designer used the correct tile size
     * and orientation.
     */
    tilemap = malloc(sizeof(*tilemap));
    tilemap->collision = NULL;
    tilemap->background = NULL;
    tilemap->foreground = NULL;
    lua_getfield(L, 1, "width");
    lua_getfield(L, 1, "height");
    tilemap->w = lua_tonumber(L, -2);
    tilemap->h = lua_tonumber(L, -1);
    lua_pop(L, 2); /* pop width and height */
    /*
     * Get the first tileset for now.
     * TODO: add support for more than one tileset.
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
        Tilemap_free(tilemap);
        lua_close(L);
        free(str);
        return NULL;
    }
    free(str);
    lua_pop(L, 1); /* pop image */
    lua_getfield(L, -1, "transparentColor");
    sscanf(lua_tostring(L, -1), "#%x", &color_key);
	SDL_SetColorKey(tileset, SDL_SRCCOLORKEY, color_key);
    lua_pop(L, 3); /* pop transparentColor, tileset 1, and tilesets */
    /*
     * Time to draw the layers.
     */
    tilemap->collision = calloc(tilemap->w * tilemap->h, sizeof(*tilemap->collision));
    tilemap->background = create_surface(tilemap->w * TILE_SZ, tilemap->h * TILE_SZ, color_key);
    tilemap->foreground = create_surface(tilemap->w * TILE_SZ, tilemap->h * TILE_SZ, color_key);
    lua_getfield(L, -1, "layers");
    layer = tilemap->background;
    n = lua_objlen(L, -1);
    for (i = 0; i < n; i++) {
        lua_rawgeti(L, -1, i + 1); /* get layer */
        lua_getfield(L, -1, "type"); /* get type */
        if (!strcmp(lua_tostring(L, -1), "tilelayer")) {
            lua_getfield(L, -2, "data");
            for (j = 0; j < lua_objlen(L, -1); j++) {
                int datum;
                lua_rawgeti(L, -1, j + 1);
                datum = lua_tointeger(L, -1);
                if (i == n - 1) { /* assume last layer is collision */
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
    SDL_FreeSurface(tileset);
    lua_close(L);
    return tilemap;
}

SDL_Surface *Tilemap_background(const Tilemap *tilemap) {
    return tilemap->background;
}

SDL_Surface *Tilemap_foreground(const Tilemap *tilemap) {
    return tilemap->foreground;
}

int Tilemap_layer_w(const Tilemap *tilemap) {
    return tilemap->background->w;
}

int Tilemap_layer_h(const Tilemap *tilemap) {
    return tilemap->background->h;
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
