CCOMP  = gcc
FLAGS  = -O0 -g -std=c11 -Wall -Wextra
BIN    = range

all: $(BIN)

$(BIN): range.c
	$(CCOMP) $(FLAGS) $< -o $@

test: $(BIN) gdb1.gdb gdb2.gdb
	@echo "▶ Test 1: current % 5 == 0"
	@gdb -q --batch -x gdb1.gdb
	@echo
	@echo "▶ Test 2: elements 28..35"
	@gdb -q --batch -x gdb2.gdb

clean:
	@rm -f $(BIN)

.PHONY: all test clean

