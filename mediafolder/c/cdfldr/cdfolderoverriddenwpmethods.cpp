/*
 * This file is (C) Chris Wohlgemuth 2002-2005
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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "cdfolder.hh"
#include "mediafolderres.h"
#include "cwcdtrack.hh"

#include "sys_funcs.h"
 
extern HMTX hmtxFileName;
extern int  iWaveDev;
PFNWP pfnwpGenericCDFrame;

/* For calling the method in the MMAudio class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
typedef ULONG   SOMLINK somTP_MMCDTrack_cwmmQueryTrackInfo(MMCDTrack *somSelf,
		char** chrString,
		ULONG ulSize,
		int iWhich);
typedef somTP_MMCDTrack_cwmmQueryTrackInfo *somTD_MMCDTrack_cwmmQueryTrackInfo;
extern somTD_MMCDTrack_cwmmQueryTrackInfo methodPtrCDTrack;

BOOL extern CDQueryCDDrives(int *iNumCD, char * cFirstDrive);

BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
HMODULE queryResModuleHandle(void);

ULONG cwRequestMutex(HMTX  hmtxBMP, ULONG ulTimeOut);
ULONG cwReleaseMutex(HMTX  hmtxBMP);

SOMClass* somhlpGetSomClass(char* chrClassName);
extern BOOL somhlpQueryCWCDTrackMethodPtr(CWMediaFolder *thisPtr);
MRESULT EXPENTRY cdFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
extern void showPlayTimeDisplay(CWMediaFolder *thisPtr, BOOL bShow);
void setPlayTimeText2(CWMediaFolder *thisPtr, char * theText);

/* Get the HWND of the play control dialog. This Dialog is on top of
   the top frame control. */
HWND playControlDialogFromFrameHWND(HWND hwndFrame);


BOOL CWCDFolder::wpSetupOnce(PSZ pSetupString)
{
  /**************************************************
   *                                                *
   *                                                *
   **************************************************/
  BOOL rcParent=FALSE;
  char buffer[CCHMAXPATH];
  ULONG bufferSize;
  int iIndex;

  rcParent=CWMediaFolder::wpSetupOnce(pSetupString);

  /* Doing our own setup here if not done by the user. */
#if 0
  /* This is already done in the media folder class */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"DEFAULTVIEW",buffer,&bufferSize))
    wpSetup( "DEFAULTVIEW=DETAILS");/* Fill in defaults */

  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"ICONVIEW",buffer,&bufferSize))
    wpSetup( "ICONVIEW=NONFLOWED,MINI");/* Fill in defaults */

  /* Size and position of the folder window */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"ICONVIEWPOS",buffer,&bufferSize))
    wpSetup( "ICONVIEWPOS=20,10,60,75");/* Fill in defaults */
#endif

  /* Set folder details- and sortclass to CWAudioShadow so track infos are shown */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"DETAILSCLASS",buffer,&bufferSize))
    wpSetup("DETAILSCLASS=MMAudio");/* Fill in defaults */
  
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"SORTCLASS",buffer,&bufferSize))
    wpSetup("SORTCLASS=MMCDTrack");/* Fill in defaults */
  
  /* This works only on Warp 4 and above */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"DETAILSTODISPLAY",buffer,&bufferSize))
    /* This works only on Warp 4 and above */
    wpSetup("DETAILSTODISPLAY=0,1,12,16,17,18");/* Fill in defaults */
  
  /* This works only on Warp 4 and above */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"SORTBYATTR",buffer,&bufferSize))
    /* This works only on Warp 4 and above */
    wpSetup("SORTBYATTR=6,24,28,29,30");/* Fill in defaults */
  
  return rcParent;
}

BOOL CWCDFolder::wpSetup(PSZ pSetupString)
{
  /**************************************************
   *                                                *
   *                                                *
   **************************************************/
  char buffer[3];
  ULONG bufferSize;

  /* Set drive letter */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, CDFLDR_DRIVE, buffer,&bufferSize))
    {
      if(buffer[1]==':' /*&& (buffer[0]>='A' && buffer[0]<='Z' )*/) {
        buffer[2]=0;
        strcpy(chrDrive, buffer);
      }
    }
  return CWMediaFolder::wpSetup(pSetupString);
}

BOOL CWCDFolder::wpSaveState()
{

  wpSaveString("CWCDFolder", KEY_DRIVE, (PBYTE)&chrDrive);

  return CWMediaFolder::wpSaveState();
}

