# Makefile mediafolder

include ../make.inc

CC           = icc
#CPPFLAGS     = /Ge- /Gm+ /G4 /Gl  /O+ /Oc+ /Ol- /Q+ $(VACDEBUG)
CPPFLAGS     = /G5 /Ge- /Gm+ /Gl  /O+ /Ss /Oc+ /Ol- /Q+ /D$(ECS) $(VACDEBUG)


LIBS	     =  mmPM2.lib rexx.lib                 
INC	=	./include

CMNINC	= 	$(COMMONSRC)/include

HELPERDIR	=	./c/helper
DEFS         = 
PROGS        = 
CDIR		=	./c
IDLDIR		=	../classes/idl


#######################################
#
# Define compile command
#
#######################################
COMPILE	=	$(CC) $(CPPFLAGS) /C+ -I$(INC) -I$(CMNINC) /Fo$@ $<

#######################################
#
# Define objects
#
#######################################
OBJDIR		=	./obj

# Mediafolder objects
MFLDROBJDIR		= ./obj
MFLDROBJS	=	$(MFLDROBJDIR)/mediafolder.obj \
			$(MFLDROBJDIR)/mediafolderoverridenwpmethods.obj \
			$(MFLDROBJDIR)/mediafolderoverridenwpclsmethods.obj \
			$(MFLDROBJDIR)/mediadialogprocs.obj

# CD-folder objects
CDFLDROBJDIR		= ./obj
CDFLDROBJS	=	$(CDFLDROBJDIR)/cdfolder.obj $(CDFLDROBJDIR)/cdfolderoverriddenwpmethods.obj \
			$(CDFLDROBJDIR)/cdfolderoverriddenwpclsmethods.obj \
			$(CDFLDROBJDIR)/cdfldrdialogprocs.obj

# M3ufolder objects
M3UFLDROBJDIR	= ./obj
M3UFLDROBJS	=	$(M3UFLDROBJDIR)/m3ufolderoverriddenwpmethods.obj \
			$(M3UFLDROBJDIR)/m3ufolderoverriddenwpclsmethods.obj

#Audio shadow objects
AUDSHWOBJDIR	=	./obj
AUDSHWOBJS	=	$(AUDSHWOBJDIR)/audioshadowoverridenwpmethods.obj \
			$(AUDSHWOBJDIR)/audioshadowoverridenwpclsmethods.obj \

# Common functions used for Media folder and the classes
LIBDIR		=	./lib
LIBOBJS	=	$(LIBDIR)/sys_funcs.obj $(LIBDIR)/menu_funcs.obj

# Some helper functions not contained in common_funcs
CMNOBJS	=	$(OBJDIR)/messagehelper.obj $(OBJDIR)/helper.obj $(OBJDIR)/windowhelper.obj \
			$(OBJDIR)/somandwpshelper.obj $(OBJDIR)/customprocs.obj $(OBJDIR)/except.obj \
			$(OBJDIR)/mediafldrrexx.obj $(OBJDIR)/launchpad.obj $(OBJDIR)/cd.obj

OBJECTS	=	 $(MFLDROBJS) $(CDFLDROBJS) $(M3UFLDROBJS) $(AUDSHWOBJS) $(LIBOBJS)\
			$(CMNOBJS)

#######################################
#
# Define Targets
#
#######################################
all:	mediafolder.dll mediafolder_en.dll mediafolder_nl.dll mediafolder_de.dll mediafolder_it.dll \
	./help/mediafldr.hlp \
	prog_tutorial\mediafldrprog.inf ./c/createcd/createcd.exe
	cd ./c/cddb && make
	-cd ./help && make

# NLV resource dll
nlv:
	-cd ./help && make
	-cd ./res && make nlv
	-cd ./prog_tutorial && ipfc -i mediafldrprog.ipf
	cd ./c/cddb && make nlv

#force a new build number whenever the mediafolder is rebuilt
#build.h:
#		-cd ../. && make bld

#######################################
#
# Compile 
#
#######################################
$(MFLDROBJDIR)/%.obj:		$(CDIR)/mediafldr/%.cpp $(INC)\mediafolder.hh $(INC)\mediafolderinc.h $(INC)/cwaudio.hh $(INC)/cwmmdataf.hh
	$(COMPILE)

$(CDFLDROBJDIR)/%.obj:	$(CDIR)/cdfldr/%.cpp  $(INC)\cdfolder.hh $(INC)/cwcdtrack.hh
	$(COMPILE)

$(M3UFLDROBJDIR)/%.obj:	$(CDIR)/m3ufolder/%.cpp  $(INC)\m3ufolder.hh
	$(COMPILE)

