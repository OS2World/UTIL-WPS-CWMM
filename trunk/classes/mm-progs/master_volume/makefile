# Makefile

include ../../../make.inc

CC           = gcc
LIB_DIR      = $d:/emx/lib 
#CPPFLAGS     = -Ge- /Gm+ -G4 -Gl  /O+ -Oc+ -Ol
#CPPFLAGS     = -O -Zomf -Zcrtdll -s
CPPFLAGS     = -O -Wall -Zomf -Zsys -s

ICCFLAGS	= /Ss
INC		= ../include -I ../../../common_functions/include -I ../../../classes/inc
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
ICONDIR =	..\res\Icons
LIBDIR	=	../lib
OBJECTS	=	$(LIBDIR)/message.obj $(LIBDIR)/helper.obj $(LIBDIR)/progbars.obj $(LIBDIR)/sys_funcs.obj
#OBJDIR	=	../obj
#OBJECTS	=	$(OBJDIR)/message.obj $(OBJDIR)/helper.obj $(OBJDIR)/progbars.obj \
			$(OBJDIR)/sys_funcs.obj

all:	volume.exe

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c
	gcc -c -o$@ -Zomf -I$(INC) $<  

volume.exe:	volume.c $(OBJECTS)
	gcc $(CPPFLAGS) -I$(INC) volume.c $(OBJECTS) -Zmt -los2me volume.def
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