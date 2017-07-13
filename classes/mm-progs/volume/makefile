# Makefile

include ../../../make.inc

# MM-progs specific makefile include
include ../mmprog.inc

OBJECTS		=	$(LIBDIR)/message.obj \
			$(LIBDIR)/helper.obj \
			$(LIBDIR)/menu_funcs.obj \
			$(LIBDIR)/sys_funcs.obj
OBJS2		=	customprocs.obj

all:	classvol.exe

classvol.exe:	classvol.c $(OBJECTS) $(OBJS2) makefile def.tpl
	cmd /C $(TOOLDIR)\create_MMPROG_def.cmd classvolume.def
#Use this command to link dynamicaly
	$(CC) $(CPPFLAGS) -I$(INC) classvol.c $(OBJECTS) $(OBJS2) -los2me classvolume.def
	rc classvolume.rc classvol.exe


	
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
