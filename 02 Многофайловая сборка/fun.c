#include <stdio.h>
#include "outlib.h"

/* 打印带序号的字符串；若 text 为 NULL 则打印空串 */
void output(const char *text) {
    const char *msg = (text ? text : "");
    printf("%d: %s\n", Count, msg);
    ++Count;
}

/* 打印用法提示与版本信息 */
void usage(const char *progname) {
    const char *name = (progname ? progname : "prog");
    fprintf(stderr,
            "%s (version %.2f)\n"
            "Usage:\n"
            "  %s <arg1> [arg2 ...]\n",
            name, VERSION, name);
}

