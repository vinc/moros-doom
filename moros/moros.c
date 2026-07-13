/* See moros.h */

#include "moros.h"

/* Process ------------------------------------------------------------- */

void exit(int code) {
    syscall1(SYS_EXIT, code);
    __builtin_unreachable();
}

void sleep(double seconds) {
    /* The kernel reads the argument as the raw bits of an f64 */
    union { double f; long i; } bits = { .f = seconds };
    syscall1(SYS_SLEEP, bits.i);
}

/* Files ---------------------------------------------------------------- */

long open(const char *path, uint8_t flags) {
    return syscall3(SYS_OPEN, (long)path, (long)strlen(path), (long)flags);
}

void close(long handle) {
    syscall1(SYS_CLOSE, handle);
}

long read(long handle, void *buf, size_t len) {
    return syscall3(SYS_READ, handle, (long)buf, (long)len);
}

long write(long handle, const void *buf, size_t len) {
    return syscall3(SYS_WRITE, handle, (long)buf, (long)len);
}

long lseek(long handle, long offset, int whence) {
    return syscall3(SYS_SEEK, handle, offset, (long)whence);
}

/* Memory ---------------------------------------------------------------
 *
 * SYS_FREE mirrors Rust's `dealloc(ptr, size, align)` so the size and
 * alignment passed to SYS_ALLOC must be given back on free. A header is
 * stored in front of each allocation to remember them.
 */

#define MALLOC_ALIGN 16 /* max_align_t on x86-64 */
#define MALLOC_MAGIC 0x0DECAFC0FFEE0D0AUL

struct malloc_header {
    size_t size;    /* total size passed to SYS_ALLOC, header included */
    uint64_t magic;
};

size_t malloc_get_size(void *ptr) {
    if (!ptr) return 0;

    struct malloc_header *header = (struct malloc_header *)ptr - 1;

    if (header->magic != MALLOC_MAGIC) {
        return 0; // Corrupted or invalid pointer
    }

    return header->size - sizeof(struct malloc_header);
}

void *malloc(size_t size) {
    if (size == 0) {
        return NULL;
    }
    size_t total = sizeof(struct malloc_header) + size;
    long addr = syscall2(SYS_ALLOC, (long)total, MALLOC_ALIGN);
    if (addr <= 0) {
        return NULL;
    }
    struct malloc_header *header = (struct malloc_header *)addr;
    header->size = total;
    header->magic = MALLOC_MAGIC;
    return (void *)(header + 1);
}

void free(void *ptr) {
    if (ptr == NULL) {
        return;
    }
    struct malloc_header *header = (struct malloc_header *)ptr - 1;
    if (header->magic != MALLOC_MAGIC) {
        print("free: bad pointer\n");
        exit(1);
    }
    header->magic = 0;
    syscall3(SYS_FREE, (long)header, (long)header->size, MALLOC_ALIGN);
}

/* Strings ---------------------------------------------------------------
 *
 * memcpy, memmove, memset, and memcmp must exist even in freestanding
 * mode: the compiler emits calls to them for struct copies and array
 * initialization.
 */

void *memcpy(void *dst, const void *src, size_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dst;
}

void *memmove(void *dst, const void *src, size_t n) {
    unsigned char *d = dst;
    const unsigned char *s = src;
    if (d < s) {
        while (n--) {
            *d++ = *s++;
        }
    } else {
        while (n--) {
            d[n] = s[n];
        }
    }
    return dst;
}

void *memset(void *s, int c, size_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

int memcmp(const void *a, const void *b, size_t n) {
    const unsigned char *x = a;
    const unsigned char *y = b;
    for (; n--; x++, y++) {
        if (*x != *y) {
            return *x - *y;
        }
    }
    return 0;
}

size_t strlen(const char *s) {
    const char *p = s;
    while (*p) {
        p++;
    }
    return (size_t)(p - s);
}

/* Console output -------------------------------------------------------- */

void print(const char *s) {
    write(1, s, strlen(s));
}

void print_num(long n) {
    char buf[24];
    char *p = buf + sizeof(buf);
    bool neg = n < 0;
    unsigned long u = neg ? -(unsigned long)n : (unsigned long)n;
    do {
        *--p = '0' + (char)(u % 10);
        u /= 10;
    } while (u);
    if (neg) {
        *--p = '-';
    }
    write(1, p, (size_t)(buf + sizeof(buf) - p));
}

void print_hex(unsigned long n) {
    char buf[18];
    char *p = buf + sizeof(buf);
    do {
        *--p = "0123456789ABCDEF"[n % 16];
        n /= 16;
    } while (n);
    *--p = 'x';
    *--p = '0';
    write(1, p, (size_t)(buf + sizeof(buf) - p));
}