BOOL CWCDFolder::wpRestoreState(ULONG ulReserved)
{
  ULONG dataSize;

  chrDrive[0]=0;
  dataSize=sizeof(chrDrive);
  if(wpRestoreString("CWCDFolder",KEY_DRIVE,(PBYTE)&chrDrive,&dataSize))
    chrDrive[2]=0;

  return CWMediaFolder::wpRestoreState(ulReserved);
}

ULONG CWCDFolder::wpFilterPopupMenu(ULONG ulFlags, HWND hwndCnr, BOOL fMultiSelect)
{
  return CWMediaFolder::wpFilterPopupMenu(ulFlags, hwndCnr, fMultiSelect)&
    ~(CTXT_SORT|CTXT_ARRANGE|CTXT_COPY|CTXT_ICON|CTXT_CHANGETOICON|CTXT_CHANGETOTREE|CTXT_CHANGETODETAILS);
}

ULONG CWCDFolder::wpAddObjectGeneralPage2(HWND hwndNotebook)
{
  return SETTINGS_PAGE_REMOVED;
}

ULONG CWCDFolder::wpAddFolderSortPage(HWND hwndNotebook)
{
  return SETTINGS_PAGE_REMOVED;
}

ULONG CWCDFolder::wpAddFolderIncludePage(HWND hwndNotebook)
{
  return SETTINGS_PAGE_REMOVED;
}

ULONG CWCDFolder::wpQueryConcurrentView()
{
  /* Only one view allowed */
  return 2;//CCVIEW_OFF
}

MRESULT CWCDFolder::wpDragOver(HWND hwndCnr,PDRAGINFO pDragInfo)
{
  return MRFROM2SHORT( DOR_NEVERDROP, DO_MOVE);/* Dropping is not allowed */
}

BOOL CWCDFolder::wpRefresh(ULONG ulView, PVOID pReserved)
{
  //  SysWriteToTrapLog("%s: view: %d, %x, %x\n", 
  //                __FUNCTION__, ulView, ulView, cwQueryMFldrFlags() );

  /* User changes the view of the CD-Folder, don't reinsert the tracks when changing from/to
     compact view. */
  if(ulView==OPEN_DETAILS &&  !(cwQueryMFldrFlags() & MFLDR_VIEW_COMPACT))
    cwInsertAllTracks(chrDrive);
  /* Normal refresh */
  if(ulView==0)
    cwInsertAllTracks(chrDrive);
  return CWMediaFolder::wpRefresh(ulView, pReserved);
}


HWND CWCDFolder::wpOpen(HWND hwndCnr,ULONG ulView,ULONG ulParam)
{

  if(ulView==OPEN_CONTENTS||ulView==OPEN_TREE||ulView==OPEN_DETAILS||ulView==OPEN_DEFAULT /*|| ulView==OPEN_MMDISK*/) {
    /* OPEN_MMDISK is a private view to indicate the folder was opened from the CD drive */
    cwInsertAllTracks(chrDrive);
    hwndFrameCDFldr=CWMediaFolder::wpOpen(hwndCnr, ulView, ulParam);

    if(hwndFrameCDFldr) {
      /* We have the folder window */
      char chrTemp[50];
      HOBJECT hObject;

      /* Add view to drives inuse list */
      sprintf(chrTemp,"<WP_DRIVE_%c>",chrDrive[0]);
      if((hObject=WinQueryObject(chrTemp))!=NULLHANDLE) {
        WPObject *wpObject;

        wpObject=((M_WPObject*)__ClassObject)->wpclsQueryObject( hObject);
        if(somIsObj(wpObject)) {
          diskUseItem.wpDiskObj=wpObject;
          diskUseItem.useItem.type=USAGE_OPENVIEW;
          memset(&diskUseItem.viewItem, 0, sizeof(VIEWITEM));
          diskUseItem.viewItem.handle=hwndFrameCDFldr;
          diskUseItem.viewItem.view=OPEN_DEFAULT;
          wpObject->wpAddToObjUseList(&diskUseItem.useItem);
        }/* somIsObj() */
      }

      /* Remove random checkbox for now... */
      WinShowWindow( WinWindowFromID(playControlDialogFromFrameHWND(hwndFrameCDFldr), IDCB_RANDOM)
                     , FALSE);
      /* Subclass frame to remove items from menubar */
      pfnwpCDFrame=WinSubclassWindow(hwndFrameCDFldr, cdFrameProc);
      pfnwpGenericCDFrame=pfnwpFrame;
    }/* if(hwnd) */
    return hwndFrameCDFldr;
  }
  return CWMediaFolder::wpOpen(hwndCnr, ulView, ulParam);
}

