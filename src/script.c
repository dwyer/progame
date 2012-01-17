#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>

#include "entity.h"
#include "event.h"
#include "tilemap.h"

int l_entity_add_frame(lua_State *L);
int l_entity_new(lua_State *L);
int l_entity_set_pos(lua_State *L);
int l_entity_set_size(lua_State *L);
int l_entity_set_sprite(lua_State *L);
int l_entity_set_vel(lua_State *L);
int l_tilemap_load(lua_State *L);
int l_new_entity(lua_State * L);

typedef struct Global {
	const char *name;
	const int value;
} Global;

static Global globals[] = {
	{ "ACTION_STANDING", ACTION_STANDING },
	{ "ACTION_WALKING", ACTION_WALKING },
	{ "DIRECTION_UP", DIRECTION_UP },
	{ "DIRECTION_DOWN", DIRECTION_DOWN },
	{ "DIRECTION_LEFT", DIRECTION_LEFT },
	{ "DIRECTION_RIGHT", DIRECTION_RIGHT },
	{ NULL, 0 }
};

static luaL_Reg regs[] = {
	{"entity_new", l_entity_new},
	{"entity_add_frame", l_entity_add_frame},
	{"entity_set_pos", l_entity_set_pos},
	{"entity_set_size", l_entity_set_size},
	{"entity_set_sprite", l_entity_set_sprite},
	{"entity_set_vel", l_entity_set_vel},
	{"tilemap_load", l_tilemap_load},
	{NULL, NULL}
};

void Script_reg(lua_State * L) {
	luaL_reg *reg;
	Global *glob;

	luaL_openlibs(L);
	for (glob = globals; glob->name != NULL; glob++) {
		lua_pushinteger(L, glob->value);
		lua_setglobal(L, glob->name);
	}
	for (reg = regs; reg->name != NULL; reg++) {
		lua_register(L, reg->name, reg->func);
	}
}

int l_entity_new(lua_State *L) {
	Entity *entity = Entity_new();

	Event_push(EVENT_ENTITY_NEW, entity, NULL);
	lua_pushlightuserdata(L, entity);
	return 1;
}

int l_entity_add_frame(lua_State *L) {
	Entity *entity = lua_touserdata(L, 1);
	int action = luaL_checkinteger(L, 2);
	int direction = luaL_checkinteger(L, 3);
	int frame = luaL_checkinteger(L, 4);

	Entity_add_frame(entity, action, direction, frame);
	return 0;
}

int l_entity_set_pos(lua_State *L) {
	Entity *entity = lua_touserdata(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	Entity_set_pos(entity, x, y);
	return 0;
}

int l_entity_set_size(lua_State *L) {
	Entity *entity = lua_touserdata(L, 1);
	int w = luaL_checkinteger(L, 2);
	int h = luaL_checkinteger(L, 3);

	Entity_set_size(entity, w, h);
	return 0;
}

int l_entity_set_sprite(lua_State *L) {
	Entity *entity = lua_touserdata(L, 1);
	const char *filename = luaL_checkstring(L, 2);

	Entity_set_sprite(entity, filename);
	return 0;
}

int l_entity_set_vel(lua_State *L) {
	Entity *entity = lua_touserdata(L, 1);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	Entity_set_vel(entity, x, y);
	return 0;
}

int l_tilemap_load(lua_State *L) {
	const char *filename = luaL_checkstring(L, 1);
	Tilemap *tilemap = Tilemap_load(filename);

	Event_push(EVENT_TILEMAP_LOAD, tilemap, NULL);
	lua_pushlightuserdata(L, tilemap);
	return 1;
}
