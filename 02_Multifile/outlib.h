#ifndef _OUTLIB_H_
#define _OUTLIB_H_

/* 对外接口声明 */
void output(const char *text);
void usage(const char *progname);

/* 全局计数器变量（由实现文件提供定义） */
extern int Count;

/* 版本号定义 */
#define VERSION 0.00

#endif /* _OUTLIB_H_ */

