

#include <stdio.h>
#include "io_public.h"

#define MILLION 1000000
#define DELAYLOOPCOUNT (400 * MILLION)
#define BUFLEN 80

void delay(void);

int main(void)
{
  char buf[BUFLEN];
  int got, lib_console_dev, ldev;

  /* Determine the SAPC's "console" device, the serial port for user i/o */
  lib_console_dev = sys_get_console_dev();  /* SAPC support lib fn */
  switch(lib_console_dev) {
  case COM1: ldev = TTY0;	/* convert to our dev #'s */
    break;
  case COM2: ldev = TTY1;
    break;
  default: printf("Unknown console device\n");
    return 1;
  }
  kprintf("Running with device TTY%d\n",ldev);
  /* Now have a usable device to talk to with i/o package-- */

  kprintf("\nTrying simple write(4 chars)...\n");
  got = write(ldev,"hi!\n",4);
  kprintf("write of 4 returned %d\n",got);
  delay();  /* time for output to finish, once write-behind is working */

  kprintf("Trying longer write (9 chars)\n");
  got = write(ldev, "abcdefghi", 9);
  kprintf("write of 9 returned %d\n",got);
  delay();  /* time for output to finish, once write-behind is working */

  kprintf("\nType 10 chars input to test typeahead while looping for delay...\n");
  delay();
  got = read(ldev, buf, 10);	/* should wait for all 10 chars, once fixed */
  kprintf("\nGot %d chars into buf. Trying write of buf...\n", got);
  write(ldev, buf, got);
  delay();

  exit(1);
  kprintf("this is after exit");

}

/* the faster the machine you're on, the longer this loop must be */
void delay()
{
  int i;

  kprintf("<doing delay>\n");
  for (i = 0; i < DELAYLOOPCOUNT; i++)
   ;
}
