AC_INIT([Progame], [0.1], [Herp@Derp])
AC_CONFIG_SRCDIR([../src/main.c])
AC_CONFIG_AUX_DIR([m4])
AC_CONFIG_MACRO_DIR([m4])

LT_INIT()
LT_LANG([C])

AM_INIT_AUTOMAKE([foreign silent-rules])

AC_SUBST([AM_CFLAGS])
AC_SUBST([AM_CPPFLAGS])

# If any flags were autodetected by AC_INIT / LT_INIT / AM_INIT_AUTOMAKE
# discard them.

CFLAGS=
CPPFLAGS=

# Requires pkg.m4, was probably shipped with pkg-config install
PKG_PROG_PKG_CONFIG

want_debug=
AC_ARG_WITH([debug],
			[AS_HELP_STRING([--with-debug],
							[use debug flags])],
			[want_debug=yes],
			[want_debug=no])

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

# Conditionally check whether --want-debug was specified
AM_CONDITIONAL([DEBUG],
			   [test "x$want_debug" = xyes])
AS_IF([test "x$want_debug" = xyes],
	  [AM_CFLAGS="-g -O0"
	   AM_CPPFLAGS="-DDEBUG"],
	  [])

# Conditionally check for lua 5.2 Beta, if --with-lua52 was specified
AM_CONDITIONAL([WANT_LUA52],
			   [test "x$want_lua52" = xyes])
AS_IF([test "x$want_lua52" != xno],
	  [PKG_CHECK_MODULES([LUA52], [lua52])],
	  [])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
