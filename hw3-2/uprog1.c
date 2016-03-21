#include "tunistd.h"
#include "tty_public.h"

#define MILLION 1000000
#define DELAY (400 * MILLION)


int main1()
{
  write(TTY1,"aaaaaaaaaa",10);

  return 4;
}
