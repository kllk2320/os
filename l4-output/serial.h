#ifndef _SERIAL_H
#define _SERIAL_H

#define SERIAL_DATA_PORT(base)      (base)
#define SERIAL_FIFO_COMMAND_PORT(base)      (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)      (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base)      (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)      (base + 5)


/**
 * Tels the serial port to expect first the highest bits on the data port,
 * the the lowest 8 bist will follow
 */
#define SERIAL_LINE_ENABLE_DLAB             0x80


#define SERIAL_DATA_BITS_MASK		0x03
#define SERIAL_DATA_BITS_5 			0x00
#define SERIAL_DATA_BITS_6 			0x01
#define SERIAL_DATA_BITS_7 			0x02
#define SERIAL_DATA_BITS_8 			0x03

#define SERIAL_STOP_BITS_MASK		0x04

#define SERIAL_PARITY_MASK			0x38
#define SERIAL_PARITY_NONE			0x00
#define SERIAL_PARITY_ODD			0x01
#define SERIAL_PARITY_EVEN			0x03
#define SERIAL_PARITY_MARK			0x05
#define SERIAL_PARITY_SPACE			0x07

#define SERIAL_8N1 					0x03
#define SERIAL_FIFO_14				0xc7
#define SERIAL_RTS_DTR				0x03

#define SERIAL_BREAK_CONTROL_MASK	0x40

void serial_init();

void serial_write(char *buf, unsigned int len);
#endif
