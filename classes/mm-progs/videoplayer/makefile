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
OBJS2		=	 customprocs.obj playaudio.obj display.obj


all:	mmplayer.exe vioplayer.exe

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c 
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  

%.obj:	%.c
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  

mmplayer.exe:	mmplayer.c $(OBJECTS) $(OBJS2) makefile
#Use this command to link dynamicaly
	gcc $(CPPFLAGS) -I$(INC) mmplayer.c $(OBJECTS) $(OBJS2) -los2me mmplayer.def
	rc mmplayer.rc mmplayer.exe

vioplayer.exe:	vioplayer.c
	gcc $(CPPFLAGS) -I$(INC) vioplayer.c $(LIBDIR)/sys_funcs.obj -los2me mmplayer.def

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