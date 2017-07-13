# Makefile

include ../../../make.inc

LANGUAGE	= 
DISTNAME	=	cwaudio-classes-$(VERSION)
CC           = icc

LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+ 
CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /C

LIBS		=	somtk.lib mmPM2.lib rexx.lib
DEFS		= 
INC		= 	..\..\inc


OBJECTS	= 

all: cwaudio$(LANGUAGE).res


cwaudio$(LANGUAGE).res:	cwaudio$(LANGUAGE).rc cwimage$(LANGUAGE).rc cwvideo$(LANGUAGE).rc cwaudio$(LANGUAGE).dlg $(INC)\cwmmres.h
	rc -r -n -I $(INC) cwaudio$(LANGUAGE).rc

clean:
	-rm cwmm$(LANGUAGE).res

cleaner:
	-rm *.*~

german:
	-cd ..\. && mkdir CWAudio-$(VERSION)-ger
	-cd ..\CWAudio-$(VERSION)-ger && rm -r *
	-cd ..\CWAudio-$(VERSION)-ger && mkdir old_inst
	-cp ./old_inst/* ..\CWAudio-$(VERSION)-ger/old_inst/
	-cp ./cwmm.dll ../CWAudio-$(VERSION)-ger
	-cp ./setFolder.cmd ../CWAudio-$(VERSION)-ger 	
	-cp ./ger/Readme.ger ../CWAudio-$(VERSION)-ger
	-cp ./FILE_ID.DIZ ../CWAudio-$(VERSION)-ger
	-cp ./cwmm049.hlp ../CWAudio-$(VERSION)-ger/cwmm.hlp
	-cp ./ger/cwaudio.ins ../CWAudio-$(VERSION)-ger
	-cp ./ger/Lizenz ../CWAudio-$(VERSION)-ger
	-rc ./cwaudio049.rc ../CWAudio-$(VERSION)-ger/cwmm.dll
	-cd ..\CWAudio-$(VERSION)-ger && lxlite *
#	-cd ../CWAudio-$(VERSION)-ger && zip -r $(DISTNAME)-ger.zip *

english:
	-cd ..\. && mkdir CWAudio-$(VERSION)-eng
	-cd ..\CWAudio-$(VERSION)-eng && rm -r *
	-cd ..\CWAudio-$(VERSION)-eng && mkdir old_inst
	-cp ./old_inst/* ..\CWAudio-$(VERSION)-eng\old_inst
	-cp ./cwmm.dll ../CWAudio-$(VERSION)-eng 	
	-cp ./cwmm.hlp ../CWAudio-$(VERSION)-eng
	-cp ./setFolder.cmd ../CWAudio-$(VERSION)-eng 	
	-cp ./eng/Readme.eng ../CWAudio-$(VERSION)-eng 	
	-cp ./FILE_ID.DIZ ../CWAudio-$(VERSION)-eng
	-cp ./eng/cwaudio.ins ../CWAudio-$(VERSION)-eng
	-cp ./eng/License ../CWAudio-$(VERSION)-eng
	-cd ..\CWAudio-$(VERSION)-eng && lxlite *
#	-cd ../CWAudio-$(VERSION)-eng && zip -r $(DISTNAME)-eng.zip *

distribution:
	make
	-make german
	make english

