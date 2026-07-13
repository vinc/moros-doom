/* stdio for MOROS DOOM, built on the raw layer in moros.c.
 *
 * Every function below is a stub that reports itself over serial when
 * called, so a running DOOM produces a worklist. Implement, rebuild,
 * run again. The design notes in each TODO describe the minimum DOOM
 * needs, not the full C standard.
 */

#include <stdio.h>
#include <string.h>
#include "moros.h"

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
    /* TODO: parse mode ("r", "rb", "w", "wb", "a"...) into MOROS
     * OpenFlags (OPEN_READ is 0 for plain files, OPEN_WRITE |
     * OPEN_CREATE | OPEN_TRUNCATE for "w", | OPEN_APPEND for "a"),
     * call open(), and on success take a free slot from files[].
     * Return NULL on failure. DOOM calls this from the WAD loader
     * (w_file_stdc.c), config loading, and savegames. Binary vs text
     * mode is meaningless here: ignore 'b'. */
    if (!path || !mode) {
      return NULL;
    }
    printf("fopen: opening '%s' (mode: '%s')\n", path, mode);
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
    /* TODO: read(f->handle, ptr, size * n) and return the number of
     * complete ITEMS read (bytes / size), not bytes. The WAD loader
     * depends on this distinction. A short read is not an error. */
    (void)ptr;
    (void)size;
    (void)n;
    (void)f;
    todo("fread");
    return 0;
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
    /* TODO: lseek(f->handle, offset, whence); return 0 on success,
     * -1 on failure. whence values match the SEEK syscall already.
     * This is the hottest stdio call in DOOM: W_ReadLump seeks
     * before every lump read. */
    (void)f;
    (void)offset;
    (void)whence;
    todo("fseek");
    return -1;
}

long ftell(FILE *f) {
    /* TODO: lseek(f->handle, 0, SEEK_CUR). Used with fseek(...,
     * SEEK_END) by W_AddFile to get the WAD size. */
    (void)f;
    todo("ftell");
    return -1;
}

int fflush(FILE *f) {
    /* Nothing is buffered: every fread/fwrite is a syscall. */
    (void)f;
    return 0;
}

/* --- The formatter -------------------------------------------------
 *
 * Everything funnels into vsnprintf. This is the one function with
 * real internals in the whole port. The format specs DOOM actually
 * uses: %s %d %i %u %x %c %% %ld %lu, field width with optional zero
 * padding (%3d, %02x), and %.Ns precision for strings. No floats are
 * ever printed. Grep the doom sources before adding anything else.
 */

int vsnprintf(char *buf, size_t size, const char *fmt, va_list ap) {
    int writen = 0;
    if (!fmt) {
      return -1;
    }

    while (*fmt) {
        if (*fmt != '%') {
            if (writen + 1 < size && buf) {
                buf[writen] = *fmt;
            }
            writen++;
            fmt++;
            continue;
        }

        fmt++; // Skip '%'

        switch (*fmt) {
            case 'c': {
                char c = (char)va_arg(ap, int);
                if (writen + 1 < size && buf) {
                    buf[writen] = c;
                }
                writen++;
                break;
            }
            case 's': {
                const char *str = va_arg(ap, const char *);
                if (!str) {
                    str = "(null)";
                }
                while (*str) {
                    if (writen + 1 < size && buf) {
                        buf[writen] = *str;
                    }
                    writen++;
                    str++;
                }
                break;
            }
            default: {
                if (*fmt) {
                    if (writen + 1 < size && buf) {
                        buf[writen] = *fmt;
                    }
                    writen++;
                }
                break;
            }
        }

        if (*fmt) {
          fmt++; // Skip specifier
        }
    }

    if (size > 0 && buf) {
        size_t null_idx = (writen < size) ? (size_t)writen : (size - 1);
        buf[null_idx] = '\0';
    }

    return writen;
}

int snprintf(char *buf, size_t size, const char *fmt, ...) {
    /* TODO: va_start, vsnprintf, va_end. Three lines. */
    (void)fmt;
    todo("snprintf");
    if (size > 0) {
        buf[0] = '\0';
    }
    return 0;
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

/* --- Small ones ---------------------------------------------------- */

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
