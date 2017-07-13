# Makefile

include ../../../make.inc

CC           = gcc
LIB_DIR      = $d:/emx/lib 
#CPPFLAGS     = -Ge- /Gm+ -G4 -Gl  /O+ -Oc+ -Ol
#CPPFLAGS     = -O -Zomf -Zcrtdll -s
CPPFLAGS     = -O -Wall -Zomf -Zmt -Zsys -s

INC		= ../include -I ../../../classes/inc -I ../../../common_functions/include
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
LIBDIR	=	../lib
OBJECTS	=	$(LIBDIR)/message.obj $(LIBDIR)/helper.obj $(LIBDIR)/progbars.obj $(LIBDIR)/sys_funcs.obj


all:	audconv.exe

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c
	gcc -c -o$@ -Zomf -I$(INC) $<  

audconv.exe:	audconv.c $(OBJECTS)
	gcc $(CPPFLAGS) -I$(INC) audconv.c $(OBJECTS) -los2me audconv.def


clean:
	-rm *.exe
	-rm *.obj

cleaner:
	-rm *.*~
	-rm *.flc


distribution:
	-cp *.exe ../../dist/bin
	
