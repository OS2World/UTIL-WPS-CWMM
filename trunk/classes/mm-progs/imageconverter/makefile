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
OBJECTS	=	$(LIBDIR)/message.obj $(LIBDIR)/helper.obj $(LIBDIR)/progbars.obj $(LIBDIR)/sys_funcs.obj

all:	imgconv.exe

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c
	gcc -c -o$@ -Zomf -I$(INC) $<  

imgconv.exe:	imgconv.c $(OBJECTS)
#Use this command to link dynamicaly
#	gcc $(CPPFLAGS) -I$(INC) imgconv.c $(OBJECTS) -Zmtd -los2me imgconv.def
	gcc $(CPPFLAGS) -I$(INC) imgconv.c $(OBJECTS) -los2me imgconv.def

clean:
	-rm *.exe
	-rm *.obj

cleaner:
	-rm *.*~
	-rm *.flc

distribution:
	
