/* MOROS platform layer for doomgeneric.
 *
 * These six functions are the entire surface between DOOM and MOROS.
 * The proof of concept in demo.c contains a working version of each
 * mechanism: mode switching, palette upload, full-frame blits,
 * scancode draining, and sleep. This file is where they become the
 * real thing.
 *
 * Under CMAP256 (set in the Makefile), i_video.c exports:
 *
 *   pixel_t *DG_ScreenBuffer;      8-bit indexed, 320 * 200 bytes
 *   struct color colors[256];      current palette, BGRA byte order
 *   boolean palette_changed;       set by I_SetPalette
 */

#include <stdio.h>
#include "moros.h"
#include "doomgeneric.h"
#include "doomkeys.h"

extern struct color {
    unsigned int b:8, g:8, r:8, a:8;
} colors[256];
extern int palette_changed;

void todo(const char *name);

void DG_Init(void) {
    /* TODO: open /dev/vga/mode, /dev/vga/palette, /dev/vga/buffer,
     * and /dev/kbd/buffer with OPEN_DEVICE, keep the handles in
     * static globals (safe under USER_ADDR), and write "320x200" to
     * the mode device. Compare demo.c test_video(). */
    todo("DG_Init");
}

void DG_DrawFrame(void) {
    /* TODO: two writes.
     *
     * 1. If palette_changed: convert colors[] (BGRA) into a 768 byte
     *    R,G,B array and write it to /dev/vga/palette, then clear
     *    palette_changed. This fires on boot and on item pickups
     *    (the yellow flash is a palette swap).
     *
     * 2. Write all 64000 bytes of DG_ScreenBuffer to
     *    /dev/vga/buffer. One write, one blit; the device always
     *    blits from offset 0. */
    todo("DG_DrawFrame");
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {
    /* TODO: read one byte from /dev/kbd/buffer. A return of 0 from
     * read() means the queue is empty: return 0. Otherwise:
     *
     *   *pressed = (scancode & 0x80) == 0;
     *   *doomKey = table[scancode & 0x7F];
     *
     * and return 1. Unmapped scancodes should be skipped by reading
     * again, not returned as key 0. The set 1 make codes DOOM wants
     * (see doomkeys.h for the KEY_* values):
     *
     *   0x01 esc      KEY_ESCAPE     0x1C enter  KEY_ENTER
     *   0x39 space    KEY_USE        0x1D lctrl  KEY_FIRE
     *   0x48 up       KEY_UPARROW    0x50 down   KEY_DOWNARROW
     *   0x4B left     KEY_LEFTARROW  0x4D right  KEY_RIGHTARROW
     *   0x11 w        KEY_UPARROW    0x1F s      KEY_DOWNARROW
     *   0x1E a        KEY_STRAFE_L   0x20 d      KEY_STRAFE_R
     *   0x2A lshift   KEY_RSHIFT     0x0F tab    KEY_TAB
     *   0x02..0x0A    '1'..'9' (weapon select)   0x15 y  'y'
     *
     * Note: 0x48/0x50/0x4B/0x4D are the extended arrow codes and
     * arrive prefixed with 0xE0 on real hardware; either handle the
     * prefix or use WASD first and refine later. */
    (void)pressed;
    (void)doomKey;
    return 0;
}

uint32_t DG_GetTicksMs(void) {
    /* TODO: monotonic milliseconds since boot. Read /dev/clk/uptime
     * (you wrote the device: check what it returns and convert to
     * ms). DOOM uses this for its 35 Hz game tick, so it must
     * actually advance or the game freezes on the first frame. */
    todo("DG_GetTicksMs");
    return 0;
}

void DG_SleepMs(uint32_t ms) {
    /* TODO: sleep(ms / 1000.0). One line with the f64 wrapper from
     * moros.c. */
    (void)ms;
    todo("DG_SleepMs");
}

void DG_SetWindowTitle(const char *title) {
    /* No window. Printing it once at boot is a nice touch. */
    (void)title;
}

int main(int argc, char **argv) {
    /* Run as: doom -iwad /path/to/doom1.wad */
    doomgeneric_Create(argc, argv);
    for (;;) {
        doomgeneric_Tick();
    }
    return 0;
}
