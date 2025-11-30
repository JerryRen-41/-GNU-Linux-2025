#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <libintl.h>

#define T_(s) gettext(s)

#ifndef LOCALE_INSTALL_DIR
#define LOCALE_INSTALL_DIR "locale"
#endif

static int read_answer(const char *yes_word, const char *no_word) {
    char *line = NULL;
    size_t sz = 0;

    while (getline(&line, &sz, stdin) != -1) {
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n'))
            line[len - 1] = '\0';

        if (strcmp(line, yes_word) == 0) {
            free(line);
            return 1;
        }
        if (strcmp(line, no_word) == 0) {
            free(line);
            return 0;
        }

        printf("%s\n", T_("Please reply with the expected answer."));
        printf("%s (%s / %s): ", T_("Enter response"), yes_word, no_word);
    }

    free(line);
    return -1;  // EOF
}

int main(void) {
    setlocale(LC_ALL, "");
    bindtextdomain("gamenumber", LOCALE_INSTALL_DIR);
    bind_textdomain_codeset("gamenumber", "UTF-8");
    textdomain("gamenumber");

    int left = 1;
    int right = 100;

    printf("%s\n", T_("Think of an integer between 1 and 100."));

    const char *yes_key = T_("yes");
    const char *no_key  = T_("no");

    while (left < right) {
        int pivot = (left + right) / 2;

        printf(T_("Is your number strictly greater than %d?\n"), pivot);
        printf("%s (%s/%s): ", T_("Your input"), yes_key, no_key);

        int ans = read_answer(yes_key, no_key);

        if (ans == -1) {
            printf("%s\n", T_("Input stream closed unexpectedly."));
            return 1;
        }
        if (ans == 1)
            left = pivot + 1;
        else
            right = pivot;
    }

    printf(T_("Your chosen number is: %d\n"), left);
    return 0;
}
