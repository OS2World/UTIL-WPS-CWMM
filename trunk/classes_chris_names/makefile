# Makefile

include ../make.inc

DISTNAME	=	cwaudio-classes-$(VERSION)
CC           = icc

LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+ 
CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /C
LIBS		=	somtk.lib mmPM2.lib rexx.lib
DEFS		= 

MOREINC	=	./include -I./inc
CDIR	=	./c
IDLDIR	=	./idl

COMPILE		=	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<

AUD_SCCOMPILEC	=	sc -I $(IDLDIR)/idl_audio -I $(IDLDIR)/idl_mm -p -r -sc -d$(CDIR)/c_audio -maddstar -mnoint -S128000 -C128000  $<
AUD_SCCOMPILEIH	=	sc -I $(IDLDIR)/idl_audio -I $(IDLDIR)/idl_mm -p -r -sih";"h -dinclude -maddstar -mnoint -S128000 -C128000  $<

#Video
VID_SCCOMPILEC	=	sc -I $(IDLDIR)/idl_video -p -r -sc -d$(CDIR)/c_video -maddstar -mnoint -S128000 -C128000  $<
VID_SCCOMPILEIH	=	sc -I $(IDLDIR)/idl_video -p -r -sih";"h -dinclude -maddstar -mnoint -S128000 -C128000  $<

#image
IMG_SCCOMPILEC	=	sc -I $(IDLDIR)/idl_image -p -r -sc -d$(CDIR)/c_image -maddstar -mnoint -S128000 -C128000  $<
IMG_SCCOMPILEIH	=	sc -I $(IDLDIR)/idl_image -p -r -sih";"h -dinclude -maddstar -mnoint -S128000 -C128000  $<

#base classes
MM_SCCOMPILEC	=	sc -I $(IDLDIR)/idl_mm -p -r -sc -d$(CDIR)/c_mm -maddstar -mnoint -S128000 -C128000  $<
MM_SCCOMPILEIH	=	sc -I $(IDLDIR)/idl_mm -p -r -sih";"h -dinclude -maddstar -mnoint -S128000 -C128000  $<

OBJECTS	= ./obj/cwaudio.obj ./obj/cwmp3.obj ./obj/cwogg.obj ./obj/cwcdtrack.obj \
	./obj/mmwav.obj \
	./obj/cwaif.obj ./obj/cwau.obj \
	./obj/cw_au.obj ./obj/cwiff.obj \
	./obj/cwsndf.obj ./obj/cwvoc.obj \
	./obj/cwmidi.obj ./obj/cwastream.obj

VID_OBJECTS	= ./obj/cwvideo.obj ./obj/cwmpg.obj ./obj/cwmov.obj \
			./obj/cwfli.obj ./obj/cwavi.obj

IMG_OBJECTS	=	./obj/cwimage.obj \
		./obj/cwavc.obj ./obj/cwdib.obj ./obj/cwgif.obj \
		./obj/cwjpg.obj ./obj/cwpcd.obj ./obj/cwpcx.obj \
		./obj/cwtga.obj ./obj/cwtiff.obj ./obj/cwmmotion.obj ./obj/cwbmp.obj \
		./obj/bitmap.obj

MM_OBJECTS	=	./obj/cwmmdataf.obj ./obj/cwmmdisk.obj ./obj/mmfolder.obj
 
COMMON_OBJECTS	=	./obj/except.obj ./obj/message.obj ./obj/cd.obj ./obj/mmrexx.obj ./obj/helper.obj \
			./obj/som_and_wps.obj

RESOBJ	=	./Res/001/cwaudio.rc ./Res/001/cwaudio.dlg ./Res/001/cwvideo.rc \
	./Res/049/cwaudio049.rc ./Res/049/cwvideo049.rc ./Res/049/cwaudio049.dlg \
	./inc/cwmmres.h

ALL_OBJECTS	=	$(MM_OBJECTS) $(OBJECTS) $(VID_OBJECTS) $(IMG_OBJECTS) \
		$(COMMON_OBJECTS)

