# Makefile for Mediafolder

#include ../../../make.inc

CC           = icc
CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
                  
INC	=	..\include
DEFS         = 
PROGS        = 
ODIR		= ./obj
CPPDIR	= .

OBJECTS	= messagehelper.obj helper.obj customprocs.obj except.obj windowhelper.obj \
	somAndWPSHelper.obj mediafldrrexx.obj cd.obj launchpad.obj

all:
	cd ../. && make	

launchpad.obj:		launchpad.cpp
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

messagehelper.obj:		messagehelper.cpp
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

helper.obj:	helper.cpp
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

cd.obj:		cd.cpp
	icc $(CPPFLAGS) /C+ /Ss -I$(INC) /Fo$@ $<
	cd ../. && make

customprocs.obj:	customprocs.cpp $(INC)\mediafolderinc.h $(INC)\mediafolderres.h
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

except.obj:	except.cpp
	icc $(CPPFLAGS) /Sm /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

windowhelper.obj:	windowhelper.cpp
	icc $(CPPFLAGS) /Sm /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

somAndWPSHelper.obj:	somAndWPSHelper.cpp
	icc $(CPPFLAGS) /Sm /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

mediafldrrexx.obj:	mediafldrrexx.cpp
	icc $(CPPFLAGS) /Sm /C+ -I$(INC) /Fo$@ $<
	cd ../. && make

clean:

cleaner:
	-rm *.*~
	-rm *.flc

distribution:

	
