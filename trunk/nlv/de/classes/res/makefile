# Makefile for CWMM resource dll

include ../../../../make.inc
include ../../../../build.inc

LANGUAGE	=	_de

CC           = icc
CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
INC	=	$(MMSRCBASE)/classes/inc
DEFS         = 

OBJDIR	=	$(NLVBASE)/out/$(LANGUAGE)

OBJECTS	= 	$(INC)\mmres.h mmres$(LANGUAGE).rc \
			audioplayer$(LANGUAGE).dlg audioplayer$(LANGUAGE).rc \
			audioconv$(LANGUAGE).dlg audioconv$(LANGUAGE).rc \
			volume$(LANGUAGE).rc volume$(LANGUAGE).dlg \
			imageconv$(LANGUAGE).dlg imageconv$(LANGUAGE).rc \
			cwaudio_de.rc cwaudio_de.dlg cwimage_de.rc cwvideo_de.rc \
			mmfind$(LANGUAGE).dlg mmfind$(LANGUAGE).rc


all:	$(OBJDIR)/mmres$(LANGUAGE).dll mmres$(LANGUAGE).def

$(OBJDIR)/mmres$(LANGUAGE).dll:	mmres$(LANGUAGE).c $(OBJECTS) mmres$(LANGUAGE).def
#		icc $(CPPFLAGS) -I$(INC) /Fe$(OBJDIR)\$@ $< mmres$(LANGUAGE).def
		icc $(CPPFLAGS) -I$(INC) /Fe$@ $< mmres$(LANGUAGE).def
		rc -D $(ECS) -i $(INC) mmres$(LANGUAGE).rc $(OBJDIR)/mmres$(LANGUAGE).dll
		rc -r -D $(ECS) -i $(INC) mmres$(LANGUAGE).rc $(OBJDIR)/mmres$(LANGUAGE).res
                		
clean:
	-rm $(OBJDIR)/*.obj
	-rm $(OBJDIR)/*.dll
	-rm *.*~
	-rm *.flc
	-rm *.res
	-rm *.obj
