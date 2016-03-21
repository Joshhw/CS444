
# CS444 hw3 makefile
# Usage "make"  to build tunix.lnx

# makefile for building C or assembly language programs for the
# Standalone 486 or Pentium IBM PC running in 32-bit protected mode,
# or "SA PC" for short.


FILES="README startup0.s startup1.c tunix.c tty.c asmswtch.s ulib.s sched.c io.c ioconf.c crt01.s crt02.s crt03.s uprog1.c uprog2.c uprog3.c sysentry.s makefile "

# also "make clean" to clean up non-source files in a directory

# system directories needed for compilers, libraries, header files--
# assumes the environment variables SAPC_TOOLS, SAPC_GNUBIN, and SAPC_SRC
# are set up, usually by the ulab module


PC_LIB = $(SAPC_TOOLS)/lib
PC_INC = $(SAPC_TOOLS)/include

WARNINGS =  -Wall -Wstrict-prototypes -Wmissing-prototypes \
		-Wno-uninitialized -Wshadow -pedantic \
		-D__USE_FIXED_PROTOTYPES__

PC_CC   = $(SAPC_GNUBIN)/i386-gcc
PC_CFLAGS = -g  $(WARNINGS) -I$(PC_INC)
PC_AS   = $(SAPC_GNUBIN)/i386-as
PC_LD   = $(SAPC_GNUBIN)/i386-ld
PC_NM   = $(SAPC_GNUBIN)/i386-nm

# File suffixes:
# .c	C source (often useful both for UNIX host and SAPC)
# .s 	assembly language source
# .opc  relocatable machine code, initialized data, etc., for SA PC
# .lnx  executable image (bits as in memory), for SA PC (Linux a.out format)
# .syms text file of .lnx's symbols and their values (the "symbol table")
# Symbol file "syms"--for most recently built executable in directory

# PC executable--tell ld to start code at 0x1000e0, load special startup
# module, special PC C libraries--
# Code has 0x20 byte header, so use "go 100100" (hopefully easier to
# remember than 100020)

all: tunix.lnx

# Object files for tiny unix OS

IO_OFILES = io.opc tty.opc ioconf.opc queue/queue.opc
KERNEL_OFILES = startup0.opc startup1.opc tunix.opc sysentry.opc \
		asmswtch.opc sched.opc $(IO_OFILES)
USER_OFILES =   crt01.opc crt02.opc crt03.opc \
		uprog1.opc uprog2.opc uprog3.opc \
		ulib.opc

tunix.lnx: $(KERNEL_OFILES) $(USER_OFILES)
	$(PC_LD) -N -Ttext 1000e0 -o tunix.lnx \
	$(KERNEL_OFILES) $(USER_OFILES) $(PC_LIB)/libc.a
	rm -f syms;$(PC_NM) -n tunix.lnx>tunix.syms;ln -s tunix.syms syms

uprog1.opc: uprog1.c tunistd.h tty_public.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o uprog1.opc uprog1.c

uprog2.opc: uprog2.c tunistd.h tty_public.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o uprog2.opc uprog2.c

uprog3.opc: uprog3.c tunistd.h tty_public.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o uprog3.opc uprog3.c

io.opc: io.c ioconf.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o io.opc io.c

tty.opc: tty.c tty.h ioconf.h tty_public.h proc.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o tty.opc tty.c

ioconf.opc: ioconf.c ioconf.h tty.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o ioconf.opc ioconf.c

crt01.opc: crt01.s
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o crt01.opc crt01.s

crt02.opc: crt02.s
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o crt02.opc crt02.s

crt03.opc: crt03.s
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o crt03.opc crt03.s

tunix.opc: tunix.c tsyscall.h tsystm.h proc.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o tunix.opc tunix.c

sched.opc: sched.c proc.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o sched.opc sched.c

sysentry.opc: sysentry.s
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o sysentry.opc sysentry.s

asmswtch.opc: asmswtch.s
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o asmswtch.opc asmswtch.s

ulib.opc: ulib.s
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o ulib.opc ulib.s

startup0.opc: startup0.s
	$(PC_CC) -c -o startup0.opc startup0.s

startup1.opc: startup1.c
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o startup1.opc startup1.c

clean:
	rm -f *.o *.opc core

# "make spotless" to remove (hopefully) everything except sources
#  use this after grading is done
spotless:
	rm -f *.o *.opc *syms *.lnx
