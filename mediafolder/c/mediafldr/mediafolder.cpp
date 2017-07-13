/*
 * This file is (C) Chris Wohlgemuth 2002
 * 
 * It's part of the Media-Folder distribution
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

#define INCL_DOSERRORS
#define INCL_REXXSAA

#include "mediafolder.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include "mediafolderres.h"

#include "sys_funcs.h"
/* typedef for somTP_MMAudio_cwmmQueryTrackInfo */
#include "cwmmquerytrackinfo.h"

int  iWaveDev=0;

extern HMTX hmtxFileName;
extern char chrInstallDir[];

HMODULE queryResModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

ULONG cwRequestMutex(HMTX  hmtxBMP, ULONG ulTimeOut);
ULONG cwReleaseMutex(HMTX  hmtxBMP);

MRESULT EXPENTRY settingsToolbarOptionDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
extern void showPlayTimeDisplay(CWMediaFolder *thisPtr, BOOL bShow);
void setPlayTimeText2(CWMediaFolder *thisPtr, char * theText);
void setPlayTimeText(CWMediaFolder *thisPtr);
extern void _resetDisplay(CWMediaFolder *thisPtr);

extern BOOL cwmmQueryCWAudioMethodPtr(CWMediaFolder *thisPtr);
extern SOMClass* queryMMAudioClass(void);
SOMClass* somhlpGetSomClass(char* chrClassName);
/* Get the HWND of the play control dialog. This Dialog is on top of
   the top frame control. */
HWND playControlDialogFromFrameHWND(HWND hwndFrame);
/* Get the HWND of the frame control at the top */
HWND topControlFromFrameHWND(HWND hwndFrame);
/* Get the HWND of the time display static text control. */
HWND playTimeControlFromFrameHWND(HWND hwndFrame);

/* This procedure is called for example from the play dialog. It's used to keep
   the code tidier. */
void callMediaFolderMethod(CWMediaFolder *thisPtr, ULONG ulAction, ULONG ulData)
{
  switch(ulAction)
    {
    case PLAY_FIRST:  
      /* Send a message to the object window created by the folder play thread */
      WinPostMsg(thisPtr->mfData->hwndPlayObject,WM_APPTERMINATENOTIFY, 0,MPFROMLONG(PLAY_FIRST));
      break;
    case STOP_TRACK:
      WinPostMsg(thisPtr->mfData->hwndPlayObject,WM_APPTERMINATENOTIFY, 0,MPFROMLONG(STOP_TRACK));
      break;
    case PLAY_NEXT:
      WinPostMsg(thisPtr->mfData->hwndPlayObject,WM_APPTERMINATENOTIFY, 0,MPFROMLONG(PLAY_NEXT));
      break;
    case PLAY_PREV:
      WinPostMsg(thisPtr->mfData->hwndPlayObject,WM_APPTERMINATENOTIFY, 0,MPFROMLONG(PLAY_PREV));
      break;
    case PAUSE_TRACK:
      WinPostMsg(thisPtr->mfData->hwndPlayObject,WM_APPTERMINATENOTIFY, 0,MPFROMLONG(PAUSE_TRACK));
      break;
    case SEEK_TRACK:
      WinPostMsg(thisPtr->mfData->hwndPlayObject,WM_APPTERMINATENOTIFY, MPFROMLONG(ulData), MPFROMLONG(SEEK_TRACK));
      break;
    default:
    break;
    }
  return;
}

MRESULT EXPENTRY playObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch (msg)
    {
    case WM_APPTERMINATENOTIFY:
      {
        THREADPARAMS* tPt=(THREADPARAMS*)WinQueryWindowULong(hwnd, QWL_USER);
        HWND hwndFrame=tPt->hwndFrame;
        CWMediaFolder *thisPtr=tPt->thisPtr;

        switch(LONGFROMMP(mp2))
          {
          case PLAY_FIRST:
            TRY_LOUD(OBJECTPROC_FIRST) {
              thisPtr->cwPlayAudioFile(hwndFrame, PLAY_FIRST);
            }/* Try */
            CATCH(OBJECTPROC_FIRST)
              {
                SysWriteToTrapLog("\nTrap occured in %s, file: %s, near line %d.\n",
                                  __FUNCTION__, __FILE__, __LINE__);
              } END_CATCH;
            break;
          case STOP_TRACK:
            thisPtr->cwStopAudioFile(hwndFrame);
            break;
          case PLAY_NEXT:
            TRY_LOUD(OBJECTPROC_NEXT) {
              thisPtr->cwPlayAudioFile(hwndFrame, PLAY_NEXT);
            }/* Try */
            CATCH(OBJECTPROC_NEXT)
              {
                SysWriteToTrapLog("\nTrap occured in %s, file: %s, near line %d.\n",
                                  __FUNCTION__, __FILE__, __LINE__);
              } END_CATCH;
            break;
          case PLAY_PREV:
            TRY_LOUD(OBJECTPROC_PREV) {
              thisPtr->cwPlayAudioFile(hwndFrame, PLAY_PREV);
            }/* Try */
            CATCH(OBJECTPROC_PREV)
              {
                SysWriteToTrapLog("\nTrap occured in %s, file: %s, near line %d.\n",
                                  __FUNCTION__, __FILE__, __LINE__);
              } END_CATCH;
            break;
          case PAUSE_TRACK:
            thisPtr->cwPauseAudioFile(hwndFrame);
            break;
          case SEEK_TRACK:
            TRY_LOUD(OBJECTPROC_SEEK) {
              thisPtr->cwSeekAudioFile(LONGFROMMP(mp1));
              /* restart timer */
              //    WinStartTimer(WinQueryAnchorBlock(hwnd), WinWindowFromID(hwndFrame), IDTIMER_PLAY, PLAYTIMER_DELAY);  
              thisPtr->ulPos=LONGFROMMP(mp1)/1000;
            }
            CATCH(OBJECTPROC_SEEK)
              {
                SysWriteToTrapLog("\nTrap occured in %s, file: %s, near line %d.\n",
                                  __FUNCTION__, __FILE__, __LINE__);
              } END_CATCH;
              break;
          default:
            break;
          }/* switch() */
        return (MRESULT)0;
      }
    default:
      break;
    }
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}

