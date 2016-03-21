/***************************************************************************
 * tsystm.h: holds the kernel prototypes for sysread, syswrite, and sysexit
 * also contains ioinit,
 * authors: Joshua Decosta & Thomas Martin
*/


/* i/o initialization loop for SAPC devices */
void ioinit(void);
/* read nchar bytes into buf from dev */
int sysread(int dev, char *buf, int nchar);
/* write nchar bytes from buf to dev */
int syswrite(int dev, char *buf, int nchar);
/* exit will return back to Tutor */
int sysexit(int exitcode);
