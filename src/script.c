#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "entity.h"
#include "event.h"
#include "script.h"
#include "tilemap.h"

#define LIBNAME_ENTITY "Entity"
#define LIBNAME_TILEMAP "Tilemap"
#define TNAME_ENTITY "progame.entity"
#define TNAME_TILEMAP "progame.tilemap"

typedef struct Global Global;

struct Script {
	lua_State *L;
};

struct Global {
	const char *name;
	const int value;
};

static Global globals[] = {
	{ "ACTION_STANDING", ACTION_STANDING },
	{ "ACTION_WALKING", ACTION_WALKING },
	{ "DIRECTION_UP", DIRECTION_UP },
	{ "DIRECTION_DOWN", DIRECTION_DOWN },
	{ "DIRECTION_LEFT", DIRECTION_LEFT },
	{ "DIRECTION_RIGHT", DIRECTION_RIGHT },
	{ NULL, 0 }
};

static int luaopen_entity(lua_State *L);
static int luaopen_tilemap(lua_State *L);

static int l_entity_add_frames(lua_State *L);
static int l_entity_new(lua_State *L);
static int l_entity_get_pos(lua_State *L);
static int l_entity_get_vel(lua_State *L);
static int l_entity_set_pos(lua_State *L);
static int l_entity_set_size(lua_State *L);
static int l_entity_set_sprite(lua_State *L);
static int l_entity_set_update_callback(lua_State *L);
static int l_entity_set_vel(lua_State *L);
static int l_tilemap_open(lua_State *L);
static int l_tilemap_get_size(lua_State *L);
static int l_tilemap_is_tile_occupied(lua_State *L);
static int l_tilemap_is_region_occupied(lua_State *L);

/** Entity functions */
static const luaL_Reg entity_f[] = {
	{ "new", l_entity_new },
	{ NULL, NULL }
};

/** Entity methods */
static const luaL_Reg entity_m[] = {
	{ "add_frames", l_entity_add_frames },
	{ "get_pos", l_entity_get_pos },
	{ "get_vel", l_entity_get_vel },
	{ "set_pos", l_entity_set_pos },
	{ "set_size", l_entity_set_size },
	{ "set_sprite", l_entity_set_sprite },
	{ "set_update_callback", l_entity_set_update_callback },
	{ "set_vel", l_entity_set_vel },
	{ NULL, NULL }
};

/** Tilemap functions */
static const luaL_Reg tilemap_f[] = {
	{ "open", l_tilemap_open },
	{ NULL, NULL }
};

/** Tilemap methods */
static const luaL_Reg tilemap_m[] = {
	{ "get_size", l_tilemap_get_size },
	{ "is_tile_occupied", l_tilemap_is_tile_occupied },
	{ "is_region_occupied", l_tilemap_is_region_occupied },
	{ NULL, NULL }
};

/** Libraries */
static const luaL_Reg libs[] = {
	{ LIBNAME_ENTITY, luaopen_entity },
	{ LIBNAME_TILEMAP, luaopen_tilemap },
	{ NULL, NULL }
};

/**
 * Creates a new Script instance.
 * \return A new Script instance.
 */
Script *Script_init(void) {
	Script *script;
	Global *glob;
	const luaL_Reg *lib;

	script = malloc(sizeof(*script));
	script->L = luaL_newstate();
	luaL_openlibs(script->L);
	/* register global vars and funcs */
	for (glob = globals; glob->name != NULL; glob++) {
		lua_pushinteger(script->L, glob->value);
		lua_setglobal(script->L, glob->name);
	}
	/* Register custom libraries. */
	for (lib = libs; lib->func; lib++) {
		lua_pushcfunction(script->L, lib->func);
		lua_pushstring(script->L, lib->name);
		lua_call(script->L, 1, 0);
	}
	return script;
}

/**
 * Frees the given Script instance.
 */
void Script_free(Script *script) {
	lua_close(script->L);
	free(script);
}

int Script_call(Script *script, int ref) {
	int ret;

	if (!ref)
		return 0;
	lua_rawgeti(script->L, LUA_REGISTRYINDEX, ref);
	if ((ret = lua_pcall(script->L, 0, LUA_MULTRET, 0)))
		fprintf(stderr, "%s\n", lua_tostring(script->L, -1));
	return ret;
}

/**
 * Runs a script.
 * \return 0 on success, non-zero on failure.
 */
int Script_run(Script *script, const char *filename) {
	int ret;

	if ((ret = luaL_dofile(script->L, filename)))
		fprintf(stderr, "%s\n", lua_tostring(script->L, -1));
	return ret;
}

void Script_unref(Script *script, int ref) {
	luaL_unref(script->L, LUA_REGISTRYINDEX, ref);
}