/* Every folder has one running play thread to handle play commands */
void _Optlink playThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  THREADPARAMS* tPt;
  CWMediaFolder *thisPtr;


  tPt=(THREADPARAMS*) arg;
  if(!tPt)
    return;
  thisPtr=(CWMediaFolder*)tPt->thisPtr;    //Pointer auf CWMediaFolder-Object
  if(!somIsObj(thisPtr))
    return;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      HWND hwnd;

      hwnd=WinCreateWindow(HWND_OBJECT,WC_STATIC,"MFObj",0,0,0,0,0,NULLHANDLE,HWND_BOTTOM,13343,NULL,NULL);
      if(hwnd) {
        thisPtr->mfData->hwndPlayObject=hwnd;
        WinSubclassWindow(hwnd,&playObjectProc);
        WinSetWindowULong(hwnd, QWL_USER,(ULONG)tPt);
        /* Window created. */
        while(WinGetMsg(hab,&qmsg,(HWND)NULL,0,0))
          WinDispatchMsg(hab,&qmsg);

        WinDestroyWindow(hwnd);
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  thisPtr->wpFreeMem((PBYTE)tPt);
  thisPtr->mfData->hwndPlayObject=NULLHANDLE;
}

void _Optlink rexxThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  CWMediaFolder *thisPtr;
  HWND hwnd;
  char *chrFileName;
  char *chrFolderName;
  char *chrRexxScript;
  ULONG ulSize;
  INSERTTRACKPARMS * threadParms;

  threadParms=(INSERTTRACKPARMS *)arg;
  if(!threadParms)
    return;

  thisPtr=(CWMediaFolder*)threadParms->thisPtr;   

  if(!somIsObj(thisPtr))
    return;

  if(!somIsObj(thisPtr->wpObjM3UFile)) {
    thisPtr->wpFreeMem((PBYTE)threadParms);
    return;
  }

  chrFileName=thisPtr->wpAllocMem( CCHMAXPATH*3+10 , &ulSize);
  if(!chrFileName) {
    thisPtr->wpFreeMem((PBYTE)threadParms);
    return;
  }
  chrFolderName=chrFileName+CCHMAXPATH+1;
  chrRexxScript=chrFolderName+CCHMAXPATH+1;

  /* Get name of M3U file */
  ulSize=CCHMAXPATH;
  if(!((WPFileSystem*)thisPtr->wpObjM3UFile)->wpQueryRealName(chrFileName, &ulSize, TRUE))
    return;
  
  /* Get name of folder */
  ulSize=CCHMAXPATH;
  if(!thisPtr->wpQueryRealName(chrFolderName, &ulSize, TRUE))
    return;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {      
      RXSTRING arg[2];                    /* argument string for REXX  */
      RXSTRING rexxretval;                /* return value from REXX    */
      APIRET   rc;                        /* return code from REXX     */
      SHORT    rexxrc = 0;                /* return code from function */

      /* By setting the strlength of the output RXSTRING to zero, we   */
      /* force the interpreter to allocate memory and return it to us. */
      /* We could provide a buffer for the interpreter to use instead. */
      rexxretval.strlength = 0L;          /* initialize return to empty*/
      
      MAKERXSTRING(arg[0], chrFileName, strlen(chrFileName));/* create input argument     */
      MAKERXSTRING(arg[1], chrFolderName, strlen(chrFolderName));/* create input argument     */              
      
      TRY_LOUD(RX_START) {
        /* Here we call the interpreter.  We don't really need to use    */
        /* all the casts in this call; they just help illustrate         */
        /* the data types used.                                          */
        rc=RexxStart((LONG)       2,             /* number of arguments   */
                     (PRXSTRING)  &arg,          /* array of arguments    */
                     //(PSZ)        chrRexxScript,/* name of REXX file     */
                     (PSZ) threadParms->chrRexxScript,
                     //   (PSZ)        "H:\\cwmm\\mediafolder\\beep.cmd",/* name of REXX file     */
                     (PRXSTRING)  0,             /* No INSTORE used       */
                     (PSZ)        "CWRXX",         /* Command env. name     */
                     (LONG)       RXSUBROUTINE,  /* Code for how invoked  */
                     (PRXSYSEXIT) 0,             /* No EXITs on this call */
                     (PSHORT)     &rexxrc,       /* Rexx program output   */
                     (PRXSTRING)  &rexxretval ); /* Rexx program output   */
        
        if(rc) {
          sprintf(chrFileName,"Error in the Rexx skript %s\n\n Get more information with 'help REX%04d'.\n", 
                  "", rc*-1);
          WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrFileName, "", 1234, MB_OK|MB_MOVEABLE|MB_ERROR);
        }
        DosFreeMem(rexxretval.strptr);          /* Release storage given to us by REXX.  */      
      }
      CATCH(RX_START)
        {}END_CATCH;
               
        WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  //  thisPtr->fContentsChanged=FALSE;
  thisPtr->wpFreeMem((PBYTE)chrFileName); 
  thisPtr->wpFreeMem((PBYTE)threadParms);
}

ULONG CWMediaFolder::cwQueryMFldrFlags(void)
{
  return ulPrivFlags;
}

BOOL CWMediaFolder::cwUnshrinkFolder(HWND hwndFrame)
{
  HWND hwndCnr;
  if((hwndCnr=WinWindowFromID(hwndFrame, FID_CLIENT))!=NULLHANDLE)
    {
      SWP swp, swp2, swp3;
      POINTL ptl;

      /* Get container size */
      WinQueryWindowPos(hwndCnr, &swp);
      if(swp.cy==0) {
        /* Container is shrinked so unshrink it */
        WinSetWindowULong(hwndFrame, QWL_STYLE, (WinQueryWindowULong(hwndFrame, QWL_STYLE) & ~FS_DLGBORDER)|FS_SIZEBORDER);
        WinSetWindowPos(hwndFrame, NULLHANDLE, swpUnshrink.x, swpUnshrink.y, swpUnshrink.cx,swpUnshrink.cy,SWP_SIZE|SWP_MOVE);
        return TRUE;
      }
    }
  return FALSE;
}


