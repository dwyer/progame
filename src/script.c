#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int load_world(lua_State *);

static luaL_Reg regs[] = {
	{ "load_world", load_world },
	{ NULL, NULL }
};

void registerLuaRegs(lua_State *L) {
	luaL_reg *reg;

	for (reg = regs; reg->name != NULL; reg++) {
		lua_register(L, reg->name, reg->func);
	}
}

int load_world(lua_State *L) {
	const char *filename = lua_tostring(L, -1);
	printf("%s\n", filename);
	return 1;
}
