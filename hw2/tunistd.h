/***************************************************************************
 * tunistd.h: holds the prototypes for usermode: read, write, and exit
 * authors: Joshua Decosta & Thomas Martin
*/

#ifndef TUNISTD_H
#define TUNISTD_H

#include "tty_public.h"


/* read nchar bytes into buf from dev */
int read(int dev, char *buf, int nchar);
/* write nchar bytes from buf to dev */
int write(int dev, char *buf, int nchar);
/* exit will return back to Tutor */
int exit(int exitcode);

#endif
