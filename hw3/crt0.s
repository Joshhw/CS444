# file:	 crt0.s
# name:	 joseph tam phui hui
# description:	 user startup module	
# date:	 3-3-97

.globl _ustart
.text
			
_ustart:	movl $0x3ffff0, %esp         #reinit the stack
		movl $0, %ebp                # and frame pointer
		call _main                   #call main in the uprog.c
                pushl %eax                   #push the retval=exit_code on stack
                call _exit                   # call sysycall exit
               

