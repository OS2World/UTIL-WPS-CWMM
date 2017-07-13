# Makefile

include ../make.inc

DISTNAME	=	cwaudio-classes-$(VERSION)
CC           = icc

LDFLAGS =	/Gm+ -G4 /Ss -Gl /O+ -Oc+ -Ol-  /Q+
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



#######################################
#
# Define objects to build
#
#######################################

# These are the object files containing general functions
LIBOBJS	=	$(LIBDIR)/menu_funcs.obj $(LIBDIR)/message.obj $(LIBDIR)/ea_funcs.obj $(LIBDIR)/som_wps_funcs.obj \
			$(LIBDIR)/sys_funcs.obj

#Audio objects
OBJECTS	=	$(OBJDIR)/cwaudio.obj $(OBJDIR)/cwmp3.obj \
			$(OBJDIR)/cwvoc.obj \
			$(OBJDIR)/cwm3u.obj $(OBJDIR)/cwflac.obj


#######################################
#
# Define targets to build
#
#######################################
all: mmtest.exe


RESDLL	:
#$(RESDLLDIR)/mmres_de.dll $(RESDLLDIR)/mmres_en.dll

mmtest.exe: mmtest.c
	$(CC) $(LDFLAGS) $< /L /Fm$(basename $@)  /Fe$@  $(LIBS)

#	$(CC) $(LDFLAGS) /L /Fm$(basename $@)  /Fe$@ $(ALL_OBJECTS) $(LIBS) ./c/cwmm.def




###################################
#
#  Rules for cleaning the tree
#
###################################

clean:
	-rm *.obj
	-rm *.exe




