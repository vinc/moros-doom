/* Minimal stdio for MOROS DOOM. Implementations in ../stdio.c */

#ifndef STDIO_H
#define STDIO_H

#include <stddef.h>
#include <stdarg.h>

#define EOF (-1)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

typedef struct FILE FILE;

extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;

FILE  *fopen(const char *path, const char *mode);
int    fclose(FILE *f);
size_t fread(void *ptr, size_t size, size_t n, FILE *f);
size_t fwrite(const void *ptr, size_t size, size_t n, FILE *f);
int    fseek(FILE *f, long offset, int whence);
long   ftell(FILE *f);
int    fflush(FILE *f);

int    printf(const char *fmt, ...);
int    fprintf(FILE *f, const char *fmt, ...);
int    vfprintf(FILE *f, const char *fmt, va_list ap);
int    snprintf(char *buf, size_t size, const char *fmt, ...);
int    vsnprintf(char *buf, size_t size, const char *fmt, va_list ap);
int    sscanf(const char *str, const char *fmt, ...);

int    puts(const char *s);
int    putc(int c, FILE *f);
int    putchar(int c);

int    remove(const char *path);
int    rename(const char *from, const char *to);

#endif
