#include <stdio.h>
#include <string.h>
#include "moros.h"

#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS   1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS       0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS      0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS       1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS   0
#include "nanoprintf.h"

void todo(const char *name); /* in libc.c: prints "TODO: <name>" */

/* A FILE is just a MOROS handle. The kernel tracks the read/write
 * cursor per handle, so fseek/ftell map directly onto the SEEK
 * syscall. A small static pool avoids malloc during early init. */

#define MAX_FILES 16

struct FILE {
    long handle;
    int used;
};

static struct FILE files[MAX_FILES] = {
    { 0, 1 }, /* stdin  */
    { 1, 1 }, /* stdout */
    { 2, 1 }, /* stderr */
};

FILE *stdin  = &files[0];
FILE *stdout = &files[1];
FILE *stderr = &files[2];

FILE *fopen(const char *path, const char *mode) {
    if (!path || !mode) {
      return NULL;
    }
    int flags = 0;
    switch (mode[0]) {
        case 'r':
            flags = 1;
            break;
        case 'w':
            flags = 2;
            break;
        case 'a':
            flags = 4;
            break;
        default:
            return NULL;
    }

    int slot = -1;
    for (int i = 0; i < MAX_FILES; i++) {
        if (!files[i].used) {
            slot = i;
            break;
        }
    }
    if (slot == -1) {
        return NULL;
    }

    long handle = open(path, flags);
    if (handle < 0) {
        return NULL;
    }

    files[slot].handle = handle;
    files[slot].used = 1;

    return &files[slot];
}

int fclose(FILE *f) {
    /* TODO: close(f->handle), release the slot. */
    (void)f;
    todo("fclose");
    return EOF;
}

size_t fread(void *ptr, size_t size, size_t n, FILE *f) {
    return read(f->handle, ptr, size * n) / size;
}

size_t fwrite(const void *ptr, size_t size, size_t n, FILE *f) {
    /* TODO: mirror of fread with write(). Used by savegames,
     * screenshots, and config writing; DOOM boots without it. */
    (void)ptr;
    (void)size;
    (void)n;
    (void)f;
    todo("fwrite");
    return 0;
}

int fseek(FILE *f, long offset, int whence) {
    return lseek(f->handle, offset, whence);
}

long ftell(FILE *f) {
    return lseek(f->handle, 0, SEEK_CUR);
}

int fflush(FILE *f) {
    /* Nothing is buffered: every fread/fwrite is a syscall. */
    (void)f;
    return 0;
}

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap) {
    return npf_vsnprintf(buf, size, fmt, ap);
}

int snprintf(char *buf, size_t size, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vsnprintf(buf, size, fmt, ap);
    va_end(ap);
    return n;
}

int vfprintf(FILE *f, const char *fmt, va_list ap) {
    char buf[1024];
    int n = vsnprintf(buf, 1024, fmt, ap);
    write(f->handle, buf, n);
    return n;
}

int fprintf(FILE *f, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vfprintf(f, fmt, ap);
    va_end(ap);
    return n;
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int n = vfprintf(stdout, fmt, ap);
    va_end(ap);
    return n;
}

int sscanf(const char *str, const char *fmt, ...) {
    /* TODO (optional): only reached through config file parsing
     * (m_config.c) and dehacked. Returning 0 ("nothing matched")
     * makes DOOM fall back to defaults, which is fine forever.
     * Implement %d and %s later if config loading is wanted. */
    (void)str;
    (void)fmt;
    return 0;
}

int puts(const char *s) {
    if (!s) {
      return -1;
    }
    size_t len = 0;
    while (s[len] != '\0') {
      len++;
    }
    if (len > 0) {
      write(stdout->handle, s, len);
    }
    char ch = '\n';
    write(stdout->handle, &ch, 1);
    return 0;
}

int putc(int c, FILE *f) {
    if (!f) {
      return -1;
    }
    char ch = (char)c;
    if (write(f->handle, &ch, 1) < 0) {
      return -1;
    }
    return (unsigned char)c;
}

int putchar(int c) {
    return putc(c, stdout);
}

int remove(const char *path) {
    /* TODO: DELETE syscall, or return -1 and let DOOM cope. */
    (void)path;
    todo("remove");
    return -1;
}

int rename(const char *from, const char *to) {
    /* Stub: MOROS has no rename; DOOM only uses it for config
     * rotation. Failing is fine. */
    (void)from;
    (void)to;
    return -1;
}