/** Register entity library. */
static int luaopen_entity(lua_State *L) {
	luaL_newmetatable(L, TNAME_ENTITY);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(L, NULL, entity_m);
	luaL_register(L, LIBNAME_ENTITY, entity_f);
	return 1;
}

/** Register tilemap library. */
static int luaopen_tilemap(lua_State *L) {
	luaL_newmetatable(L, TNAME_TILEMAP);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(L, NULL, tilemap_m);
	luaL_register(L, LIBNAME_TILEMAP, tilemap_f);
	return 1;
}

static int l_entity_new(lua_State *L) {
	Entity *entity = Entity_new();
	Entity **udata = lua_newuserdata(L, sizeof(*udata));

	*udata = entity;
	Event_push(EVENT_ENTITY_NEW, entity, NULL);
	luaL_getmetatable(L, TNAME_ENTITY);
	lua_setmetatable(L, -2);
	return 1;
}

static int l_entity_add_frames(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	int action = luaL_checkinteger(L, 2);
	int direction = luaL_checkinteger(L, 3);
	int n = lua_objlen(L, 4);
	int i;

	luaL_argcheck(L, action >= 0 && action < NUM_ACTIONS, 2, "out of range");
	luaL_argcheck(L, direction >= 0 && direction < NUM_DIRECTIONS, 2, "out of range");
	for (i = 1; i <= n; i++) {
		int frame;
		lua_rawgeti(L, 4, i);
		frame = luaL_checkinteger(L, -1);
		Entity_add_frame(entity, action, direction, frame);
		lua_pop(L, 1);
	}
	return 0;
}

static int l_entity_get_pos(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	SDL_Rect pos = Entity_get_pos(entity);

	lua_pushinteger(L, pos.x);
	lua_pushinteger(L, pos.y);
	lua_pushinteger(L, pos.w);
	lua_pushinteger(L, pos.h);
	return 4;
}

static int l_entity_get_vel(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	SDL_Rect vel = Entity_get_vel(entity);

	lua_pushinteger(L, vel.x);
	lua_pushinteger(L, vel.y);
	return 2;
}

static int l_entity_set_pos(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	Entity_set_pos(entity, x, y);
	return 0;
}

static int l_entity_set_size(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	int w = luaL_checkinteger(L, 2);
	int h = luaL_checkinteger(L, 3);

	Entity_set_size(entity, w, h);
	return 0;
}

static int l_entity_set_sprite(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	const char *filename = luaL_checkstring(L, 2);

	Entity_set_sprite(entity, filename);
	return 0;
}

static int l_entity_set_update_callback(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	int ref; 

	luaL_checktype(L, -1, LUA_TFUNCTION);
	ref = luaL_ref(L, LUA_REGISTRYINDEX);
	Entity_set_update_callback_ref(entity, ref);
	return 0;
}

static int l_entity_set_vel(lua_State *L) {
	Entity *entity = *(Entity **)luaL_checkudata(L, 1, TNAME_ENTITY);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	Entity_set_vel(entity, x, y);
	return 0;
}

static int l_tilemap_open(lua_State *L) {
	const char *filename = luaL_checkstring(L, 1);
	Tilemap *tilemap = Tilemap_open(filename);
	Tilemap **udata = lua_newuserdata(L, sizeof(*udata));

	*udata = tilemap;
	Event_push(EVENT_TILEMAP_OPEN, tilemap, NULL);
	luaL_getmetatable(L, TNAME_TILEMAP);
	lua_setmetatable(L, -2);
	return 1;
}

static int l_tilemap_get_size(lua_State *L) {
	Tilemap *tilemap = *(Tilemap **)luaL_checkudata(L, 1, TNAME_TILEMAP);
	SDL_Rect size = Tilemap_get_size(tilemap);

	lua_pushinteger(L, size.w);
	lua_pushinteger(L, size.h);
	return 2;
}

static int l_tilemap_is_region_occupied(lua_State *L) {
	Tilemap *tilemap = *(Tilemap **)luaL_checkudata(L, 1, TNAME_TILEMAP);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	int w = luaL_checkinteger(L, 4);
	int h = luaL_checkinteger(L, 5);
	bool res = Tilemap_is_region_occupied(tilemap, x, y, w, h);

	lua_pushboolean(L, res);
	return 1;
}

static int l_tilemap_is_tile_occupied(lua_State *L) {
	Tilemap *tilemap = *(Tilemap **)luaL_checkudata(L, 1, TNAME_TILEMAP);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);
	bool res = Tilemap_is_tile_occupied(tilemap, x, y);

	lua_pushboolean(L, res);
	return 1;
}
