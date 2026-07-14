#ifndef MOROS_H
#define MOROS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "syscall.h"

/* Process */
void exit(int code) __attribute__((noreturn));
void sleep(double seconds);

/* Files */
long open(const char *path, uint8_t flags);
void close(long handle);
long read(long handle, void *buf, size_t len);
long write(long handle, const void *buf, size_t len);
long lseek(long handle, long offset, int whence);

/* Memory */
size_t malloc_get_size(void *ptr);
void *malloc(size_t size);
void free(void *ptr);

/* Strings */
void *memcpy(void *dst, const void *src, size_t n);
void *memmove(void *dst, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int memcmp(const void *a, const void *b, size_t n);
size_t strlen(const char *s);

/* Console output */
void print(const char *s);
void print_num(long n);
void print_hex(unsigned long n);

#endif /* MOROS_H */
