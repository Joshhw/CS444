/* cpu.c: library support for cpu-related actions */
#include <cpu.h>
#include <gates.h>

/* non-debug version */
void set_intr_gate(int n, const IntHandler *inthand_addr)
{
  debug_set_intr_gate(n, inthand_addr, 0);
}

/* write the nth idt descriptor as an interrupt gate to inthand_addr */
void debug_set_intr_gate(int n, const IntHandler *inthand_addr, int debug)
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
  desc->flags = GATE_P|GATE_DPL_KERNEL|GATE_TRAPGATE; /* valid, interrupt */
  desc->zero = 0;
}


/* Turn on cache-disable bits in CR0 to disable i486's on-chip cache
 * Also, empty out both on-chip cache and external cache.
 */
void cpu_cache_off()
{
  unsigned int cr0;		/* contents of CPU's CR0 reg */

  cr0 = get_cr0();
  cr0 |= (CR0_CD|CR0_NW);	/* turn on cache-disable flags */
  set_cr0(cr0);
  invd();			/* invalidate both caches' contents */
}

/* turn off cache-disable bits in CR0 to enable i486's CPU cache */
void cpu_cache_on()
{
  unsigned int cr0;

  cr0 = get_cr0();
  cr0 &= ~(CR0_CD|CR0_NW);		/* enable cache */
  set_cr0(cr0);
}
