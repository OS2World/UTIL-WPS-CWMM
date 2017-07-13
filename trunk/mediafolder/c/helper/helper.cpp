/*
 * This file is (C) Chris Wohlgemuth 1999-2004
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

#define INCL_WIN
#define INCL_DOS
#define INCL_REXXSAA

#include <os2.h>
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include "except.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "sys_funcs.h"

#ifndef BS_NOTEBOOKBUTTON
#define BS_NOTEBOOKBUTTON 8L   /* Warp 4 notebook style */  
#endif

extern char chrInstallDir[];


/* Mutex semaphores to protect filename generation */
ULONG cwCreateMutex(HMTX * hmtxBMP) {

  return DosCreateMutexSem( NULL, hmtxBMP, 0, FALSE);
}

ULONG cwCloseMutex(HMTX  hmtxBMP) {

  return DosCloseMutexSem( hmtxBMP );
}

ULONG cwRequestMutex(HMTX  hmtxBMP, ULONG ulTimeOut) {

  return DosRequestMutexSem( hmtxBMP, ulTimeOut );
}

ULONG cwReleaseMutex(HMTX  hmtxBMP) {

  return DosReleaseMutexSem( hmtxBMP );
}

/****************************************************
 *                                                  *
 * This function moves the default buttons off      *
 * notebook pages on Warp 4.                        *
 * usDelta specifies the units all controls have    *
 * to be moved down. usID is the ID of the last     *
 * button that has to be moved on the main frame    *
 * of the notebook. This value is a threshold.      *
 *                                                  *
 ****************************************************/
BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, USHORT usID, USHORT usDelta)
{
  if (SysQueryOSRelease()>=40) {
    HENUM henum;
    HWND hwnd;
    
    /* Move the default notebook buttons */
    if((henum=WinBeginEnumWindows(hwndDlg))!=NULLHANDLE) {
      while((hwnd=WinGetNextWindow(henum))!=NULLHANDLE) {
        if(WinQueryWindowUShort(hwnd,QWS_ID)<=usID)
          WinSetWindowBits(hwnd, QWL_STYLE,
                           BS_NOTEBOOKBUTTON, BS_NOTEBOOKBUTTON);
        else {
          SWP swp;
          POINTL ptl= {0};
          ptl.y=usDelta;

          WinMapDlgPoints(hwndDlg, &ptl, 1, TRUE);
          /* Move all other controls */
          if(WinQueryWindowPos(hwnd, &swp))
            WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y-ptl.y,0,0, SWP_MOVE);
        }
      }
      WinEndEnumWindows(henum);
    }
  }
  return TRUE;
}

BOOL launchRexx(PSZ rexxFile)
{
  char text[1024];      
  
  RXSTRING arg[1];                       /* argument string for REXX  */
  RXSTRING rexxretval;                /* return value from REXX    */
  APIRET   rc;                        /* return code from REXX     */
  SHORT    rexxrc = 0;                /* return code from function */
  char theScript[CCHMAXPATH];  
  /* By setting the strlength of the output RXSTRING to zero, we   */
  /* force the interpreter to allocate memory and return it to us. */
  /* We could provide a buffer for the interpreter to use instead. */
  rexxretval.strlength = 0L;          /* initialize return to empty*/
  
  //  MAKERXSTRING(arg[0], chrRexxEnv, strlen(chrRexxEnv));/* create input argument     */
  //  MAKERXSTRING(arg[1], chrThis, strlen(chrThis));/* create input argument     */              
  //  MAKERXSTRING(arg[2], chrHwnd, strlen(chrHwnd));/* create input argument     */
  
  sprintf(theScript, "%s\\bin\\%s", chrInstallDir, rexxFile);

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
    DosFreeMem(rexxretval.strptr);          /* Release storage given to us by REXX.  */      
  }
  CATCH(RX_START)
    {}END_CATCH;
    return TRUE;
}

#if 0
ULONG launchRexx( PSZ pszTitle, PSZ rexxFile, PSZ parameters)
{
  STARTDATA startData={0};
  APIRET rc;
  PID pid;
  ULONG ulSessionID=0;
  char chrLoadError[CCHMAXPATH];
  char startParams[CCHMAXPATH*4];
  char tempText[CCHMAXPATH]= {0};
  char *charPtr;
  char trackname[CCHMAXPATH];
    
  memset(&startData,0,sizeof(startData));
  startData.Length=sizeof(startData);
  startData.Related=SSF_RELATED_INDEPENDENT;
  startData.FgBg=SSF_FGBG_BACK;
  startData.TraceOpt=SSF_TRACEOPT_NONE;
  startData.PgmTitle=pszTitle;
  
  startData.PgmName="cmd.exe";
  startData.InheritOpt=SSF_INHERTOPT_SHELL;
  startData.SessionType=SSF_TYPE_WINDOWABLEVIO;
  startData.PgmControl|=SSF_CONTROL_INVISIBLE;//|SSF_CONTROL_MAXIMIZE|SSF_CONTROL_NOAUTOCLOSE;
  startData.InitXPos=30;
  startData.InitYPos=30;
  startData.InitXSize=500;
  startData.InitYSize=400;
  startData.ObjectBuffer=chrLoadError;
  startData.ObjectBuffLen=(ULONG)sizeof(chrLoadError);

  sprintf(startParams," /C \"%s\\bin\\%s\"", chrInstallDir, rexxFile);
  startData.PgmInputs=startParams;
  
  rc=DosStartSession(&startData,&ulSessionID,&pid);   
  return rc;   
}
#endif

/* This function launches the wrapper <wrapperExe>         */
/*  with the params given as a PM-session */
/* in PSZ parameter. PSZ folderPath is the path to put the */
/* write.log. HWND hwnd is the window waiting for the      */
/* WM_APPTERMINATE message                                 */
ULONG launchPMWrapper(PSZ pszTitle, PSZ wrapperExe, PSZ parameters)
{
  STARTDATA startData={0};
  APIRET rc;
  PID pid;
  ULONG ulSessionID=0;
  char chrLoadError[CCHMAXPATH];
  
  memset(&startData,0,sizeof(startData));
  startData.Length=sizeof(startData);
  startData.Related=SSF_RELATED_INDEPENDENT;
  startData.FgBg=SSF_FGBG_FORE;
  startData.TraceOpt=SSF_TRACEOPT_NONE;
  startData.PgmTitle=pszTitle;
    
  startData.PgmName=wrapperExe;
  startData.InheritOpt=SSF_INHERTOPT_SHELL;
  startData.SessionType=SSF_TYPE_PM;
  startData.PgmControl=0;
  startData.InitXPos=30;
  startData.InitYPos=30;
  startData.InitXSize=500;
  startData.InitYSize=400;
  startData.ObjectBuffer=chrLoadError;
  startData.ObjectBuffLen=(ULONG)sizeof(chrLoadError);

  startData.PgmInputs=parameters;

  rc=DosStartSession(&startData,&ulSessionID,&pid);   
  return 0;   
}

