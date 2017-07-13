# Makefile

#include ../../../make.inc

LANGUAGE	= 049
DISTNAME	=
RESNAME		= volume
CC           = icc

LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+ 
CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /C

LIBS		=	somtk.lib mmPM2.lib rexx.lib
DEFS		= 
INC		= 	..\..


OBJECTS	= 

all: $(RESNAME)$(LANGUAGE).res


$(RESNAME)$(LANGUAGE).res:	$(RESNAME)$(LANGUAGE).rc $(RESNAME)$(LANGUAGE).dlg $(INC)\$(RESNAME)res.h
	rc -r -n -I $(INC) $(RESNAME)$(LANGUAGE).rc

clean:
	-rm $(RESNAME)$(LANGUAGE).res

cleaner:
	-rm *.*~

german:

english:

distribution:

