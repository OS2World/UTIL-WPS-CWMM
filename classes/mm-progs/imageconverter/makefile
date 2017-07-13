# Makefile

include ../../../make.inc
include ../mmprog.inc

OBJECTS	=	$(LIBDIR)/message.obj \
		$(LIBDIR)/helper.obj \
		$(LIBDIR)/progbars.obj \
		$(LIBDIR)/sys_funcs.obj

all:	imgconv.exe


imgconv.exe:	imgconv.c $(OBJECTS)
	cmd /C $(TOOLDIR)\create_MMPROG_def.cmd imgconv.def
#Use this command to link dynamicaly
	$(CC) $(CPPFLAGS) -I$(INC) imgconv.c $(OBJECTS) -los2me imgconv.def

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
