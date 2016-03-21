# asm startup file
# very first module in load

.globl _ustart

_ustart:
   movl $0x3ffff0, %esp   # set user program stack
   movl $0, %ebp          # make debugger backtrace terminate here
   call _main             # call C startup, which calls user main
   call _exit             # call assembly exit program
