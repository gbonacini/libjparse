#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([jconfCpp], m4_esyscmd([ tr -d '\n' < version ]))

CXXFLAGS=""
CFLAGS=""
CPPFLAGS=""

AC_ARG_WITH([test],
	[AS_HELP_STRING([    --with-test], [Test mode On])],
	[],
	[with_test=no])

AS_IF([test "x$with_test" != xno],
        [
        AM_CONDITIONAL(WITH_TEST, true)
        AC_DEFINE(WITH_TEST,[],[Use Test Mode])
        ], [
        AM_CONDITIONAL(WITH_TEST, false)
        ])

AC_CONFIG_SRCDIR([src/jsonCppWrap.cpp])

AC_CONFIG_HEADER([include/config.h])

LT_INIT
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([disable])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile
		src/Makefile
		test/Makefile])

# Checks for programs.
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

AC_CANONICAL_HOST
case $host_os in
  darwin*)
              if test "x$_DEBUG_MEM_" = xyes; then
                        OPTIMIZATION=" -O0 -g "
              elif test "x$_DEBUG_MEM_" = xlight; then
                        OPTIMIZATION=" -O0 -g -Weverything -Wno-padded -Wno-c++98-compat "
              else
                        OPTIMIZATION=" -D_FORTIFY_SOURCE=2 -O3 -fstack-protector --param=ssp-buffer-size=4 "
              fi

              if test "x$CC" = xgcc; then
                    CPPFLAGS=" -std=c++11 ${OPTIMIZATION} "
              else
                    CPPFLAGS=" -O0 -g -Wall "
              fi

        ;;
  linux*)
              OPTIMIZATION=" -O3 -D_FORTIFY_SOURCE=2  -fstack-protector --param=ssp-buffer-size=4 "
              if test "x$_DEBUG_MEM_" = xyes; then
                        OPTIMIZATION=" -O0 -g "
              fi

              if test "x$CC" = xgcc; then
                    CPPFLAGS=" -std=c++11 -Wextra -Wall -Wformat -Werror=format-security \
                             -D_XOPEN_SOURCE\
                              ${OPTIMIZATION} "
                    LDFLAGS=" $LDFLAGS -Wl,-z,relro "
              else
                    CPPFLAGS=" -O0 -g -Wall "
              fi
        ;;
    *)
            AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac

AC_OUTPUT
