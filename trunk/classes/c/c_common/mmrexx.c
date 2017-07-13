/*
 * (C) Chris Wohlgemuth 2002-2003
 *
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
/*
 * If you need another license for your project/product contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 */

#define INCL_REXXSAA
#define INCL_PM
#define INCL_DOS

#include <os2.h>
#include <stdio.h>
#include "except.h"
#include <rexxsaa.h>                   /* needed for Rexx */
#include "cwmmdataf.h"
#include "cwaudioinc.h"
#include "sys_funcs.h"

PSZ queryInstallDir(void);

typedef ULONG (APIENTRY   FNWPSFUNC)(WPObject* ,...);
typedef FNWPSFUNC *PFNWPSFUNC;


BOOL getStringFromRexxScript(PSZ rexxFile, char* chrResult, ULONG ulSize)
{

  RXSTRING arg[1];                       /* argument string for REXX  */
  RXSTRING rexxretval;                /* return value from REXX    */
  APIRET   rc;                        /* return code from REXX     */
  SHORT    rexxrc = 0;                /* return code from function */
  char theScript[CCHMAXPATH];  
  /* By setting the strlength of the output RXSTRING to zero, we   */
  /* force the interpreter to allocate memory and return it to us. */
  /* We could provide a buffer for the interpreter to use instead. */
  rexxretval.strlength = 0L;          /* initialize return to empty*/
  
  
  sprintf(theScript, "%s\\bin\\%s", queryInstallDir(), rexxFile);

  TRY_LOUD(RX_START) {
    /* Here we call the interpreter.  We don't really need to use    */
    /* all the casts in this call; they just help illustrate         */
    /* the data types used.                                          */
    rc=RexxStart((LONG)       0,             /* number of arguments   */
                 (PRXSTRING)  &arg,          /* array of arguments    */
                 (PSZ)        theScript,/* name of REXX file     */
                 (PRXSTRING)  0,             /* No INSTORE used       */
                 (PSZ)        "CWRXX",         /* Command env. name     */
                 (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
                 (PRXSYSEXIT) 0,             /* No EXITs on this call */
                 (PSHORT)     &rexxrc,       /* Rexx program output   */
                 (PRXSTRING)  &rexxretval ); /* Rexx program output   */


#if 0
    if(rc) {
      sprintf(text,"Error in the Rexx skript %s\n\n Get more information with 'help REX%04d'.\n", 
              tPt->rexxSkript, rc*-1);
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_OK|MB_MOVEABLE|MB_ERROR);
    }
#endif
    if(!rc) {
      if(ulSize>rexxretval.strlength) {
        strncpy(chrResult, rexxretval.strptr, rexxretval.strlength);
        chrResult[rexxretval.strlength]=0;
      }
      else
        strncpy(chrResult, rexxretval.strptr, ulSize);
      chrResult[ulSize-1]=0;
    }
    if(rexxretval.strptr)
      DosFreeMem(rexxretval.strptr);          /* Release storage given to us by REXX.  */      
  }
  CATCH(RX_START)
    {}END_CATCH;
    return TRUE;
}

void parameterError(char *chrMethod, ULONG ulReq, ULONG ulActual)
{
  char text[200];
  sprintf(text, "%s(): wrong count of parameters. Required: %d, actual: %d",
          chrMethod, ulReq, ulActual);

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

/*
  This function will be registered in the wpclsInitData() of MMDataFile
 */

/* This function handles new WPS functions introduced by private classes */
ULONG EXPENTRY rxCallCWMMFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring)
{
  ULONG ulHash;
  ULONG ulReturn;
  WPObject* wpObject;

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
            //          case 0x2e0e: /* mmQueryTrackInfo */ /* New generic name with V0.2.5 */
          case 0x2a1b: /* cwmmQueryTrackInfo */
            {
              ULONG ulParam1;

              if(argc<3) {
                parameterError(argv[0].strptr, 3 , argc);
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
                case IDINFO_BITRATE:
                  sprintf(retstring->strptr,"%d",procAddr(wpObject, NULLHANDLE, 0, ulParam1));
                  break;
                case IDINFO_NAME:
                case IDINFO_ARTIST:
                case IDINFO_GENRE:
                case IDINFO_YEAR:
                  /* Maximum length of the returned value is 256 bytes (default retstring length) */
                  if(!procAddr(wpObject, retstring->strptr, retstring->strlength, ulParam1))
                    sprintf(retstring->strptr,"ERROR:");
                  break;
                default:
                  {
                    if(!procAddr(wpObject, retstring->strptr, retstring->strlength, ulParam1))
                      //if(!procAddr(wpObject, text, 100, ))
                      sprintf(retstring->strptr,"ERROR:");
                    }
                }
              retstring->strlength=strlen(retstring->strptr);
              break;
            }
          case 0x220f: /* cwmmQueryImageInfo New with V0.2.5 */
            //          case 0x0502: /* mmQueryImageInfo */ /* New with V0.2.5 */
            {
              ULONG ulParam1;
                           
              if(argc<3) {
                parameterError(argv[0].strptr, 3 , argc);
                ulReturn=40;
                break;
              }
              
              ulParam1=atol(argv[2].strptr);/* ulWhich */

              switch(ulParam1)
                {
                case IDINFO_WIDTH:
                case IDINFO_HEIGHT:
                case IDINFO_BPP:
                  sprintf(retstring->strptr,"%d",procAddr(wpObject, NULLHANDLE, 0, ulParam1));
                  break;
                default:
                  if(!procAddr(wpObject, &retstring->strptr, retstring->strlength, ulParam1))
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


/*

0x2e0e:  mmQueryTrackInfo  New generic name with V0.2.5 
0x2a1b:  cwmmQueryTrackInfo

0x0502: mmQueryImageInfo New with V0.2.5 
0x220f: cwmmQueryImageInfo New with V0.2.5 
 */
