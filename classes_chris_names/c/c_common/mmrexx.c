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
/*
 * If you need another license for your project/product contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 */

#define INCL_REXXSAA
#define INCL_PM

#include <os2.h>
#include <stdio.h>
#include <rexxsaa.h>                   /* needed for Rexx */
#include "cwmmdataf.ih"
#include "cwaudio.ih"
#include "cwaudioinc.h"


typedef ULONG (APIENTRY   FNWPSFUNC)(WPObject* ,...);
typedef FNWPSFUNC *PFNWPSFUNC;

void parameterError(char *chrMethod)
{
  char text[200];
  sprintf(text, "%s: wrong number of parameters.", chrMethod);

  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Method error",
                1234, MB_OK|MB_MOVEABLE);
  
}

void wpObjectError(char *chrMethod)
{
  char text[200];
  sprintf(text, "Method %s: object pointer is not valid.", chrMethod);

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
ULONG EXPENTRY rxCallCWMMFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  char text[200];
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;
  WPFileSystem* wpFSObject;

  /* At least WPObject* and name of method */
  if(argc<2)
    return 40;

  /*  TRY_LOUD(RX) {*/
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
        /* method not found */
        sprintf(retstring->strptr, "ERROR_METHOD:");
        retstring->strlength=strlen(retstring->strptr);
      }
      else {
        switch(ulHash)
          {
          case 0x2a1b: /* cwmmQueryTrackInfo */
            {
              ULONG ulParam1;
             char text[100];

              if(argc<3) {
                parameterError(argv[0].strptr);
                ulReturn=40;
                break;
              }

              ulParam1=atol(argv[2].strptr);/* ulWhich */

              switch(ulParam1)
                {
                case IDINFO_PLAYTIME:
                case IDINFO_BPS:
                case IDINFO_CHANNELS:
                case IDINFO_SAMPLERATE:
                  sprintf(retstring->strptr,"%d",procAddr(wpObject, NULLHANDLE, 0, ulParam1));
                  break;
                default:
                  if(!procAddr(wpObject, &retstring->strptr, retstring->strlength, ulParam1))
                    //if(!procAddr(wpObject, text, 100, ))
                    sprintf(retstring->strptr,"ERROR:");
                }
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

    /*  }
        CATCH(RX)
        {}END_CATCH;*/
    
    return ulReturn;
}
