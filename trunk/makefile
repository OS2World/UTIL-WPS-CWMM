# Makefile

include	./make.inc

LANG	=	ger

CLASSESNAME	= cwmm
#JPGIONAME	= jpg-io-procedure
#JPGIOVERSION	= 0_1_0


OBJECTS	= 

all:
	@echo ''
	@echo '[32;1;m********** Compiling classes... ***********[0;m'
	@echo ''
	cd ./classes && make
	@echo ''
	@echo '[32;1;m***** Compiling JPEG IO procedure... ******[0;m'
	@echo ''
	cd ./JPGPROC/source && make
	@echo ''
	@echo '[32;1;m******** Compiling media folder... ********[0;m'
	@echo ''
	cd ./mediafolder && make

nlv_tst:
#this is a target used to test NLV building outside the source tree
# Will eventually superseed nlv target.
	cd ./nlv && make

nlv:
	cd ./classes && make nlv
	cd ./mediafolder && make nlv
	@echo ''
	@echo '[32;1;m******** Copying NLV files to ./nlv... ********[0;m'
	@echo ''
#This is just for your convenience...
	-cp ./classes/res/dll/mmres_en.dll ./nlv-files/
	-cp ./classes/res/dll/mmres_de.dll ./nlv-files/
	-cp ./classes/help/de/cwmm049.hlp ./nlv-files/
	-cp ./classes/help/en/cwmm.hlp ./nlv-files/
	-cp ./mediafolder/help/*.hlp ./nlv-files/
	-cp ./mediafolder/res/obj/*.dll ./nlv-files/
	-cp ./mediafolder/c/cddb/*.dll ./nlv-files/
	-cp ./mediafolder/prog_tutorial/*.inf ./nlv-files/

clean:
	-cd ./classes && make clean
	-cd ./JPGPROC/source && make clean
	-cd ./nlv-files && rm *
	cd ./mediafolder && make clean

cleaner:
	-cd ./classes && make cleaner
	-cd ./JPGPROC/source && make cleaner
	cd ./mediafolder && make cleaner

german:
	@echo ''
	@echo '[32;1;m** Creating german release package... ***[0;m'
	@echo ''
# base packet
	-cd ../. && mkdir $(PKGNAME)-$(VERSION)-ger
	cp ./install-global/FILE_ID.DIZ ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/ger/Liesmich.txt ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/ger/Lizenz ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/ger/install.in_ ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/Runme1st.cmd ../$(PKGNAME)-$(VERSION)-ger/
	cp -R ./install-global/install ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/install.exe ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/cwinst2.in_ ../$(PKGNAME)-$(VERSION)-ger/
	cp ./install-global/InstIBM.CMD  ../$(PKGNAME)-$(VERSION)-ger/install/
# CW-Multimedia classes
	-cd ../$(PKGNAME)-$(VERSION)-ger && mkdir $(CLASSESNAME)-$(VERSION)-ger
#	-cd ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger && rm -r *
	-cd ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger && mkdir bin
	-cd ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger && mkdir DLL
	cp ./classes/cwmm.dll ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/
#	-cd ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger && lxlite *.dll
	cp ./classes/cwmm.sym ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/
	cp ./classes/mm-progs/audioconverter/audconv.exe ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/mm-progs/master_volume/volume.exe  ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/mm-progs/volume/classvol.exe  ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/mm-progs/imageconverter/imgconv.exe  ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/mm-progs/mmplayer/mmplayer.exe  ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/mm-progs/mmfind/mmfind.exe  ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/res/dll/mmres_DE.dll ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
#	cd ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin && lxlite *
	cp ./classes/bin/*.CMD ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/bin/*.rx ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/bin/
	cp ./classes/help/de/cwmm049.hlp ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/cwmm.hlp
	cp ./classes/doc_and_inst/ger/cwmm.ins ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/
# JPEG IO-Proc
	cp ./JPGPROC/source/JPEGIO.DLL ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/DLL/
	cp ./JPGPROC/source/JPEGIO.SYM ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/DLL/
	cp ./JPGPROC/DLL/GBM.DLL ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/DLL/
#	cd ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-ger/DLL && lxlite *.dll
#Slide-show
	cp -R ./Slide-Show ../$(PKGNAME)-$(VERSION)-ger/
	rm ../$(PKGNAME)-$(VERSION)-ger/Slide-Show/slideshoweng.ins
#Adding Media folder
	@echo ''
	@echo '[32;1;m*** Adding media folder to package... ***[0;m'
	@echo ''
	cd ./mediafolder && make german


english:
	@echo ''
	@echo '[32;1;m** Creating english release package... ***[0;m'
	@echo ''
# base packet
	-cd ../. && mkdir $(PKGNAME)-$(VERSION)-eng
	cp ./install-global/FILE_ID.DIZ ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/eng/Readme.1st ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/eng/License ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/eng/install.in_ ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/Runme1st.cmd ../$(PKGNAME)-$(VERSION)-eng/
	cp -R ./install-global/install ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/install.exe ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/cwinst2.in_ ../$(PKGNAME)-$(VERSION)-eng/
	cp ./install-global/InstIBM.CMD  ../$(PKGNAME)-$(VERSION)-eng/install/
# CW-Multimedia classes
	-cd ../$(PKGNAME)-$(VERSION)-eng && mkdir $(CLASSESNAME)-$(VERSION)-eng
#	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && rm -r *
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && mkdir bin
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && mkdir DLL
	cp ./classes/cwmm.dll ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && lxlite *.dll
	cp ./classes/cwmm.sym ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	cp ./classes/mm-progs/audioconverter/audconv.exe ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/mm-progs/master_volume/volume.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/mm-progs/volume/classvol.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/mm-progs/imageconverter/imgconv.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/mm-progs/mmplayer/mmplayer.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/mm-progs/mmfind/mmfind.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/res/dll/mmres_EN.dll ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin && lxlite *
	cp ./classes/bin/*.CMD ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/bin/*.rx ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	cp ./classes/help/en/cwmm.hlp ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/cwmm.hlp
	cp ./classes/doc_and_inst/eng/cwmm.ins ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
# JPEG IO-Proc
	cp ./JPGPROC/source/JPEGIO.DLL ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL/
	cp ./JPGPROC/source/JPEGIO.SYM ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL/
	cp ./JPGPROC/DLL/GBM.DLL ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL/
	cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL && lxlite *.dll
#Slide-show
	cp -R ./Slide-Show ../$(PKGNAME)-$(VERSION)-eng/
	rm ../$(PKGNAME)-$(VERSION)-eng/Slide-Show/slideshowger.ins
#Adding Media folder
	@echo ''
	@echo '[32;1;m*** Adding media folder to package... ***[0;m'
	@echo ''
	cd ./mediafolder && make english


old_eng:
	-cd ../. && mkdir $(PKGNAME)-$(VERSION)-eng
	-cp ./install-global/FILE_ID.DIZ ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/eng/Readme.1st ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/eng/License ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/eng/install.in_ ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/Runme1st.cmd ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/install.exe ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/cwinst2.in_ ../$(PKGNAME)-$(VERSION)-eng/
	-cp -R ./install-global/install ../$(PKGNAME)-$(VERSION)-eng/
	-cp ./install-global/patchdll.exe ../$(PKGNAME)-$(VERSION)-eng/install/
	-cp ./install-global/InstIBM.CMD  ../$(PKGNAME)-$(VERSION)-eng/install/
# CW-Multimedia classes	
	-cd ../$(PKGNAME)-$(VERSION)-eng && mkdir $(CLASSESNAME)-$(VERSION)-eng
#	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && rm -r *
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && mkdir bin
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && mkdir DLL
	-cp ./classes/cwmm.dll ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	rc ./classes/res/001/cwaudio.res ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/cwmm.dll
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && lxlite *
	-cp ./classes/cwmm.sym ../$(PKGNAME)-$(VERSION)-ger/$(CLASSESNAME)-$(VERSION)-eng/
	-cp ./classes/mm-progs/audioconverter/audconv.exe ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	rc ./classes/mm-progs/audioconverter/res/001/pmconv.res ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/audconv.exe
	-cp ./classes/mm-progs/volume/volume.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	rc ./classes/mm-progs/volume/res/001/volume.res ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/volume.exe
	-cp ./classes/mm-progs/imageconverter/imgconv.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	rc ./classes/mm-progs/imageconverter/res/001/pmconv.res ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/imgconv.exe
	-cp ./classes/mm-progs/mmplayer/mmplayer.exe  ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	rc ./classes/mm-progs/mmplayer/res/001/pmconv.res ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/mmplayer.exe
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin && lxlite *
	-cp ./classes/help/001/cwmm.hlp ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/cwmm.hlp
	-cp ./classes/doc_and_inst/eng/Readme.eng ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	-cp ./classes/doc_and_inst/eng/license ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	-cp ./classes/doc_and_inst/eng/cwmm.ins ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	-cp ./classes/doc_and_inst/FILE_ID.DIZ ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/
	-cp ./classes/doc_and_inst/installmm.cmd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin
	-cp ./classes/doc_and_inst/uninstmm.cmd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin
	-cp ./classes/bin/*.CMD ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
	-cp ./classes/bin/*.rx ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/bin/
# JPEG IO-Proc
	-cp ./JPGPROC/source/JPEGIO.DLL ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL/
	-cp ./JPGPROC/source/JPEGIO.SYM ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL/
	-cp ./JPGPROC/DLL/GBM.DLL ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL/
	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng/DLL && lxlite *.dll


distribution:
	make 
	make  german
	make  english
#mediafolder
	@echo ''
	@echo '[32;1;m*** Adding media folder to packages... ***[0;m'
	@echo ''
	cd ./mediafolder && make distribution



ecsenglish:
	@echo '[32;1;m******* Packaging MM classes... *********[0;m'
# base packet
	-cd ../. && mkdir $(ECSBASE)-eng
	-cd ../$(ECSBASE)-eng && mkdir DLL
# CW-Multimedia classes	
	-cd ../$(ECSBASE)-eng && mkdir $(ECSCLASSDIR)
#	-cd ../$(PKGNAME)-$(VERSION)-eng/$(CLASSESNAME)-$(VERSION)-eng && rm -r *
	-cd ../$(ECSBASE)-eng/$(ECSCLASSDIR) && mkdir bin
	-cp ./classes/cwmm.dll ../$(ECSBASE)-eng/$(ECSCLASSDIR)/
	rc ./classes/res/001/cwaudio.res ../$(ECSBASE)-eng/$(ECSCLASSDIR)/cwmm.dll
	-cd ../$(ECSBASE)-eng/$(ECSCLASSDIR) && lxlite *
	-cp ./classes/mm-progs/audioconverter/audconv.exe ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/
	rc ./classes/mm-progs/audioconverter/res/001/pmconv.res ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/audconv.exe
	-cp ./classes/mm-progs/volume/volume.exe  ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/
	rc ./classes/mm-progs/volume/res/001/volume.res ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/volume.exe
	-cp ./classes/mm-progs/imageconverter/imgconv.exe  ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/
	rc ./classes/mm-progs/imageconverter/res/001/pmconv.res ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/imgconv.exe
	-cd ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin && lxlite *
	-cp ./classes/help/001/cwmm.hlp ../$(ECSBASE)-eng/$(ECSCLASSDIR)/cwmm.hlp
	-cp ./classes/bin/*.CMD ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/
	-cp ./classes/bin/*.rx ../$(ECSBASE)-eng/$(ECSCLASSDIR)/bin/
# JPEG IO-Proc
	-cp ./JPGPROC/source/JPEGIO.DLL ../$(ECSBASE)-eng/DLL/
	-cp ./JPGPROC/source/JPEGIO.SYM ../$(ECSBASE)-eng/DLL/
	-cp ./JPGPROC/DLL/GBM.DLL ../$(ECSBASE)-eng/DLL/
	-cd ../$(ECSBASE)-eng/DLL && lxlite *.dll



ecsdistribution:
	@echo ''
	@echo '[36;1;m*****************************************[0;m'
	@echo 'Creating eCS multimedia class package...'
	@echo '[36;1;m*****************************************[0;m'
	@echo ''
	make -s
	make -s ecsenglish
#mediafolder
	cd ./mediafolder && make -s ecsdistribution
#Documentation for eCS
	-cp ./add2FOURMEG.SCR ../$(ECSBASE)-eng/



help:
	@echo ''
	@echo 'make:                    Compile english and german NLV.'
	@echo ''
	@echo 'make distribution:       create CW-Multimedia class package'
	@echo '                         in english and german.'
	@echo ''
	@echo 'make ecsdistribution:    create eCS Multimedia class package'
	@echo '                         to be added to eCS base system in'
	@echo '                         english and german.'

bld:
	@build.cmd
