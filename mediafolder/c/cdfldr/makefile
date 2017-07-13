# Makefile for Audio-CD-Creator Data-CD-Creator

include ../../../make.inc

CC           = icc
#CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
CPPFLAGS     = /G5 /Ge- /Gm+ /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
LIBS	     =  mmPM2.lib rexx.lib                 
INC	=	..\include
HELPERDIR	=	..\helper
DEFS         = 
PROGS        = 
ODIR		= ./obj
CPPDIR	= .


OBJECTS	= cdfolder.obj cdfolderoverriddenwpmethods.obj cdfolderoverriddenwpclsmethods.obj \
	..\helper\cd.obj cdfldrdialogprocs.obj

all:	mediafolder.dll


.cpp.obj:
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<

mediafolder.dll:
		cd ../. && make

cdfolder.obj:	cdfolder.cpp $(INC)\mediafolderres.h $(INC)\cdfolder.hh

cdfolderoverriddenwpmethods.obj:	cdfolderoverriddenwpmethods.cpp $(INC)\mediafolderres.h $(INC)\cdfolder.hh

cdfolderoverriddenwpclsmethods.obj:	cdfolderoverriddenwpclsmethods.cpp $(INC)\mediafolderres.h $(INC)\cdfolder.hh

cdfldrdialogprocs.obj:	cdfldrdialogprocs.cpp

..\helper\cd.obj:	..\helper\cd.cpp $(INC)\cddb.h
	-cd ..\helper && make

clean:

cleaner:
	-rm *.*~
	-rm *.flc

distger:

disteng:

distribution:

