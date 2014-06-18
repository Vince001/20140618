#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([Camera], [V1.1], [xcc001@139.com])

AM_INIT_AUTOMAKE(foreign -Wall -Werror)

AC_CONFIG_SRCDIR([camera.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

# CPP compile flags
CPPFLAGS="$CPPFLAGS -I /usr/include -L /usr/lib"

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset select strdup strerror])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
