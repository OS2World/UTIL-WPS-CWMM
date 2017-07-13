# Makefile for Audio-CD-Creator Data-CD-Creator

include ../../../make.inc

CC           = icc
#CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
CPPFLAGS     = /G5 /Ge- /Gm+ /Gl  /O+ /Oc+ /Ol- /Q+ /D$(ECS) $(VACDEBUG)
LIBS	     =  mmPM2.lib rexx.lib                 
INC	=	..\include
HELPERDIR	=	..\helper
DEFS         = 
PROGS        = 
ODIR		= ./obj
CPPDIR	= .


all:	mediafolder.dll

.cpp.obj:
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<

mediafolder.dll:
		cd ../. && make


clean:

cleaner:
	-rm *.*~
	-rm *.flc

distger:

disteng:

distribution:

