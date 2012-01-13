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
    int *collision;
	SDL_Surface *background;
	SDL_Surface *foreground;
};

/**
 * Helper function to assist in creating a surface and setting its transparency.
 * \param w Width of the new surface.
 * \param h Height of the new surface.
 * \param color_key Color to represent transparent pixels, e.g. 0xff00ff.
 * \return An SDL_Surface.
 */
SDL_Surface *create_surface(int w, int h, Uint32 color_key) {
    SDL_Surface *surface = NULL;

	surface = SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, SCREEN_BPP, 0, 0, 0, 0);
	SDL_FillRect(surface, NULL, color_key);
	SDL_SetColorKey(surface, SDL_SRCCOLORKEY, color_key);
    return surface;
}

/**
 * Loads and returns a tilemap.
 * \param filename Path of the tilemap to be loaded. Must be a Tiled tilemap
 *        exported to Lua.
 * \return The loaded tilemap.
 */
Tilemap *Tilemap_load(const char *filename) {
    Tilemap *tilemap = NULL;
    SDL_Surface *tileset = NULL;
    SDL_Surface *layer = NULL;
    SDL_Rect src = { 0, 0, TILE_SZ, TILE_SZ };
    SDL_Rect dst = src;
    Uint32 color_key = 0;
    lua_State *L = NULL;
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
    lua_pushstring(L, TILEMAP_DIR);
    lua_getfield(L, -2, "image");
    lua_concat(L, 2);
    if ((tileset = SDL_LoadBMP(lua_tostring(L, -1))) == NULL) {
        fprintf(stderr, "SDL error: %s\n", SDL_GetError());
        Tilemap_free(tilemap);
        lua_close(L);
        return NULL;
    }
    lua_pop(L, 1); /* pop TILEMAP_DIR+image */
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

/**
 * Free the tilemap and all its attributes.
 * \param tilemap Tilemap to be freed.
 */
void Tilemap_free(Tilemap * tilemap) {
	int i, j;

    free(tilemap->collision);
	free(tilemap->background);
	free(tilemap->foreground);
	free(tilemap);
}

/**
 * Returns an SDL_Rect representing the area of the tilemap.
 * \param tilemap A tilemap.
 * \return Area of the tilemap.
 */
SDL_Rect Tilemap_get_area(const Tilemap *tilemap) {
    SDL_Rect area = { 0, 0, 0, 0 };

    area.w = tilemap->background->w;
    area.h = tilemap->foreground->h;
    return area;
}

/**
 * Returns non-zero if the given tile has collision data.
 */
int Tilemap_tile_is_occupied(const Tilemap * tilemap, int x, int y) {
	return x < 0 || y < 0 || x >= tilemap->w || y >= tilemap->h
		|| tilemap->collision[x + y * tilemap->w];
}

/**
 * Returns non-zero if the given pixel has collision data.
 */
int Tilemap_pixel_is_occupied(const Tilemap * tilemap, int x, int y) {
	return (Tilemap_tile_is_occupied(tilemap, x / TILE_SZ, y / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, (x + TILE_SZ - 1) / TILE_SZ, y / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, x / TILE_SZ, (y + TILE_SZ - 1) / TILE_SZ) ||
			Tilemap_tile_is_occupied(tilemap, (x + TILE_SZ - 1) / TILE_SZ, (y + TILE_SZ - 1) / TILE_SZ));
}

/**
 * Draw the background to the screen at the given camera position.
 */
int Tilemap_draw_background(const Tilemap *tilemap, SDL_Surface *screen, SDL_Rect camera) {
    return SDL_BlitSurface(tilemap->background, &camera, screen, NULL);
}

/**
 * Draw the foreground to the screen at the given camera position.
 */
int Tilemap_draw_foreground(const Tilemap *tilemap, SDL_Surface *screen, SDL_Rect camera) {
    return SDL_BlitSurface(tilemap->foreground, &camera, screen, NULL);
}
