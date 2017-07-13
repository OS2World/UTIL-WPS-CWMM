# Makefile for Audio-CD-Creator Data-CD-Creator

#include ../../../make.inc


CC           = icc
#CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
CPPFLAGS     = /G5 /Ge- /Gm+ /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
LIBS	     =  mmPM2.lib rexx.lib                 
INC	=	..\include
HELPERDIR	=	.\helper
DEFS         = 
PROGS        = 
ODIR		= ./obj
CPPDIR	= .


OBJECTS	=	audioshadowoverridenwpmethods.obj audioshadowoverridenwpclsmethods.obj

all:	mediafolder.dll

mediafolder.dll:	$(OBJECTS)
		cd ../. && make

audioshadowoverridenwpclsmethods.obj:	audioshadowoverridenwpclsmethods.cpp $(INC)\audioshadow.hh
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<

audioshadowoverridenwpmethods.obj:	audioshadowoverridenwpmethods.cpp $(INC)\audioshadow.hh
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<



clean:

cleaner:
	-rm *.*~
	-rm *.flc

distger:
