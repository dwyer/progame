ACLOCAL_AMFLAGS = -I m4

# Makefile.am should be eventually placed directly into source directory
# (Specifying sources with "blah.c" instead of "../src/blah.c" for example)

bin_PROGRAMS =

bin_PROGRAMS += game
game_SOURCES = ../src/config.c ../src/input.c ../src/script.c ../src/entity.c ../src/main.c ../src/tmx.c ../src/event.c	../src/player.c ../src/world.c
game_CFLAGS = ${SDL_CFLAGS} ${LUA_CFLAGS}
game_LDADD = ${SDL_LIBS} ${LUA_LIBS}

bin_PROGRAMS += lua_cross_yield51
lua_cross_yield51_SOURCES = ../src/test/lua_cross_yield51.c
lua_cross_yield51_CFLAGS = ${LUA_CFLAGS}
lua_cross_yield51_LDADD = ${LUA_LIBS} -lm

bin_PROGRAMS += lua_cross_yield52
lua_cross_yield52_SOURCES = ../src/test/lua_cross_yield52.c
lua_cross_yield52_CFLAGS = ${LUA52_CFLAGS}
lua_cross_yield52_LDADD = ${LUA52_LIBS} -lm
