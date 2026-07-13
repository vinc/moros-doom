#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

#define NULL ((void *)0)

void  *malloc(size_t size);
void  *calloc(size_t n, size_t size);
void  *realloc(void *ptr, size_t size);
void   free(void *ptr);

void   exit(int code) __attribute__((noreturn));
int    system(const char *cmd);
char  *getenv(const char *name);

int    abs(int n);
int    atoi(const char *s);
double atof(const char *s);
long   strtol(const char *s, char **end, int base);
double strtod(const char *s, char **end);

#endif
