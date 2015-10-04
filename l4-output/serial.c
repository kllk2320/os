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
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), ((divisor >> 8) & 0x00ff));
    outb(SERIAL_DATA_PORT(com), (divisor & 0x00ff));
}
/** serial_configure_line
 *
 * Configures the line of the given serial port. The port is set to have a
 * data lenght of 8 bits, no parity bits, one stop bits and break control
 * disabled. 
 *
 * @param com: the serial port to configure
 * @param parity: the parity  
 * @param data_bits:  the data bits
 * @param bc: break control, 0: disable, 1: enable
 * @param stop_bit: the stop bit, 0: 1 stop-bit, 1: 1.5 or 2 stop-bits
 */
#if 1
void serial_configure_line(unsigned short com, unsigned char config)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), config);
}

#else
void serial_configure_line(unsigned short com, unsigned char parity, unsigned char data_bits,
						   unsigned char bc, unsigned char stop_bit)
{
	unsigned char l;	

	l = ((bc << 6) & SERIAL_BREAK_CONTROL_MASK) &
		((parity << 3) & SERIAL_PARITY_MASK) &
		((stop_bit << 2) & SERIAL_STOP_BITS_MASK) &
		(data_bits & SERIAL_DATA_BITS_MASK);

    outb(SERIAL_LINE_COMMAND_PORT(com), l);
}
#endif

/**
 * Configures the fifo buffer of the given serial port. The port is set as: 
 * FIFO enabled
 * Both receiver and transmission FIFO queues cleared
 * Use 14 bytes as the size of queue
 */
void serial_configure_fifo(unsigned short com, unsigned char config)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), config);
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
	/*Set baud rate to be 115200*/
	serial_configure_baud_rate(SERIAL_COM1, 2);

	/* 8 bits, no parity, one stop bit, no break control */
	serial_configure_line(SERIAL_COM1, SERIAL_8N1);

	serial_configure_fifo(SERIAL_COM1, SERIAL_FIFO_14);
	serial_configure_modem(SERIAL_COM1, SERIAL_RTS_DTR);
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

