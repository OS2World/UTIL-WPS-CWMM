/*
 * This file is (C) Chris Wohlgemuth 2002
 * It is part of the MediaFolder package
 */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_SW
#define INCL_REXXSAA

#include "mediafolder.hh"
#include "mediafolderres.h"

//#pragma SOMAsDefault(pop)
#include <os2.h>
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <stdio.h>
#include <stdlib.h>
//#pragma SOMAsDefault(on)



typedef ULONG (APIENTRY *PFNWPSFUNC)(WPObject* ,...);

extern ULONG calculateHash(char * theString);
ULONG calculateHash2(char * theString);
void parameterError(char *chrMethod);
extern SOMClass* somhlpGetSomClass(char* chrClassName);


void parameterError(char *chrMethod)
{
  char text[200];
  sprintf(text, "%s(): wrong number of parameters.", chrMethod);

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Method error",
                1234, MB_OK|MB_MOVEABLE);
  
}

void wpObjectError(char *chrMethod)
{
  char text[200];
  sprintf(text, "%s(): object pointer is not valid.", chrMethod);

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Object error",
                1234, MB_OK|MB_MOVEABLE);
  
}


#define HASHSIZE 101
ULONG calculateHash(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+31*ulHash;
  
  return ulHash%HASHSIZE;
}

#define HASHSIZE2 123
ULONG calculateHash2(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+37*ulHash;
  
  return ulHash%HASHSIZE;
}

/* This function handles new WPS functions introduced by private classes */
ULONG EXPENTRY rxCallCWWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  char text[200];
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;
  WPFileSystem* wpFSObject;

  /* At least WPObject* and name of method */
  if(argc<2)
    return 40;

  TRY_LOUD(RX) {

    /* Get a hash for the methodname for the switch() statement */
    ulHash=calculateHash(argv[0].strptr)<<8;
    ulHash+=calculateHash2(argv[0].strptr);
    ulReturn=0;

    wpObject=(WPObject*)atol(argv[1].strptr);
    /* Check if it's a valid object */
    if(somIsObj(wpObject)) {
      PFNWPSFUNC procAddr;

      /* Try to find the requested method */
      if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[0].strptr))==NULL) {
        /* method not found. Check if it's a wrapped one */
        switch(ulHash)
          {
          case 0x1b2a:  /* cwIsA */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "somIsA");
            break;
          case 0x2c21:  /* cwIsInstanceOf */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "somIsInstanceOf");
            break;
          case 0x5226:  /* cwQueryRealName */ 
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "wpQueryRealName");
            break;
#if 0
          case 0x24 : /* cwQueryFilename */
            procAddr=(PFNWPSFUNC) somResolveByName(wpObject, "wpQueryFilename");
            break;