/**************************************************************/
/*                                                            */
/* Shrink the folder so only the play control is visible.     */
/*                                                            */
/**************************************************************/
BOOL CWMediaFolder::cwShrinkFolderToCompact(HWND hwndFrame)
{
  HWND hwndCnr;
  if((hwndCnr=WinWindowFromID(hwndFrame, FID_CLIENT))!=NULLHANDLE)
    {
      SWP swp, swp2, swp3;
      POINTL ptl;
      char chrText[20];
      WinQueryWindowPos(hwndFrame, &swp2);
      /* Save size for unshrinking */
      swpUnshrink=swp2;
      /* Get container size */
      WinQueryWindowPos(hwndCnr, &swp);
      if(swp.cy>0) {
        /* Shrink folder to hide container */
        swp2.y+=swp.cy;
        swp2.cy-=swp.cy;
        /* Get size of statusline */
        // WinQueryWindowPos(bottomControlFromFrameHWND(hwndFrame), &swp);
        // WinSetWindowPos(bottomControlFromFrameHWND(hwndFrame), NULLHANDLE, 0,0,swp.cx, 0, SWP_SIZE);
        /* Shrink folder to hide status line */
        //swp2.y+=swp.cy;
        //swp2.cy-=swp.cy;
        /* Get size of control area at the top */
        WinQueryWindowPos(topControlFromFrameHWND(hwndFrame), &swp);
        /* Get size of the play control area */
        WinQueryWindowPos(playControlDialogFromFrameHWND(hwndFrame), &swp3);
        swp2.cx-=(swp.cx-swp3.cx)+10;

        /* DLGBORDER is smaller than a size border */
        WinSendMsg(hwndFrame, WM_QUERYBORDERSIZE, &ptl, 0L);
        swp2.y+=2*(ptl.y-WinQuerySysValue(HWND_DESKTOP, SV_CYDLGFRAME))+2;
        swp2.cy-=2*(ptl.y-WinQuerySysValue(HWND_DESKTOP, SV_CYDLGFRAME))+2;
        WinSetWindowULong(hwndFrame, QWL_STYLE, (WinQueryWindowULong(hwndFrame, QWL_STYLE) & ~FS_SIZEBORDER)|FS_DLGBORDER);

        WinSetWindowPos(hwndFrame, NULLHANDLE, swp2.x, swp2.y, swp2.cx,swp2.cy,SWP_SIZE|SWP_MOVE);
        /* 
           By changing the to contents view we trick the WPS to show the 'Details view' menu item in
           the view menu of the folders system menu. In that sub menu the WPS always inserts all known
           views a user may switch to. Tree and contents view are removed in the
           frame proc. Compact view will not be inserted if already in compact view (see wpModifyPopupMenu()).
           The system knows nothing about the 'compact view' and all the handling is done by us.
           As a side effect the folder position information for the details
           view isn't changed when the folder is closed in 'Compact view'. 
           */
        wpMenuItemSelected(hwndFrame, 0x2cc);

        /* Change the view name shown in the title */
        getMessage(chrText, IDSTR_VIEWCOMPACT, sizeof(chrText), queryResModuleHandle(),hwndFrame);
        wpRegisterView(hwndFrame, chrText);
      }
      return TRUE;
    }
  return FALSE;
}

BOOL CWMediaFolder::cwInsertAllTracks(char* chrM3UFile)
{
  INSERTTRACKPARMS * threadParms;
  ULONG ulError;

  if((threadParms=(INSERTTRACKPARMS*)wpAllocMem(sizeof(INSERTTRACKPARMS), &ulError))==NULLHANDLE)
    return FALSE;
  threadParms->thisPtr=this;
  sprintf(threadParms->chrRexxScript,"%s%s",  chrInstallDir, RX_INSERT_MEDIAFLDRTRACKS);

  _beginthread(rexxThreadFunc,NULL,8192*16,(void*)threadParms); //Fehlerbehandlung fehlt
  return TRUE;
}

void CWMediaFolder::cwSetStatusLineText(char * chrText)
{

  if(!chrText) {
    char text[100]="";
    char text2[100]="";
    char time[50];
    char chrBuf[2];

    if(mfData) {
      PrfQueryProfileString(HINI_USER, "PM_Default_National", "sThousand",
                            ",", chrBuf, sizeof(chrBuf));
      chrBuf[sizeof(chrBuf)-1]=0;
      
      if(mfData->ulTotalTime<=3600) {
        getMessage( text, IDSTR_STATUSTIME1, sizeof(text), queryResModuleHandle(), HWND_DESKTOP);
        sprintf(time, text, mfData->ulTotalTime/60, mfData->ulTotalTime%60);
      }
      else if(mfData->ulTotalTime<=3600*24) {
        getMessage( text, IDSTR_STATUSTIME2, sizeof(text), queryResModuleHandle(),HWND_DESKTOP);
        sprintf(time, text, mfData->ulTotalTime/3600,  (mfData->ulTotalTime%3600)/60, mfData->ulTotalTime%60);
      }
      else {
        getMessage( text, IDSTR_STATUSTIME3, sizeof(text), queryResModuleHandle(),HWND_DESKTOP);
        sprintf(time, text, mfData->ulTotalTime/(3600*24),
                (mfData->ulTotalTime%(3600*24))/3600,  (mfData->ulTotalTime%3600)/60, mfData->ulTotalTime%60);
      }      
      if(mfData->ulSize<=0xffffffff) {
        getMessage( text2, IDSTR_STATUSTIME4, sizeof(text2), queryResModuleHandle(),HWND_DESKTOP);
        sprintf(text, text2, 
                mfData->ulNumTracks, mfData->ulSize/1000000 ,chrBuf, (mfData->ulSize%1000000)/1000,
                chrBuf,  mfData->ulSize%1000,
                time);
      }
      else {
        getMessage( text2, IDSTR_STATUSTIME5, sizeof(text2), queryResModuleHandle(),HWND_DESKTOP);
        sprintf(text, text2, mfData->ulNumTracks);
      }
    }
    WinSetWindowText(WinWindowFromID(hwndBottom, IDST_STATUSTEXT),text);
  }
  else
    WinSetWindowText(WinWindowFromID(hwndBottom, IDST_STATUSTEXT),chrText);
}

