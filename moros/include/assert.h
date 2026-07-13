#ifndef ASSERT_H
#define ASSERT_H

#ifdef NDEBUG
#define assert(x) ((void)0)
#else
void assert_fail(const char *expr, const char *file, int line);
#define assert(x) ((x) ? (void)0 : assert_fail(#x, __FILE__, __LINE__))
#endif

#endif