$(AUDSHWOBJDIR)/%.obj:	$(CDIR)/audioshadow/%.cpp $(INC)\audioshadow.hh
	$(COMPILE)

$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c
	$(COMPILE)

#except.c needs other compiler flags
$(OBJDIR)/except.obj:	$(HELPERDIR)/except.cpp
	icc $(CPPFLAGS) /Sm /C+ -I$(INC) /Fo$@ $<

$(OBJDIR)/%.obj:	$(HELPERDIR)/%.cpp
	$(COMPILE) 

$(HELPERDIR)/%.obj:	./helper/%.cpp $(INC)\mediafolder.hh
	$(COMPILE)

mediafolder.dll:	$(OBJECTS) progfldr.lib $(INC)\mediafolderres.h ./res/mediafolder.res
		-cd ../. && make bld
		@create_mfldr_def.CMD mediafolder.DEF
		icc -Ge- /Q+ /D$(ECS) /FM $(OBJECTS) progfldr.lib $(LIBS) mediafolder.def
		rc ./res/mediafolder.res $@ 
		MAPSYM mediafolder

mediafolder_en.dll:	
		-cd ./res && make

mediafolder_de.dll:
		-cd ./res && make

mediafolder_nl.dll:
		-cd ./res && make

mediafolder_it.dll:
		-cd ./res && make

./res/mediafolder.res:	./res/mediafolder.rc ../build.inc
		-cd ./res && make

./c/createcd/createcd.exe:	./c/createcd/createcd.c
	cd ./c/createcd && make


prog_tutorial\mediafldrprog.inf:	prog_tutorial\mediafldrprog.ipf
	-cd ./prog_tutorial && make
#	-cd ./prog_tutorial && ipfc -i mediafldrprog.ipf

./help/mediafldr.hlp:	./help/mediafldr.ipf ./help/mediafldr049.ipf \
			./help/mediafldr031.ipf ./help/mediafldr039.ipf
	-cd ./help && make

#######################################
#
# Create *.hh files from IDL files
#
#######################################
$(INC)/cwaudio.hh: ../classes/idl/idl_audio/cwaudio.idl
	sc -shh -S 64000  -I $(IDLDIR)/idl_mm  -dinclude -m noqualifytypes $<

$(INC)/cwcdtrack.hh: ../classes/idl/idl_audio/cwcdtrack.idl
	sc -shh -S 64000  -I $(IDLDIR)/idl_audio -I $(IDLDIR)/idl_mm -dinclude -m noqualifytypes $<

$(INC)/cwmmdataf.hh:	../classes/idl/idl_mm/cwmmdataf.idl
	sc -shh -S 64000  -I $(IDLDIR)/idl_mm -dinclude -m noqualifytypes $<


#######################################
#
# Clean directories
#
#######################################
clean:
	-rm *.obj
	-rm *.dll
	-rm *.hlp
	-rm *.exe
	-rm *.map
	-rm *.sym
	-cd prog_tutorial && rm *.inf
	-cd ./res && make clean
	-cd ./obj && rm *.obj
	-cd ./lib && rm *.obj
	-cd ./c && make clean
	-cd ./help && make clean
	-rm mediafolder.def

cleaner:
	-rm *.*~
	-rm *.flc
	-cd ./c/helper && make cleaner
	-cd ./res && make cleaner
	-cd ./c && make cleaner
	-cd ./include && rm *.*~
	-cd ./help && make clean

