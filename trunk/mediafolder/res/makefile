# Makefile for Audio-CD-Creator Data-CD-Creator

include ../../make.inc
include ../../build.inc

CC           = icc
CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
INC	=	..\include
INC2	=	../include
DEFS         = 
PROGS        = 

OBJDIR	=	./obj

HELPSOURCE	=	$(RESDIR2)/ipf

OBJECTS	= 

all:		$(OBJDIR)/version.h \
		mediafolder.res \
		nlv

nlv:		$(OBJDIR)/version.h
		@echo "[35;1;mCreating laguage files.[0;m"
		@for i in $(LANGUAGES); \
		do \
			echo "[33;1;m Creating mediafolder_"$$i".dll[0;m";  \
			make LANGUAGE=$$i $(OBJDIR)/mediafolder_$$i.dll ; \
		done


$(OBJDIR)/version.h:	../../build.inc
		@echo '#define MFLDR_VERSION "'$(VERSION_STRING)'"\n#define MFLDR_AUTHOR "'$(AUTHOR_STRING)'"' > $(OBJDIR)/version.h
		@echo '#define BUILD_STRING "'$(BUILD_STRING)'"' > $(OBJDIR)/build.h

#### Resource DLL ####
$(OBJDIR)/mediafolder_$(LANGUAGE).dll:	./$(LANGUAGE)/mediafolder_$(LANGUAGE).c \
						$(OBJDIR)/mediafolder_$(LANGUAGE).res
		@./$(LANGUAGE)/create_res_$(LANGUAGE)_def.CMD $(OBJDIR)/mediafolder_$(LANGUAGE).def
		icc $(CPPFLAGS) -I$(INC)  -c  /Fo$(OBJDIR)/mediafolder_$(LANGUAGE).obj $< 
		icc $(CPPFLAGS) -I$(INC) /Fe$@  $(OBJDIR)/mediafolder_$(LANGUAGE).obj ./obj/mediafolder_$(LANGUAGE).def
		cd $(OBJDIR) && rc  mediafolder_$(LANGUAGE).res mediafolder_$(LANGUAGE).dll
	
$(OBJDIR)/mediafolder_$(LANGUAGE).res:	./$(LANGUAGE)/mediafolder_$(LANGUAGE).rc \
						./$(LANGUAGE)/mediafolder_$(LANGUAGE).dlg \
						$(INC)\mediafolderres.h \
						../../build.inc
		cd ./$(LANGUAGE) && rc -D $(ECS) -i ../$(INC) -i ../$(OBJDIR) -r mediafolder_$(LANGUAGE).rc
		cp ./$(LANGUAGE)/mediafolder_$(LANGUAGE).res ./obj/
		-rm ./$(LANGUAGE)/mediafolder_$(LANGUAGE).res


mediafolder.res:		mediafolder.rc $(INC)\mediafolderres.h
		rc -D $(ECS) -i $(INC) -i $(OBJDIR) -r mediafolder.rc 

clean:
	-cd $(OBJDIR) && rm *
	-rm *.res

cleaner:
	-cd de && make cleaner
	-cd en && make cleaner
	-rm *.flc
	-rm *.*~

distribution:

	
