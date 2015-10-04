#include "fb.h"
#include "serial.h"

int sum_of_three(int arg1, int arg2, int arg3)
{
	return arg1 + arg2 + arg3;
}

void kmain(void)
{
	char buf[11]={'H','e','l','l','o',' ',
	   		   	  'W','o','r','l','d'};

	serial_init();
	serial_write(buf, 11);

	fb_driver_init();
	fb_write(buf, 11);
}
