# -------- build config --------
SHELL := /bin/bash
.DEFAULT_GOAL := help
.ONESHELL:

CC      ?= gcc
CSTD    ?= c11
OPT     ?= -O2
WARN    ?= -Wall -Wextra -Wpedantic
CFLAGS  ?= -std=$(CSTD) $(OPT) $(WARN)

SRC      := esub.c
BUILDDIR := build
BIN      := $(BUILDDIR)/esub

# -------- phony targets --------
.PHONY: help build clean test run

help: ## 显示可用目标
	@echo "Targets:"
	@echo "  build   - 编译 $(BIN)"
	@echo "  test    - 运行测试（test.sh）"
	@echo "  run     - 示例运行：日期重排"
	@echo "  clean   - 清理生成物"

$(BUILDDIR):
	@mkdir -p $@

$(BIN): $(SRC) | $(BUILDDIR)
	@echo "[CC] $< -> $@"
	@$(CC) $(CFLAGS) -o $@ $<

build: $(BIN) ## 编译

test: $(BIN) ## 执行测试
	@echo "[TEST] running test.sh"
	@chmod +x ./test.sh
	@ESUB_BIN="$(BIN)" bash ./test.sh

run: $(BIN) ## 演示一次替换
	@$(BIN) '([0-9]+)-([0-9]+)-([0-9]+)' '\3.\2.\1' '2025-11-04'

clean: ## 清理
	@rm -rf $(BUILDDIR)

