#include <stdio.h>
#include "outlib.h"

int main(int argc, char **argv) {
    /* 无参数：打印用法并退出（早返回） */
    if (argc <= 1) {
        usage((argc > 0 && argv[0]) ? argv[0] : "prog");
        return 0;
    }

    /* 记录参数个数到全局计数器 */
    Count = argc;

    /* 起止标记 */
    output("<INIT>");

    /* 使用指针遍历 argv，避免与传统下标循环同构 */
    for (char **p = argv + 1; *p; ++p) {
        output(*p);
    }

    output("<DONE>");
    return 0;
}