MM_IHFILES	=	./include/cwmmdataf.ih ./include/cwmmdisk.ih ./include/mmfolder.ih

IHFILES	=	 ./include/mmaudio.ih \
		./include/cwaudio.ih ./include/cwmp3.ih ./include/cwogg.ih \
		./include/cwcdtrack.ih  \
		./include/mmwav.ih \
		./include/cwaif.ih ./include/cwau.ih ./include/cw_au.ih \
		./include/cwiff.ih ./include/cwsndf.ih ./include/cwvoc.ih \
		./include/cwmidi.ih ./include/cwastream.ih

VID_IHFILES	=	./include/cwvideo.ih ./include/cwmpg.ih ./include/cwmov.ih \
			./include/cwfli.ih ./include/cwavi.ih

IMG_IHFILES	=	./include/wpimage.ih ./include/cwimage.ih \
		./include/cwavc.ih ./include/cwdib.ih ./include/cwgif.ih \
		./include/cwjpg.ih ./include/cwpcd.ih ./include/cwpcx.ih \
		./include/cwtga.ih ./include/cwtiff.ih ./include/cwmmotion.ih ./include/cwbmp.ih

all: cwmm.dll ./Help/049/cwmm049.hlp ./Help/001/cwmm.hlp
# convert.exe

.idl.ih:

.c.obj:
	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<

convert.exe:	$(CDIR)\c\convert.c
	gcc $< -los2me

./Help/049/cwmm049.hlp:
	cd ./Help/049 && make 	

./Help/001/cwmm.hlp:
	cd ./Help/001 && make

cwmm.dll: $(MM_IHFILES) $(IHFILES) $(VID_IHFILES) $(IMG_IHFILES) \
	$(ALL_OBJECTS) $(RESOBJ)
	cd ./Res/001 && make
	cd ./Res/049 && make
	$(CC) $(LDFLAGS) /L /Fm$(basename $@)  /Fe$@ $(ALL_OBJECTS) $(LIBS) ./c/cwmm.def
	rc ./Res/049/cwaudio049.res cwmm.dll
	MAPSYM $(basename $@)


./include/cwmmdataf.ih:	$(IDLDIR)/idl_mm/cwmmdataf.idl
	$(MM_SCCOMPILEC)
	$(MM_SCCOMPILEIH)

./include/cwmmdisk.ih:	$(IDLDIR)/idl_mm/cwmmdisk.idl
	$(MM_SCCOMPILEC)
	$(MM_SCCOMPILEIH)


./include/mmfolder.ih:	$(IDLDIR)/idl_mm/mmfolder.idl
	$(MM_SCCOMPILEC)
	$(MM_SCCOMPILEIH)

./include/mmaudio.ih:	$(IDLDIR)/idl_audio/mmaudio.idl
	$(AUD_SCCOMPILEIH)

./include/cwaudio.ih:	$(IDLDIR)/idl_audio/cwaudio.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)
#	sc -shh -I ./idl -dinclude -m noqualifytypes $<

./include/cwcdtrack.ih:	$(IDLDIR)/idl_audio/cwcdtrack.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)
#	sc -shh -I ./idl -dinclude -m noqualifytypes $<

./include/cwmp3.ih:	$(IDLDIR)/idl_audio/cwmp3.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwogg.ih:	$(IDLDIR)/idl_audio/cwogg.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/mmwav.ih:	$(IDLDIR)/idl_audio/mmwav.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwaif.ih:	$(IDLDIR)/idl_audio/cwaif.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwau.ih:	$(IDLDIR)/idl_audio/cwau.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cw_au.ih:	$(IDLDIR)/idl_audio/cw_au.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwiff.ih:	$(IDLDIR)/idl_audio/cwiff.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwsndf.ih:	$(IDLDIR)/idl_audio/cwsndf.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwvoc.ih:	$(IDLDIR)/idl_audio/cwvoc.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwmidi.ih:	$(IDLDIR)/idl_audio/cwmidi.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

