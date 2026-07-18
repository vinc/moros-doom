# MOROS DOOM

moros = ../../moros
DG = ../doomgeneric

CC = x86_64-elf-gcc
LD = x86_64-elf-ld
#CC = cc
#LD = ld

# -ffreestanding        no hosted libc environment
# -nostdinc             only our include/ and the compiler's own headers
# -fno-pic              absolute addressing, resolved at link time
# -mcmodel=large        user space lives at 0x80_0000_0000, above the
#                       4 GB reach of the small model's address constants
# -mno-red-zone         signal delivery may use the user stack
#
# SSE stays enabled: DOOM's config code passes doubles, and the SysV
# ABI returns them in XMM0. Requires the "sse" bootloader feature in
# MOROS. Safe while DOOM is the only float-using process: the kernel
# and Rust userspace are +soft-float and never touch XMM.
# -fno-builtin          call our functions, not compiler intrinsics
GCC_INCLUDE = $(shell $(CC) -print-file-name=include)
CFLAGS = -O2 -std=gnu99 \
	-ffreestanding -nostdinc -isystem $(GCC_INCLUDE) -I include -I . \
	-fno-pic -mcmodel=large -mno-red-zone \
	-fno-stack-protector -fno-builtin -fno-asynchronous-unwind-tables \
	-fcf-protection=none
DOOM_CFLAGS = $(CFLAGS) -I $(DG) -DCMAP256 \
	-DDOOMGENERIC_RESX=320 -DDOOMGENERIC_RESY=200 -w

LDFLAGS = -nostdlib -static -n -e _start -Ttext=0x8000800000

DOOM_SRC = $(wildcard $(DG)/*.c)
DOOM_OBJ = $(patsubst $(DG)/%.c, build/dg/%.o, $(DOOM_SRC))

MOROS_SRC = crt0.c moros.c libc.c stdio.c doomgeneric_moros.c
MOROS_OBJ = $(patsubst %.c, build/%.o, $(MOROS_SRC))

build/doomgeneric_moros.o: CFLAGS += -I $(DG) -DCMAP256

doom: bin/doom

bin/doom: $(MOROS_OBJ) $(DOOM_OBJ)
	@mkdir -p bin
	$(LD) $(LDFLAGS) -o $@ $^

build/%.o: %.c
	@mkdir -p build
	$(CC) $(CFLAGS) -Wall -Wextra -c -o $@ $<

build/dg/%.o: $(DG)/%.c
	@mkdir -p build/dg
	$(CC) $(DOOM_CFLAGS) -c -o $@ $<

bin/%: build/%.o build/crt0.o build/moros.o
	@mkdir -p bin
	$(LD) $(LDFLAGS) -o $@ build/crt0.o build/moros.o build/$*.o

install: bin/doom
	cp bin/doom $(moros)/dsk/bin/doom

clean:
	rm -rf bin build

.PHONY: doom install clean
