# like linux SAVE_MOST and RESTORE_MOST macros in irq.h
.text
.globl _syscall
.globl _syscallc

_syscall:
	pushl %edx	# save C scratch regs
	pushl %ecx
	pushl %ebx
	pushl %eax
	call _syscallc  # call C interrupt handler
	popl %eax
	popl %ebx
	popl %ecx
	popl %edx
	iret
