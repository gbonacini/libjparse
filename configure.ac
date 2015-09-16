#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([jconf], m4_esyscmd([ tr -d '\n' < version ]))

AC_CONFIG_SRCDIR([src/parseJsonConfig.c])
AC_CONFIG_SRCDIR([src/json.l])
AC_CONFIG_SRCDIR([src/jsonGram.y])
AC_CONFIG_SRCDIR([test/parseJsonFile.c])

AC_CONFIG_HEADER([include/config.h])

LT_INIT
AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE([disable])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_FILES([Makefile
		src/Makefile
		test/Makefile])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_YACC
AC_PROG_LEX

# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T

# Libs list autmatically generated from dependecy script
# AC_CHECK_LIB([crypto],[BIO_ctrl],[],[AC_MSG_FAILURE([could not find lib crypto])])


AC_CANONICAL_HOST
case $host_os in
  darwin*)
              if test "x$_DEBUG_MEM_" = xyes; then
                        OPTIMIZATION=" -Weverything -O0 -g "
              elif test "x$_DEBUG_MEM_" = xlight; then
                        OPTIMIZATION=" -O0 -g -Weverything -Wno-unused-macros -Wno-documentation \
				       -Wno-padded  "
              else
                        OPTIMIZATION=" -D_FORTIFY_SOURCE=2 -O3 -fstack-protector --param=ssp-buffer-size=4 "
              fi
              if test "x$CC" = xgcc; then
                    CFLAGS=" ${OPTIMIZATION} "
              else
                    CFLAGS=" -O0 -g -Wall "
              fi

        ;;
  linux*)
              OPTIMIZATION=" -O3 -D_FORTIFY_SOURCE=2  -fstack-protector --param=ssp-buffer-size=4 "
              if test "x$_DEBUG_MEM_" = xyes; then
                        OPTIMIZATION=" -O0 -g -Wextra -Wall -Wformat -Werror=format-security  "
              fi
              if test "x$CC" = xgcc; then
                    CFLAGS=" -D_XOPEN_SOURCE ${OPTIMIZATION} "
                    LDFLAGS=" $LDFLAGS -Wl,-z,relro "
              else
                    CFLAGS=" -O0 -g -Wall "
              fi
        ;;
    *)
            AC_MSG_ERROR([Your platform is not currently supported])
        ;;
esac

AC_OUTPUT
