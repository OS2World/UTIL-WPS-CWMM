# Makefile

include ../../../make.inc

CC           = icc
LIB_DIR      = $d:/emx/lib 
CPPFLAGS     = -O -Zomf -Zcrtdll -s
CPPFLAGSVAC  = /Ge /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ /Ss
INC		= ../../include
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
BINDIR		=	../../dist/bin
HELPERDIR	=	..\helper


all:	 creatcd.exe



creatcd.exe:	createcd.c 
ifdef    ECS_COMPILE
	 icc  $(CPPFLAGSVAC) -I$(INC) $<  
else
	icc  $(CPPFLAGSVAC) -I$(INC) $< 
#	gcc  $(CPPFLAGS) -I$(INC) $< -Zlinker /PMTYPE:VIO
endif

clean:
	-rm *.obj
	-rm *.exe

cleaner:
	-rm *.*~

	
