#ifndef _IO_H
#define _IO_H
/** outb:
 *  Sends the given data to the given I/O port. Defined in io.s
 *
 */
void outb(unsigned short port, unsigned char data);


/**
 * Read a byte form the given I/O port.
 */
unsigned char inb(unsgined short port);

#endif
