/* MOROS program entry point.
 *
 * The kernel enters `_start` with:
 *
 *   RDI: pointer to an array of Rust `&str` fat pointers, one per
 *        argument, stored as consecutive (ptr: u64, len: u64) pairs
 *   RSI: number of arguments
 *
 * See `exec()` in `src/sys/process/spawn.rs`.
 *
 * The argument strings are NOT null-terminated (Rust strings carry
 * their length), so each one is copied into a stack buffer with a
 * terminating NUL to build a C `argv`.
 */

#include "moros.h"

#define MAX_ARGS 16
#define ARG_BUF_SIZE 1024

int main(int argc, char **argv);

struct fat_str {
    const char *ptr;
    uint64_t len;
};

/* The kernel enters with RSP fresh from `iretq` (16-byte aligned), not
 * the 8-mod-16 a compiled function expects after a `call`, so the stack
 * is realigned on entry. With SSE enabled this is load-bearing: the
 * compiler emits aligned SSE stores for stack locals and spills, and a
 * misaligned stack faults on the first one. */
__attribute__((force_align_arg_pointer))
void _start(const struct fat_str *args, uint64_t len) {
    char buf[ARG_BUF_SIZE]; /* stack, not static: see moros.c */
    char *argv[MAX_ARGS + 1];

    if (len > MAX_ARGS) {
        len = MAX_ARGS;
    }

    int argc = 0;
    size_t used = 0;
    for (uint64_t i = 0; i < len; i++) {
        size_t n = args[i].len;
        if (used + n + 1 > ARG_BUF_SIZE) {
            break;
        }
        memcpy(buf + used, args[i].ptr, n);
        buf[used + n] = '\0';
        argv[argc++] = buf + used;
        used += n + 1;
    }
    argv[argc] = NULL;

    exit(main(argc, argv));
}
