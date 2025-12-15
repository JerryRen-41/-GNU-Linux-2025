AC_INIT([myproject],[0.1])
AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_CONFIG_SRCDIR([src/myprog.c])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CC
AM_PROG_AR
LT_INIT

AC_CHECK_PROGS([MSGFMT],[msgfmt], [no])
AS_IF([test "x$MSGFMT" = "xno"], [AC_MSG_ERROR([msgfmt not found (install gettext)])])

AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.21])

AC_CONFIG_FILES([
  Makefile
  src/Makefile
  po/Makefile
  man/Makefile
])
AC_OUTPUT
