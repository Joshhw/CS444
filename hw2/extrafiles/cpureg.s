# cpureg.s:	Get Special Regs for C
.text
.global _locate_idt, _get_cr0, _get_cr2,  _get_cr3, _set_cr3, _cli, _sti
.globl _get_eflags, _set_eflags, _set_cr0, _invd
# get address and limit for current IDT
# C call:	 locate_idt(&limit,&addr)
	
_locate_idt:
	sidt idt_info		# SIDT instruction: store-IDT
	movl 4(%esp), %ecx	# pointer to place for limit
	movw idt_limit, %dx     # return limit to caller
	movw %dx,(%ecx)		# ...
	movl 8(%esp), %ecx      # pointer to place for addr
	movl idt_addr, %edx     # return addr to caller
	movl %edx,(%ecx)	# ...
	ret

_locate_gdt:
	sgdt gdt_info		# SGDT instruction: store-GDT
	movl 4(%esp), %ecx	# pointer to place for limit
	movw gdt_limit, %dx     # return limit to caller
	movw %dx,(%ecx)		# ...
	movl 8(%esp), %ecx      # pointer to place for addr
	movl gdt_addr, %edx     # return addr to caller
	movl %edx,(%ecx)	# ...
	ret

# CPU control bits
_get_cr0:	
	movl %cr0, %eax
	ret

_set_cr0:	
	movl 4(%esp),%eax	# get argued new-cr0-val into eax
	movl %eax,%cr0		# then into cr0
	ret
		
# PF linear addr
_get_cr2:	
	movl %cr2, %eax
	ret
	
# page dir linear addr, flags
_get_cr3:	
	movl %cr3, %eax
	ret
	
_set_cr3:	
	movl 4(%esp), %eax
	movl %eax, %cr3
	ret
	
	
#get_eflags
# C call:	 eflags = get_eflags()
		
_get_eflags:	
	pushfl			# push eflags on stack
	popl %eax		# pop back into eax
	ret

#set_eflags
# C call:	set_eflags(int eflags)
	
_set_eflags:
	pushl 4(%esp)
	popfl
	ret

# special instructions--
_cli:	cli
	ret

_sti:	sti
	ret
#invalidate cache
_invd:
	pushl $0                # push something on stack
	wbinvd			# clobbers top of stack
	addl $4,%esp		# pop junk off
	ret
		
.data
idt_info:     # SIDT stores info into argued memory location--
idt_limit:	.word 1
idt_addr:	.long 1  # linear address
gdt_info:     # SGDT stores info into argued memory location--
gdt_limit:	.word 1
gdt_addr:	.long 1  # linear address
