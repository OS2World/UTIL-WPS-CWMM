# Makefile

include ../../../make.inc

CC           = gcc
LIB_DIR      = $d:/emx/lib 
#CPPFLAGS     = -Ge- /Gm+ -G4 -Gl  /O+ -Oc+ -Ol
#CPPFLAGS     = -O -Zomf -Zcrtdll -Zmtd -s
CPPFLAGS     = -O -Wall -Zomf -Zmt -Zsys -s
INC		= ../include -I ../../../classes/inc -I ../../../common_functions/include
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
LIBDIR	=	../lib
OBJECTS	=	$(LIBDIR)/message.obj $(LIBDIR)/helper.obj $(LIBDIR)/menu_funcs.obj $(LIBDIR)/sys_funcs.obj
OBJS2		=	 customprocs.obj
# display.obj


all:	classvol.exe

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c 
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  

%.obj:	%.c
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  

classvol.exe:	classvol.c $(OBJECTS) $(OBJS2) makefile
#Use this command to link dynamicaly
	gcc $(CPPFLAGS) -I$(INC) classvol.c $(OBJECTS) $(OBJS2) -los2me classvolume.def
	rc classvolume.rc classvol.exe


clean:
	-rm *.exe
	-rm *.obj
	-rm *.ini
	-rm *.res

cleaner:
	-rm *.*~
	-rm *.flc

distribution:
	
