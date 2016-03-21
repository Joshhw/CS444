/*********************************************************************
*
*       file:        sched.c
*                    scheduler code: includes functions wakeup
*                    sleep, schedule
*                    Author(s): Joshua Decosta, Thomas Martin
*/

#include <cpu.h>
#include "sched.h"
#include "proc.h"

/*====================================================================
*
* schedule function: (pseudocode)
* look for process to run within curproc
* if nothing available, run process 0
* by calling asmswtch(old process, new process)
*
*/

void schedule(void)
{
  int i, first;
  int numZombie = 0;
  char dbgmsg[300];

  PEntry *oldCursor = curproc;

  for(i = 0; i < NPROC; i++){
    if(curproc == &proctab[i]){
      first = i;
    }
  }

  for (i=1; i<NPROC; i++){
    if (proctab[i].p_status == RUN) {
      curproc=&proctab[i];
      if(proctab[first].p_status == ZOMBIE){
        sprintf(dbgmsg, "|(%dZ-%d)|",first, i);
        break;
      } else {
        sprintf(dbgmsg, "|(%d-%d)|", first, i);
        break;
      }
    }
    else if (proctab[i].p_status == ZOMBIE) {
      numZombie++;
    }
  }

  if(numZombie == NPROC -1){
    curproc = &proctab[0];
    if(proctab[first].p_status == ZOMBIE){
        sprintf(dbgmsg, "|(%dZ-0)|",first);
    } else {
      sprintf(dbgmsg, "|(%d-0)|", first);
    }

  }
  debug_log(dbgmsg);
  asmswtch(oldCursor,curproc);
  return;
}

void sleep(WaitCode event)
{
  cli();
  curproc->p_status = BLOCKED;
  curproc->p_waitcode = event;
  schedule();
  sti();
  return;
}

void wakeup(WaitCode event)
{
  int i;
  for(i =1; i < NPROC; i++)
  {
    if(proctab[i].p_status == BLOCKED)// && proctab[i].p_waitcode == event)
      proctab[i].p_status = RUN;
  }
  return;
}