#endif
          default:
            break;
          }
      }

      if(procAddr==NULL) {
        /* method not found and no private one */
        sprintf(retstring->strptr, "ERROR_METHOD:");
        retstring->strlength=strlen(retstring->strptr);
      }
      else {
        switch(ulHash)
          {
            /* WPS method without a parameter returning ULONG or WPObject* */
          case 0x0f10: /* cwQueryCurrentlyPlayingObject */
            {
              sprintf(retstring->strptr,"%d",procAddr(wpObject));
              retstring->strlength=strlen(retstring->strptr);
              break;            
            }
            /* Functions with one (ULONG) param returning ULONG, BOOL or WPObject* */
            /* WPObject */
          case 0x3f5e: /* cwStopAudioFile       */ /* Obsolete with V0.2.5 */
          case 0x303d: /* cwPauseAudioFile      */ /* Obsolete with V0.2.5 */
          case 0x5954: /* cwGetFileSystemObject */
          case 0x244a: /* mfldrStopAudioFile    */ /* new with V0.2.5      */
          case 0x3601: /* mfldrPauseAudioFile   */ /* new with V0.2.5      */
            {
              ULONG ulParam1;

              if(argc<3) {
                parameterError(argv[0].strptr);
                ulReturn=40;
                break;
              }

              ulParam1=atol(argv[2].strptr);
              sprintf(retstring->strptr,"%d",procAddr(wpObject, ulParam1));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
            /* Functions with one (ULONG) param returning a string: */
          case 0x5226: /* cwQueryRealName */
            {
              ULONG ulParam1;
             
              if(argc<3) {
                parameterError(argv[0].strptr);
                ulReturn=40;
                break;
              }
              ulParam1=atol(argv[2].strptr);/* BOOL fQualified */

              if(!procAddr(wpObject, retstring->strptr, &retstring->strlength, ulParam1))
                sprintf(retstring->strptr,"ERROR:");
              /*
                if(!procAddr(wpObject, &retstring->strptr, &retstring->strlength, ulParam1))*/
              
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
            /* Function with two ULONG (or alike) params returning ULONG */
          case 0x2f58:  /* cwPlayAudioFile    */ /* Obsolete with V0.2.5 */
          case 0x1423:  /* mfldrPlayAudioFile */ /* new with V0.2.5      */
            {
              ULONG ulParam1;
              ULONG ulParam2;

              if(argc<4) {
                parameterError(argv[0].strptr);
                ulReturn=40;
                break;
              }
              ulParam1=atol(argv[2].strptr);
              ulParam2=atol(argv[3].strptr);

              sprintf(retstring->strptr,"%d", procAddr(wpObject, ulParam1, ulParam2));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x631e: /* mfldrSetStatusLineText */ /* new with V0.2.5      */
          case 0x5121: /* cwSetStatusLineText    */ /* Obsolete with V0.2.5 */
            {
              if(argc<3) {
                parameterError(argv[0].strptr);
                ulReturn=40;
                break;
              }
              if(atoi(argv[2].strptr)==0 && strlen(argv[2].strptr)==1)
                procAddr(wpObject, NULLHANDLE);
              else
                procAddr(wpObject, argv[2].strptr);
              break;
            }
            /* Function with one param returning ULONG */
          case 0x1b2a:  /* cwIsA */
          case 0x2c21:  /* cwIsInstanceOf */
            {
              SOMClass *somClass;

              if(argc<3) {
                parameterError(argv[0].strptr);
                ulReturn=40;
                break;
              }

              somClass=somhlpGetSomClass(argv[2].strptr);
              if(!somIsObj(somClass)) {
                sprintf(retstring->strptr,"ERROR_CLASS:");/* Return error */
                retstring->strlength=strlen(retstring->strptr);
                break;
              }

              sprintf(retstring->strptr,"%d", procAddr(wpObject, somClass));
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          default:
            ulReturn=40;        
          }/* switch */
      }/* else if((procAddr=(PFNWPSFUNC) somResolveByName(wpObject, argv[1].strptr))==NULL) */
    }/* if(somIsObj(wpObject)) */
    else {
      /* wpObject is not valid */
      wpObjectError(argv[0].strptr);

      ulReturn=40;
    }
    /*
      sprintf(text, "%s HAsh: %x, Hash2: %x",argv[0].strptr, ulHash, calculateHash2(argv[0].strptr));
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
      1234, MB_OK|MB_MOVEABLE);
      */
  }
  CATCH(RX)
    {}END_CATCH;

  return ulReturn;
}
/*

Functions without params returning a string:


Functions with one (ULONG) param returning ULONG, BOOL or WPObject*:

0x3f: cwStopAudioFile             0x5e   obsolete with V0.2.5
0x30: cwPauseAudioFile            0x3d   obsolete with V0.2.5
0x59: cwGetFileSystemObject       0x54
0x1b: cwIsA                       0x2a
0x244a: mfldrStopAudioFile               new with V0.2.5
0x3601: mfldrPauseAudioFile              new with V0.2.5

Functions with one (ULONG) param returning a string:
0x52:  cwQueryRealName            0x26
 0x24 : cwQueryFilename 

Function with two ULONG (or alike) params:
0x2f:   cwPlayAudioFile            0x58   obsolete with V0.2.5
0x1423: mfldrPlayAudioFile                new with V0.2.5

0x2c:  cwIsInstanceOf                 0x21
0x51: cwSetStatusLineText             0x21
0x631e: mfldrSetStatusLineText

( 0x2e: cwGetVersionNumbers       hash2: 0x58)

0xf:  cwQueryCurrentlyPlayingObject   0x10

 */
































