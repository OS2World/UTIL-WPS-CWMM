# Makefile for Audio-CD copy

include ../../../make.inc

CC           = gcc
CPPFLAGS     = -O -Zomf -Zcrtdll -s
#CPPFLAGSSTATIC     = -O -Zomf -Zsys -Zmt -s

CPPFLAGSVAC  = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+
INC		= ../../include
MOREINC		= $(COMMONSRC)/include
DEFS         = 
PROGS        =  
ODIR		= .
CPPDIR	= .
LIBDIR		=	./lib
BINDIR		=	../../dist/bin
HELPERDIR	=	..\helper
OBJECTS		=	cddb.obj \
			cd.obj \
			helper.obj \
			cddbhelper.obj \
			message.obj
LIBOBJECTS	= $(LIBDIR)/sys_funcs.obj

#printHelper.obj
#all:	 querycddb.exe cddbcls.obj

all:	 pmcddb.exe pmdb001.dll pmdb049.dll pmdb031.dll pmdb039.dll pmdb088.dll

nlv:	pmdb001.dll pmdb049.dll pmdb031.dll pmdb039.dll pmdb088.dll 

pmdb001.dll:	./res/pmcddb001.c ./res/pmcddb001.res
		-rm pmdb001.dll
		cp ./res/pmcddb001.dll pmdb001.dll

./res/pmcddb001.res:	./res/pmcddb001.rc ./res/pmcddb001.dlg $(INC)/pmcddbres.h 
		cd ./Res && make

pmdb049.dll:	./res/pmcddb049.c ./res/pmcddb049.res
		-rm pmdb049.dll
		cp ./res/pmcddb049.dll pmdb049.dll

./res/pmcddb049.res:	./res/pmcddb049.rc ./res/pmcddb049.dlg $(INC)/pmcddbres.h 
		cd ./Res && make

pmdb031.dll:	./res/pmcddb031.c ./res/pmcddb031.res
		-rm pmdb031.dll
		cp ./res/pmcddb031.dll pmdb031.dll

./res/pmcddb031.res:	./res/pmcddb031.rc ./res/pmcddb031.dlg $(INC)/pmcddbres.h 
		cd ./Res && make

pmdb039.dll:	./res/pmcddb039.c ./res/pmcddb039.res
		-rm pmdb039.dll
		cp ./res/pmcddb039.dll pmdb039.dll

./res/pmcddb039.res:	./res/pmcddb039.rc ./res/pmcddb039.dlg $(INC)/pmcddbres.h 
		cd ./Res && make



pmdb088.dll:	./res/pmcddb088.c ./res/pmcddb088.res
		-rm pmdb088.dll
		cp ./res/pmcddb088.dll pmdb088.dll

./res/pmcddb088.res:	./res/pmcddb088.rc ./res/pmcddb088.dlg $(INC)/pmcddbres.h 
		cd ./Res && make


$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c 
	$(CC) -c -Wall -o$@ -Zomf -I$(INC) -I$(MOREINC) $< 

cd.obj:	cd.c
		gcc -c $< -I$(INC) -Zcrtdll -Zomf

helper.obj:	helper.c
		gcc -c $< -I$(INC) -Zcrtdll -Zomf

message.obj:	message.c
		gcc -c $< -I$(INC) -Zcrtdll -Zomf

pmcddb.exe:	pmcddb.cc $(OBJECTS) $(LIBOBJECTS) $(CDDBOBJ) $(INC)\cddb.h $(INC)\cddb.hh
	$(CC) $< $(CPPFLAGS) -I$(INC) -I$(MOREINC) $(LIBOBJECTS) $(OBJECTS) $(CDDBOBJ)  -lsocket pmcddb.def

cddb.obj:	cddb.cpp $(INC)\cddb.hh
		gcc -c cddb.cpp -I$(INC) -Zcrtdll -Zomf

cddbcls.obj:	cddbcls.cpp $(INC)\cddb.hh $(INC)\cddb.h
		cd ../pm_launchers && make


printHelper.obj:	printHelper.cpp
		gcc -c printHelper.cpp -I$(INC) -Zcrtdll -Zomf

cddbhelper.obj:	$(HELPERDIR)\cddbhelper.cpp
		gcc -c $< -I$(INC) -Zcrtdll -Zomf

clean:
	-rm *.obj
	-rm *.exe
	-rm *.dll
	-cd ./res && make clean

cleaner:
	-rm *.*~

	
