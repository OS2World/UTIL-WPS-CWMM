# Makefile for CWMM resource dll

include ../../../make.inc
include ../../../build.inc

LANGUAGE	=	_en

CC           = icc
CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
INC	=	../../inc
DEFS         = 
PROGS        = 

OBJDIR	=	../dll

OBJECTS	= 	$(INC)\mmres.h mmres$(LANGUAGE).rc\
			audioplayer$(LANGUAGE).dlg audioplayer$(LANGUAGE).rc \
			audioconv$(LANGUAGE).dlg audioconv$(LANGUAGE).rc \
			volume$(LANGUAGE).rc volume$(LANGUAGE).dlg \
			imageconv$(LANGUAGE).dlg imageconv$(LANGUAGE).rc \
			cwaudio_en.rc cwaudio_en.dlg cwimage_en.rc cwvideo_en.rc \
			mmfind_en.dlg mmfind_en.rc

all:	$(OBJDIR)/mmres$(LANGUAGE).dll

$(OBJDIR)/mmres$(LANGUAGE).dll:	mmres$(LANGUAGE).c $(OBJECTS) mmres$(LANGUAGE).def
#		icc $(CPPFLAGS) -I$(INC) /Fo$(OBJDIR) $< mmres$(LANGUAGE).def
		icc $(CPPFLAGS) -I$(INC) /Fe$(OBJDIR)\$@ $< mmres$(LANGUAGE).def
		rc -D $(ECS) -i $(INC) mmres$(LANGUAGE).rc $(OBJDIR)/mmres$(LANGUAGE).dll
#		rc  mmres$(LANGUAGE).res mmres$(LANGUAGE).dll

#mmres$(LANGUAGE).res:	mmres$(LANGUAGE).rc $(OBJECTS)
#		rc -D $(ECS) -i $(INC) -r mmres$(LANGUAGE).rc 


                		
clean:
	-rm $(OBJDIR)/*.obj
	-rm $(OBJDIR)/*.dll
	-rm *.*~
	-rm *.flc
	-rm *.res
	-rm *.obj


	
