// range.c — 等差序列生成器（带 probe() 以便 GDB 下断点）
// 用法：range N | range M N | range M N S   （S != 0）

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

/* 供 GDB 读取的全局变量（保持与脚本一致） */
int start = 0;
int stop  = 0;
int step  = 1;
int current = 0;
int index   = 0;

/* 空探针：GDB 在此处断点，不再依赖行号 */
__attribute__((noinline)) static void probe(void) { /* for GDB */ }

/* 辅助函数 */
static void show_help(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s N        -> 0,1,...,N-1\n"
        "  %s M N      -> M,...,N-1\n"
        "  %s M N S    -> M, M+S, ... < N  (S != 0)\n",
        prog, prog, prog);
}

static int str_to_i32(const char *s, int *out) {
    char *endp = NULL;
    errno = 0;
    long v = strtol(s, &endp, 10);
    if (errno || endp == s || *endp != '\0') return -1;
    if (v < INT_MIN || v > INT_MAX) return -1;
    *out = (int)v;
    return 0;
}

static int need_emit(int cur, int stp, int lim) {
    return (stp > 0) ? (cur < lim) : (stp < 0 ? (cur > lim) : 0);
}

static void bump(int *cur, int stp) {
    *cur += stp;
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        show_help(argv[0]);
        return 1;
    }

    if (argc == 2) {
        start = 0;
        if (str_to_i32(argv[1], &stop) != 0) {
            fprintf(stderr, "invalid N: %s\n", argv[1]);
            return 1;
        }
        step = 1;
    } else if (argc == 3) {
        if (str_to_i32(argv[1], &start) != 0 ||
            str_to_i32(argv[2], &stop)  != 0) {
            fprintf(stderr, "invalid M or N\n");
            return 1;
        }
        step = 1;
    } else { /* argc == 4 */
        if (str_to_i32(argv[1], &start) != 0 ||
            str_to_i32(argv[2], &stop)  != 0 ||
            str_to_i32(argv[3], &step)  != 0) {
            fprintf(stderr, "invalid M/N/S\n");
            return 1;
        }
        if (step == 0) {
            fprintf(stderr, "step must be non-zero\n");
            return 1;
        }
    }

    current = start;
    index   = 0;

    for (;;) {
        if (!need_emit(current, step, stop)) break;
        probe();                    /* ← GDB 在这里断点，最稳 */
        printf("%d\n", current);
        index++;
        bump(&current, step);
    }
    return 0;
}

