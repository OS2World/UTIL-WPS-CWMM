# Makefile

#include ../../../make.inc

CC           = gcc
LIB_DIR      = $d:/emx/lib 
#CPPFLAGS     = -Ge- /Gm+ -G4 -Gl  /O+ -Oc+ -Ol
CPPFLAGS     = -O -Zomf -Zcrtdll -s -W
ICCFLAGS	= /Ss
INC		= ../include -I ../../../classes/inc
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
RESDIR	=	..\res\$(LANGUAGE)
ICONDIR =	..\res\Icons
HELPERDIR	=	../../common
LIBDIR	=	../lib
OBJECTS	=	$(LIBDIR)/helper.obj $(LIBDIR)/message.obj $(LIBDIR)/sys_funcs.obj

#OBJECTS	=	$(HELPERDIR)\message.obj $(HELPERDIR)\helper.obj $(HELPERDIR)\progbars.obj 

RESOBJ	=	./Res/001/volume.rc ./Res/001/volume.dlg \
	./Res/049/volume049.rc ./Res/049/volume049.dlg \
	./volumeres.h

all:	tst.exe

tst.exe:	volume.c $(OBJECTS) $(RESOBJ)
	cd ./Res/001 && make
	cd ./Res/049 && make
#	icc $(ICCFLAGS) -I$(INC) volume.c  MMPM2.lib volume.def
	gcc $(CPPFLAGS) -I$(INC) volume.c $(OBJECTS) -Zmtd -los2me volume.def
	rc ./Res/049/volume049.res volume.exe



%.obj:	%.c
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  


clean:
	-rm *.exe
	-rm *.o
	-rm *.obj
	-cd $(HELPERDIR) && make clean

cleaner:
	-rm *.*~
	-rm *.flc
	-cd $(HELPERDIR) && make cleaner


distribution:
	
