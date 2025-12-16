#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MIN_VALUE 1
#define MAX_VALUE 100
#define ROMAN_BUF_SIZE 32

static void chomp(char *s) {
    size_t len;
    if (!s) return;
    len = strlen(s);
    if (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
    }
}

static int int_to_roman(int value, char *buffer, size_t size) {
    struct { int value; const char *numeral; } table[] = {
        {1000,"M"},{900,"CM"},{500,"D"},{400,"CD"},
        {100,"C"},{90,"XC"},{50,"L"},{40,"XL"},
        {10,"X"},{9,"IX"},{5,"V"},{4,"IV"},
        {1,"I"}
    };
    size_t pos = 0;
    if (value <= 0 || value > 3999 || !buffer || size == 0) return -1;
    buffer[0] = '\0';
    for (size_t i = 0; i < sizeof(table)/sizeof(table[0]) && value > 0; ++i) {
        while (value >= table[i].value) {
            size_t len = strlen(table[i].numeral);
            if (pos + len + 1 > size) return -1;
            strcpy(buffer + pos, table[i].numeral);
            pos += len;
            value -= table[i].value;
        }
    }
    buffer[pos] = '\0';
    return 0;
}

static int roman_to_int(const char *roman) {
    int result = 0;
    size_t i = 0;
    if (!roman || !*roman) return -1;
    while (roman[i]) {
        char c = toupper((unsigned char)roman[i]);
        char n = toupper((unsigned char)roman[i+1]);
        int v = 0, nv = 0;
        switch (c) {
            case 'M': v = 1000; break;
            case 'D': v = 500; break;
            case 'C': v = 100; break;
            case 'L': v = 50; break;
            case 'X': v = 10; break;
            case 'V': v = 5; break;
            case 'I': v = 1; break;
            default: return -1;
        }
        switch (n) {
            case 'M': nv = 1000; break;
            case 'D': nv = 500; break;
            case 'C': nv = 100; break;
            case 'L': nv = 50; break;
            case 'X': nv = 10; break;
            case 'V': nv = 5; break;
            case 'I': nv = 1; break;
            default: nv = 0;
        }
        if (nv > v) {
            result += nv - v;
            i += 2;
        } else {
            result += v;
            i += 1;
        }
    }
    if (result < 1 || result > 3999) return -1;
    return result;
}

static void print_help(const char *p) {
    printf("Usage: %s [OPTIONS]\n", p);
    printf("Guess-the-number game (1..100).\n\n");
    printf("Options:\n");
    printf("  -r, --roman    Use Roman numerals\n");
    printf("  -h, --help     Show help and exit\n");
    printf("  -V, --version  Show version and exit\n");
}

static void print_version(void) {
    printf("number-game 1.0\n");
}

static int game_loop(int use_roman) {
    int secret;
    char line[128];
    srand((unsigned)time(NULL));
    secret = MIN_VALUE + rand() % (MAX_VALUE - MIN_VALUE + 1);

    if (use_roman) printf("Enter a Roman numeral: ");
    else printf("Enter a number: ");

    while (fgets(line, sizeof(line), stdin)) {
        int guess = 0;
        chomp(line);

        if (use_roman) {
            guess = roman_to_int(line);
            if (guess == -1) {
                printf("Invalid Roman numeral. Try again: ");
                continue;
            }
        } else {
            char *e = NULL;
            guess = strtol(line, &e, 10);
            if (e == line || *e != '\0') {
                printf("Invalid input. Try again: ");
                continue;
            }
        }

        if (guess < secret) printf("Too low. Try again: ");
        else if (guess > secret) printf("Too high. Try again: ");
        else {
            char r[ROMAN_BUF_SIZE];
            printf("Correct! The number was %d.\n", secret);
            if (int_to_roman(secret, r, sizeof(r)) == 0)
                printf("Roman numeral: %s\n", r);
            return 0;
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    int use_roman = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-r") || !strcmp(argv[i],"--roman")) use_roman = 1;
        else if (!strcmp(argv[i],"-h") || !strcmp(argv[i],"--help")) { print_help(argv[0]); return 0; }
        else if (!strcmp(argv[i],"-V") || !strcmp(argv[i],"--version")) { print_version(); return 0; }
        else return 1;
    }
    return game_loop(use_roman);
}

