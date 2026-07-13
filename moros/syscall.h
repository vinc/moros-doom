/* MOROS syscall interface for freestanding C programs.
 *
 * A syscall follows the System V ABI convention with its number in RAX
 * and its arguments in RDI, RSI, RDX, and R8. It is entered with
 * `int 0x80` and the return value comes back in RAX.
 *
 * Numbers must match `src/sys/syscall/number.rs` in MOROS.
 */

#ifndef MOROS_SYSCALL_H
#define MOROS_SYSCALL_H

#include <stdint.h>
#include <stddef.h>

#define SYS_EXIT    0x01
#define SYS_SPAWN   0x02
#define SYS_READ    0x03
#define SYS_WRITE   0x04
#define SYS_OPEN    0x05
#define SYS_CLOSE   0x06
#define SYS_INFO    0x07
#define SYS_DUP     0x08
#define SYS_DELETE  0x09
#define SYS_STOP    0x0A
#define SYS_SLEEP   0x0B
#define SYS_POLL    0x0C
#define SYS_CONNECT 0x0D
#define SYS_LISTEN  0x0E
#define SYS_ACCEPT  0x0F
#define SYS_ALLOC   0x10
#define SYS_FREE    0x11
#define SYS_KIND    0x12
#define SYS_SEEK    0x13

/* Flags for SYS_OPEN, matching `OpenFlag` in `src/sys/fs/mod.rs` */
#define OPEN_READ     (1 << 0)
#define OPEN_WRITE    (1 << 1)
#define OPEN_APPEND   (1 << 2)
#define OPEN_CREATE   (1 << 3)
#define OPEN_TRUNCATE (1 << 4)
#define OPEN_DIR      (1 << 5)
#define OPEN_DEVICE   (1 << 6)

/* Whence for SYS_SEEK, matching the dispatcher in
 * `src/sys/syscall/mod.rs`: Start(u32), Current(i32), End(i32) */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* Any negative return value indicates an error. */

static inline long syscall0(long n) {
    long res;
    __asm__ volatile("int $0x80"
        : "=a"(res)
        : "a"(n)
        : "rcx", "r11", "memory");
    return res;
}

static inline long syscall1(long n, long a1) {
    long res;
    __asm__ volatile("int $0x80"
        : "=a"(res)
        : "a"(n), "D"(a1)
        : "rcx", "r11", "memory");
    return res;
}

static inline long syscall2(long n, long a1, long a2) {
    long res;
    __asm__ volatile("int $0x80"
        : "=a"(res)
        : "a"(n), "D"(a1), "S"(a2)
        : "rcx", "r11", "memory");
    return res;
}

static inline long syscall3(long n, long a1, long a2, long a3) {
    long res;
    __asm__ volatile("int $0x80"
        : "=a"(res)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3)
        : "rcx", "r11", "memory");
    return res;
}

static inline long syscall4(long n, long a1, long a2, long a3, long a4) {
    register long r8 __asm__("r8") = a4;
    long res;
    __asm__ volatile("int $0x80"
        : "=a"(res)
        : "a"(n), "D"(a1), "S"(a2), "d"(a3), "r"(r8)
        : "rcx", "r11", "memory");
    return res;
}

#endif /* MOROS_SYSCALL_H */
