#include "tunistd.h"
#include "tty_public.h"

int main3()
{
  write(TTY1,"ccccccccccccc",10);
  return 6;
}
