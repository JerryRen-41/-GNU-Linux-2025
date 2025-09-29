# 编译器与选项
COMPILER = gcc
CFLAGS = -std=c11 -Wall -Wextra
LIBS = -lncursesw

# 源文件与目标程序
SRC = show.c
OUT = show

# 默认目标
all: $(OUT)

# 编译规则
$(OUT): $(SRC)
	$(COMPILER) $(CFLAGS) -o $(OUT) $(SRC) $(LIBS)

# 清理生成文件
clean:
	-rm -f $(OUT)

