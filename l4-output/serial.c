#include "io.h"
#include "serial.h"

#define SERIAL_COM1		            0x3f8   /* COM1 base port */


/* The I/O port commands */


/** 
 * Configures the baud rate of the given serial port.  The base clock rate is 115200 bps
 *
 *	@param com: The serial port to configure
 *	@param divisor: the divisor.
 */
void serial_configure_baud_rate(unsigned short com, unsigned char divisor)
{
	outb(SERIAL_IRQ_COMMAND_PORT(com), 0);   // Disable all interrupts
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_BAUD_RATE_LOW_PORT(com), divisor);
    outb(SERIAL_BAUD_RATE_HIGH_PORT(com), 0);
}
/** serial_configure_line
 *
 * Configures the line of the given serial port. The port is set to have a
 * data lenght of 8 bits, no parity bits, one stop bits and break control
 * disabled. 
 */
void serial_configure_line(unsigned short com, unsigned char config)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), config);
}

/**
 * Configures the fifo buffer of the given serial port. The port is set as: 
 * FIFO enabled
 * Both receiver and transmission FIFO queues cleared
 * Use 14 bytes as the size of queue
 */
void serial_configure_fifo(unsigned short com, unsigned char config)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), config);
}

/**
 * Configure the modem with rts/dtr set
 */
void serial_configure_modem(unsigned short com, unsigned char config)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), config);
}
/**
 * 
 */
void serial_init() 
{
#if 0
   outb(SERIAL_COM1 + 1, 0x00);    // Disable all interrupts
   outb(SERIAL_COM1 + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(SERIAL_COM1 + 0, 0x01);    // Set divisor to 3 (lo byte) 38400 baud
   outb(SERIAL_COM1 + 1, 0x00);    //                  (hi byte)
   outb(SERIAL_COM1 + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(SERIAL_COM1 + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(SERIAL_COM1 + 4, 0x0B);    // IRQs enabled, RTS/DSR set
#else
	/*Set baud rate to be 115200*/
	serial_configure_baud_rate(SERIAL_COM1, 1);

	/* 8 bits, no parity, one stop bit, no break control */
	serial_configure_line(SERIAL_COM1, SERIAL_8N1);

	serial_configure_fifo(SERIAL_COM1, SERIAL_FIFO_14);
	serial_configure_modem(SERIAL_COM1, SERIAL_RTS_DTR);
#endif
}
/**
 *  Checks whether the transmit FIFO queue is empty or not for the given COM port
 *
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_empty(unsigned short com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}
int serial_is_received(unsigned short com)
{
	/* 0x01 = 0000 0001*/
	return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x01;
}

void serial_write_one(char a)
{
	while(serial_is_transmit_empty(SERIAL_COM1) == 0);

	outb(SERIAL_COM1,a); 
}
void serial_write(char *buf, unsigned int len)
{
	unsigned int i;
	for(i = 0; i < len; i++) {
		serial_write_one(buf[i]);
	}
}

