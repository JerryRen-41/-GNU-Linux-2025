# =========================
# Toolchain & flags
# =========================
CC        ?= gcc
WARN       = -Wall -Wextra
OPT        = -O2
PIC        = -fPIC
CFLAGS     = $(WARN) $(OPT) $(PIC)

AR        ?= ar
ARFLAGS    = rcs
RM        ?= rm -f

# =========================
# Sources / Objects
# =========================
SRCS       = fun.c const.c prog.c
OBJS       = $(SRCS:.c=.o)

LIB_SRCS   = fun.c const.c
LIB_OBJS   = $(LIB_SRCS:.c=.o)

# =========================
# Outputs
# =========================
LIB_STATIC = liboutput.a
LIB_SHARED = liboutput.so

BINS       = prog prog-a prog-so

# 默认目标
.DEFAULT_GOAL := build

.PHONY: build all test clean

# 兼容原有 all 目标名
all: build

# 生成静态库、共享库与可执行文件
build: $(LIB_STATIC) $(LIB_SHARED) $(BINS)

# -------------------------
# Libraries
# -------------------------
$(LIB_STATIC): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(LIB_SHARED): $(LIB_OBJS)
	$(CC) -shared -Wl,-soname,$(LIB_SHARED) -o $@ $^

# -------------------------
# Generic compile rule
# -------------------------
%.o: %.c outlib.h
	$(CC) $(CFLAGS) -c $< -o $@

# -------------------------
# Binaries
# -------------------------
# prog / prog-a: 静态库链接
prog: prog.o $(LIB_STATIC)
	$(CC) -o $@ prog.o $(LIB_STATIC)

prog-a: prog.o $(LIB_STATIC)
	$(CC) -o $@ prog.o $(LIB_STATIC)

# prog-so: 共享库链接；运行时从可执行文件目录加载
prog-so: prog.o $(LIB_SHARED)
	$(CC) -o $@ prog.o -L. -loutput -Wl,-rpath,'$$ORIGIN'

# -------------------------
# Tests
# -------------------------
test: build
	@echo "== run without args =="
	@for exe in $(BINS); do ./$$exe > $$exe.noarg.out 2>&1 || true; done
	@echo "== run with one arg =="
	@for exe in $(BINS); do ./$$exe foo > $$exe.one.out 2>&1 || true; done
	@echo "== run with three args =="
	@for exe in $(BINS); do ./$$exe a b c > $$exe.three.out 2>&1 || true; done
	@echo "== compare =="
	@for exe in $(BINS); do \
	  if cmp -s $$exe.noarg.out $$exe.one.out; then \
	    echo "$$exe: noarg vs one OK"; else echo "$$exe: noarg vs one MISMATCH"; fi; \
	  if cmp -s $$exe.noarg.out $$exe.three.out; then \
	    echo "$$exe: noarg vs three OK"; else echo "$$exe: noarg vs three MISMATCH"; fi; \
	done

# -------------------------
# Cleanup
# -------------------------
clean:
	$(RM) $(OBJS) $(LIB_OBJS) $(LIB_STATIC) $(LIB_SHARED) $(BINS) \
	      *.noarg.out *.one.out *.three.out

