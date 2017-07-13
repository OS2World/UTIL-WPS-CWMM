# Makefile

include ../../make.inc

.SUFFIXES:	.obj .c
.PHONY:		clean cleaner

CC           = gcc
LIB_DIR      = $d:/emx/lib 
#CPPFLAGS     = -Ge- /Gm+ -G4 -Gl  /O+ -Oc+ -Ol
CPPFLAGS     = -O -Wall -Zomf -Zcrtdll -s
INC		=	./include -I ../../classes/inc -I ../../common_functions/include
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
LIBDIR	=	./lib
#export COMMONSRC	=	../common_functions
LIBOBJS	=	$(LIBDIR)/message.obj $(LIBDIR)/helper.obj $(LIBDIR)/progbars.obj $(LIBDIR)/menu_funcs.obj \
			$(LIBDIR)/sys_funcs.obj


$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c
	gcc -c -Wall -o$@ -Zomf -I$(INC) $<  		

all:	$(LIBOBJS)
	cd ./master_volume && make
	cd ./audioconverter && make
	cd ./imageconverter && make
	cd ./mmplayer && make
	cd ./volume && make
	cd ./mmfind && make

clean:
	-cd ./lib && rm *.obj
	-cd ./volume && make clean
	-cd ./audioconverter && make clean
	-cd ./imageconverter && make clean
	-cd ./mmplayer && make clean
	-cd ./master_volume && make clean
	-cd ./mmfind && make clean


cleaner:
	-cd ./volume && make cleaner
	-cd ./audioconverter && make cleaner
	-cd ./imageconverter && make cleaner
	-cd ./mmplayer && make cleaner
	-cd ./master_volume && make cleaner
	-rm ./include/*.*~ 

	
