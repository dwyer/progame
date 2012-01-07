AC_INIT([Progame], [0.1], [progame@desu.no-ip.info])
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


######
# WITH_DEBUG option handling
######

want_debug=
AC_ARG_WITH([debug],
			[AS_HELP_STRING([--with-debug],
							[use debug flags])],
			[want_debug=yes],
			[want_debug=no])

# Conditionally check whether --want-debug was specified
AM_CONDITIONAL([DEBUG],
			   [test "x$want_debug" = xyes])
AS_IF([test "x$want_debug" = xyes],
	  [AM_CFLAGS="-g -O0"
	   AM_CPPFLAGS="-DDEBUG"],
	  [])


# If --with-blah was not given on command line at all, assume want to check.
# If --with-blah was given, test whether it was --with-blah=no,
# in that case disable, otherwise enable.

######
# WITH_ASSIMP option handling
######

want_assimp=
AC_ARG_WITH([assimp],
			[AS_HELP_STRING([--with-assimp],
							[use assimp])],
			[AS_IF([test "x$with_assimp" = xno],
				   [want_assimp=no],
				   [want_assimp=yes])],
			[want_assimp=check])

AS_IF([test "x$want_assimp" = check],
	  [PKG_CHECK_EXISTS([assimp],
						[want_assimp=yes],
						[want_assimp=no])])

AS_IF([test "x$want_assimp" = xyes],
	  [PKG_CHECK_MODULES([ASSIMP], [assimp])],
	  [])

AM_CONDITIONAL([WANT_ASSIMP],
			   [test "x$want_assimp" = xyes])

######
# WITH_LUA52 option handling
######

want_lua52=
AC_ARG_WITH([lua52],
			[AS_HELP_STRING([--with-lua52],
							[use lua52])],
			[AS_IF([test "x$with_lua52" = xno],
				   [want_lua52=no],
				   [want_lua52=yes])],
			[want_lua52=check])

AS_IF([test "x$want_lua52" = check],
	  [PKG_CHECK_EXISTS([lua52],
						[want_lua52=yes],
						[want_lua52=no])])

AS_IF([test "x$want_lua52" = xyes],
	  [PKG_CHECK_MODULES([LUA52], [lua52])],
	  [])

AM_CONDITIONAL([WANT_LUA52],
			   [test "x$want_lua52" = xyes])


# Check for SDL
# Requires sdl.m4, was probably shipped with SDL install
AM_PATH_SDL([],
			[],
			[AC_MSG_ERROR([SDL NOT FOUND])])

# Check for lua
PKG_CHECK_MODULES([LUA], [lua])


AC_CONFIG_FILES([Makefile])

AC_OUTPUT
