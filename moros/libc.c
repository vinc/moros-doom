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

/* --- string.h ------------------------------------------------------ */

// Source: musl
int strcmp(const char *l, const char *r) {
    for (; *l==*r && *l; l++, r++) {};
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
    if (!dst || !src) {
      return dst;
    }
    char *orig = dst;
    while ((*dst++ = *src++) != '\0') {}
    return orig;
}

char *strncpy(char *dst, const char *src, size_t n) {
    if (!dst || !src) {
      return dst;
    }

    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dst[i] = src[i];
    }

    // Pad the remaining bytes
    for (; i < n; i++) {
        dst[i] = '\0';
    }

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
    if (!s) {
      return NULL;
    }

    const char *last = NULL;
    char ch = (char)c;

    while (*s != '\0') {
        if (*s == ch) {
            last = s;
        }
        s++;
    }

    if (ch == '\0') {
        return (char *)s;
    }

    return (char *)last;
}

char *strstr(const char *hay, const char *needle) {
    /* TODO: naive nested loop is fine at DOOM's string sizes. Empty
     * needle returns hay. */
    (void)needle;
    todo("strstr");
    return (char *)hay;
}

// Source: musl
char *strdup(const char *s) {
    size_t l = strlen(s);
    char *d = malloc(l + 1);
    if (!d) {
      return NULL;
    }
    return memcpy(d, s, l + 1);
}

/* --- stdlib.h ------------------------------------------------------ */

void *calloc(size_t n, size_t size) {
    if (n != 0 && size > SIZE_MAX / n) {
        return NULL; // Overflow
    }

    size_t total_bytes = n * size;

    if (total_bytes == 0) {
        return NULL;
    }

    void *ptr = malloc(total_bytes);
    if (!ptr) {
        return NULL; // Allocation failed
    }

    memset(ptr, 0, total_bytes);

    return ptr;
}

void *realloc(void *ptr, size_t size) {
    if (!ptr) {
        return malloc(size);
    }

    if (size == 0) {
        free(ptr);
        return NULL;
    }

    size_t old_payload_size = malloc_get_size(ptr);

    void *new_ptr = malloc(size);
    if (!new_ptr) {
        return NULL;
    }

    size_t copy_size = (old_payload_size < size) ? old_payload_size : size;
    memcpy(new_ptr, ptr, copy_size);

    free(ptr);

    return new_ptr;
}

int abs(int n) {
    return (n < 0) ? -n : n;
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

/* --- ctype.h -------------------------------------------------------*/

int toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
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
    todo("isalpha");
    (void)c;
    return 0;
}

int isupper(int c) {
    todo("isupper");
    (void)c;
    return 0;
}

int islower(int c) {
    todo("islower");
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