BOOL CWCDFolder::cwClose(HWND hwndFrame)
{
  BOOL bRC;

  bRC=CWMediaFolder::cwClose(hwndFrame);

  if(bRC) {
    /* User confirmed close */
    if(somIsObj(diskUseItem.wpDiskObj)) {
      diskUseItem.wpDiskObj->wpDeleteFromObjUseList(&diskUseItem.useItem);
      /* The disk object is locked because of wpclQueryObject in wpOpen() */
      diskUseItem.wpDiskObj->wpUnlockObject();
      hwndFrameCDFldr=NULLHANDLE;
    }
  }
  return bRC;
}

ULONG CWCDFolder::cwPlayAudioFile(HWND hwndFrame, ULONG ulWhich)
{
  HWND hwndContainer;
  PMINIRECORDCORE mrc, mrcTemp;
  WPObject * contentObject;
  WPObject * tempObject;
  SOMClass *mmCDTrackClass;
  char chrCommand[CCHMAXPATH*2];
  char chrDevice[30];
  char retMsg[20];
  ULONG rc;
  int iWavePriv;
  int iTime2;
  int iPrevTrack=0;


  hwndContainer=WinWindowFromID(hwndFrame, FID_CLIENT);
  if(!WinIsWindow(WinQueryAnchorBlock(hwndFrame), hwndContainer))
    return 0;

  /* Get class object */
  mmCDTrackClass=somhlpGetSomClass("MMCDTrack");
  if(!mmCDTrackClass)
    return 0;

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
          if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE)
            continue;
          tempObject=contentObject;
        }
        else
          return 0;/* Error */
        
        if(contentObject->somIsA(mmCDTrackClass))
          notFound=FALSE;
        else
            /* Try next object */
            mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                            MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
      }while(notFound && mrc);
      break;
    }
  case PLAY_NEXT:
  case PLAY_NEXT_CDFLDR:
    if(iWave==0)
      return cwPlayAudioFile(hwndFrame, PLAY_FIRST);/* Not yet playing */

    /* We are currently playing */
    mrcTemp=mrcPlaying;

    /* Get next container item of our folder */
    mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,mrcTemp,
                                    MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));

    if(!mrc || (int)mrc==-1)
      return 0;

    notFound=TRUE;
    while(notFound && mrc && (int)mrc!=-1){ 
      contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
      if(somIsObj(contentObject)) {
        /* Get file system object or NULL */
        if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE)
          continue;
        tempObject=contentObject;
      }
      else
        return 0;/* Error */
      
      if(contentObject->somIsA(mmCDTrackClass))
        notFound=FALSE;
      else
        /* Try next object */
        mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                        MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
    };

    /* Remove from inuse list */
    if(myViewItem.bIsInUseList)
      {
        WPObject *wpObject=cwGetFileSystemObject(currentObject);
        if(wpObject->wpDeleteFromObjUseList(&myViewItem.useItem))
          myViewItem.bIsInUseList=FALSE;
      };

    /* Deselect the object */
    if(WinIsWindow(WinQueryAnchorBlock(hwndFrame), hwndContainer))
      WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrcPlaying),
                 MPFROM2SHORT(FALSE, CRA_SELECTED));  
    
    if(!mrc) {
      if(ulFlags & FLAG_REPEAT)
        return cwPlayAudioFile(hwndFrame, PLAY_FIRST);
      else {
        cwStopAudioFile(hwndFrame);
        return 0;
      }
    }

    break;
  case PLAY_PREV:

    if(iWave==0)
      return cwPlayAudioFile(hwndFrame, PLAY_FIRST);/* Not yet playing, so start the first one. */
 
   /* We are currently playing */
    mrcTemp=mrcPlaying;
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
        if((contentObject=cwGetFileSystemObject(contentObject))==NULLHANDLE)
          continue;
        tempObject=contentObject;
      }
      else
        return 0;/* Error!! */

      /* Check if it's an audio file thus playable */
      if(contentObject->somIsA(mmCDTrackClass))
        notFound=FALSE;
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
  }/* switch */

  //  if(ulWhich!=PLAY_NEXT_CDFLDR) {
    if(!getMessage(chrCommand, IDSTR_STARTINGTRACK, sizeof(chrCommand), queryResModuleHandle(),  hwndFrame))
      sprintf(chrCommand, "Starting track...");
    setPlayTimeText2(this, chrCommand);
    // }

  strncpy(chrDevice,"CDAUDIO", sizeof(chrDevice));

  PSZ pszObjectID;
  if((pszObjectID=wpQueryObjectID())!=NULLHANDLE) {
   int iNumCD;
   char chrFirstCD;

   strncpy(chrCommand, pszObjectID,sizeof(chrCommand));
   chrCommand[sizeof(chrCommand)-1]=0;
   if(CDQueryCDDrives(&iNumCD, &chrFirstCD))
     sprintf(chrDevice,"CDAUDIO%02d",  chrCommand[strlen(chrCommand)-2]-chrFirstCD+1);
  }
   
  chrDevice[sizeof(chrDevice)-1]=0;

  /* Get track num */
  if(!methodPtrCDTrack)
    somhlpQueryCWCDTrackMethodPtr(this);
  if(methodPtrCDTrack)
    iTime2=methodPtrCDTrack((MMCDTrack*)contentObject, NULLHANDLE, 0, IDINFO_TRACKNUM);
  if(methodPtrCDTrack && currentObject)
    iPrevTrack=methodPtrCDTrack((MMCDTrack*)currentObject, NULLHANDLE, 0, IDINFO_TRACKNUM);

  //    HlpWriteToTrapLog("PrevTrack: %d new track: %d\n", iPrevTrack, iTime2);

  if(iPrevTrack+1!=iTime2 || !mrcPlaying) {
    iWave=iWavePriv;

    /* Start audio file */  
    sprintf(chrCommand,"open \"%s\" alias wave%d SHAREABLE wait", chrDevice, iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), playControlDialogFromFrameHWND(hwndFrame), 0);

    if((rc & 0x0000ffff)!=MCIERR_SUCCESS)
      return 0;

    /* Set volume */
    sprintf(chrCommand,"SET wave%d AUDIO VOLUME %d wait", iWavePriv, ulVolume);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
      sprintf(chrCommand,"close wave%d",iWavePriv);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      return 0;
    }

    sprintf(chrCommand,"SET wave%d TIME FORMAT TMSF wait", iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
      sprintf(chrCommand,"close wave%d wait",iWavePriv);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      return 0;
    }

    //sprintf(chrCommand,"play wave%d FROM %d TO %d", iWavePriv, iTime2, iTime2+1);
    sprintf(chrCommand,"play wave%d FROM %d ", iWavePriv, iTime2);
    //    HlpWriteToTrapLog(chrCommand);
    //    HlpWriteToTrapLog("\niTime2: %d\n", iTime2);

    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
      sprintf(chrCommand,"close wave%d",iWavePriv);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      return 0;
    }
  }/*   if( ...|| !mrcPlaying) */

  if(iPrevTrack+1==iTime2 && ulWhich!=PLAY_NEXT_CDFLDR) {
    /* Previous track ended, now playing the next. Or user pressed 'Next' while playing */
    if(ulPos*1000 < ulTotalLength) {
      WinStopTimer(WinQueryAnchorBlock(hwndFrame), playControlDialogFromFrameHWND(hwndFrame), IDTIMER_PLAY);
      sprintf(chrCommand,"SET wave%d TIME FORMAT TMSF wait", iWave);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
        sprintf(chrCommand,"close wave%d wait",iWave);
        rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
        return 0;
      }

      sprintf(chrCommand,"play wave%d FROM %d ", iWave, iTime2);
      //      HlpWriteToTrapLog(chrCommand);
      rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
      if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
        sprintf(chrCommand,"close wave%d",iWavePriv);
        rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
        return 0;
      }
    }
  }

  /* Get length in ms */
  if(!methodPtrCDTrack)
    somhlpQueryCWCDTrackMethodPtr(this);
  if(methodPtrCDTrack){  /* Query playtime in secs */
    iTime2=methodPtrCDTrack((MMCDTrack*)contentObject, NULLHANDLE, 0, IDINFO_PLAYTIME);
  }

  sprintf(chrCommand,"%d:%02d  %d:%02d  -%d:%02d", iTime2/60, iTime2%60,
          0, 0,
          iTime2/60, iTime2%60);

  
  //  WinSetWindowText(WinWindowFromID(hwndTop, IDST_PLAYTIME), chrCommand);
  //    WinSetWindowText(playTimeControlFromFrameHWND(HWND hwndFrame), chrCommand);
  ulTotalLength=iTime2*1000; /* convert to ms */
  //  HlpWriteToTrapLog("ulTotalLength:  %d\n", ulTotalLength);

  /* Set emphasis by adding the object to the inuse list */
  memset(&myViewItem, 0,sizeof(myViewItem));
  myViewItem.useItem.type=USAGE_OPENVIEW;
  myViewItem.viewItem.view=OPEN_PLAYING;

  // HlpWriteToTrapLog("Adding to inuse list\n");
  WPObject* wpObject=cwGetFileSystemObject(tempObject);
  // HlpWriteToTrapLog("wpObject: %x somIsObj(wpObject): %x \n", wpObject, somIsObj(wpObject));


  if(!myViewItem.bIsInUseList) {
    if(wpObject->wpAddToObjUseList(&myViewItem.useItem))
      myViewItem.bIsInUseList=TRUE;
  };

  /* Select the object */
  WinSendMsg(hwndContainer,CM_SETRECORDEMPHASIS, MPFROMP(mrc),
             MPFROM2SHORT(TRUE, CRA_SELECTED));  

  /* Start pos timer. SET MCIPOSITIONADVICE fails when seeking in the wave. 
     Don't know why. */
  WinStartTimer(WinQueryAnchorBlock(hwndFrame), playControlDialogFromFrameHWND(hwndFrame), 
                IDTIMER_PLAY, PLAYTIMER_DELAY);

  currentObject=wpObject;
  mrcPlaying=mrc;
  /* The start position of the play command */
  ulStartPosition=0;
  ulPos=0;
  bPaused=FALSE;
  return 0;
}


