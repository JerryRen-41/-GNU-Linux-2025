CC      = gcc
CFLAGS  = -g -O0 -Wall -Wextra -std=c11

all: range

range: range.c
	$(CC) $(CFLAGS) -o $@ $<

test: range gdb1.gdb gdb2.gdb
	@echo "===== Test 1: ./range 1 12 (current % 5 == 0) ====="
	@gdb --batch --quiet -x gdb1.gdb
	@echo
	@echo "===== Test 2: ./range -100 100 3 (elements 28..35) ====="
	@gdb --batch --quiet -x gdb2.gdb

clean:
	rm -f range

.PHONY: all test clean

