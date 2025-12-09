AC_INIT([mbuf], [1.0], [you@example.com])
AC_CONFIG_SRCDIR([src/mbuf.c])
AC_CONFIG_HEADERS([config.h])

# Automake 设置：不要再加 -Werror，避免警告变成错误
AM_INIT_AUTOMAKE([foreign -Wall])

# 检查 C 编译器
AC_PROG_CC

# 关键：为了让 libtool 使用合适的 archiver（修复你现在的报错）
AM_PROG_AR

# libtool 初始化
LT_INIT

# 检查 check 单元测试库
PKG_CHECK_MODULES([CHECK], [check >= 0.9.10])

# 覆盖率选项
AC_ARG_ENABLE([coverage],
  AS_HELP_STRING([--enable-coverage], [Enable gcov coverage]),
  [enable_coverage=yes],
  [enable_coverage=no]
)

if test "x$enable_coverage" = "xyes"; then
  CFLAGS="$CFLAGS -fprofile-arcs -ftest-coverage"
  LDFLAGS="$LDFLAGS -fprofile-arcs -ftest-coverage"
fi

# 这里用“推荐的新写法”：先声明要生成哪些文件
AC_CONFIG_FILES([
  Makefile
  src/Makefile
  tests/Makefile
])

# 然后再调用 AC_OUTPUT（不带参数）
AC_OUTPUT

