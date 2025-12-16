#include <libintl.h>
#include <locale.h>
#include <stdio.h>
#include "libmylib.h"

#define _(x) gettext(x)

int main() {
    setlocale(LC_ALL, "");
    bindtextdomain("myproject", LOCALEDIR);
    textdomain("myproject");
    printf(_("Result: %d\n"), add(2, 3));
    return 0;
}
