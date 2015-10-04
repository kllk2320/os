#include "io.h"
#include "fb.h"

/* The I/O ports */
#define FB_COMMAND_PORT     0x3d4
#define FB_DATA_PORT        0x3d5

/* The I/O commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define FB_ROW_NUM				25
#define FB_COLUMN_NUM			80	
#define FB_CELLS_NUM 			(FB_ROW_NUM * FB_COLUMN_NUM)

/*The starting address of the memory-mapped I/O for the framebuffer */
#define FB_BASE_ADDR					0x000b8000
#define FB_CHAR_PTR				((char *)0x000b8000)
#define FB_SHORT_PTR			((short *)0x000b8000)

#define FB_CELL_PTR(row, column)	(FB_CHAR_PTR + 2 * (FB_COLUMN_NUM * (row) + (column)))	

struct {
	unsigned int row;
	unsigned int column;
	unsigned char fg_color;
	unsigned char bg_color;
	int scroll_mode;
}fb_state;


/** fb_write_cell
 *  Writes a character with the given foreground and background to position i in the framebuffer
 */
void fb_write_cell(unsigned int row, unsigned int column,
				char c, unsigned char fg, unsigned char bg)
{
	char * fb = FB_CELL_PTR(row, column);

    fb[0] = c;
    fb[1] = ((fg & 0x0f) << 4) | (bg & 0x0f);
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position 
 */
void fb_move_cursor(unsigned int row, unsigned int column)
{
	unsigned int pos = row * FB_COLUMN_NUM + column;

    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00ff));

    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, (pos & 0x00ff));
}

void fb_init(void)
{
	fb_state.row = 0;
	fb_state.column = 0;
	fb_state.fg_color = FB_BLACK;
	fb_state.bg_color = FB_WHITE;
	fb_state.scroll_mode = 1;
}
/** fb_clear_screen()
 * Clear the screen, move cursor to the beginning of the screen.
 */
void fb_clear_screen()
{
	unsigned int i;

	for(i = 0; i < FB_CELLS_NUM; i++){
		FB_SHORT_PTR[i] = 0x0f20;
	}
	fb_init();
	fb_move_cursor(0, 0);
}
/** fb_scroll()
 *	Scroll down the screen by given rows;
 *
 *	@param n: number of rows to scroll down, must be greater than 0 and smaller than FB_ROW_NUM
 */
void fb_scroll(int n)
{
	int i; 	
	short *fb = FB_SHORT_PTR;
	int offset = n * FB_COLUMN_NUM;
	
	for(i = 0; i < (FB_CELLS_NUM - offset ); i++)
	{
		fb[i] = fb[offset + i];
	}

	//Blank the bottom lines, fill the empty cells with space. 
	for(i = FB_CELLS_NUM - offset; i < FB_CELLS_NUM; i++){
		fb[i] = 0x0f20;
	}
	fb_state.row -= n;
}
/**
 *
 * @param len: number of characters to write, len must be smaller than FB_CELLS_NUM
 */
void fb_write(char *buf, unsigned int len)
{
	unsigned int i;
	int scroll_num ;
	
	scroll_num = ((fb_state.column + len)/FB_COLUMN_NUM  + fb_state.row) / FB_ROW_NUM;

	if(scroll_num > 0 ){	
		fb_scroll(scroll_num);
	}

	for( i = 0; i < len; i++){
		fb_write_cell(fb_state.row, fb_state.column, buf[i], 
				fb_state.fg_color, fb_state.bg_color);

		fb_state.column += 1;
		fb_state.row += (fb_state.column /FB_COLUMN_NUM);
		fb_state.column = fb_state.column % FB_COLUMN_NUM;
	}

	fb_move_cursor(fb_state.row, fb_state.column);
}
void fb_driver_init(void)
{
	fb_init();
	fb_clear_screen();
}