MMAudio* CWMediaFolder::cwQueryCurrentlyPlayingObject(void)
{
  return (MMAudio*)currentObject;
}

BOOL CWMediaFolder::cwAddToolbarOptionPage(HWND hwndNotebook)
{
  PAGEINFO pageinfo;
    char pageName[100];

  //Clear the pageinfo structure
  memset((PCH)&pageinfo, 0, sizeof(PAGEINFO));
  //Fill the pageinfo structure
  pageinfo.cb = sizeof(PAGEINFO);
  pageinfo.hwndPage = NULLHANDLE;
  pageinfo.usPageStyleFlags = BKA_MAJOR | BKA_STATUSTEXTON;
  pageinfo.usPageInsertFlags = BKA_FIRST;
  //We want page numbers
  pageinfo.usSettingsFlags = SETTINGS_PAGE_NUMBERS;
  //The dialog procedure for this page
  pageinfo.pfnwp = settingsToolbarOptionDlgProc;
  //The resource DLL
  pageinfo.resid = queryResModuleHandle();
  //pageinfo.resid = queryModuleHandle();
  //The ID of the dialog template
  pageinfo.dlgid = IDDLG_TOOLBARSETTINGS;
  //We need a pointer to our WPS-object in the dialog procedure
  //to call class functions
  pageinfo.pCreateParams = this;
  //The ID of the help panel for this page
  //pageinfo.idDefaultHelpPanel = 2000;
  //Tell the WPS the help library name
  //pageinfo.pszHelpLibraryName = AFHELPLIBRARY;
  //We have a major tab so we need a name
  /* pageName: "General" */
  getMessage(pageName, IDSTRSETTING_TOOLBAR, sizeof(pageName), queryResModuleHandle(), hwndNotebook);
  pageinfo.pszName = pageName;
  
  //Insert the page into the settings notebook
  return wpInsertSettingsPage(hwndNotebook,&pageinfo);
}

BOOL mmGetOtherPlayingAudioFile( CWMediaFolder * mediaFolder, HWND hwndContainer, WPObject** wpObject,   PMINIRECORDCORE* pmrc)
{
  PMINIRECORDCORE mrc;
  WPObject * contentObject;
  
  /* See if we have an audio file played with doubleclick */
  mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS,MPFROMLONG(CMA_FIRST),
                                  MPFROMSHORT(CRA_INUSE));
  if(mrc) {
    SOMClass *cwAudioClass=somhlpGetSomClass("MMAudio");
    contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
    if(somIsObj(contentObject) && somIsObj(cwAudioClass)) {
      if((contentObject=mediaFolder->cwGetFileSystemObject(contentObject))!=NULLHANDLE) {
        /* Check if it's an audio file thus playable */
        if(contentObject->somIsA(cwAudioClass)) {
          if(((MMAudio*)contentObject)->wpQueryDefaultView()==0xbbf9){
            /* If stop menu is default, this means the track is playing. */
            *wpObject=contentObject;
            *pmrc=mrc;
            return TRUE;
          };
        }
      }
    }/* somIsObj() */
  }
  return FALSE;
}

ULONG CWMediaFolder::cwPlayAudioFile(HWND hwndFrame, ULONG ulWhich)
{
  HWND hwndContainer;
  PMINIRECORDCORE mrc, mrcTemp;
  WPObject * contentObject;
  SOMClass *mmMIDIClass;
  SOMClass *cwAudioClass=somhlpGetSomClass("MMAudio");

  char name[CCHMAXPATH];
  ULONG ulNameSize;
  char chrCommand[CCHMAXPATH*2];
  char chrDevice[30];
  char retMsg[20];
  ULONG rc;
  int iWavePriv;
  int iTime2;
  HWND hwndNotify;

  if(ulFlags&FLAG_RANDOM)
    return cwPlayRandomAudioFile( hwndFrame);

  hwndContainer=WinWindowFromID(hwndFrame, FID_CLIENT);
  if(!WinIsWindow(WinQueryAnchorBlock(hwndFrame), hwndContainer))
    return 0;

  /* Get class object */
  mmMIDIClass=somhlpGetSomClass("MMMIDI");

  /* Get new ID for wave device */
  if(cwRequestMutex(hmtxFileName, 100000)==ERROR_TIMEOUT)
    return FALSE;  

  iWaveDev++;
  if(iWaveDev==1000)
    iWaveDev=1;
  iWavePriv=iWaveDev;
  cwReleaseMutex(hmtxFileName);
  BOOL notFound;

  switch(ulWhich) {
  case PLAY_FIRST:
    {
      WPObject *wpObject;
      PMINIRECORDCORE pmrc;
      
#if 0
      /* There's already a file playing using doubleclick */
      if(mmGetOtherPlayingAudioFile( this, hwndContainer, &wpObject, &pmrc)) 
        return 0;
#endif

      /* Get first selected container item of our folder */
      mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, MPFROMSHORT(CMA_FIRST),
                                      MPFROMSHORT(CRA_SELECTED));

      if(iWave!=0 && mrc && (int)mrc!=-1) {
        cwStopAudioFile(hwndFrame);
        return cwPlayAudioFile(hwndFrame, PLAY_FIRST);
      }

      if(!mrc || (int)mrc==-1)
        /* Get first container item of our folder */
        mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,NULL,
                                        MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));

      if(!mrc || (int)mrc==-1)
        return 0;

      notFound=TRUE;
      do {
        contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
        if(somIsObj(contentObject)) {
          /* Get file system object or NULL */
          if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE) {
            //       HlpWriteToTrapLog("Tried to resolve shadow, no success, continue...\n");
            /* Try next object */
            mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                            MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
            //            HlpWriteToTrapLog("New mrc: %x\n", mrc);
            continue;
          }
        }
        else
          return 0;/* Error */
        
        if( contentObject->somIsA(cwAudioClass) || 
           /*contentObject->somIsA(mmAudioClass) ||*/
           contentObject->somIsA(mmMIDIClass)) {
          notFound=FALSE;
          /* Get the name of the audio file */
          ulNameSize=sizeof(name);
          ((WPFileSystem*)contentObject)->wpQueryRealName(name,&ulNameSize,TRUE);
        }
        else
          {
            /* Try next object */
            mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                            MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
          }
      }while(notFound && mrc && (int)mrc!=-1);
      //      HlpWriteToTrapLog("Left loop...\n");
      break;
    }
  case PLAY_NEXT:

    mrcTemp=mrcPlaying;

    if(iWave==0) {
      WPObject *wpObject;
      PMINIRECORDCORE pmrc;

      if(mmGetOtherPlayingAudioFile( this, hwndContainer, &wpObject, &pmrc)) {
        mrcTemp=pmrc;
      }
      else
        return cwPlayAudioFile(hwndFrame, PLAY_FIRST);/* Not yet playing */
    }

    /* Get next container item of our folder */
    mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,mrcTemp,
                                    MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));

    if((int)mrc==-1) { 
      /* Wrong parameter -> user deleted the shadow. */
      /* Stop wave, remove in use emphasis. */
      cwStopAudioFile(hwndFrame);
      return 0;
    }
    //  HlpWriteToTrapLog("Next mrc is %x\n", mrc);
 
    notFound=TRUE;
    while(notFound && mrc && (int)mrc!=-1){ 
      contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
      if(somIsObj(contentObject)) {
        /* Get file system object or NULL */
        if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE) {
            mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                            MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
          continue;
        }
      }
      else
        return 0;/* Error */
      
      if(contentObject->somIsA(cwAudioClass) ||
         /* contentObject->somIsA(mmAudioClass) || */
         contentObject->somIsA(mmMIDIClass)) {
        notFound=FALSE;
        ulNameSize=sizeof(name);
        ((WPFileSystem*)contentObject)->wpQueryRealName(name,&ulNameSize,TRUE);
      }
      else
        {
          /* Try next object */
          mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                          MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
        }
    };
    cwStopAudioFile(hwndFrame);
    if(!mrc) {
      if(ulFlags & FLAG_REPEAT)
        return cwPlayAudioFile(hwndFrame, PLAY_FIRST);
      else
        return 0;
    }
    break;
  case PLAY_PREV:

    mrcTemp=mrcPlaying;
    /* We are currently playing? */
    if(iWave==0) {
#if 0
      WPObject *wpObject;
      PMINIRECORDCORE pmrc;
      /* Check if there's a playing file started by the user */
      if(mmGetOtherPlayingAudioFile( this, hwndContainer, &wpObject, &pmrc)) {
        mrcTemp=pmrc;
        cwStopAudioFile(hwndFrame);
        /* Select the object */
        WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(pmrc),
                   MPFROM2SHORT(TRUE, CRA_SELECTED));  

        return cwPlayAudioFile(hwndFrame, PLAY_FIRST);/* Not yet playing, so start the first one. */
      }
      else