BOOL  CWCDFolder::cwSeekAudioFile(LONG lPosMSSec)
{
  char chrCommand[50];
  char retMsg[20];
  APIRET rc;
  int iCurrentTrack;
  ULONG ulPosTrack;

  sprintf(chrCommand,"SET wave%d TIME FORMAT MS wait", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    return 0;
  }
  
  sprintf(chrCommand,"STATUS wave%d CURRENT TRACK WAIT", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    return FALSE;
  }
  iCurrentTrack=atoi(retMsg);

  sprintf(chrCommand,"STATUS wave%d POSITION TRACK %d WAIT", iWave, iCurrentTrack);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. Clean up. The Timer will be stopped in the method */
    return FALSE;
  }
  ulPosTrack=atoi(retMsg);

  sprintf(chrCommand,"PLAY wave%d from %d ", iWave, lPosMSSec+ulPosTrack);
  rc=mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  ulStartPosition=lPosMSSec/1000;
  bPaused=FALSE;

#if 0
  sprintf(chrCommand,"STATUS wave%d POSITION IN TRACK WAIT", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)==MCIERR_SUCCESS) {
    /* Problem with MCI. Clean up. The Timer will be stopped in the method */
    ulPos=atoi(retMsg)/1000;
  }
#endif 

  ulPos=lPosMSSec/1000;

  sprintf(chrCommand,"SET wave%d TIME FORMAT TMSF wait", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);

  return TRUE;
}