german:
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\bin
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\DLL
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\Doc
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\Skripte
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\FreeDB
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\FreeDB\logfiles
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\FreeDB\cddbdata
	cp prog_tutorial/mediafldrprog.inf ../../$(PKGNAME)-$(VERSION)-ger\CWMFldr-$(VERSION)-ger\Doc\mediafldr.inf 
	cp bin/*.cmd ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin/
	cp bin/*.rx ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin/
	cp bin/*.dll ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/dll/
	cp ./c/createcd/createcd.exe  ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin/
	cp ./mediafolder.dll ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin/mediafld.dll
	cp ./res/obj/mediafolder_de.dll ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin/mfldr_de.dll
	cp ./help/mediafldr049.hlp ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin/mediafld.hlp
	cp ./install/MFLDRGER.ins  ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/MFLDRGER.INS
	cp ./c/cddb/pmcddb.exe ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/FreeDB/
	cp ./c/cddb/Res/pmcddb049.dll ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/FreeDB/pmdb049.dll
	cp ./c/cddb/cddb.ini ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/FreeDB/
	cp ./install/freedbgr.ins ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/FreeDB/
	cp Skripte_ger/* ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/Skripte/
	-cd ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/bin && lxlite *
	-cd ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/DLL && lxlite *
	-cd ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/FreeDB && lxlite *

done:
	-cp doc/Readme.ger ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/
	-cp doc/FILE_ID.DIZ ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/
	-cp doc/Lizenz ../../$(PKGNAME)-$(VERSION)-ger/CWMFldr-$(VERSION)-ger/

english:
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\bin
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\DLL
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\Doc
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\Skripte
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\FreeDB
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\FreeDB\logfiles
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\FreeDB\cddbdata
	cp prog_tutorial/mediafldrprog.inf ../../$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\Doc\mediafldr.inf 
	cp bin/*.cmd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/
	cp bin/*.rx ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/
	cp bin/*.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/dll/
	cp ./c/createcd/createcd.exe  ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/
	cp ./mediafolder.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/mediafld.dll
	cp ./res/obj/mediafolder_en.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/mfldr_en.dll
	cp ./help/mediafldr.hlp ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/mediafld.hlp
	cp ./install/MEDIAFLD.ins  ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/MEDIAFLD.INS
	cp ./c/cddb/pmcddb.exe ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/
	cp ./c/cddb/Res/pmcddb001.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/pmdb001.dll
	cp ./c/cddb/cddb.ini ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/
	cp ./install/freedb.ins ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/
	cp Skripte_eng/* ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/Skripte/
	-cd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin && lxlite *
	-cd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/DLL && lxlite *
	-cd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB && lxlite *

oldEnglish:
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\bin
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\DLL
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\Doc
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\Skripte
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\FreeDB
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\FreeDB\logfiles
	-mkdir ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\FreeDB\cddbdata
	-cp prog_tutorial\mediafldrprog.inf ..\..\$(PKGNAME)-$(VERSION)-eng\CWMFldr-$(VERSION)-eng\Doc\mediafldr.inf 
	-cp doc/Readme.eng ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/
	-cp doc/FILE_ID.DIZ ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/
	-cp doc/License ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/
	-cp bin/*.cmd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/
	-cp bin/*.rx ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/
	-cp bin/*.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/dll/
	-cp Skripte_eng/* ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/Skripte/
	-cp ./help/mediafldr.hlp ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/mediafld.hlp
	-cp ./mediafolder.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/mediafld.dll
	-cp ./mediafolder001.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin/media001.dll
	-cp ./install/MEDIAFLD.INS  ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/
	-cp ./install/createcd.exe  ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin
	-cp cddb/pmcddb.exe ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/
	-cp cddb/Res/pmcddb001.dll ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/pmdb001.dll
	-cp cddb/cddb.ini ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/
	-cp install/freedb.ins ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB/
	-cd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/bin && lxlite *
	-cd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/DLL && lxlite *
	-cd ../../$(PKGNAME)-$(VERSION)-eng/CWMFldr-$(VERSION)-eng/FreeDB && lxlite *

distribution:
#	make all
	make english
	make german

ecsdisteng:
	@echo '[32;1;m******* Packaging Media Folder... *******[0;m'
	-mkdir ..\..\$(ECSBASE)-eng\$(ECSMFLDRDIR)
	-mkdir ..\..\$(ECSBASE)-eng\$(ECSMFLDRDIR)\bin
	-mkdir ..\..\$(ECSBASE)-eng\$(ECSMFLDRDIR)\Doc
	-mkdir ..\..\$(ECSBASE)-eng\FreeDB
	-mkdir ..\..\$(ECSBASE)-eng\FreeDB\logfiles
	-mkdir ..\..\$(ECSBASE)-eng\FreeDB\cddbdata
	-cp prog_tutorial/mediafldrprog.inf ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/Doc/mediafldr.inf 
	-cp bin/*.cmd ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin/
	-cp bin/*.rx ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin/
	-cp bin/progfldr.dll ../../$(ECSBASE)-eng/dll/
	-cp ./help/mediafldr.hlp ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin/mediafld.hlp
	-cp ./mediafolder.dll ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin/mediafld.dll
	-cp ./mediafolder001.dll ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin/media001.dll
	-cp ./install/createcd.exe  ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin
	-cd ../../$(ECSBASE)-eng/$(ECSMFLDRDIR)/bin && lxlite *
#FreeDB client
	-cp cddb/pmcddb.exe ../../$(ECSBASE)-eng/FreeDB/
	-cp cddb/Res/pmcddb001.dll ../../$(ECSBASE)-eng/FreeDB/pmdb001.dll
	-cp cddb/cddb.ini ../../$(ECSBASE)-eng/FreeDB/
	-cd ../../$(ECSBASE)-eng/FreeDB && lxlite *


ecsdistribution:
	cd cddb && make
	make ecsdisteng
