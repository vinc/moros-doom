/* string.h, stdlib.h, ctype.h, math.h, and errno for MOROS DOOM.
 *
 * Same convention as stdio.c: every stub reports itself when called,
 * so running DOOM produces the worklist. memcpy, memmove, memset,
 * memcmp, strlen, malloc, free, and exit already live in moros.c.
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "moros.h"

int errno = 0;

void todo(const char *name) {
    print("TODO: ");
    print(name);
    print("\n");
}

void assert_fail(const char *expr, const char *file, int line) {
    print("assert failed: ");
    print(expr);
    print(" at ");
    print(file);
    print(":");
    print_num(line);
    print("\n");
    exit(1);
}

/* --- string.h ------------------------------------------------------ */

// Source: musl
int strcmp(const char *l, const char *r) {
    for (; *l==*r && *l; l++, r++);
	  return *(unsigned char *)l - *(unsigned char *)r;
}

// Source: musl
int strncmp(const char *l, const char *r, size_t n) {
    //const unsigned char *l=(void *)_l, *r=(void *)_r;
    if (!n--) return 0;
    for (; *l && *r && n && *l == *r ; l++, r++, n--);
    return *l - *r;
}

// Source: musl
int strcasecmp(const char *l, const char *r) {
    //const unsigned char *l=(void *)_l, *r=(void *)_r;
    for (; *l && *r && (*l == *r || tolower(*l) == tolower(*r)); l++, r++);
    return tolower(*l) - tolower(*r);
}

int strncasecmp(const char *l, const char *r, size_t n) {
    //const unsigned char *l=(void *)_l, *r=(void *)_r;
    if (!n--) return 0;
    for (; *l && *r && n && (*l == *r || tolower(*l) == tolower(*r)); l++, r++, n--);
    return tolower(*l) - tolower(*r);
}

char *strcpy(char *dst, const char *src) {
    /* TODO: copy including the NUL; return dst. */
    (void)src;
    todo("strcpy");
    return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
    /* TODO: copy at most n bytes, NUL-pad the remainder if src is
     * shorter. Note the standard's trap: if src is longer than n,
     * dst is NOT NUL terminated. DOOM relies on that for its 8-byte
     * lump names, so implement it faithfully. */
    (void)src;
    (void)n;
    todo("strncpy");
    return dst;
}

char *strchr(const char *s, int c) {
    /* TODO: first occurrence of c, or NULL. c == '\0' finds the
     * terminator. */
    (void)s;
    (void)c;
    todo("strchr");
    return NULL;
}

char *strrchr(const char *s, int c) {
    /* TODO: last occurrence of c, or NULL. Used for path extension
     * checks (".wad"). */
    (void)s;
    (void)c;
    todo("strrchr");
    return NULL;
}

char *strstr(const char *hay, const char *needle) {
    /* TODO: naive nested loop is fine at DOOM's string sizes. Empty
     * needle returns hay. */
    (void)needle;
    todo("strstr");
    return (char *)hay;
}

char *strdup(const char *s) {
    /* TODO: malloc(strlen + 1) + memcpy. */
    (void)s;
    todo("strdup");
    return NULL;
}

/* --- stdlib.h ------------------------------------------------------ */

void *calloc(size_t n, size_t size) {
    /* TODO: overflow-check n * size, malloc, memset to zero. */
    (void)n;
    (void)size;
    todo("calloc");
    return NULL;
}

void *realloc(void *ptr, size_t size) {
    /* TODO: NULL ptr means plain malloc; size 0 means free + NULL.
     * Otherwise malloc the new size, memcpy the smaller of old and
     * new sizes, free the old block. The old size lives in the
     * malloc header in moros.c: either expose a helper there or move
     * the malloc family into this file. Only reached from network
     * game checksums, so it can wait. */
    (void)ptr;
    (void)size;
    todo("realloc");
    return NULL;
}

int abs(int n) {
    /* TODO: one line. Called from the renderer's inner loops. */
    todo("abs");
    return n;
}

int atoi(const char *s) {
    /* TODO: skip leading spaces, optional '-', accumulate digits.
     * Used for command line args like "-skill 4". */
    (void)s;
    todo("atoi");
    return 0;
}

long strtol(const char *s, char **end, int base) {
    /* TODO: DOOM only uses base 10 and 16 (config values, dehacked).
     * Setting *end to where parsing stopped matters for callers. */
    (void)s;
    (void)base;
    todo("strtol");
    if (end) {
        *end = (char *)s;
    }
    return 0;
}

double strtod(const char *s, char **end) {
    /* TODO: only config floats like "1.5" (mouse sensitivity) reach
     * this. Integer part, optional '.', fractional part. No
     * exponents, no hex floats. */
    (void)s;
    todo("strtod");
    if (end) {
        *end = (char *)s;
    }
    return 0.0;
}

double atof(const char *s) {
    return strtod(s, NULL);
}

int system(const char *cmd) {
    /* Stub: nothing sensible to launch. DOOM survives failure. */
    (void)cmd;
    return -1;
}

char *getenv(const char *name) {
    /* Stub: no environment. NULL means "not set", which makes DOOM
     * use its defaults (e.g. no DOOMWADDIR). */
    (void)name;
    return NULL;
}

int mkdir(const char *path, unsigned int mode) {
    /* TODO: OPEN with OPEN_CREATE | OPEN_DIR, then close. Only used
     * for the savegame directory; failing just disables saves. */
    (void)path;
    (void)mode;
    todo("mkdir");
    return -1;
}

/* --- ctype.h -------------------------------------------------------
 *
 * TODO: all of these are one-liners on ASCII. No locales, no tables.
 */

int toupper(int c) {
    todo("toupper");
    return c;
}

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

int isspace(int c) {
    todo("isspace");
    (void)c;
    return 0;
}

int isdigit(int c) {
    todo("isdigit");
    (void)c;
    return 0;
}

int isalpha(int c) {
    (void)c;
    return 0;
}

int isupper(int c) {
    (void)c;
    return 0;
}

int islower(int c) {
    (void)c;
    return 0;
}

/* --- math.h -------------------------------------------------------- */

double fabs(double x) {
    /* TODO: one line: x < 0 ? -x : x compiles to x87 without SSE.
     * Reached only from mouse acceleration math. */
    todo("fabs");
    return x;
}
