#include "tunistd.h"
#include "tty_public.h"

int main2()
{
  write(TTY1,"bbbbbbbbbbbbb",10);
  return 4;
}
