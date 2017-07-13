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

RESDIR	=	..\res\$(LANGUAGE)
RESBASE2=	../res
RESDIR2	=	../res/$(LANGUAGE)

HELPSOURCE	=	$(RESDIR2)/ipf

OBJECTS	=	mediafolder.obj mediadialogprocs.obj \
			mediafolderoverridenwpmethods.obj mediafolderoverridenwpclsmethods.obj \

all:
	cd ../. && make

mediafolder.dll:
#$(OBJECTS)
		cd ../. && make

mediafolder.obj:	mediafolder.cpp $(INC)\mediafolderres.h $(INC)\mediafolder.hh
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<

mediadialogprocs.obj:	mediadialogprocs.cpp $(INC)\mediafolder.hh $(INC)\mediafolderres.h
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<

mediafolderoverridenwpclsmethods.obj:	mediafolderoverridenwpclsmethods.cpp $(INC)\mediafolder.hh $(INC)\mediafolderres.h
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<

mediafolderoverridenwpmethods.obj:	mediafolderoverridenwpmethods.cpp $(INC)\mediafolder.hh $(INC)\mediafolderres.h
	icc $(CPPFLAGS) /C+ -I$(INC) /Fo$@ $<


clean:

cleaner:
	-rm *.*~
	-rm *.flc

distger:

disteng:

distribution:
