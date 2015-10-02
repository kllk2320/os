#include "io.h"

/* Color */
enum {
    FB_BLACK = 0,
    FB_BLUE,
    FB_GREEN,
    FB_CYAN,
    FB_RED,
    FB_MAGENTA,
    FB_BROWN,
    FB_LIGHT_GREY,
    FB_DARK_GREY,
    FB_LIGHT_BLUE,
    FB_LIGHT_GREEN,
    FB_LIGHT_CYAN,
    FB_LIGHT_RED,
    FB_LIGHT_MAGENTA,
    FB_LIGHT_BROWN,
    FB_WHITE,
};

/* The I/O ports */
#define FB_COMMAND_PORT     0x3d4
#define FB_DATA_PORT        0x3d5

/* The I/O commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15


char *fb = (char *) 0x000b8000;

/** fb_write_cell
 *  Writes a character with the given foreground and background to position i in the framebuffer
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0f) << 4) | (bg &0x0f);
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position 
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    out(FB_DATA_PORT, ((pos >> 8) & 0x00ff));

    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    out(FB_DATA_PORT, (pos & 0x00ff));
}

void fb_write(char *buf, unsigned int len)
{
}


