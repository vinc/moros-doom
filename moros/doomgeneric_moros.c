#include <stdio.h>
#include "moros.h"
#include "doomgeneric.h"
#include "doomkeys.h"
#include "i_system.h"

extern struct color {
    unsigned int b:8, g:8, r:8, a:8;
} colors[256];

extern int palette_changed;

static unsigned char palette[768];

static const unsigned char keys[128] = {
    [0x01] = KEY_ESCAPE,
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0F] = KEY_TAB,
    [0x11] = KEY_UPARROW,   // W
    [0x15] = 'y',
    [0x1C] = KEY_ENTER,
    [0x1D] = KEY_FIRE,      // Left Ctrl
    [0x1E] = KEY_STRAFE_L,  // A
    [0x1F] = KEY_DOWNARROW, // S
    [0x20] = KEY_STRAFE_R,  // D
    [0x2A] = KEY_RSHIFT,    // Left Shift
    [0x39] = KEY_USE,       // Spacebar
    [0x48] = KEY_UPARROW,   // Arrow Up
    [0x4B] = KEY_LEFTARROW, // Arrow Left
    [0x4D] = KEY_RIGHTARROW,// Arrow Right
    [0x50] = KEY_DOWNARROW, // Arrow Down
};

void todo(const char *name);

static long clk_boot_fh = -1;
static long vga_mode_fh = -1;
static long vga_palette_fh = -1;
static long vga_buffer_fh = -1;
static long kbd_buffer_fh = -1;

static void restore_text_mode(void) {
    write(vga_mode_fh, "80x25", 5);
    printf("\x1b[2J\x1b[1;1H"); // Clear screen and move to top
}

void DG_Init(void) {
    clk_boot_fh    = open("/dev/clk/boot", 64);
    vga_mode_fh    = open("/dev/vga/mode", 64);
    vga_palette_fh = open("/dev/vga/palette", 64);
    vga_buffer_fh  = open("/dev/vga/buffer", 64);
    kbd_buffer_fh  = open("/dev/kbd/buffer", 64);
}

void DG_DrawFrame(void) {
    static int graphics_on = 0;
    if (!graphics_on) { // Setup
        graphics_on = 1;
        write(vga_mode_fh, "320x200", 7);
        I_AtExit(restore_text_mode, 1);
    }

    if (palette_changed) {
        for (int i = 0; i < 256; i++) {
            palette[i * 3 + 0] = colors[i].r;
            palette[i * 3 + 1] = colors[i].g;
            palette[i * 3 + 2] = colors[i].b;
        }
        write(vga_palette_fh, palette, 768);
    }
    write(vga_buffer_fh, DG_ScreenBuffer, 64000);
}

int DG_GetKey(int *pressed, unsigned char *doomKey) {
    unsigned char scancode;
    while (1) {
        if (read(kbd_buffer_fh, &scancode, 1) < 1) {
            return 0; // Empty buffer
        }
        if (scancode == 0xE0) {
            continue;
        }
        unsigned char mapped = keys[scancode & 0x7F];
        if (mapped == 0) {
            continue;
        }
        *pressed = (scancode & 0x80) == 0;
        *doomKey = mapped;
        return 1;
    }
}

uint32_t DG_GetTicksMs(void) {
    char buf[32]; // Example: "123.45678" seconds

    long bytes = read(clk_boot_fh, buf, sizeof(buf) - 1);
    if (bytes <= 0) {
        return 0;
    }
    buf[bytes] = '\0';

    uint32_t seconds = 0; // 123
    uint32_t fraction = 0; // 456
    uint32_t scale = 100;

    const char *ptr = buf;
    while (*ptr >= '0' && *ptr <= '9') {
        seconds = seconds * 10 + (*ptr - '0');
        ptr++;
    }
    if (*ptr == '.') {
        ptr++;
        while (*ptr >= '0' && *ptr <= '9' && scale > 0) {
            fraction += (*ptr - '0') * scale;
            scale /= 10;
            ptr++;
        }
    }

    return (seconds * 1000) + fraction; // 123456
}

void DG_SleepMs(uint32_t ms) {
    sleep(ms / 1000.0);
}

void DG_SetWindowTitle(const char *title) {
    (void)title;
}

int main(int argc, char **argv) {
    doomgeneric_Create(argc, argv);
    for (;;) {
        doomgeneric_Tick();
    }
    return 0;
}
