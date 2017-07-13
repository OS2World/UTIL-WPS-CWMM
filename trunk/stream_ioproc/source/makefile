#/*
# * Copyright (c) Chris Wohlgemuth 2002 
# * All rights reserved.
# *
# * http://www.geocities.com/SiliconValley/Sector/5785/
# * http://www.os2world.com/cdwriting
# *
# * Redistribution and use in source and binary forms, with or without
# * modification, are permitted provided that the following conditions
# * are met:
# * 1. Redistributions of source code must retain the above copyright
# *    notice, this list of conditions and the following disclaimer.
# * 2. Redistributions in binary form must reproduce the above copyright
# *    notice, this list of conditions and the following disclaimer in the
# *    documentation and/or other materials provided with the distribution.
# * 3. The authors name may not be used to endorse or promote products
# *    derived from this software without specific prior written permission.
# *
# * THIS SOFTWARE IS PROVIDED ``AS IS'' AND
# * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
# * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
# * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# * SUCH DAMAGE.
# *
# */
#################################################################################
#
# Makefile for JPG IOProc
#
#   Generates
#         JPEGIO.dll       - JPG Image IOproc
#
# Copyright Chris Wohlgemuth 2002
#
# http://www.os2world.com/cdwriting
# http://www.geocities.com/SiliconValley/Sector/5785/
#
# Use VAC++ V3.08 and gmake to compile
#
#################################################################################

.SUFFIXES: .exe .obj .asm .inc .def .lrf .lst .sym .map .c .h .lib .dlg .txt .cod  \
           .res .rc .l$(LANG) .dll


#################################
CC=icc
CFLAGS       =   -Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol- /Q+ /C

#################################
# Definitions for linker
# LIBRARIES:
#  MMPM2  == MMPM2 Library
#################################
MAPSYM=mapsym
LIBS=	MMPM2.LIB
LDFLAGS =	-Ge- /Gm+ -G4 /ss -Gl /O+ -Oc+ -Ol-  /Q+ /B"/NOE"


#################################
# Object file lists
#################################
OBJS =	streamproc.obj streamfunc.obj

#################################
# Target Descriptions
#################################
all:	STREAMIO.DLL testproc.exe
#iotest.exe 

testproc.exe:	testproc.c
	icc /ss testproc.c $(LIBS)

iotest.exe:	iotest.c
	icc iotest.c $(LIBS)

STREAMIO.DLL:		$(OBJS)
	$(CC) $(LDFLAGS)  /Fm$(basename $@) /Fe$@  $(OBJS) $(LIBS) streamio.def
	rc streamproc.rc STREAMIO.DLL
	mapsym streamio.map



streamproc.obj:	streamproc.c
	$(CC) $(CFLAGS) /Fo$@ $<

streamfunc.obj:	streamfunc.c
	$(CC) $(CFLAGS) /Fo$@ $<

clean:
	-rm *.obj
	-rm *.dll
	-rm *.res
	-rm *.*~
	-rm *.map
	-rm *.sym