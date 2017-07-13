# Makefile

include ../make.inc

DISTNAME	=	cwaudio-classes-$(VERSION)
CC           = icc

LDFLAGS =	-Ge- /Gm+ -G4 /Ss -Gl /O+ -Oc+ -Ol-  /Q+
#-Wcnd /Weff /Wini /Wpar /Wpro /Wuse /Wrea
#CFLAGS       =    /Wuse /Wrea -Ge- /Gm+ -G4 /Ss -Gl /O+ -Oc+ -Ol- /Q+ /C
CFLAGS       =    -Ge- /Gm+ -G4 /Ss -Gl /O+ -Oc+ -Ol- /Q+ /C 

LIBS		=	somtk.lib mmPM2.lib rexx.lib
DEFS		= 

MOREINC	=	./include -I./inc -I../common_functions/include
CDIR	=	./c
IDLDIR	=	./idl
OBJDIR	=	./obj
# Object files of common functions are going here
LIBDIR	=	./lib
#Where the resource dlls are going 
RESDLLDIR	=	./RES/dll


#######################################
#
# Define compile command
#
#######################################
COMPILE		=	$(CC) -I $(MOREINC) $(CFLAGS)  /Fo$@ $<

#Audio classes
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



#######################################
#
# Define objects to build
#
#######################################

# These are the object files containing general functions
LIBOBJS	=	$(LIBDIR)/menu_funcs.obj \
		$(LIBDIR)/message.obj \
		$(LIBDIR)/ea_funcs.obj \
		$(LIBDIR)/som_wps_funcs.obj \
		$(LIBDIR)/sys_funcs.obj

#Audio objects
OBJECTS	=	$(OBJDIR)/cwmp3.obj \
			$(OBJDIR)/cwogg.obj \
			$(OBJDIR)/cwcdtrack.obj \
			$(OBJDIR)/cwaudio.obj \
			$(OBJDIR)/mmwav.obj \
			$(OBJDIR)/cwaif.obj \
			$(OBJDIR)/cwau.obj \
			$(OBJDIR)/cw_au.obj \
			$(OBJDIR)/cwiff.obj \
			$(OBJDIR)/cwsndf.obj \
			$(OBJDIR)/cwvoc.obj \
			$(OBJDIR)/cwmidi.obj \
			$(OBJDIR)/cwastream.obj \
			$(OBJDIR)/cwm3u.obj \
			$(OBJDIR)/cwflac.obj

VID_OBJECTS	=	$(OBJDIR)/cwvideo.obj \
			$(OBJDIR)/cwmpg.obj \
			$(OBJDIR)/cwmov.obj \
			$(OBJDIR)/cwfli.obj \
			$(OBJDIR)/cwavi.obj

IMG_OBJECTS	=	$(OBJDIR)/cwimage.obj \
			$(OBJDIR)/cwavc.obj \
			$(OBJDIR)/cwdib.obj \
			$(OBJDIR)/cwgif.obj \
			$(OBJDIR)/cwjpg.obj \
			$(OBJDIR)/cwpcd.obj \
			$(OBJDIR)/cwpcx.obj \
			$(OBJDIR)/cwtga.obj \
			$(OBJDIR)/cwtiff.obj \
			$(OBJDIR)/cwmmotion.obj \
			$(OBJDIR)/cwbmp.obj \
			$(OBJDIR)/bitmap.obj \
			$(OBJDIR)/cwpng.obj

MM_OBJECTS	=	$(OBJDIR)/cwmmdataf.obj \
			$(OBJDIR)/cwmmdisk.obj \
			$(OBJDIR)/mmfolder.obj
 
COMMON_OBJECTS	=	$(OBJDIR)/except.obj \
				$(OBJDIR)/message.obj \
				$(OBJDIR)/cd.obj \
				$(OBJDIR)/mmrexx.obj \
				$(OBJDIR)/helper.obj \
				$(OBJDIR)/som_and_wps.obj

ALL_OBJECTS	=	$(MM_OBJECTS) $(OBJECTS) $(VID_OBJECTS) $(IMG_OBJECTS) \
			$(COMMON_OBJECTS) $(LIBOBJS)


HLP_FILES	=	./Help/de/cwmm049.hlp \
			./Help/en/cwmm.hlp \
			./Help/nl/cwmm031.hlp \
			./Help/it/cwmm039.hlp


#######################################
#
# Define *.ih files.
#
#######################################
MM_IHFILES	=	./include/cwmmdataf.ih ./include/cwmmdisk.ih ./include/mmfolder.ih

IHFILES	=		./include/cwmp3.ih \
			./include/cwogg.ih \
			./include/cwaudio.ih \
			./include/cwcdtrack.ih  \
			./include/mmwav.ih \
			./include/cwaif.ih \
			./include/cwau.ih \
			./include/cw_au.ih \
			./include/cwiff.ih \
			./include/cwsndf.ih \
			./include/cwvoc.ih \
			./include/cwastream.ih \
			./include/cwmidi.ih \
			./include/cwm3u.ih \
			./include/cwflac.ih

VID_IHFILES	=	./include/cwvideo.ih \
			./include/cwmpg.ih \
			./include/cwmov.ih \
			./include/cwfli.ih \
			./include/cwavi.ih

