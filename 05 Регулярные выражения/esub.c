#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define MAX_MATCHES     10     // pm[0] + 9 组
#define MAX_BACKREFS    100
#define MSG_BUF         256

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <regexp> <substitution> <string>\n", prog);
}

static void print_regerr(const char *where, int code, const regex_t *re) {
    char buf[MSG_BUF];
    regerror(code, re, buf, sizeof(buf));
    fprintf(stderr, "esub: %s: %s\n", where, buf);
}

/* 扫描替换串：
 * - 统计反向引用次数（>MAX_BACKREFS 报错）
 * - 计算最终输出长度（仅估算，不写入）
 * - 规则：
 *    \\  -> 字面 '\'
 *    \1..\9 -> 若 idx > nsub 报错；若本次未匹配 -> 空串；否则追加该组长度
 *    其他 \X -> 当作字面 X
 */
static int estimate_size(const char *repl,
                         const char *input,
                         const regmatch_t pm[],
                         size_t nsub,
                         size_t *out_len,
                         int *out_refcnt)
{
    size_t total = 0;
    int refcnt = 0;

    for (size_t i = 0; repl[i]; ++i) {
        char c = repl[i];
        if (c != '\\') {
            ++total;
            continue;
        }
        char nxt = repl[++i];
        if (nxt == '\0') {         // 尾部单个 '\'
            ++total;
            break;
        }
        if (nxt == '\\') {         // 转义反斜杠
            ++total;
            continue;
        }
        if (nxt >= '0' && nxt <= '9') {
            int idx = nxt - '0';
            ++refcnt;
            if (refcnt > MAX_BACKREFS) return -3; // 引用过多
            if ((size_t)idx > nsub) return -2;    // 引用不存在的分组
            if (idx == 0) {
                // 本作业仅 1..9；这里把 \0 当作字面 '0'
                ++total;            // 等价于写入 '0'
                continue;
            }
            // 未匹配组 -> 空串
            if (pm[idx].rm_so >= 0 && pm[idx].rm_eo >= 0) {
                total += (size_t)(pm[idx].rm_eo - pm[idx].rm_so);
            }
            continue;
        }
        // 其他转义：当作字面
        ++total;
    }

    if (out_len)    *out_len   = total;
    if (out_refcnt) *out_refcnt = refcnt;
    return 0;
}

/* 第二遍：按同样规则把内容真正写入到 dst 中 */
static void materialize(char *dst,
                        const char *repl,
                        const char *input,
                        const regmatch_t pm[],
                        size_t nsub)
{
    size_t w = 0;

    for (size_t i = 0; repl[i]; ++i) {
        char c = repl[i];
        if (c != '\\') {
            dst[w++] = c;
            continue;
        }
        char nxt = repl[++i];
        if (nxt == '\0') {
            dst[w++] = '\\';
            break;
        }
        if (nxt == '\\') {
            dst[w++] = '\\';
            continue;
        }
        if (nxt >= '0' && nxt <= '9') {
            int idx = nxt - '0';
            if (idx == 0) {         // 与上面估算保持一致：把 \0 当作字面 '0'
                dst[w++] = '0';
                continue;
            }
            if (idx <= 9 && pm[idx].rm_so >= 0 && pm[idx].rm_eo >= 0) {
                size_t len = (size_t)(pm[idx].rm_eo - pm[idx].rm_so);
                memcpy(dst + w, input + pm[idx].rm_so, len);
                w += len;
            }
            continue;
        }
        dst[w++] = nxt;
    }
    dst[w] = '\0';
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage(argv[0]);
        return 2;
    }

    const char *pat  = argv[1];
    const char *repl = argv[2];
    const char *src  = argv[3];

    regex_t re;
    int rc = regcomp(&re, pat, REG_EXTENDED);
    if (rc != 0) {
        print_regerr("regcomp", rc, &re);
        regfree(&re);  // regerror 可能要 re 的上下文
        return 2;
    }

    regmatch_t pm[MAX_MATCHES];
    rc = regexec(&re, src, MAX_MATCHES, pm, 0);
    if (rc == REG_NOMATCH) {
        // 单次替换：无匹配 -> 原样打印
        puts(src);
        regfree(&re);
        return 0;
    }
    if (rc != 0) {
        print_regerr("regexec", rc, &re);
        regfree(&re);
        return 2;
    }

    // 捕获组数量（不含 pm[0]），作业只允许 \1..\9
    size_t nsub = re.re_nsub;
    if (nsub > 9) nsub = 9;

    // 输出 = 前缀 + 替换展开 + 后缀
    size_t prefix_len = (size_t)pm[0].rm_so;
    size_t suffix_len = strlen(src) - (size_t)pm[0].rm_eo;

    size_t repl_len_est = 0;
    int backrefs = 0;
    int est = estimate_size(repl, src, pm, nsub, &repl_len_est, &backrefs);
    if (est == -2) {
        fprintf(stderr, "esub: backreference refers to non-existent group\n");
        regfree(&re);
        return 2;
    } else if (est == -3) {
        fprintf(stderr, "esub: too many backreferences (> %d)\n", MAX_BACKREFS);
        regfree(&re);
        return 2;
    }

    size_t out_len = prefix_len + repl_len_est + suffix_len;
    char *out = (char*)malloc(out_len + 1);
    if (!out) {
        perror("malloc");
        regfree(&re);
        return 2;
    }

    // 写前缀
    memcpy(out, src, prefix_len);
    out[prefix_len] = '\0';

    // 写替换展开
    materialize(out + prefix_len, repl, src, pm, nsub);

    // 写后缀
    size_t w = strlen(out);
    memcpy(out + w, src + pm[0].rm_eo, suffix_len);
    out[w + suffix_len] = '\0';

    puts(out);

    free(out);
    regfree(&re);
    return 0;
}

