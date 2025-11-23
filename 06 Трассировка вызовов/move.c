#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_LEN 8192


enum {
    ERR_USAGE      = 1,
    ERR_OPEN_IN    = 2,
    ERR_FSTAT_IN   = 3,
    ERR_OPEN_OUT   = 4,
    ERR_READ       = 5,
    ERR_WRITE      = 6,
    ERR_FSYNC_OUT  = 7,
    ERR_CLOSE_IN   = 8,
    ERR_CLOSE_OUT  = 9,
    ERR_UNLINK_IN  = 10,
    ERR_SAME_PATH  = 11
};

static int fail_with_errno(const char *ctx, int code)
{
    perror(ctx);
    return code;
}

static int safe_unlink_out(const char *outpath, int base_code)
{
    int saved = errno;   
    if (unlink(outpath) == -1) {
        perror("unlink(outfile)");
    }
    errno = saved;
    return base_code;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s infile outfile\n", argv[0]);
        return ERR_USAGE;
    }

    const char *inpath  = argv[1];
    const char *outpath = argv[2];


    if (strcmp(inpath, outpath) == 0) {
        fprintf(stderr, "infile and outfile must be different\n");
        return ERR_SAME_PATH;
    }

    int fd_in  = -1;
    int fd_out = -1;
    char buf[BUF_LEN];
    ssize_t nread;

    struct stat st_in;

    fd_in = open(inpath, O_RDONLY);
    if (fd_in == -1) {
        return fail_with_errno(inpath, ERR_OPEN_IN);
    }

    if (fstat(fd_in, &st_in) == -1) {
        int rc = fail_with_errno("fstat(infile)", ERR_FSTAT_IN);
        close(fd_in);
        return rc;
    }


    mode_t mode = st_in.st_mode & 0777;

    fd_out = open(outpath,
                  O_WRONLY | O_CREAT | O_TRUNC,
                  mode);
    if (fd_out == -1) {
        int rc = fail_with_errno(outpath, ERR_OPEN_OUT);
        close(fd_in);
        return rc;
    }


    while ((nread = read(fd_in, buf, sizeof(buf))) > 0) {
        ssize_t written = 0;
        while (written < nread) {
            ssize_t n = write(fd_out, buf + written, (size_t)(nread - written));
            if (n == -1) {
                int rc = fail_with_errno("write", ERR_WRITE);
                close(fd_in);
                close(fd_out);
                return safe_unlink_out(outpath, rc);
            }
            written += n;
        }
    }

    if (nread == -1) {
        int rc = fail_with_errno("read", ERR_READ);
        close(fd_in);
        close(fd_out);
        return safe_unlink_out(outpath, rc);
    }


    if (fsync(fd_out) == -1) {
        int rc = fail_with_errno("fsync", ERR_FSYNC_OUT);
        close(fd_in);
        close(fd_out);
        return safe_unlink_out(outpath, rc);
    }

    if (close(fd_in) == -1) {
        return fail_with_errno("close(infile)", ERR_CLOSE_IN);
    }

    if (close(fd_out) == -1) {
        int rc = fail_with_errno("close(outfile)", ERR_CLOSE_OUT);
        return rc;
    }


    if (unlink(inpath) == -1) {
        return fail_with_errno("unlink(infile)", ERR_UNLINK_IN);
    }

    return 0;
}

