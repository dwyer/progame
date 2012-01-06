ACLOCAL_AMFLAGS = -I m4

# Makefile.am should be eventually placed directly into source directory
# (Specifying sources with "blah.c" instead of "../src/blah.c" for example)

bin_PROGRAMS = game
game_SOURCES = ../src/config.c ../src/input.c ../src/script.c ../src/entity.c ../src/main.c ../src/tmx.c ../src/event.c	../src/player.c ../src/world.c
game_CFLAGS = ${SDL_CFLAGS} ${LUA_CFLAGS}
game_LDADD = ${SDL_LIBS} ${LUA_LIBS}
