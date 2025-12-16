#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

typedef int (*unlink_func_t)(const char *);
typedef int (*unlinkat_func_t)(int, const char *, int);

static unlink_func_t   real_unlink   = NULL;
static unlinkat_func_t real_unlinkat = NULL;

static void init_unlink_symbols(void)
{
    if (!real_unlink) {
        real_unlink = (unlink_func_t)dlsym(RTLD_NEXT, "unlink");
        if (!real_unlink) {
            fprintf(stderr, "[protect] cannot resolve unlink: %s\n", dlerror());
            abort();
        }
    }
    if (!real_unlinkat) {
        real_unlinkat = (unlinkat_func_t)dlsym(RTLD_NEXT, "unlinkat");
        if (!real_unlinkat) {
            fprintf(stderr, "[protect] cannot resolve unlinkat: %s\n", dlerror());
            abort();
        }
    }
}

static int should_block(const char *path)
{
    return strstr(path, "PROTECT") != NULL;
}

int unlink(const char *pathname)
{
    init_unlink_symbols();

    if (should_block(pathname)) {
        fprintf(stderr,
                "[protect] deny unlink(\"%s\"): contains \"PROTECT\"\n",
                pathname);
        errno = EPERM;
        return -1;
    }

    return real_unlink(pathname);
}

int unlinkat(int dirfd, const char *pathname, int flags)
{
    init_unlink_symbols();

    if (should_block(pathname)) {
        fprintf(stderr,
                "[protect] deny unlinkat(%d, \"%s\", 0x%x): contains \"PROTECT\"\n",
                dirfd, pathname, flags);
        errno = EPERM;
        return -1;
    }

    return real_unlinkat(dirfd, pathname, flags);
}

