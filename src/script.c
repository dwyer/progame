#include <lua.h>
#include <lauxlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	const char *name;
	int firstgid;
	int tilewidth;
	int tileheight;
	int spacing;
	int margin;
	const char *image;
	int imagewidth;
	int imageheight;
	const char *transparentColor;
	void *properties;
	void *tiles;
} TilemapTileset;

typedef struct {
	const char *type;
	const char *name;
	int x;
	int y;
	int width;
	int height;
	bool visible;
	int opacity;
	void *properties;
	int *data;
} TilemapLayer;

typedef struct {
	int version;
	int luaversion;
	const char *orientation;
	int width;
	int height;
	int tilewidth;
	int tileheight;
	void *properties;
	TilemapTileset *tilesets;
	TilemapLayer *layers;
} Tilemap;

int load_map(lua_State *L);
int load_world(lua_State *L);

static luaL_Reg regs[] = {
	{ "load_map", load_map },
	{ "load_world", load_world },
	{ NULL, NULL }
};

void registerLuaRegs(lua_State *L) {
	luaL_reg *reg;

	luaL_openlibs(L);
	for (reg = regs; reg->name != NULL; reg++) {
		lua_register(L, reg->name, reg->func);
	}
}

int load_map(lua_State *L) {
	Tilemap *tmp = malloc(sizeof(Tilemap));
	int t = 1;

	lua_pushnil(L);
	while (lua_next(L, t)) {
		const char *key = lua_tostring(L, -2);
		const char *type = lua_typename(L, lua_type(L, -1));
		
		printf("%s - %s\n", key, type);
		if (!strcmp(key, "width"))
			tmp->width = lua_tonumber(L, -1);
		else if (!strcmp(key, "height"))
			tmp->height = lua_tonumber(L, -1);
		else if (!strcmp(key, "tilewidth"))
			tmp->tilewidth = lua_tonumber(L, -1);
		else if (!strcmp(key, "tileheight"))
			tmp->tileheight = lua_tonumber(L, -1);
		else if (!strcmp(key, "layers")) {
			/* keep walking... */
		}
		lua_pop(L, 1);
	}
	printf("%dx%d tilemap with %dx%d tiles\n",
			tmp->width,
			tmp->height,
			tmp->tilewidth,
			tmp->tileheight
			);
	return 1;
}

int load_world(lua_State *L) {
	return 1;
}
