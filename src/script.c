#include <lua.h>
#include <lauxlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int l_load_map(lua_State *L);
int l_load_world(lua_State *L);

static luaL_Reg regs[] = {
	{ "load_map", l_load_map },
	{ "load_world", l_load_world },
	{ NULL, NULL }
};

void Script_reg(lua_State *L) {
	luaL_reg *reg;

	luaL_openlibs(L);
	for (reg = regs; reg->name != NULL; reg++) {
		lua_register(L, reg->name, reg->func);
	}
}

/**
 * Loads the map. Takes one argument, which should be the Lua table exported by
 * Tiled's Lua plugin. Call with `load_map(dofile(filename))` where filename is
 * the name of the file exported by tiled.
 */
int l_load_map(lua_State *L) {
	return 1;
}

int l_load_world(lua_State *L) {
	return 1;
}
