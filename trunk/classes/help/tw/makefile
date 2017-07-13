# Makefile

include ../../../make.inc

LANGUAGE	= 088
DISTNAME	=	cwaudio-classes-$(VERSION)
CC           = icc

LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+ 
CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /C

LIBS		=	somtk.lib mmPM2.lib rexx.lib
DEFS		= 



OBJECTS	= 

all: cwmm$(LANGUAGE).hlp

cwmm$(LANGUAGE).hlp:	cwmm$(LANGUAGE).ipf
	ipfc cwmm$(LANGUAGE).ipf	


clean:
	-rm *.hlp

cleaner:
	-rm *.*~

taiwan:
	-cd ..\. && mkdir CWAudio-$(VERSION)-tw
	-cd ..\CWAudio-$(VERSION)-tw && rm -r *
	-cd ..\CWAudio-$(VERSION)-tw && mkdir old_inst
	-cp ./old_inst/* ..\CWAudio-$(VERSION)-tw\old_inst
	-cp ./cwmm.dll ../CWAudio-$(VERSION)-tw 	
	-cp ./cwmm.hlp ../CWAudio-$(VERSION)-tw
	-cp ./setFolder.cmd ../CWAudio-$(VERSION)-tw 	
	-cp ./tw/Readme.tw ../CWAudio-$(VERSION)-tw 	
	-cp ./FILE_ID.DIZ ../CWAudio-$(VERSION)-tw
	-cp ./tw/cwaudio.ins ../CWAudio-$(VERSION)-tw
	-cp ./tw/License ../CWAudio-$(VERSION)-tw
	-cd ..\CWAudio-$(VERSION)-tw && lxlite *
#	-cd ../CWAudio-$(VERSION)-tw && zip -r $(DISTNAME)-tw.zip *

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
	make taiwan
	-make english

