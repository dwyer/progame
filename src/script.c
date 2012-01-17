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

#define TNAME_ENTITY "progame.entity"

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

int l_entity_add_frame(lua_State *L);
int l_entity_new(lua_State *L);
int l_entity_set_pos(lua_State *L);
int l_entity_set_size(lua_State *L);
int l_entity_set_sprite(lua_State *L);
int l_entity_set_vel(lua_State *L);
int l_tilemap_load(lua_State *L);

static luaL_Reg regs[] = {
	{ "tilemap_load", l_tilemap_load },
	{ NULL, NULL }
};

static const struct luaL_Reg entity_f[] = {
	{ "new", l_entity_new },
	{ NULL, NULL }
};

static const struct luaL_Reg entity_m[] = {
	{ "add_frame", l_entity_add_frame },
	{ "set_pos", l_entity_set_pos },
	{ "set_size", l_entity_set_size },
	{ "set_sprite", l_entity_set_sprite },
	{ "set_vel", l_entity_set_vel },
	{ NULL, NULL }
};

Script *Script_init(void) {
	Script *script;
	Global *glob;
	luaL_reg *reg;
	lua_State *L;

	script = malloc(sizeof(*script));
	script->L = L = luaL_newstate();
	luaL_openlibs(script->L);
	/* register global vars and funcs */
	for (glob = globals; glob->name != NULL; glob++) {
		lua_pushinteger(script->L, glob->value);
		lua_setglobal(script->L, glob->name);
	}
	for (reg = regs; reg->name != NULL; reg++) {
		lua_register(script->L, reg->name, reg->func);
	}
	/* register entity module */
	luaL_newmetatable(script->L, TNAME_ENTITY);
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_register(script->L, NULL, entity_m);
	luaL_register(script->L, "Entity", entity_f);
	return script;
}

void Script_free(Script *script) {
	lua_close(script->L);
	free(script);
}

int Script_run(Script *script, const char *filename) {
	return luaL_dofile(script->L, filename);
}

int l_entity_new(lua_State *L) {
	Entity *entity = Entity_new();

	Event_push(EVENT_ENTITY_NEW, entity, NULL);
	lua_pushlightuserdata(L, entity);
	luaL_getmetatable(L, TNAME_ENTITY);
	lua_setmetatable(L, -2);
	return 1;
}

int l_entity_add_frame(lua_State *L) {
	Entity *entity = luaL_checkudata(L, 1, TNAME_ENTITY);
	int action = luaL_checkinteger(L, 2);
	int direction = luaL_checkinteger(L, 3);
	int frame = luaL_checkinteger(L, 4);

	Entity_add_frame(entity, action, direction, frame);
	return 0;
}

int l_entity_set_pos(lua_State *L) {
	Entity *entity = luaL_checkudata(L, 1, TNAME_ENTITY);
	int x = luaL_checkinteger(L, 2);
	int y = luaL_checkinteger(L, 3);

	Entity_set_pos(entity, x, y);
	return 0;
}

int l_entity_set_size(lua_State *L) {
	Entity *entity = luaL_checkudata(L, 1, TNAME_ENTITY);
	int w = luaL_checkinteger(L, 2);
	int h = luaL_checkinteger(L, 3);

	Entity_set_size(entity, w, h);
	return 0;
}

int l_entity_set_sprite(lua_State *L) {
	Entity *entity = luaL_checkudata(L, 1, TNAME_ENTITY);
	const char *filename = luaL_checkstring(L, 2);

	Entity_set_sprite(entity, filename);
	return 0;
}

int l_entity_set_vel(lua_State *L) {
	Entity *entity = luaL_checkudata(L, 1, TNAME_ENTITY);
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