#endif
        return cwPlayAudioFile(hwndFrame, PLAY_FIRST);/* Not yet playing, so start the first one. */
    }
 
    /* Check the current time. If we are at the very beginning of the track skip to the previous track
       otherwise skip to the beginning of the current track. */
    if(ulPos<=2) {
      /* Get previous container item of our folder */
      mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,mrcTemp,
                                      MPFROM2SHORT(CMA_PREV,CMA_ITEMORDER));
    }
    else
      mrc=NULLHANDLE;

    cwStopAudioFile(hwndFrame);

    if(!mrc)
      mrc=mrcTemp;

    /* Find an audio file */
    notFound=TRUE;
    while(notFound && mrc && (int)mrc!=-1){ 
      contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
      if(somIsObj(contentObject)) {
        /* Get file system object or NULL */
        if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE) {
          mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                          MPFROM2SHORT(CMA_PREV,CMA_ITEMORDER));
          continue;
        }
      }
      else
        return 0;/* Error!! */

      /* Check if it's an audio file thus playable */
      if(contentObject->somIsA(cwAudioClass) || 
         /*   contentObject->somIsA(mmAudioClass) || */
         contentObject->somIsA(mmMIDIClass)) {
        notFound=FALSE;
        ulNameSize=sizeof(name);
        ((WPFileSystem*)contentObject)->wpQueryRealName(name,&ulNameSize,TRUE);
      }
      else
        /* Try next object */
        mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                        MPFROM2SHORT(CMA_PREV,CMA_ITEMORDER));
    };
    if(!mrc)
      return 0;
    
    break;
  default:
    return 0;
  }

  /* Found a valid object. Now play it. */

  if(!getMessage(chrCommand, IDSTR_STARTINGTRACK, sizeof(chrCommand), queryResModuleHandle(),  hwndFrame))
    sprintf(chrCommand, "Starting track...");
  setPlayTimeText2(this, chrCommand);

  if(contentObject->somIsA(mmMIDIClass))
    strncpy(chrDevice,"SEQUENCER", sizeof(chrDevice));
  else /*if(contentObject->somIsA(cwAudioClass) || contentObject->somIsA(mmAudioClass))*/
    strncpy(chrDevice,"WAVEAUDIO", sizeof(chrDevice));
  
  chrDevice[sizeof(chrDevice)-1]=0;

  hwndNotify=playControlDialogFromFrameHWND(hwndFrame);//WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE);

  /* Start audio file */  
  sprintf(chrCommand,"open \"%s\"  type %s alias wave%d SHAREABLE wait",name, chrDevice, iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), hwndNotify, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS)
    return 0;

  /* Set time format */
  sprintf(chrCommand,"SET wave%d TIME FORMAT MILLISECONDS wait", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d wait",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }      

  sprintf(chrCommand,"SETPOSITIONADVISE wave%d ON EVERY 1000 notify", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg),hwndNotify, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d wait",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }

  /* Get wave length in ms */
  sprintf(chrCommand,"STATUS wave%d LENGTH WAIT", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)==MCIERR_SUCCESS) {
    iTime2=atoi(retMsg);
    sprintf(chrCommand,"%d:%02d  %d:%02d  -%d:%02d",iTime2/1000/60,(iTime2/1000)%60,
            0, 0,
            iTime2/1000/60,(iTime2/1000)%60);
    //    WinSetWindowText(WinWindowFromID(hwndTop, IDST_PLAYTIME), chrCommand);
    //    WinSetWindowText(playTimeControlFromFrameHWND(hwndFrame), chrCommand);
  }
  ulTotalLength=iTime2;

  /* Set volume */
  sprintf(chrCommand,"SET wave%d AUDIO VOLUME %d wait", iWavePriv, ulVolume);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }

  sprintf(chrCommand,"play wave%d FROM 0", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }

  /* Start pos timer. SET MCIPOSITIONADVICE fails when seeking in the wave. 
     Don't know why. */

  //WinStartTimer(WinQueryAnchorBlock(hwndFrame), WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE), IDTIMER_PLAY, PLAYTIMER_DELAY);  
  WinStartTimer(WinQueryAnchorBlock(hwndFrame), playControlDialogFromFrameHWND(hwndFrame), IDTIMER_PLAY, PLAYTIMER_DELAY);

  /* Set emphasis by adding the object to the inuse list */
  memset(&myViewItem, 0,sizeof(myViewItem));
  myViewItem.useItem.type=USAGE_OPENVIEW;
  myViewItem.viewItem.view=OPEN_PLAYING;

  //WPObject* wpObject=cwGetFileSystemObject(tempObject);
  WPObject* wpObject=contentObject;

  if(wpObject->wpAddToObjUseList(&myViewItem.useItem))
    {
      myViewItem.bIsInUseList=TRUE;
    };
  /* Select the object */
  WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrc),
             MPFROM2SHORT(TRUE, CRA_SELECTED));  

  iWave=iWavePriv;
  currentObject=wpObject;
  mrcPlaying=mrc;
  /* The start position of the play command */
  ulStartPosition=0;
  ulPos=0;
  bPaused=FALSE;
  /* Set time display */
  setPlayTimeText(this);
  return 0;
}

