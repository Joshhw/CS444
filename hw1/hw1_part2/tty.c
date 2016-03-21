/*********************************************************************
*
*       file:           tty.c
*       author:         betty o'neil
*
*       tty driver--device-specific routines for ttys 
*
*/
#include <stdio.h>  /* for kprintf prototype */
#include <serial.h>
#include <cpu.h>
#include <pic.h>
#include "ioconf.h"
#include "tty_public.h"
#include "tty.h"
#include "queue/queue.h"

struct tty ttytab[NTTYS];        /* software params/data for each SLU dev */

/* tell C about the assembler shell routines */
extern void irq3inthand(void), irq4inthand(void);

/* C part of interrupt handlers--specific names called by the assembler code */
extern void irq3inthandc(void), irq4inthandc(void); 

/* the common code for the two interrupt handlers */                           
static void irqinthandc(int dev); 



/* external queues to be shared, possibly */
Queue inqueue;		/* creating a queue to read */
Queue outqueue;		/* create local queue to write */
Queue echoqueue;	/* create echo queue */
Queue *rqueue = &inqueue;		/* creating a queue pointer to read */
Queue *wqueue = &outqueue;		/* creating a queue pointer to write */
Queue *equeue = &echoqueue;		/* creating a queue pointer to echo */
/*====================================================================
*
*       tty specific initialization routine for COM devices
*
*/

void ttyinit(int dev)
{
  int baseport;
  struct tty *tty;		/* ptr to tty software params/data block */
  init_queue(rqueue, 6); /* create the queue for read */
  init_queue(wqueue, 6); /* create the queue for write */
  init_queue(equeue, 6); /* create the queue for echo */
  baseport = devtab[dev].dvbaseport; /* pick up hardware addr */
  tty = (struct tty *)devtab[dev].dvdata; /* and software params struct */

  if (baseport == COM1_BASE) {
      /* arm interrupts by installing int vec */
      set_intr_gate(COM1_IRQ+IRQ_TO_INT_N_SHIFT, &irq4inthand);
      pic_enable_irq(COM1_IRQ);
  } else if (baseport == COM2_BASE) {
      /* arm interrupts by installing int vec */
      set_intr_gate(COM2_IRQ+IRQ_TO_INT_N_SHIFT, &irq3inthand);
      pic_enable_irq(COM2_IRQ);
  } else {
      kprintf("Bad TTY device table entry, dev %d\n", dev);
      return;			/* give up */
  }
  tty->echoflag = 1;		/* default to echoing */
  tty->rin = 0;               /* initialize indices */
  tty->rout = 0;
  tty->rnum = 0;              /* initialize counter */
  tty->tin = 0;               /* initialize indices */
  tty->tout = 0;
  tty->tnum = 0;              /* initialize counter */

  /* enable interrupts on receiver */
  outpt(baseport+UART_IER, UART_IER_RDI); /* RDI = receiver data int */
}


/*====================================================================
*
*       Useful function when emptying/filling the read/write buffers
*
*/

#define min(x,y) (x < y ? x : y)


/*====================================================================
*
*       tty-specific read routine for TTY devices
*
*/

int ttyread(int dev, char *buf, int nchar)
{
  int baseport;
  struct tty *tty;
  int i;
  int saved_eflags;        /* old cpu control/status reg, so can restore it */


  baseport = devtab[dev].dvbaseport; /* hardware addr from devtab */
  tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */
  for (i = 0; i < nchar; i++) {
    saved_eflags = get_eflags();
    cli();									/* disable ints in CPU */
	if(queuecount(rqueue) != 0) {			/* check the read queue has characters */
		buf[i] = dequeue(rqueue);			/*insert char from the queue to the user buf */
	} else {
		i--;								/*decrement the count and keep looping */
	}
    set_eflags(saved_eflags);     /* back to previous CPU int. status */
  }

  return nchar;       /* corrected*/
}


/*====================================================================
*
*       tty-specific write routine for SAPC devices
*       (cs444: note that the buffer tbuf is superfluous in this code, but
*        it still gives you a hint as to what needs to be done for
*        the interrupt-driven case)
*
*/

int ttywrite(int dev, char *buf, int nchar)
{
  char ch;
  int baseport;
  struct tty *tty;
  int i, saved_eflags;

  baseport = devtab[dev].dvbaseport; 		/* hardware addr from devtab */
  tty = (struct tty *)devtab[dev].dvdata;   /* software data for line */

  for (i=0; i < nchar; i++) {
	saved_eflags = get_eflags();										/* save the eflags */
	cli();
	if((ch = enqueue(wqueue, buf[i])) == -1) {							/*if the write queue is full, decrement counter to acquire more chars */
		i--;
	} else {
		outpt(baseport+UART_IER, UART_IER_RDI |  UART_IER_THRI);		/*else if the write queue is not full*/
																		/*output the char by turning on the transmit holding interrupt */ 
	}
	

	set_eflags(saved_eflags);											/* restore the saved eflags */
  }

  while(queuecount(wqueue) != 0) {
	outpt(baseport+UART_IER, UART_IER_RDI | UART_IER_THRI);				/* continuously turn on interrupts until wqueue is empty. VM Int fix */
  }
  outpt(baseport+UART_IER, UART_IER_RDI);				/* we iz done with TX, let us shut it DOWN! */
  return nchar;
}

/*====================================================================
*
*       tty-specific control routine for TTY devices
*
*/

int ttycontrol(int dev, int fncode, int val)
{
  struct tty *this_tty = (struct tty *)(devtab[dev].dvdata);

  if (fncode == ECHOCONTROL)
    this_tty->echoflag = val;
  else return -1;
  return 0;
}



/*====================================================================
*
*       tty-specific interrupt routine for COM ports
*
*   Since interrupt handlers don't have parameters, we have two different
*   handlers.  However, all the common code has been placed in a helper 
*   function.
*/
  
void irq4inthandc()
{
  irqinthandc(TTY0);
}                              
  
void irq3inthandc()
{
  irqinthandc(TTY1);
}                              

void irqinthandc(int dev){  
  int ch,status;
  struct tty *tty = (struct tty *)(devtab[dev].dvdata);
  int baseport = devtab[dev].dvbaseport; /* hardware i/o port */;

  
  pic_end_int();                										/* notify PIC that its part is done */
  status = inpt(baseport+UART_LSR);										/* gets Line Status Register to enquire about said status */

  if((status&UART_LSR_DR) != 0) {  									/*LSR is in recieve mode, take in characters */
 	  ch = inpt(baseport+UART_RX);										/* read char, ack the device */
	if (tty->echoflag == 1)												/*if echo flag is on, enqueue chars onto echo queue */
	  enqueue(equeue, ch);												/* enqueue char onto echo queue */
    enqueue(rqueue, ch);												/* enqueue char onto read queue be default FOREVER! */
  }
  if ((status&UART_LSR_THRE) != 0) { 									/* transmit mode, dequeue until no more characters */
	  if ((ch = dequeue(equeue)) != -1) {  								/*this means the echo queue is NOT empty */
	  	outpt(baseport+UART_TX, ch); 									/*output character from echo queue */
	  }  
	  if ((ch = dequeue(wqueue)) != -1) { 								/*this means the write queue is NOT empty */
		outpt(baseport+UART_TX, ch);									/* output the character since its there */
	  }
	  	outpt(baseport+UART_IER, UART_IER_RDI); 						/*disable transmit interrupt, and leave recieve enabled */
  }
}
