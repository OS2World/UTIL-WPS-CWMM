#**************************************************************************
# *
# * File Name        : MAKEFILE
# *
# * Description      : Makefile for MMBrowse.exe
# *
# * Copyright        : COPYRIGHT IBM CORPORATION, 1991, 1992
# *                    Copyright Microsoft Corporation, 1990
# *                    LICENSED MATERIAL - PROGRAM PROPERTY OF IBM
# *                    REFER TO COPYRIGHT INSTRUCTION FORM#G120-2083
# *                    RESTRICTED MATERIALS OF IBM
# *
# ****************************************************************************/

CFLAGS  = /c /Ss
CDEBUG  = /Ti+ /O-

COMPILE = icc $(CFLAGS) $(CDEBUG)

LFLAGS  = /noi /map /nod /noe /ALIGN:16 /EXEPACK /M /BASE:0x10000
LDEBUG  = /codeview

LINK    = ILINK /NOFREE $(LFLAGS) $(LDEBUG)
LIBS    = CPPOM30 + OS2386 + mmpm2.lib + rexx.lib

HEADERS_MAIN = mmbrowse.h

all: mmbrowse.exe mmbrowse.hlp

mmbrowse.lnk: 	makefile
	    echo mmbrowse.obj framecontrol.obj             > mmbrowse.lnk
	    echo mmbrowse.exe            >> mmbrowse.lnk
	    echo mmbrowse.map           >> mmbrowse.lnk
	    echo $(LIBS)                      >> mmbrowse.lnk
	    echo mmbrowse.def            >> mmbrowse.lnk

mmbrowse.res:	 mmbrowse.rc mmbrowse.ico mmbrowse.h mmbrowse.exe

mmbrowse.hlp:	 mmbrowse.ipf
	     ipfc mmbrowse.ipf

mmbrowse.obj:	 mmbrowse.c mmbrowse.h
	    $(COMPILE) mmbrowse.c

framecontrol.obj:	 framecontrol.c mmbrowse.h
	    $(COMPILE)  framecontrol.c

mmbrowse.exe:	 mmbrowse.obj framecontrol.obj mmbrowse.def mmbrowse.lnk makefile mmbrowse.ico mmbrowse.rc
	    $(LINK) @mmbrowse.lnk
	    rc -p -x mmbrowse.rc mmbrowse.exe
