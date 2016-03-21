#include "tunistd.h"
#include "tty_public.h"

#define MILLION 1000000
#define DELAY (400 * MILLION)


int main1()
{
  int i;
  write(TTY1,"aaaaaaaaaa",10);
  for (i=0;i<DELAY;i++)
    ;

  return 4;
}
