AC_INIT([rhasher], [2.0], [nobody@example.com])
AC_CONFIG_SRCDIR([src/rhasher.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_CC

dnl =======================
dnl  查找必需库：librhash
dnl =======================
AC_CHECK_HEADER([rhash.h],
  [],
  [AC_MSG_ERROR([rhash.h not found. Install librhash development package.])])

LIBRHASH_LIBS=""
AC_CHECK_LIB([rhash], [rhash_library_init],
  [LIBRHASH_LIBS="-lrhash"],
  [AC_MSG_ERROR([librhash not found. Install librhash library.])])
AC_SUBST([LIBRHASH_LIBS])

dnl ============================
dnl  可选库：readline 支持开关
dnl ============================
AC_ARG_WITH([readline],
  AS_HELP_STRING([--without-readline], [build without readline even if present]),
  [with_readline="$withval"],
  [with_readline="yes"])

READLINE_LIBS=""
have_readline="no"

if test "x$with_readline" != "xno"; then
  AC_CHECK_HEADER([readline/readline.h],
    [AC_CHECK_LIB([readline], [readline],
       [READLINE_LIBS="-lreadline"
        AC_DEFINE([HAVE_READLINE], [1], [Define if readline is available])
        have_readline="yes"],
       [have_readline="no"])],
    [have_readline="no"])
fi

AM_CONDITIONAL([USE_READLINE], [test "x$have_readline" = "xyes"])
AC_SUBST([READLINE_LIBS])

AC_CHECK_FUNCS([strcasecmp], [], [])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tests/Makefile
])

AC_OUTPUT