//#define RANDOM_DEBUG

static int _getRandomTrack(  MEDIAFLDRDATA* mfData)
{
  int zufall;
  DATETIME DT;
  INT  seed, start,  iCheck=0;
  BOOL bFound=FALSE;

  /** Get a random number in the appropriate range                    */  
  DosGetDateTime((PDATETIME)&DT);      /* Get the time               */
  seed = DT.hours*60 + DT.minutes;     /* convert to hundreths       */
  seed = seed*60 + DT.seconds;
  seed = seed*100 + DT.hundredths;
  srand((INT)seed);                    /* Set random number seed     */
  start=zufall;

  while(!bFound) {
    zufall=rand()%mfData->iNumToPlay;    /* Range is 0...iNumToPlay-1  */
    iCheck++;

#ifdef RANDOM_DEBUG
    HlpWriteToTrapLog("\n     Zufall: %d       %d\n", zufall, mfData->pRandomArray[zufall]);  
#endif
    /* Get a random number in the appropriate range */
    if(mfData->pRandomArray[zufall]==0) {
      mfData->pRandomArray[zufall]=1;
#ifdef RANDOM_DEBUG
      HlpWriteToTrapLog("2     Zufall: %d       %d\n", zufall, mfData->pRandomArray[zufall]);  
#endif
      return zufall;
    }

    /* Already played number. Find another */
#if 0
    if (zufall == start && !bFound) {
#ifdef RANDOM_DEBUG
      HlpWriteToTrapLog("Error     Zufall: %d       %d\n", zufall, mfData->pRandomArray[zufall]);  
#endif
      return -1;
    }
#endif

    if(iCheck>MAX_RANDOM_OBJECTS)
      break; /* Emergency exit... */
  };
  return -1; /* Error */
}

