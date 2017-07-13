# Makefile

include ../../../make.inc
include ../mmprog.inc

OBJECTS	=	$(LIBDIR)/message.obj \
		$(LIBDIR)/helper.obj \
		$(LIBDIR)/menu_funcs.obj \
		$(LIBDIR)/sys_funcs.obj
OBJS2		=	 customprocs.obj playaudio.obj display.obj


all:	mmplayer.exe


mmplayer.exe:	mmplayer.c $(OBJECTS) $(OBJS2) makefile def.tpl
	cmd /C $(TOOLDIR)\create_MMPROG_def.cmd mmplayer.def
#Use this command to link dynamicaly
	$(CC) $(CPPFLAGS) -I$(INC) mmplayer.c $(OBJECTS) $(OBJS2) -los2me mmplayer.def
	rc mmplayer.rc mmplayer.exe


clean:
	-rm *.exe
	-rm *.dll
	-rm *.obj
	-rm *.res
	-rm *.ini

cleaner:
	-rm *.*~
	-rm *.flc

distribution:
	
