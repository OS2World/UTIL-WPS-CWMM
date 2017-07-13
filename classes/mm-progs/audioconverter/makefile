# Makefile

include ../../../make.inc

OBJECTS	=	$(LIBDIR)/message.obj \
		$(LIBDIR)/helper.obj \
		$(LIBDIR)/progbars.obj \
		$(LIBDIR)/sys_funcs.obj

include ../mmprog.inc


all:	audconv.exe


audconv.exe:	audconv.c $(OBJECTS) makefile def.tpl
	cmd /C $(TOOLDIR)\create_MMPROG_def.cmd audconv.def
	$(CC) $(CPPFLAGS) -I$(INC) audconv.c $(OBJECTS) -los2me audconv.def


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
	-cp *.exe ../../dist/bin