ULONG CWMediaFolder::cwPlayRandomAudioFile(HWND hwndFrame)
{
  HWND hwndContainer;
  PMINIRECORDCORE mrc, mrcTemp;
  WPObject * contentObject;
  /* SOMClass *mmAudioClass; */
  SOMClass *mmMIDIClass;
  SOMClass *cwAudioClass=somhlpGetSomClass("MMAudio");

  char name[CCHMAXPATH];
  ULONG ulNameSize;
  char chrCommand[CCHMAXPATH*2];
  char chrDevice[30];
  char retMsg[20];
  ULONG rc;
  int iWavePriv;
  int iTime2;
  HWND hwndNotify;
  BOOL notFound;
  int iTrack,a;

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("Entering %s\n", __FUNCTION__);
#endif

#ifdef RANDOM_DEBUG_2
  HlpWriteToTrapLog("%s  mfData: %x\n", __FUNCTION__, mfData);
  HlpWriteToTrapLog("%s  random array: %x\n", __FUNCTION__, mfData->pRandomArray);
#endif

  /* Check if we currently play the last track. */
  if((mfData->iNumPlayed>=mfData->iNumToPlay) && mfData->pRandomArray) {
    cwStopAudioFile(hwndFrame);/* Reset playing vars and remove in use emphasis */        
    if(!(ulFlags&FLAG_REPEAT))
      return 0;
  }

  hwndContainer=WinWindowFromID(hwndFrame, FID_CLIENT);
  if(!WinIsWindow(WinQueryAnchorBlock(hwndFrame), hwndContainer))
    return 0;

  /* Get class object */
  //  mmAudioClass=queryMMAudioClass();
  mmMIDIClass=somhlpGetSomClass("MMMIDI");

  /* Get new ID for wave device */
  if(cwRequestMutex(hmtxFileName, 100000)==ERROR_TIMEOUT)
    return FALSE;  
  iWaveDev++;
  if(iWaveDev==1000)
    iWaveDev=1;
  iWavePriv=iWaveDev;
  cwReleaseMutex(hmtxFileName);

#ifdef RANDOM_DEBUG
  //  HlpWriteToTrapLog("%s  mfData: %x\n", __FUNCTION__, mfData);
  HlpWriteToTrapLog("%s  random array: %x\n", __FUNCTION__, mfData->pRandomArray);
#endif
  if(!mfData->pRandomArray) {
    /* Not yet playing */
    CNRINFO cnrInfo;
    ULONG ulError;

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s line: %d - No random Array yet\n", __FUNCTION__, __LINE__);
#endif

    WinSendMsg(hwndContainer,CM_QUERYCNRINFO, MPFROMP(&cnrInfo),
               MPFROMLONG(sizeof(cnrInfo)));
    /* No objects?? */
    if(cnrInfo.cRecords==0)
      return 0;
    if((mfData->pRandomArray=wpAllocMem(MAX_RANDOM_OBJECTS , &ulError))==NULLHANDLE)
      return 0;

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: alloced random array\n", __FUNCTION__);
#endif

    memset(mfData->pRandomArray, 0, MAX_RANDOM_OBJECTS);

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: set array to 0\n", __FUNCTION__);
#endif

    mfData->iNumToPlay=cnrInfo.cRecords; /* # tracks to be played */
    mfData->iNumPlayed=0;
    if(mfData->iNumToPlay>MAX_RANDOM_OBJECTS)
      mfData->iNumToPlay=MAX_RANDOM_OBJECTS;
  }

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: Now searchin audio file...\n", __FUNCTION__);
#endif

  /* Find an audio file */
  notFound=TRUE;
  while(notFound){ 
    /* Find track to play */
    if((iTrack=_getRandomTrack(mfData))==-1) {
#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: _getRandomTrack returned an error! Quitting.\n", __FUNCTION__);
#endif
      cwStopAudioFile(hwndFrame);
      return 0;
    }

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: _getRandomTrack returned: %d.\n", __FUNCTION__, iTrack);
#endif
    
    for(a=0,mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,NULL,
                                            MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
        a<iTrack &&  mrc && (int)mrc!=-1;
        a++)
      {
        mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                        MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
#ifdef RANDOM_DEBUG
        HlpWriteToTrapLog("a: %d, %x\n", a, mrc);
#endif
      };

#ifdef RANDOM_DEBUG
        HlpWriteToTrapLog(" a: %d, %x\n", a, mrc);
#endif

    if(!mrc) 
      {
#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: No mrc. Quitting.\n", __FUNCTION__);
#endif
        cwStopAudioFile(hwndFrame);/* Clean up */
        return 0;
      }

    contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
    if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE)
      continue;

    /* Check if it's an audio file thus playable */
    if(contentObject->somIsA(cwAudioClass) || 
       /*       contentObject->somIsA(mmAudioClass) ||*/
       contentObject->somIsA(mmMIDIClass)) {
      notFound=FALSE;
      ulNameSize=sizeof(name);
      ((WPFileSystem*)contentObject)->wpQueryRealName(name,&ulNameSize,TRUE);
    }
  };/* while */

#ifdef RANDOM_DEBUG
  HlpWriteToTrapLog("%s: Got an object. Trying to start it...\n", __FUNCTION__);
#endif

  /* Now start it */
  PBYTE pByte=mfData->pRandomArray;
  /* Prevent stop method from deleting our vars */
  mfData->pRandomArray=NULLHANDLE;

  cwStopAudioFile(hwndFrame);/* Reset playing vars and remove in use emphasis */

  if(!getMessage(chrCommand, IDSTR_STARTINGTRACK, sizeof(chrCommand), queryResModuleHandle(),  hwndFrame))
    sprintf(chrCommand, "Starting track...");
  setPlayTimeText2(this, chrCommand);

  mfData->pRandomArray=pByte;

  if(contentObject->somIsA(mmMIDIClass))
    strncpy(chrDevice,"SEQUENCER", sizeof(chrDevice));
  else /*if(contentObject->somIsA(cwAudioClass) || contentObject->somIsA(mmAudioClass))*/
    strncpy(chrDevice,"WAVEAUDIO", sizeof(chrDevice));
  
  chrDevice[sizeof(chrDevice)-1]=0;

  hwndNotify=playControlDialogFromFrameHWND(hwndFrame);//WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE);

  /* Start audio file */  
  sprintf(chrCommand,"open \"%s\"  type %s alias wave%d SHAREABLE wait",name, chrDevice, iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), hwndNotify, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS)
    return 0;
  /* Set time format */
  sprintf(chrCommand,"SET wave%d TIME FORMAT MILLISECONDS wait", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d wait",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }      

  sprintf(chrCommand,"SETPOSITIONADVISE wave%d ON EVERY 1000 notify", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg),hwndNotify, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d wait",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }

  /* Get wave length in ms */
  sprintf(chrCommand,"STATUS wave%d LENGTH WAIT", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)==MCIERR_SUCCESS) {
    iTime2=atoi(retMsg);
    sprintf(chrCommand,"%d:%02d  %d:%02d  -%d:%02d",iTime2/1000/60,(iTime2/1000)%60,
            0, 0,
            iTime2/1000/60,(iTime2/1000)%60);
    //    WinSetWindowText(WinWindowFromID(hwndTop, IDST_PLAYTIME), chrCommand);
    //    WinSetWindowText(WinWindowFromID(playControlDialogFromFrameHWND(hwndFrame), IDST_PLAYTIME), chrCommand);
    //    WinSetWindowText(playTimeControlFromFrameHWND(hwndFrame), chrCommand);
  }
  ulTotalLength=iTime2;

  /* Set volume */
  sprintf(chrCommand,"SET wave%d AUDIO VOLUME %d wait", iWavePriv, ulVolume);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }

  sprintf(chrCommand,"play wave%d FROM 0", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }
  /* Start pos timer. SET MCIPOSITIONADVICE fails when seeking in the wave. 
     Don't know why. */
  //  WinStartTimer(WinQueryAnchorBlock(hwndFrame), WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE), IDTIMER_PLAY, PLAYTIMER_DELAY);  
  WinStartTimer(WinQueryAnchorBlock(hwndFrame), playControlDialogFromFrameHWND(hwndFrame), IDTIMER_PLAY, PLAYTIMER_DELAY);  

  /* Set emphasis by adding the object to the inuse list */
  memset(&myViewItem, 0,sizeof(myViewItem));
  myViewItem.useItem.type=USAGE_OPENVIEW;
  myViewItem.viewItem.view=OPEN_PLAYING;

  WPObject* wpObject=contentObject;

  if(wpObject->wpAddToObjUseList(&myViewItem.useItem))
    {
      myViewItem.bIsInUseList=TRUE;
    };
  /* Select the object */
  WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrc),
             MPFROM2SHORT(TRUE, CRA_SELECTED));  

  iWave=iWavePriv;
  currentObject=wpObject;
  mrcPlaying=mrc;
  /* The start position of the play command */
  ulStartPosition=0;
  ulPos=0;
  bPaused=FALSE;
  mfData->iNumPlayed++;

  return 0;
}