void CWCDFolder::cwPlayTimeCallback(HWND hwndFrame, LONG lPosSec)
{
  char chrCommand[50];
  char retMsg[20];
  ULONG rc;
  int iPrevTrack;
  int iCurrentTrack;

  //SysWriteToTrapLog("\n%s:\n", __FUNCTION__);

  sprintf(chrCommand,"STATUS wave%d MODE WAIT", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. Clean up. The Timer will be stopped in the method */
    cwStopAudioFile(hwndFrame); 
    return;
  }
  else
    if(!stricmp(retMsg, "stopped")) {
      /* Audio file played. Start next */
      cwPlayAudioFile(hwndFrame, PLAY_NEXT); 
      showPlayTimeDisplay(this, TRUE);/* Ensure display is visible */
      return;
    }

  /* Get track num */
  if(!methodPtrCDTrack)
    somhlpQueryCWCDTrackMethodPtr(this);
  if(methodPtrCDTrack && currentObject)
    iPrevTrack=methodPtrCDTrack((MMCDTrack*)currentObject, NULLHANDLE, 0, IDINFO_TRACKNUM);

  sprintf(chrCommand,"STATUS wave%d CURRENT TRACK WAIT", iWave);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  //  HlpWriteToTrapLog("Track: %d, retmsg: %s\n", iPrevTrack, retMsg);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. Clean up. The Timer will be stopped in the method */
    cwStopAudioFile(hwndFrame); 
  }
  else {
    iCurrentTrack=atoi(retMsg);
    //SysWriteToTrapLog(" iWave: %d. iPrevTrack: %d, iCurrentrack: %d\n", iWave, iPrevTrack, iCurrentTrack);
    /* Timeout. Check if track is still playing */
    if(iCurrentTrack==iPrevTrack+1) {
      /* Audio file played. Start next */
      //SysWriteToTrapLog(" iWave: %d. iPrevTrack: %d, iCurrentrack: %d, startinb next (%d)\n", iWave, iPrevTrack, iCurrentTrack, __LINE__);
      cwPlayAudioFile(hwndFrame, PLAY_NEXT_CDFLDR); 
    }
  }
}



