CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
TARGET  = move
LIBRARY = protect.so

all: $(TARGET) $(LIBRARY)

$(TARGET): move.c
	$(CC) $(CFLAGS) -o $(TARGET) move.c

$(LIBRARY): protect.c
	$(CC) $(CFLAGS) -fPIC -shared -o $(LIBRARY) protect.c -ldl

test: $(TARGET) $(LIBRARY)
	@echo "=== Test 1: normal move (no outfile) ==="
	@echo "hello world" > in1.txt
	@rm -f out1.txt
	@./$(TARGET) in1.txt out1.txt && echo "  [OK] move exited with 0" || echo "  [FAIL] move returned error"
	@test ! -f in1.txt && echo "  [OK] source removed" || echo "  [FAIL] source still exists"
	@test -f out1.txt && grep -q "hello world" out1.txt && echo "  [OK] data copied" || echo "  [FAIL] data mismatch or no outfile"
	@echo

	@echo "=== Test 2: destination already exists (should be overwritten) ==="
	@echo "old" > out2.txt
	@echo "new content" > in2.txt
	@./$(TARGET) in2.txt out2.txt && echo "  [OK] move succeeded" || echo "  [FAIL] move failed unexpectedly"
	@test ! -f in2.txt && echo "  [OK] source removed" || echo "  [FAIL] source still exists"
	@test -f out2.txt && grep -q "new content" out2.txt && echo "  [OK] dest overwritten" || echo "  [FAIL] dest not overwritten"
	@echo

	@echo "=== Test 3: source does not exist ==="
	@rm -f no_such_file.txt out3.txt
	@./$(TARGET) no_such_file.txt out3.txt 2>/dev/null && echo "  [FAIL] move unexpectedly succeeded" || echo "  [OK] move failed as expected"
	@test ! -f out3.txt && echo "  [OK] no outfile was created" || echo "  [FAIL] outfile should not exist"
	@echo

	@echo "=== Test 4: strace error injection on write() ==="
	@echo "fault-test" > in4.txt
	@rm -f out4.txt
	@if strace -o /dev/null -e fault=write:error=EIO:when=1 ./$(TARGET) in4.txt out4.txt 2>/dev/null; then \
	  echo "  [FAIL] move succeeded, but write was supposed to fail"; \
	else \
	  echo "  [OK] move failed under injected write error"; \
	fi
	@test -f in4.txt && echo "  [OK] source preserved after error" || echo "  [FAIL] source removed unexpectedly"
	@test ! -f out4.txt && echo "  [OK] broken outfile removed" || echo "  [FAIL] outfile should not remain"
	@echo

	@echo "=== Test 5: LD_PRELOAD protect PROTECT files ==="
	@echo "secret" > file_PROTECT_data.txt
	@echo "normal" > normal.txt
	@LD_PRELOAD=./$(LIBRARY) ./$(TARGET) file_PROTECT_data.txt moved_protect.txt 2>/dev/null && \
		echo "  [WARN] move succeeded (expected failure, check logic)" || \
		echo "  [OK] move failed for PROTECT file"
	@test -f file_PROTECT_data.txt && echo "  [OK] PROTECT file still exists" || echo "  [FAIL] PROTECT file was removed"
	@LD_PRELOAD=./$(LIBRARY) ./$(TARGET) normal.txt moved_normal.txt && echo "  [OK] normal file moved under LD_PRELOAD" || echo "  [FAIL] move failed for normal file"
	@if [ ! -f normal.txt ] && [ -f moved_normal.txt ]; then \
		echo "  [OK] normal move semantics preserved"; \
	else \
		echo "  [FAIL] normal move did not behave correctly"; \
	fi
	@echo

clean:
	rm -f $(TARGET) $(LIBRARY) *.o \
	      in1.txt out1.txt \
	      in2.txt out2.txt \
	      in4.txt out4.txt \
	      file_PROTECT_data.txt moved_protect.txt \
	      normal.txt moved_normal.txt \
	      no_such_file.txt out3.txt

.PHONY: all test clean

