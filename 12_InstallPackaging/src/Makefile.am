AM_CPPFLAGS = -I$(top_srcdir)/src -DLOCALEDIR=\"$(localedir)\"

lib_LTLIBRARIES = libmylib.la
libmylib_la_SOURCES = libmylib.c libmylib.h
libmylib_la_LDFLAGS = -version-info 0:0:0

bin_PROGRAMS = myprog
myprog_SOURCES = myprog.c
myprog_LDADD = libmylib.la

check_PROGRAMS = test_basic
TESTS = test_basic
test_basic_SOURCES = test_basic.c
test_basic_LDADD = libmylib.la
