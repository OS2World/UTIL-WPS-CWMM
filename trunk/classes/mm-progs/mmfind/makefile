# Makefile

include ../../../make.inc
include ../mmprog.inc

LIBOBJECTS	=	$(LIBDIR)/helper.obj \
			$(LIBDIR)/message.obj \
			$(LIBDIR)/sys_funcs.obj \
			$(LIBDIR)/window_funcs.obj \
			$(LIBDIR)/img_funcs.obj \
			$(LIBDIR)/ini_funcs.obj


OBJECTS		=	image.obj	 


all:	mmfind.exe

mmfind.exe:	mmfind.c $(LIBOBJECTS) $(OBJECTS) makefile mmfind.rc def.tpl
#Use this command to link dynamicaly
	cmd /C $(TOOLDIR)\create_MMPROG_def.cmd mmfind.def
	gcc $(CPPFLAGS) -I$(INC) mmfind.c $(LIBOBJECTS) $(OBJECTS) -los2me mmfind.def
	rc mmfind.rc mmfind.exe

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