./include/cwastream.ih:	$(IDLDIR)/idl_audio/cwaudiostream.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)


./obj/cwmmdataf.obj:	$(CDIR)/c_mm/cwmmdataf.c ./include/cwmmdataf.ih
	$(COMPILE)

./obj/cwmmdisk.obj:	$(CDIR)/c_mm/cwmmdisk.c ./include/cwmmdisk.ih
	$(COMPILE)

./obj/mmfolder.obj:	$(CDIR)/c_mm/mmfolder.c ./include/mmfolder.ih
	$(COMPILE)

./obj/cwaudio.obj:	$(CDIR)/c_audio/cwaudio.c ./include/cwaudio.ih
	$(COMPILE)

./obj/cwmp3.obj:	$(CDIR)/c_audio/cwmp3.c ./include/cwmp3.ih
	$(COMPILE)

./obj/cwogg.obj:	$(CDIR)/c_audio/cwogg.c ./include/cwogg.ih
	$(COMPILE)

./obj/mmwav.obj:	$(CDIR)/c_audio/mmwav.c ./include/mmwav.ih
	$(COMPILE)

./obj/cwaif.obj:	$(CDIR)/c_audio/cwaif.c ./include/cwaif.ih
	$(COMPILE)

./obj/cwau.obj:		$(CDIR)/c_audio/cwau.c ./include/cwau.ih
	$(COMPILE)

./obj/cw_au.obj:	$(CDIR)/c_audio/cw_au.c ./include/cw_au.ih
	$(COMPILE)

./obj/cwiff.obj:	$(CDIR)/c_audio/cwiff.c ./include/cwiff.ih
	$(COMPILE)

./obj/cwsndf.obj:	$(CDIR)/c_audio/cwsndf.c ./include/cwsndf.ih
	$(COMPILE)

./obj/cwvoc.obj:	$(CDIR)/c_audio/cwvoc.c ./include/cwvoc.ih
	$(COMPILE)

./obj/cwmidi.obj:	$(CDIR)/c_audio/cwmidi.c ./include/cwmidi.ih
	$(COMPILE)

./obj/cwcdtrack.obj:	$(CDIR)/c_audio/cwcdtrack.c ./include/cwcdtrack.ih
	$(COMPILE)

./obj/cwastream.obj:	$(CDIR)/c_audio/cwastream.c ./include/cwastream.ih
	$(COMPILE)


./include/cwvideo.ih:	$(IDLDIR)/idl_video/cwvideo.idl
	$(VID_SCCOMPILEC)
	$(VID_SCCOMPILEIH)

./include/cwmpg.ih:	$(IDLDIR)/idl_video/cwmpg.idl
	$(VID_SCCOMPILEC)
	$(VID_SCCOMPILEIH)

./include/cwmov.ih:	$(IDLDIR)/idl_video/cwmov.idl
	$(VID_SCCOMPILEC)
	$(VID_SCCOMPILEIH)

./include/cwfli.ih:	$(IDLDIR)/idl_video/cwfli.idl
	$(VID_SCCOMPILEC)
	$(VID_SCCOMPILEIH)

./include/cwavi.ih:	$(IDLDIR)/idl_video/cwavi.idl
	$(VID_SCCOMPILEC)
	$(VID_SCCOMPILEIH)

./obj/cwvideo.obj:	$(CDIR)/c_video/cwvideo.c ./include/cwvideo.ih
	$(COMPILE)

./obj/cwmpg.obj:	$(CDIR)/c_video/cwmpg.c ./include/cwmpg.ih
	$(COMPILE)

./obj/cwmov.obj:	$(CDIR)/c_video/cwmov.c ./include/cwmov.ih
	$(COMPILE)

./obj/cwfli.obj:	$(CDIR)/c_video/cwfli.c ./include/cwfli.ih
	$(COMPILE)