IMG_IHFILES	=	./include/wpimage.ih \
			./include/cwimage.ih \
			./include/cwavc.ih \
			./include/cwdib.ih \
			./include/cwgif.ih \
			./include/cwjpg.ih \
			./include/cwpcd.ih \
			./include/cwpcx.ih \
			./include/cwtga.ih \
			./include/cwtiff.ih \
			./include/cwmmotion.ih \
			./include/cwbmp.ih \
			./include/cwpng.ih

#######################################
#
# Define targets to build
#
#######################################
all: cwmm.dll $(HLP_FILES) 
	cd ./mm-progs && make


RESDLL	:

cwmm.dll: $(MM_IHFILES) $(IHFILES) $(VID_IHFILES) $(IMG_IHFILES) \
	$(ALL_OBJECTS)
	@create_cwmm_def.CMD ./c/cwmm.def
	$(CC) $(LDFLAGS) /L  /Fm$(basename $@)  /Fe$@ $(ALL_OBJECTS) $(LIBS) ./c/cwmm.def
#Add icons to DLL
	cd ./res && make
	rc ./res/common_res/cwaudio.res cwmm.dll
	MAPSYM $(basename $@)

###################################
#
#  Rules for generating online help
#
###################################
# German
./Help/de/cwmm049.hlp:	./Help/de/cwmm049.ipf 
	cd ./Help/de && make 	

# English
./Help/en/cwmm.hlp:	./Help/en/cwmm.ipf
	cd ./Help/en && make

# Dutch
./Help/nl/cwmm031.hlp:	./Help/nl/cwmm031.ipf
	cd ./Help/nl && make

# Italian
./Help/it/cwmm039.hlp:	./Help/it/cwmm039.ipf
	cd ./Help/it && make

###################################
#
#  Create all the *.ih files from the IDL files        #
#
###################################

##### Create *.ih files for base MM classes ####
./include/%.ih:	$(IDLDIR)/idl_mm/%.idl
	$(MM_SCCOMPILEC)
	$(MM_SCCOMPILEIH)

#### Create *.ih files for audio classes ####
./include/%.ih:	$(IDLDIR)/idl_audio/%.idl
	$(AUD_SCCOMPILEC)
	$(AUD_SCCOMPILEIH)

#### Create *.ih files for video classes ####
./include/%.ih:	$(IDLDIR)/idl_video/%.idl
	$(VID_SCCOMPILEC)
	$(VID_SCCOMPILEIH)

#### Create *.ih files for image classes ####
./include/wpimage.ih:	$(IDLDIR)/idl_image/wpimage.idl
	$(IMG_SCCOMPILEIH)

./include/cwimage.ih:	$(IDLDIR)/idl_image/cwimage.idl
	$(IMG_SCCOMPILEIH)
	$(IMG_SCCOMPILEC)
	sc -shh -I $(IDLDIR)/idl_image -S128000 -dinclude -m noqualifytypes $<

./include/%.ih:	$(IDLDIR)/idl_image/%.idl
	$(IMG_SCCOMPILEC)
	$(IMG_SCCOMPILEIH)

###################################
#
#    Rules for compiling
#
###################################

# Compile base MM classes
$(OBJDIR)/%.obj:	$(CDIR)/c_mm/%.c ./include/%.ih
	$(COMPILE)

# Compile audio MM classes
$(OBJDIR)/%.obj:	$(CDIR)/c_audio/%.c ./include/%.ih
	$(COMPILE)

# Compile video MM classes
$(OBJDIR)/%.obj:	$(CDIR)/c_video/%.c ./include/%.ih
	$(COMPILE)

# Compile image MM classes
$(OBJDIR)/%.obj:	$(CDIR)/c_image/%.c ./include/%.ih
	$(COMPILE)

$(OBJDIR)/bitmap.obj:	$(CDIR)/c_image/bitmap.c
	$(COMPILE)

# Compile common functions
$(OBJDIR)/except.obj:	$(CDIR)/c_common/except.c ./inc/omf.h ./inc/sym.h
	$(COMPILE)

$(OBJDIR)/%.obj:	$(CDIR)/c_common/%.c
	$(COMPILE)

# Compile lib files containg generic function
$(LIBDIR)/%.obj:	$(COMMONSRC)/%.c
	$(COMPILE)

###################################
#
#  Rules for cleaning the tree
#
###################################

clean:
	-cd $(OBJDIR) && rm *.obj
	-cd ./include && rm *.ih
	-cd ./include && rm *.h
	-cd ./include && rm *.hh
	-cd $(LIBDIR) && rm *.obj
	-rm *.dll
	-rm *.map
	-rm *.sym
	-cd ./Help/de && make clean
	-cd ./Help/en && make clean
	-cd ./Help/nl && make clean
	-cd ./Help/it && make clean
	-cd ./res && make clean
	-cd ./mm-progs && make clean


cleaner:
	-cd ./c && make clean
	-cd ./common_functions && make cleaner
	-cd ./mm-progs && make cleaner
	-cd ./Help/de && make cleaner
	-cd ./Help/en && make cleaner
	-cd ./Help/nl && make cleaner
	-cd ./Help/it && make cleaner
	-cd ./res && make cleaner
	-cd ./mm-progs && make cleaner
	-cd ./include && rm *.*~
	-cd ./inc && rm *.*~
	-cd $(IDLDIR)/idl_audio && rm *.*~
	-cd $(IDLDIR)/idl_image && rm *.*~
	-cd $(IDLDIR)/idl_mm && rm *.*~
	-cd $(IDLDIR)/idl_video && rm *.*~

distribution:
	make

