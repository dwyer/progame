AC_INIT([Progame], [0.1], [Herp@Derp])
AC_CONFIG_SRCDIR([../src/main.c])
AC_CONFIG_AUX_DIR([m4])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign silent-rules])

LT_INIT()
LT_LANG([C])

# Requires pkg.m4, was probably shipped with pkg-config install
PKG_PROG_PKG_CONFIG

want_lua52=
AC_ARG_WITH([lua52],
  [AS_HELP_STRING([--with-lua52],
    [use lua52])],
  [want_lua52=yes],
  [want_lua52=no])

# Check for SDL
# Requires sdl.m4, was probably shipped with SDL install
AM_PATH_SDL([],
		[],
		[AC_MSG_ERROR([SDL NOT FOUND])])

# Check for lua
PKG_CHECK_MODULES([LUA], [lua])

# Conditionally check for lua 5.2 Beta, if --with-lua52 was specified
AS_IF([test "x$want_lua52" != xno],
			[PKG_CHECK_MODULES([LUA52], [lua52])],
			[])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
