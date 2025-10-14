#include <stdio.h>
#include <stdlib.h>

static void help(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s N          -> 0,1,...,N-1\n"
        "  %s M N        -> M,...,N-1\n"
        "  %s M N S      -> M, M+S, ... , < N\n",
        prog, prog, prog);
}

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 4) {
        help(argv[0]);
        return 1;
    }

    int start, stop, step;
    if (argc == 2) {
        start = 0;
        stop  = atoi(argv[1]);
        step  = 1;
    } else if (argc == 3) {
        start = atoi(argv[1]);
        stop  = atoi(argv[2]);
        step  = 1;
    } else { // argc == 4
        start = atoi(argv[1]);
        stop  = atoi(argv[2]);
        step  = atoi(argv[3]);
        if (step == 0) {
            fprintf(stderr, "step must be non-zero\n");
            return 1;
        }
    }

    int current = start;   // ← GDB 脚本里要用到的变量名
    int index   = 0;       // 可用于需要时做第几项的统计（本题第二脚本用不到它）

    while ((step > 0 && current < stop) || (step < 0 && current > stop)) {
        printf("%d\n", current);
        current += step;
        index++;
    }
    (void)index; // 防未使用告警
    return 0;
}