BOOL CWMediaFolder::cwStopAudioFile(HWND hwndFrame)
{
  char chrCommand[50];
  char retMsg[100];
  ULONG rc;
  HWND hwndContainer=WinWindowFromID(hwndFrame, FID_CLIENT);

  TRY_LOUD(MEDIAFLDR_STOP_AUDIO) {
    if(iWave) {
      if(mfData->pRandomArray) {
        wpFreeMem(mfData->pRandomArray);
        mfData->pRandomArray=NULLHANDLE;
        mfData->iNumToPlay=0; /* # tracks to be played */
        mfData->iNumPlayed=0;
      }
      
      sprintf(chrCommand,"stop wave%d wait", iWave);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      
      sprintf(chrCommand,"close wave%d wait", iWave);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      
      _resetDisplay(this);
        //_resetDisplay(topControlFromFrameHWND(hwndFrame));
      //      WinStopTimer(WinQueryAnchorBlock(hwndFrame), WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE), IDTIMER_PLAY);
      WinStopTimer(WinQueryAnchorBlock(hwndFrame), playControlDialogFromFrameHWND(hwndFrame), IDTIMER_PLAY);

      /* Remove from inuse list */
      if(myViewItem.bIsInUseList && somIsObj(currentObject))
        {
          WPObject *wpObject=cwGetFileSystemObject(currentObject);
          if(somIsObj(wpObject))
            if(wpObject->wpDeleteFromObjUseList(&myViewItem.useItem))
              myViewItem.bIsInUseList=FALSE;
        };
      
      //HlpWriteToTrapLog("\n1 line %d.\n");
      
      /* Deselect the object */
      if(WinIsWindow(WinQueryAnchorBlock(hwndFrame), hwndContainer))
        WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrcPlaying),
                   MPFROM2SHORT(FALSE, CRA_SELECTED));  
      
      iWave=0;
      currentObject=NULLHANDLE;
      mrcPlaying=NULLHANDLE;
      ulPos=0;
    }
    else
      {
        PMINIRECORDCORE mrc;
        WPObject * contentObject;

        if(mmGetOtherPlayingAudioFile( this, hwndContainer, &contentObject,  &mrc)) {
          ((MMAudio*)contentObject)->wpViewObject( NULLHANDLE,0xbbf9, NULLHANDLE);
#if 0
          iWave=0;
          currentObject=NULLHANDLE;
          mrcPlaying=NULLHANDLE;
          ulPos=0;
#endif
          WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrc),
                     MPFROM2SHORT(FALSE, CRA_SELECTED));        
        }
#if 0
        PMINIRECORDCORE mrc;
        WPObject * contentObject;

        /* See if we have an audio file played with doubleclick */
        mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS,MPFROMLONG(CMA_FIRST),
                                        MPFROMSHORT(CRA_INUSE));
        if(mrc) {
          SOMClass *cwAudioClass=cwGetSomClass("MMAudio");
          contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
          if(somIsObj(contentObject) && somIsObj(cwAudioClass)) {
            if((contentObject=cwGetFileSystemObject(contentObject))!=NULLHANDLE) {
              /* Check if it's an audio file thus playable */
              if(contentObject->somIsA(cwAudioClass)) {
                if(((MMAudio*)contentObject)->wpQueryDefaultView()==0xbbf9){
                  /* If stop menu is default, this means the track is playing. */
                  ((MMAudio*)contentObject)->wpViewObject( NULLHANDLE,0xbbf9, NULLHANDLE);
                  iWave=0;
                  currentObject=NULLHANDLE;
                  mrcPlaying=NULLHANDLE;
                  ulPos=0;
                  WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrc),
                             MPFROM2SHORT(FALSE, CRA_SELECTED));  

                };
              }
            }
          }/* somIsObj() */
        }
#endif
      }
  }/* Try */
  CATCH(MEDIAFLDR_STOP_AUDIO)
    {
      SysWriteToTrapLog("\nTrap occured in %s, file: %s, near line %d.\n",
                        __FUNCTION__, __FILE__, __LINE__);
    } END_CATCH;
    
    return TRUE;
}

BOOL CWMediaFolder::cwPauseAudioFile(HWND hwndFrame)
{
  char chrCommand[50];
  char retMsg[100];
  ULONG rc;

  if(!bPaused) {
    sprintf(chrCommand,"PAUSE wave%d wait", iWave);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    bPaused=TRUE;
  }else
    {
      sprintf(chrCommand,"RESUME wave%d wait", iWave);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      bPaused=FALSE;
      showPlayTimeDisplay(this, TRUE);
    }

  return TRUE;
}

BOOL  CWMediaFolder::cwSeekAudioFile(LONG lPosSec)
{
  char chrCommand[50];
  char retMsg[20];
  APIRET rc;

  sprintf(chrCommand,"PLAY wave%d from %d", iWave, lPosSec);
  rc=mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  ulStartPosition=lPosSec;
  bPaused=FALSE;

  return TRUE;
}

void CWMediaFolder::cwPlayTimeCallback(HWND hwndFrame, LONG lPosSec)
{
  char chrCommand[50];
  char retMsg[20];
  ULONG rc;

  /* Timeout. Check if wave is still playing */
  sprintf(chrCommand,"STATUS wave%d MODE WAIT", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. Clean up. The Timer will be stopped in the method */
    cwStopAudioFile(hwndFrame); 
  }
  else {
    if(!stricmp(retMsg, "stopped")) {
      /* Audio file played. Start next */
      cwPlayAudioFile(hwndFrame, PLAY_NEXT); 
    }
    showPlayTimeDisplay(this, TRUE);/* Ensure display is visible */
  }
}


