# file:	 crt01.s
# name:	 joseph tam phui hui
# description:	 user startup module
# date:	 3-3-97

.globl _ustart1
.text

_ustart1:
		call _main1                  #call main in the uprog.c
    pushl %eax                   #push the retval=exit_code on stack
    call _exit                   # call sysycall exit
