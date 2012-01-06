#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define LUA_OK 0

#define LUA51
#include "lua_cross_yield_common.c"

int main (int argc, char **argv) {
	 lua_State *L;
	 int ret;

	 L = luaL_newstate ();
	 luaL_openlibs (L);

	 return_normally (L);

	 return_using_yield (L);

	 return EXIT_SUCCESS;
}
