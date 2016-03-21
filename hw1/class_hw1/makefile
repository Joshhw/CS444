# makefile for cs444 hw1
# Usage: "make"  makes testio.lnx
# system directories needed for compilers, libraries, header files--
# assumes the environment variables SAPC_TOOLS, SAPC_GNUBIN, and SAPC_SRC
# are set up, usually by the ulab module
PC_LIB = $(SAPC_TOOLS)/lib
PC_INC = $(SAPC_TOOLS)/include

WARNINGS =  -Wall -Wstrict-prototypes -Wmissing-prototypes \
		-Wno-uninitialized -Wshadow -pedantic \
		-D__USE_FIXED_PROTOTYPES__

PC_CC   = $(SAPC_GNUBIN)/i386-gcc
PC_CFLAGS = -g $(WARNINGS) -I$(PC_INC)
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

all: testio.lnx

# for CS444 hw1
# Object files for dev indep i/o package
# Feel free to add to these lists, and put in newmodule.opc rule
#  in the later part of this file
IO_OFILES = io.opc tty.opc ioconf.opc queue/queue.opc # add queue/queue.opc here 

testio.lnx: testio.opc $(IO_OFILES)
	$(PC_LD) -N -Ttext 1000e0 -o testio.lnx \
	  $(PC_LIB)/startup0.opc $(PC_LIB)/startup.opc \
	  testio.opc $(IO_OFILES) $(PC_LIB)/libc.a
	rm -f syms;$(PC_NM) -n testio.lnx>testio.syms;ln -s testio.syms syms

testio.opc: testio.c tty_public.h 
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o testio.opc testio.c

io.opc: io.c ioconf.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o io.opc io.c

tty.opc: tty.c tty.h 
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o tty.opc tty.c

ioconf.opc: ioconf.c ioconf.h tty.h
	$(PC_CC) $(PC_CFLAGS) -I$(PC_INC) -c -o ioconf.opc ioconf.c

clean:
	rm -f *.o *.opc
# "make spotless" to remove (hopefully) everything except sources
#  use this after grading is done
spotless:
	rm -f *.o *.opc *syms *.lnx














