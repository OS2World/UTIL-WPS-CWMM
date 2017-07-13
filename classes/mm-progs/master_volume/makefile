# Makefile

include ../../../make.inc
include ../mmprog.inc

ICONDIR =	..\res\Icons
OBJECTS	=	$(LIBDIR)/message.obj \
		$(LIBDIR)/helper.obj \
		$(LIBDIR)/progbars.obj \
		$(LIBDIR)/sys_funcs.obj
#OBJECTS	=	$(OBJDIR)/message.obj $(OBJDIR)/helper.obj $(OBJDIR)/progbars.obj \
			$(OBJDIR)/sys_funcs.obj

all:	volume.exe


volume.exe:	volume.c $(OBJECTS) makefile def.tpl
	cmd /C $(TOOLDIR)\create_MMPROG_def.cmd volume.def
	$(CC) $(CPPFLAGS) -I$(INC) volume.c $(OBJECTS) -Zmt -los2me volume.def
	rc -I $(INC) volume.rc volume.exe

clean:
	-rm *.exe
	-rm *.obj
	-rm *.res
	-rm *.ini

cleaner:
	-rm *.*~
	-rm *.flc


distribution:
