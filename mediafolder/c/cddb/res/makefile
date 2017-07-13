# Makefile for Audio-CD copy

CC           = icc
#CPPFLAGSVAC  = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+
CPPFLAGS  =  -Zdll -Zmt -Zno-rte
INC		= ../../../include
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
BINDIR		=	../../dist/bin
HELPERDIR	=	..\helper

CPPFILES	=
all:	 pmcddb001.dll pmcddb049.dll pmcddb031.dll pmcddb039.dll pmcddb088.dll



pmcddb001.dll:	pmcddb001.c pmcddb001.res
		gcc -Zdll -Zomf -Zno-rte -I$(INC) pmcddb001.c pmcddb001.def
		rc pmcddb001.res pmcddb001.dll
#		icc $(CPPFLAGS) -I$(INC) pmcddb001.c pmcddb001.def

pmcddb001.res:	pmcddb001.rc pmcddb001.dlg $(INC)\pmcddbres.h 
		rc  -i $(INC) -r pmcddb001.rc 


pmcddb049.dll:	pmcddb049.c pmcddb049.res
		gcc -Zdll -Zomf -Zno-rte -I$(INC) pmcddb049.c pmcddb049.def
		rc pmcddb049.res pmcddb049.dll
#		icc $(CPPFLAGS) -I$(INC) pmcddb001.c pmcddb04á.def

pmcddb049.res:	pmcddb049.rc pmcddb049.dlg $(INC)\pmcddbres.h 
		rc  -i $(INC) -r pmcddb049.rc 

pmcddb031.dll:	pmcddb031.c pmcddb031.res
		gcc -Zdll -Zomf -Zno-rte -I$(INC) pmcddb031.c pmcddb031.def
		rc pmcddb031.res pmcddb031.dll
#		icc $(CPPFLAGS) -I$(INC) pmcddb001.c pmcddb04á.def

pmcddb031.res:	pmcddb031.rc pmcddb031.dlg $(INC)\pmcddbres.h 
		rc  -i $(INC) -r pmcddb031.rc 

pmcddb039.dll:	pmcddb039.c pmcddb039.res
		gcc -Zdll -Zomf -Zno-rte -I$(INC) pmcddb039.c pmcddb039.def
		rc pmcddb039.res pmcddb039.dll
#		icc $(CPPFLAGS) -I$(INC) pmcddb001.c pmcddb04á.def

pmcddb039.res:	pmcddb039.rc pmcddb039.dlg $(INC)\pmcddbres.h 
		rc  -i $(INC) -r pmcddb039.rc 

pmcddb088.dll:	pmcddb088.c pmcddb088.res
		gcc -Zdll -Zomf -Zno-rte -I$(INC) pmcddb088.c pmcddb088.def
		rc pmcddb088.res pmcddb088.dll

pmcddb088.res:	pmcddb088.rc pmcddb039.dlg $(INC)\pmcddbres.h 
		rc  -i $(INC) -r pmcddb088.rc 

clean:
	-rm *.obj
	-rm *.dll
	-rm *.res

cleaner:
	-rm *.*~

	
