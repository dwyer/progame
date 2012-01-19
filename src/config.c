#include <stdbool.h>

#include <SDL/SDL.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "event.h"
#include "input.h"

typedef struct {
	const char *name;
	int n;
} KeyReg;

KeyReg key_regs[] = {
	{"SDLK_UNKNOWN", SDLK_UNKNOWN},
	{"SDLK_FIRST", SDLK_FIRST},
	{"SDLK_BACKSPACE", SDLK_BACKSPACE},
	{"SDLK_TAB", SDLK_TAB},
	{"SDLK_CLEAR", SDLK_CLEAR},
	{"SDLK_RETURN", SDLK_RETURN},
	{"SDLK_PAUSE", SDLK_PAUSE},
	{"SDLK_ESCAPE", SDLK_ESCAPE},
	{"SDLK_SPACE", SDLK_SPACE},
	{"SDLK_EXCLAIM", SDLK_EXCLAIM},
	{"SDLK_QUOTEDBL", SDLK_QUOTEDBL},
	{"SDLK_HASH", SDLK_HASH},
	{"SDLK_DOLLAR", SDLK_DOLLAR},
	{"SDLK_AMPERSAND", SDLK_AMPERSAND},
	{"SDLK_QUOTE", SDLK_QUOTE},
	{"SDLK_LEFTPAREN", SDLK_LEFTPAREN},
	{"SDLK_RIGHTPAREN", SDLK_RIGHTPAREN},
	{"SDLK_ASTERISK", SDLK_ASTERISK},
	{"SDLK_PLUS", SDLK_PLUS},
	{"SDLK_COMMA", SDLK_COMMA},
	{"SDLK_MINUS", SDLK_MINUS},
	{"SDLK_PERIOD", SDLK_PERIOD},
	{"SDLK_SLASH", SDLK_SLASH},
	{"SDLK_0", SDLK_0},
	{"SDLK_1", SDLK_1},
	{"SDLK_2", SDLK_2},
	{"SDLK_3", SDLK_3},
	{"SDLK_4", SDLK_4},
	{"SDLK_5", SDLK_5},
	{"SDLK_6", SDLK_6},
	{"SDLK_7", SDLK_7},
	{"SDLK_8", SDLK_8},
	{"SDLK_9", SDLK_9},
	{"SDLK_COLON", SDLK_COLON},
	{"SDLK_SEMICOLON", SDLK_SEMICOLON},
	{"SDLK_LESS", SDLK_LESS},
	{"SDLK_EQUALS", SDLK_EQUALS},
	{"SDLK_GREATER", SDLK_GREATER},
	{"SDLK_QUESTION", SDLK_QUESTION},
	{"SDLK_AT", SDLK_AT},
	{"SDLK_LEFTBRACKET", SDLK_LEFTBRACKET},
	{"SDLK_BACKSLASH", SDLK_BACKSLASH},
	{"SDLK_RIGHTBRACKET", SDLK_RIGHTBRACKET},
	{"SDLK_CARET", SDLK_CARET},
	{"SDLK_UNDERSCORE", SDLK_UNDERSCORE},
	{"SDLK_BACKQUOTE", SDLK_BACKQUOTE},
	{"SDLK_a", SDLK_a},
	{"SDLK_b", SDLK_b},
	{"SDLK_c", SDLK_c},
	{"SDLK_d", SDLK_d},
	{"SDLK_e", SDLK_e},
	{"SDLK_f", SDLK_f},
	{"SDLK_g", SDLK_g},
	{"SDLK_h", SDLK_h},
	{"SDLK_i", SDLK_i},
	{"SDLK_j", SDLK_j},
	{"SDLK_k", SDLK_k},
	{"SDLK_l", SDLK_l},
	{"SDLK_m", SDLK_m},
	{"SDLK_n", SDLK_n},
	{"SDLK_o", SDLK_o},
	{"SDLK_p", SDLK_p},
	{"SDLK_q", SDLK_q},
	{"SDLK_r", SDLK_r},
	{"SDLK_s", SDLK_s},
	{"SDLK_t", SDLK_t},
	{"SDLK_u", SDLK_u},
	{"SDLK_v", SDLK_v},
	{"SDLK_w", SDLK_w},
	{"SDLK_x", SDLK_x},
	{"SDLK_y", SDLK_y},
	{"SDLK_z", SDLK_z},
	{"SDLK_DELETE", SDLK_DELETE},
	{"SDLK_WORLD_0", SDLK_WORLD_0},
	{"SDLK_WORLD_1", SDLK_WORLD_1},
	{"SDLK_WORLD_2", SDLK_WORLD_2},
	{"SDLK_WORLD_3", SDLK_WORLD_3},
	{"SDLK_WORLD_4", SDLK_WORLD_4},
	{"SDLK_WORLD_5", SDLK_WORLD_5},
	{"SDLK_WORLD_6", SDLK_WORLD_6},
	{"SDLK_WORLD_7", SDLK_WORLD_7},
	{"SDLK_WORLD_8", SDLK_WORLD_8},
	{"SDLK_WORLD_9", SDLK_WORLD_9},
	{"SDLK_WORLD_10", SDLK_WORLD_10},
	{"SDLK_WORLD_11", SDLK_WORLD_11},
	{"SDLK_WORLD_12", SDLK_WORLD_12},
	{"SDLK_WORLD_13", SDLK_WORLD_13},
	{"SDLK_WORLD_14", SDLK_WORLD_14},
	{"SDLK_WORLD_15", SDLK_WORLD_15},
	{"SDLK_WORLD_16", SDLK_WORLD_16},
	{"SDLK_WORLD_17", SDLK_WORLD_17},
	{"SDLK_WORLD_18", SDLK_WORLD_18},
	{"SDLK_WORLD_19", SDLK_WORLD_19},
	{"SDLK_WORLD_20", SDLK_WORLD_20},
	{"SDLK_WORLD_21", SDLK_WORLD_21},
	{"SDLK_WORLD_22", SDLK_WORLD_22},
	{"SDLK_WORLD_23", SDLK_WORLD_23},
	{"SDLK_WORLD_24", SDLK_WORLD_24},
	{"SDLK_WORLD_25", SDLK_WORLD_25},
	{"SDLK_WORLD_26", SDLK_WORLD_26},
	{"SDLK_WORLD_27", SDLK_WORLD_27},
	{"SDLK_WORLD_28", SDLK_WORLD_28},
	{"SDLK_WORLD_29", SDLK_WORLD_29},
	{"SDLK_WORLD_30", SDLK_WORLD_30},
	{"SDLK_WORLD_31", SDLK_WORLD_31},
	{"SDLK_WORLD_32", SDLK_WORLD_32},
	{"SDLK_WORLD_33", SDLK_WORLD_33},
	{"SDLK_WORLD_34", SDLK_WORLD_34},
	{"SDLK_WORLD_35", SDLK_WORLD_35},
	{"SDLK_WORLD_36", SDLK_WORLD_36},
	{"SDLK_WORLD_37", SDLK_WORLD_37},
	{"SDLK_WORLD_38", SDLK_WORLD_38},
	{"SDLK_WORLD_39", SDLK_WORLD_39},
	{"SDLK_WORLD_40", SDLK_WORLD_40},
	{"SDLK_WORLD_41", SDLK_WORLD_41},
	{"SDLK_WORLD_42", SDLK_WORLD_42},
	{"SDLK_WORLD_43", SDLK_WORLD_43},
	{"SDLK_WORLD_44", SDLK_WORLD_44},
	{"SDLK_WORLD_45", SDLK_WORLD_45},
	{"SDLK_WORLD_46", SDLK_WORLD_46},
	{"SDLK_WORLD_47", SDLK_WORLD_47},
	{"SDLK_WORLD_48", SDLK_WORLD_48},
	{"SDLK_WORLD_49", SDLK_WORLD_49},
	{"SDLK_WORLD_50", SDLK_WORLD_50},
	{"SDLK_WORLD_51", SDLK_WORLD_51},
	{"SDLK_WORLD_52", SDLK_WORLD_52},
	{"SDLK_WORLD_53", SDLK_WORLD_53},
	{"SDLK_WORLD_54", SDLK_WORLD_54},
	{"SDLK_WORLD_55", SDLK_WORLD_55},
	{"SDLK_WORLD_56", SDLK_WORLD_56},
	{"SDLK_WORLD_57", SDLK_WORLD_57},
	{"SDLK_WORLD_58", SDLK_WORLD_58},
	{"SDLK_WORLD_59", SDLK_WORLD_59},
	{"SDLK_WORLD_60", SDLK_WORLD_60},
	{"SDLK_WORLD_61", SDLK_WORLD_61},
	{"SDLK_WORLD_62", SDLK_WORLD_62},
	{"SDLK_WORLD_63", SDLK_WORLD_63},
	{"SDLK_WORLD_64", SDLK_WORLD_64},
	{"SDLK_WORLD_65", SDLK_WORLD_65},
	{"SDLK_WORLD_66", SDLK_WORLD_66},
	{"SDLK_WORLD_67", SDLK_WORLD_67},
	{"SDLK_WORLD_68", SDLK_WORLD_68},
	{"SDLK_WORLD_69", SDLK_WORLD_69},
	{"SDLK_WORLD_70", SDLK_WORLD_70},
	{"SDLK_WORLD_71", SDLK_WORLD_71},
	{"SDLK_WORLD_72", SDLK_WORLD_72},
	{"SDLK_WORLD_73", SDLK_WORLD_73},
	{"SDLK_WORLD_74", SDLK_WORLD_74},
	{"SDLK_WORLD_75", SDLK_WORLD_75},
	{"SDLK_WORLD_76", SDLK_WORLD_76},
	{"SDLK_WORLD_77", SDLK_WORLD_77},
	{"SDLK_WORLD_78", SDLK_WORLD_78},
	{"SDLK_WORLD_79", SDLK_WORLD_79},
	{"SDLK_WORLD_80", SDLK_WORLD_80},
	{"SDLK_WORLD_81", SDLK_WORLD_81},
	{"SDLK_WORLD_82", SDLK_WORLD_82},
	{"SDLK_WORLD_83", SDLK_WORLD_83},
	{"SDLK_WORLD_84", SDLK_WORLD_84},
	{"SDLK_WORLD_85", SDLK_WORLD_85},
	{"SDLK_WORLD_86", SDLK_WORLD_86},
	{"SDLK_WORLD_87", SDLK_WORLD_87},
	{"SDLK_WORLD_88", SDLK_WORLD_88},
	{"SDLK_WORLD_89", SDLK_WORLD_89},
	{"SDLK_WORLD_90", SDLK_WORLD_90},
	{"SDLK_WORLD_91", SDLK_WORLD_91},
	{"SDLK_WORLD_92", SDLK_WORLD_92},
	{"SDLK_WORLD_93", SDLK_WORLD_93},
	{"SDLK_WORLD_94", SDLK_WORLD_94},
	{"SDLK_WORLD_95", SDLK_WORLD_95},
	{"SDLK_KP0", SDLK_KP0},
	{"SDLK_KP1", SDLK_KP1},
	{"SDLK_KP2", SDLK_KP2},
	{"SDLK_KP3", SDLK_KP3},
	{"SDLK_KP4", SDLK_KP4},
	{"SDLK_KP5", SDLK_KP5},
	{"SDLK_KP6", SDLK_KP6},
	{"SDLK_KP7", SDLK_KP7},
	{"SDLK_KP8", SDLK_KP8},
	{"SDLK_KP9", SDLK_KP9},
	{"SDLK_KP_PERIOD", SDLK_KP_PERIOD},
	{"SDLK_KP_DIVIDE", SDLK_KP_DIVIDE},
	{"SDLK_KP_MULTIPLY", SDLK_KP_MULTIPLY},
	{"SDLK_KP_MINUS", SDLK_KP_MINUS},
	{"SDLK_KP_PLUS", SDLK_KP_PLUS},
	{"SDLK_KP_ENTER", SDLK_KP_ENTER},
	{"SDLK_KP_EQUALS", SDLK_KP_EQUALS},
	{"SDLK_UP", SDLK_UP},
	{"SDLK_DOWN", SDLK_DOWN},
	{"SDLK_RIGHT", SDLK_RIGHT},
	{"SDLK_LEFT", SDLK_LEFT},
	{"SDLK_INSERT", SDLK_INSERT},
	{"SDLK_HOME", SDLK_HOME},
	{"SDLK_END", SDLK_END},
	{"SDLK_PAGEUP", SDLK_PAGEUP},
	{"SDLK_PAGEDOWN", SDLK_PAGEDOWN},
	{"SDLK_F1", SDLK_F1},
	{"SDLK_F2", SDLK_F2},
	{"SDLK_F3", SDLK_F3},
	{"SDLK_F4", SDLK_F4},
	{"SDLK_F5", SDLK_F5},
	{"SDLK_F6", SDLK_F6},
	{"SDLK_F7", SDLK_F7},
	{"SDLK_F8", SDLK_F8},
	{"SDLK_F9", SDLK_F9},
	{"SDLK_F10", SDLK_F10},
	{"SDLK_F11", SDLK_F11},
	{"SDLK_F12", SDLK_F12},
	{"SDLK_F13", SDLK_F13},
	{"SDLK_F14", SDLK_F14},
	{"SDLK_F15", SDLK_F15},
	{"SDLK_NUMLOCK", SDLK_NUMLOCK},
	{"SDLK_CAPSLOCK", SDLK_CAPSLOCK},
	{"SDLK_SCROLLOCK", SDLK_SCROLLOCK},
	{"SDLK_RSHIFT", SDLK_RSHIFT},
	{"SDLK_LSHIFT", SDLK_LSHIFT},
	{"SDLK_RCTRL", SDLK_RCTRL},
	{"SDLK_LCTRL", SDLK_LCTRL},
	{"SDLK_RALT", SDLK_RALT},
	{"SDLK_LALT", SDLK_LALT},
	{"SDLK_RMETA", SDLK_RMETA},
	{"SDLK_LMETA", SDLK_LMETA},
	{"SDLK_LSUPER", SDLK_LSUPER},
	{"SDLK_RSUPER", SDLK_RSUPER},
	{"SDLK_MODE", SDLK_MODE},
	{"SDLK_COMPOSE", SDLK_COMPOSE},
	{"SDLK_HELP", SDLK_HELP},
	{"SDLK_PRINT", SDLK_PRINT},
	{"SDLK_SYSREQ", SDLK_SYSREQ},
	{"SDLK_BREAK", SDLK_BREAK},
	{"SDLK_MENU", SDLK_MENU},
	{"SDLK_POWER", SDLK_POWER},
	{"SDLK_EURO", SDLK_EURO},
	{"SDLK_UNDO", SDLK_UNDO},
	{"SDLK_LAST", SDLK_LAST},
};

