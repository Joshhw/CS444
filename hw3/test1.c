/*********************************************************************
*
*       file:           testio.c
*       author:         betty o'neil
*
*       test program (application code) for 
*       hw2: three system calls, with optional return values
*
*/

#include "stdio.h"              /* for lib's device # defs, protos */
#include "tunistd.h"		/* for our packages devs, API prototypes */
#include "tty_public.h"         /* or this should be included in tunistd.h */

#define MILLION 1000000
#define DELAY (400 * MILLION)
/* for debugging, sharing a kernel debugging tool (a bit of a cheat) */
extern void debug_log(char * msg);
void delay(void);

int main(void)
{
  char buf[80];
  int got, i;
  int ldev = TTY1;

  /* note no call to ioinit() here--done by kernel */
  kprintf("\nTrying simple write...\n");
  debug_log("user start");
  got = write(ldev,"hi!\n",4);
  kprintf("write of 4 returned %d\n", got);
  kprintf("trying longer write\n");
  write(ldev, "abcdefghi", 9);
  kprintf("write of 9 returned, doing delay...\n",got);
  debug_log("<");  /* mark beginning of delay loop in log */
  delay();  /* time for output to finish */
  debug_log(">");  /* --and end of delay loop */
  for (i = 0; i < 80; i++)
      buf[i] = 'A'+ i/2;
  kprintf("\nTrying write of buf...\n");
  write(ldev, buf, 80);
  kprintf("\nwrite of 80 done\n");
  debug_log("<");
  delay();
  debug_log(">");

  kprintf("\nTrying read...\n");
  read(ldev, buf, 10);
  kprintf("\nReturned from read, trying write of buf...\n");
  write(ldev, buf, 10);
  return 0;
}

/* the faster the machine you're on, the longer this loop must be */
void delay()
{
  int i;

  for (i = 0; i < DELAY; i++)
      ;
}
