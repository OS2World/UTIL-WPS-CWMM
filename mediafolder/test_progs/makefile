# Makefile for Audio-CD-Creator Data-CD-Creator

#include ../../make.inc

CC           = icc
#CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
CPPFLAGS     = /G5 /Ge- /Gm+ /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
LIBS	     =  mmPM2.lib rexx.lib                 
INC	=	.\include
HELPERDIR	=	.\helper
DEFS         = 
PROGS        = 
ODIR		= ./obj
CPPDIR	= .



all:	pmtst.exe replace2.exe repclass.exe 


replace2.exe:	replace2.c
	gcc -Zcrtdll replace2.c

repclass.exe:	repclass.c
	gcc -Zcrtdll repclass.c

pmtst.exe:	pmtst.c
	gcc -O -Zomf -Zcrtdll -s $<  pmtst.def
	rc pmtst.res pmtst.exe 

clean:
	-rm *.obj
	-rm *.exe

cleaner:
	-rm *.*~
	-rm *.flc

distger:

disteng:

distribution:
