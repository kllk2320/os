#include "io.h"

#define SERIAL_COM1_BASE            0x3f8   /* COM1 base port */

#define SERIAL_DATA_PORT(base)      (base)
#define SERIAL_FIFO_COMMAND_PORT(base)      (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)      (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)      (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)      (base + 5)

/* The I/O port commands */

/**
 * Tels the serial port to expect first the highest bits on the data port,
 * the the lowest 8 bist will follow
 */
#define SERIAL_LINE_ENABLE_DLAB             0x80


void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com), ((divisor >> 8) & 0x00ff));
    outb(SERIAL_DATA_PORT(com), (divisor & 0x00ff));
}
/**
 * Configures the line of the given serial port. The port is set to have a
 * data lenght of 8 bits, no parity bits, one stop bits and break control
 * disabled. 
 */
void serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

/**
 * Configures the fifo buffer of the given serial port. The port is set as: 
 * FIFO enabled
 * Both receiver and transmission FIFO queues cleared
 * Use 14 bytes as the size of queue
 *
 */
void serial_configure_fifo(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), 0xc7);
}


void serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}

/**
 *  Checks whether the transmit FIFO queue is empty or not for the given COM port
 *
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


void serial_write(char *buf, unsigned int len)
{

}

