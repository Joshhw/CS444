/* Tiny unix aka Tunix.c:
 * - calls kernel_init
 * - calls main
 * - returns to startup
 * - then shuts down
*/
#include <cpu.h>
#include <gates.h>
#include <stdio.h>
#include <pic.h>
#include "io_public.h"
#include "tsystm.h"
#include "tsyscall.h"

void kernel_init(void);
void set_trap_gate(int n, IntHandler *inthand_addr);
void debug_set_trap_gate(int n, IntHandler *inthand_addr, int debug);
void syscallc(int user_eax, int arg1, char * arg2, int arg3);

/* tell C about the assembler shell routine */
extern void syscall(void);
/* tell C about the assebler for exit */
// extern void endexit(int exitcode);
/* tell C about the ustart fuction shell routine */
extern void ustart(void);
/* included for calling using C */
extern void locate_idt(unsigned int *limitp, char ** idtp);


void kernel_init(void)
{
  /* call ioinit to initialize io package/possibly kernel_init */
  ioinit();
  /* call trap gate with correct parameters to initialize trap */
  set_trap_gate(0x80, &syscall);
  /* call main, ref to suggested steps in hw2 eventually ustart */
  ustart();
  return;
}

void syscallc(int user_eax, int arg1, char * arg2, int arg3)
{
  switch (user_eax)
  {
    case SYSREAD_NUM:
      user_eax = sysread(arg1, arg2, arg3);
      break;
    case SYSWRITE_NUM:
      user_eax = syswrite(arg1, arg2, arg3);
      break;
    case SYSEXIT_NUM:
      user_eax = sysexit(arg1);
      break;
    default:
      printf("invalid syscall I.D.: %d\n", user_eax);
      break;


  }
}

int sysexit(int exitcode)
{
  return endexit(exitcode);

}


/* non-debug version */
void set_trap_gate(int n, IntHandler *inthand_addr)
{
  debug_set_trap_gate(n, inthand_addr, 0);
}

/* write the nth idt descriptor as an interrupt gate to inthand_addr */
void debug_set_trap_gate(int n, IntHandler *inthand_addr, int debug)
{
  char *idt_LAaddr;		/* linear address of IDT */
  char *idt_VAaddr;		/* virtual address of IDT */
  Gate_descriptor *idt, *desc;
  unsigned int limit = 0;

  if (debug)
    printf("Calling locate_idt to do sidt instruction...\n");
  locate_idt(&limit,&idt_LAaddr);
  /* convert to virtual address, i.e., ordinary address */
  idt_VAaddr = idt_LAaddr - KERNEL_BASE_LA;  /* usable address of IDT */
  /* convert to a typed pointer, for an array of Gate_descriptor */
  idt = (Gate_descriptor *)idt_VAaddr;
  if (debug)
    printf("Found idt at %x, lim %x\n",idt, limit);
  desc = &idt[n];		/* select nth descriptor in idt table */
  /* fill in descriptor */
  if (debug)
    printf("Filling in desc at %x with addr %x\n",(unsigned int)desc,
	   (unsigned int)inthand_addr);
  desc->selector = KERNEL_CS;	/* CS seg selector for int. handler */
  desc->addr_hi = ((unsigned int)inthand_addr)>>16; /* CS seg offset of inthand  */
  desc->addr_lo = ((unsigned int)inthand_addr)&0xffff;
  desc->flags = GATE_P|GATE_DPL_KERNEL|GATE_TRAPGATE; /* valid, interrupt hw2: modified use */
  desc->zero = 0;
}
