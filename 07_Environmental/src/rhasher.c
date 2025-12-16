#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <rhash.h>

#ifdef HAVE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define MAX_LINE_LEN 1024


typedef struct {
    const char *canon_name;   
    unsigned int algo_id;     
} algo_desc_t;

static const algo_desc_t g_algos[] = {
    { "MD5",  RHASH_MD5  },
    { "SHA1", RHASH_SHA1 },
    { "TTH",  RHASH_TTH  }
};
static const int g_algo_count = (int)(sizeof(g_algos) / sizeof(g_algos[0]));


static int lookup_algo(const char *name, unsigned int *algo_out) {
    if (!name || !algo_out) return 0;

    char tmp[64];
    size_t n = strlen(name);
    if (n >= sizeof(tmp)) n = sizeof(tmp) - 1;
    for (size_t i = 0; i < n; ++i) {
        tmp[i] = (char)toupper((unsigned char)name[i]);
    }
    tmp[n] = '\0';

    for (int i = 0; i < g_algo_count; ++i) {
        if (strcmp(tmp, g_algos[i].canon_name) == 0) {
            *algo_out = g_algos[i].algo_id;
            return 1;
        }
    }
    return 0;
}

static int use_hex_output(const char *name) {
    if (!name || !name[0]) return 1; 
    return isupper((unsigned char)name[0]) ? 1 : 0;
}

static void format_digest(unsigned int algo,
                          const unsigned char *digest,
                          int hex_not_base64,
                          char *out_buf,
                          size_t out_cap)
{
    if (!out_buf || out_cap == 0) return;

    int size = rhash_get_digest_size(algo);
    if (size <= 0) {
        snprintf(out_buf, out_cap, "digest size error");
        return;
    }

    int flags = hex_not_base64 ? RHPR_HEX : RHPR_BASE64;
    rhash_print_bytes(out_buf, digest, size, flags);
}

static int compute_hash_of_string(unsigned int algo,
                                  const char *str,
                                  int hex_not_base64,
                                  char *out_buf,
                                  size_t out_cap)
{
    unsigned char digest[64];
    if (!str) return -1;

    if (rhash_msg(algo, str, strlen(str), digest) < 0) {
        return -1;
    }
    format_digest(algo, digest, hex_not_base64, out_buf, out_cap);
    return 0;
}

static int compute_hash_of_file(unsigned int algo,
                                const char *fname,
                                int hex_not_base64,
                                char *out_buf,
                                size_t out_cap)
{
    unsigned char digest[64];
    if (!fname) return -1;

    if (rhash_file(algo, fname, digest) < 0) {
        return -1;
    }
    format_digest(algo, digest, hex_not_base64, out_buf, out_cap);
    return 0;
}

static char* trim_spaces(char *s) {
    if (!s) return s;
    while (*s && isspace((unsigned char)*s)) s++;
    if (*s == '\0') return s;
    char *end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) {
        *end = '\0';
        --end;
    }
    return s;
}

static void handle_command(const char *algo_token, const char *rest) {
    if (!algo_token || !rest) return;

    unsigned int algo_id = 0;
    if (!lookup_algo(algo_token, &algo_id)) {
        fprintf(stderr, "Unknown algorithm: %s\n", algo_token);
        return;
    }

    int hex_not_base64 = use_hex_output(algo_token);

    char buf[MAX_LINE_LEN];
    strncpy(buf, rest, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';

    char *input = trim_spaces(buf);
    if (*input == '\0') {
        fprintf(stderr, "Missing input for algorithm: %s\n", algo_token);
        return;
    }

    int as_string = 0;
    char *payload = input;

    if (*payload == '"') {
        as_string = 1;
        payload++;  

        char *p_end = payload + strlen(payload) - 1;
        if (p_end >= payload && *p_end == '"') {
            *p_end = '\0';
        }
    }

    char result[256];

    int rc;
    if (as_string) {
        rc = compute_hash_of_string(algo_id, payload,
                                    hex_not_base64,
                                    result, sizeof(result));
        if (rc != 0) {
            fprintf(stderr, "Error hashing string\n");
            return;
        }
    } else {
        rc = compute_hash_of_file(algo_id, payload,
                                  hex_not_base64,
                                  result, sizeof(result));
        if (rc != 0) {
            fprintf(stderr, "Error reading file: %s\n", payload);
            return;
        }
    }

    printf("%s\n", result);
}

static void parse_and_execute_line(const char *line) {
    if (!line) return;

    const char *p = line;
    while (*p && isspace((unsigned char)*p)) p++;
    if (*p == '\0' || *p == '#') return;

    char tmp[MAX_LINE_LEN];
    strncpy(tmp, p, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char *algo = strtok(tmp, " \t");
    if (!algo) return;

    char *rest = strtok(NULL, "");
    if (!rest) {
        fprintf(stderr, "Missing input for algorithm: %s\n", algo);
        return;
    }

    handle_command(algo, rest);
}

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage:\n"
        "  %s <ALGO> <input>\n"
        "  %s           # interactive mode\n\n"
        "ALGO:\n"
        "  MD5, SHA1, TTH  (uppercase → HEX)\n"
        "  md5, sha1, tth  (lowercase → Base64)\n\n"
        "Input:\n"
        "  \"text\"   - string (starts with \")\n"
        "  file.txt  - file name\n\n",
        prog, prog);
}

int main(int argc, char *argv[]) {
    rhash_library_init();

    if (argc > 1) {
        if (argc < 3) {
            print_usage(argv[0]);
            return 1;
        }
        handle_command(argv[1], argv[2]);
        return 0;
    }

#ifdef HAVE_READLINE
    printf("RHash calculator (type 'exit' or 'quit' to leave)\n");
    char *line = NULL;
    while ((line = readline("rhasher> ")) != NULL) {
        if (line[0] != '\0') add_history(line);
        if (strcmp(line, "exit") == 0 || strcmp(line, "quit") == 0) {
            free(line);
            break;
        }
        parse_and_execute_line(line);
        free(line);
    }
    printf("\n");
#else
    printf("RHash calculator (type 'exit' or 'quit' to leave)\n");
    char line_buf[MAX_LINE_LEN];
    while (1) {
        printf("rhasher> ");
        fflush(stdout);
        if (!fgets(line_buf, sizeof(line_buf), stdin)) {
            break;
        }
        line_buf[strcspn(line_buf, "\n")] = '\0';
        if (strcmp(line_buf, "exit") == 0 || strcmp(line_buf, "quit") == 0) {
            break;
        }
        parse_and_execute_line(line_buf);
    }
    printf("\n");
#endif

    return 0;
}

