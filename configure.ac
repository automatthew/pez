#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)

AC_INIT([pez],[1.5.2],[pete.elmore@gmail.com])
AC_CONFIG_HEADERS([config.h:config.h.in])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([pez.c])

# AM_INIT_AUTOMAKE(pez, 1.5.2)
AM_INIT_AUTOMAKE

AC_PROG_LIBTOOL

AC_CANONICAL_HOST

# Checks for programs.
AC_PROG_CC
AC_PROG_MAKE_SET

AC_CHECK_PROG([RUBY], [ruby], 1, 0)
AC_CHECK_PROG([RLWRAP], [rlwrap], rlwrap,)

# Checks for libraries.
AC_CHECK_LIB([m], [log], [],
	     [echo "Missing the standard C math library, -lm" >&2; exit 1])
AC_CHECK_LIB([dl], [dlopen], [],
	     [echo "Missing the dynamic library loading library, -ldl" >&2;
	      exit 1])
AC_CHECK_LIB([gc], [GC_malloc], [],
	     [echo "Missing the Boehm GC library, -lgc" >&2; exit 1])

# Checks for header files.
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(
	[memory.h stdlib.h string.h sys/time.h unistd.h limits.h gc/gc.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_TIME
AC_STRUCT_TM

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_TYPE_SIGNAL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([gettimeofday pow setenv strchr strtoul])

AC_DEFINE([EXPORT], [1], [TODO: These are going away.])
AC_DEFINE([READONLYSTRINGS], [1], [TODO: These are going away.])
AC_DEFINE([MEMSTAT], [1], [TODO: These are going away.])

AC_PROG_INSTALL

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
# AC_CONFIG_FILES([Makefile lib/Makefile doc/Makefile])
# AC_OUTPUT
