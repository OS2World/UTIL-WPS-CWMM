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
OBJECTS	=	$(LIBDIR)/helper.obj $(LIBDIR)/message.obj $(LIBDIR)/sys_funcs.obj
# $(LIBDIR)/menu_funcs.obj 
OBJS2		=	image.obj	 
#customprocs.obj playaudio.obj display.obj


all:	mmfind.exe

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c 
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  

%.obj:	%.c
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  

mmfind.exe:	mmfind.c $(OBJECTS) $(OBJS2) makefile
#Use this command to link dynamicaly
	gcc $(CPPFLAGS) -I$(INC) mmfind.c $(OBJECTS) $(OBJS2) -los2me mmfind.def
#	rc mmfind.rc mmfind.exe


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
	