/**
 * Loads the configuration file. Currently it only configures keyboard input.
 * \param filename Path to configuration file.
 * \return 0
 */
int Config_run(const char *filename) {
	lua_State *L = NULL;
	KeyReg *reg = NULL;
	const char **s = NULL;
	const char *input_fields[] = {
		"up",
		"down",
		"left",
		"right",
		"quit",
		NULL
	};

	L = luaL_newstate();
	luaL_openlibs(L);
	/* register global intergers */
	for (reg = key_regs; reg->name != NULL; reg++) {
		lua_pushnumber(L, reg->n);
		lua_setglobal(L, reg->name);
	}

	if (luaL_dofile(L, filename))
		fprintf(stderr, "No config file: %s\n", filename);
	else {
		lua_getglobal(L, "input");
		if (lua_isnil(L, -1) || !lua_istable(L, -1))
			printf("No table 'input'\n");
		else {
			for (s = input_fields; *s != NULL; s++) {
				int code = 0;
				if (!strcmp(*s, "up"))
					code = EVENT_INPUT_MOVE_UP;
				else if (!strcmp(*s, "down"))
					code = EVENT_INPUT_MOVE_DOWN;
				else if (!strcmp(*s, "left"))
					code = EVENT_INPUT_MOVE_LEFT;
				else if (!strcmp(*s, "right"))
					code = EVENT_INPUT_MOVE_RIGHT;
				else if (!strcmp(*s, "quit"))
					code = EVENT_INPUT_QUIT;
				lua_getfield(L, -1, *s);	/* push '[field]' table */
				if (lua_istable(L, -1)) {
					int i, n = lua_objlen(L, -1);
					for (i = 1; i <= n; i++) {
						InputCode *ic = malloc(sizeof(InputCode));
						lua_pushnumber(L, i);	/* push k */
						lua_gettable(L, -2);	/* replace k with t[k] */
						ic->code = code;
						ic->sym = lua_tonumber(L, -1);
						Event_push(EVENT_CONFIG_BINDKEY, ic, NULL);
						lua_pop(L, 1);	/* pop t[k] */
					}
				}
				lua_pop(L, 1);	/* pop '[field]' table */
			}
		}
		lua_pop(L, 1);			/* pop 'input' table */
	}
	lua_pop(L, 1);
	lua_close(L);
	return 0;
}
