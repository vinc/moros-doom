/* Minimal freestanding support code for MOROS programs written in C.
 *
 * This is deliberately not a libc: it is the smallest surface needed to
 * write a program against the MOROS syscalls, meant to grow into the
 * support layer of MOROS DOOM.
 */

#ifndef MOROS_H
#define MOROS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "syscall.h"

/* Process */
void      exit(int code) __attribute__((noreturn));
void      sleep(double seconds);

/* Files (handles 0, 1, 2 are stdin, stdout, stderr) */
long      open(const char *path, uint8_t flags);
void      close(long handle);
long      read(long handle, void *buf, size_t len);
long      write(long handle, const void *buf, size_t len);
long      lseek(long handle, long offset, int whence);

/* Memory */
void     *malloc(size_t size);
void      free(void *ptr);

/* Strings (the compiler also emits calls to mem* on its own) */
void     *memcpy(void *dst, const void *src, size_t n);
void     *memmove(void *dst, const void *src, size_t n);
void     *memset(void *s, int c, size_t n);
int       memcmp(const void *a, const void *b, size_t n);
size_t    strlen(const char *s);

/* Console output (no printf yet: that belongs to the real port) */
void      print(const char *s);
void      print_num(long n);
void      print_hex(unsigned long n);

#endif /* MOROS_H */