./obj/cwavi.obj:	$(CDIR)/c_video/cwavi.c ./include/cwavi.ih
	$(COMPILE)



./include/wpimage.ih:	$(IDLDIR)/idl_image/wpimage.idl
	$(IMG_SCCOMPILEIH)

./include/cwimage.ih:	$(IDLDIR)/idl_image/cwimage.idl
	$(IMG_SCCOMPILEIH)
	$(IMG_SCCOMPILEC)
	sc -shh -I $(IDLDIR)/idl_image -dinclude -m noqualifytypes $<

./include/cwavc.ih:	$(IDLDIR)/idl_image/cwavc.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwdib.ih:	$(IDLDIR)/idl_image/cwdib.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwgif.ih:	$(IDLDIR)/idl_image/cwgif.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwjpg.ih:	$(IDLDIR)/idl_image/cwjpg.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwpcd.ih:	$(IDLDIR)/idl_image/cwpcd.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwpcx.ih:	$(IDLDIR)/idl_image/cwpcx.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwtga.ih:	$(IDLDIR)/idl_image/cwtga.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwtiff.ih:	$(IDLDIR)/idl_image/cwtif.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwmmotion.ih:	$(IDLDIR)/idl_image/cw_im.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./include/cwbmp.ih:	$(IDLDIR)/idl_image/cwbmp.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

./obj/cwimage.obj:	$(CDIR)/c_image/cwimage.c ./include/cwimage.ih
	$(COMPILE)

./obj/cwavc.obj:	$(CDIR)/c_image/cwavc.c ./include/cwavc.ih
	$(COMPILE)

./obj/cwdib.obj:	$(CDIR)/c_image/cwdib.c ./include/cwdib.ih
	$(COMPILE)

./obj/cwgif.obj:	$(CDIR)/c_image/cwgif.c ./include/cwgif.ih
	$(COMPILE)

./obj/cwjpg.obj:	$(CDIR)/c_image/cwjpg.c ./include/cwjpg.ih
	$(COMPILE)

./obj/cwpcd.obj:	$(CDIR)/c_image/cwpcd.c ./include/cwpcd.ih
	$(COMPILE)

./obj/cwpcx.obj:	$(CDIR)/c_image/cwpcx.c ./include/cwpcx.ih
	$(COMPILE)

./obj/cwtga.obj:	$(CDIR)/c_image/cwtga.c ./include/cwtga.ih
	$(COMPILE)

./obj/cwtiff.obj:	$(CDIR)/c_image/cwtiff.c ./include/cwtiff.ih
	$(COMPILE)

./obj/cwmmotion.obj:	$(CDIR)/c_image/cwmmotion.c ./include/cwmmotion.ih
	$(COMPILE)

./obj/cwbmp.obj:	$(CDIR)/c_image/cwbmp.c ./include/cwbmp.ih
	$(COMPILE)

./obj/bitmap.obj:	$(CDIR)/c_image/bitmap.c
	$(COMPILE)

./obj/except.obj:	$(CDIR)/c_common/except.c ./inc/omf.h ./inc/sym.h
	$(COMPILE)

./obj/message.obj:	$(CDIR)/c_common/message.c
	$(COMPILE)

./obj/cd.obj:	$(CDIR)/c_common/cd.c
	$(COMPILE)

./obj/mmrexx.obj:	$(CDIR)/c_common/mmrexx.c
	$(COMPILE)

./obj/helper.obj:	$(CDIR)/c_common/helper.c
	$(COMPILE)

./obj/som_and_wps.obj:	$(CDIR)/c_common/som_and_wps.c
	$(COMPILE)

clean:
	cd ./obj && rm *.obj
	-rm *.dll
	-rm *.map
	cd ./Res/049 && make clean
	cd ./Res/001 && make clean
	cd ./Help/049 && make clean
	cd ./Help/049 && make clean


cleaner:
	-cd ./c && make clean

german:

english:

distribution:
	make
	-make german
	make english

