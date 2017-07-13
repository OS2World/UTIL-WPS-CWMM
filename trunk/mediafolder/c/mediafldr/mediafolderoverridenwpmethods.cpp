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
#define INCL_DOSERRORS
#define INCL_GPIBITMAPS

#include "mediafolder.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mediafolderres.h"
#include "menufolder.hh"

#include "sys_funcs.h"

/* typedef for somTP_MMAudio_cwmmQueryTrackInfo */
/* For calling the method in the CWAudio class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
#include "cwmmquerytrackinfo.h"

extern LOADEDBITMAP allBMPs[];
extern somTD_MMAudio_cwmmQueryTrackInfo methodPtr;

BOOL g_fRegisteredWindowClass=FALSE;
ULONG g_ulFrameDataOffset=0;

/* The following offsets will be calclated in wpOpen() */
ULONG ulQWP_CONTAINERPROC=0;
ULONG ulQWP_THISPTR=4;
ULONG ulQWL_TOPCTRLHWND=8;
ULONG ulQWL_BOTTOMCTRLHWND=12;
ULONG ulQWL_RIGHTCTRLHWND=16;
ULONG ulQWL_LEFTCTRLHWND=20;

PFNWP pfnwpGenericFrame;

void errorResource();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
void getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
void sendCommand(char *chrObject, char* command);
HMODULE queryResModuleHandle(void);

MRESULT EXPENTRY topCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY rightCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY leftCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY bottomCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY mediaFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY mediaFldrContainerProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
/* This function sets the repeat button checkstate */
void winCheckRepeatButton(CWMediaFolder *thisPtr);
/* This function sets the position of the volume slider */
void winSetVolumeSlider(CWMediaFolder *thisPtr);
HWND playTimeControlFromFrameHWND(HWND hwndFrame);
HWND bottomControlFromFrameHWND(HWND hwndFrame);
HWND playControlDialogFromFrameHWND(HWND hwndFrame);
HWND topControlFromFrameHWND(HWND hwndFrame);

extern void _Optlink playThreadFunc (void *arg);/* Every folder has one running play thread to handle play commands */

MRESULT cwInsertMenuSeparator(int iPosition, HWND hwndMenu, HWND hwndSubMenu);

SOMClass* somhlpQueryMMAudioClass(void);
SOMClass* somhlpGetSomClass(char* chrClassName);
BOOL somhlpQueryCWAudioMethodPtr(CWMediaFolder *thisPtr);

/* ID tag defines */
#define IDINFO_NAME              1
#define IDINFO_ARTIST            2
#define IDINFO_ALBUM             3
#define IDINFO_YEAR              4
#define IDINFO_COMMENT           5
#define IDINFO_GENRE             6

#define IDINFO_PLAYTIME          7
#define IDINFO_BPS               8
#define IDINFO_SAMPLERATE        9
#define IDINFO_CHANNELS          10

#define IDINFO_LASTINFO          10

/***************************************************************/
/*                                                             */
/* This thread calculates the size of the audio files in this  */
/* folder. It is started by the status line dialog after the    */
/* folder is opened.                                           */
/*                                                             */
/***************************************************************/
void _Optlink openThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  THREADPARAMS* tPt;
  CWMediaFolder *thisPtr;
  MMAudio* cwAudio;

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
      WPObject *wpObject;
      WPObject *fsObject;
      HWND hwndContainer;
      CNRINFO cnrInfo;
      char text[100];
      char time[50];
      int iNum=0;
      somId    mySomId;
      SOMClass *cwAudioClass=somhlpGetSomClass("MMAudio");
      ULONG ulSize=0;
      ULONG ulSecs=0;

      TRY_LOUD(MEDIAFLDR_OPEN_THREAD) {
        /* Wait until folder is populated */
        while(!(thisPtr->wpQueryFldrFlags() & FOI_POPULATEDWITHALL))
          DosSleep(300);
        
        hwndContainer=WinWindowFromID(tPt->hwndFrame, FID_CLIENT);
        WinSendMsg(hwndContainer,CM_QUERYCNRINFO, MPFROMP(&cnrInfo),
                   MPFROMLONG(sizeof(cnrInfo)));
        
        /* Get class object of MMAudio. We need it so we can check if a file is an audio file. */
        if(somIsObj(cwAudioClass)) {
          /* We have the class object */
          char chrBuf[2];
          /* Get first object in the container */
          wpObject=thisPtr->wpQueryContent(NULLHANDLE, QC_FIRST);
          /* Works only for size < 4Gb up to now */
          while(wpObject)
            {
              /* Follow shadows to the filesystem object (if any) */
              if((fsObject=thisPtr->cwGetFileSystemObject(wpObject))!=NULLHANDLE){/* Filesystem object or NULL */
                /* Check if the file is an audio file */
                if(fsObject->somIsA(cwAudioClass)) {
                  /* It's an audio file */
                  if(!methodPtr)
                    somhlpQueryCWAudioMethodPtr(thisPtr);
                  ulSize+=((WPFileSystem*)fsObject)->wpQueryFileSize();
                  iNum++;
                  if(methodPtr)/* Query playtime in secs */
                    ulSecs+=methodPtr((MMAudio*)fsObject, NULLHANDLE, 0, IDINFO_PLAYTIME);
                }
              }
              wpObject=thisPtr->wpQueryContent(wpObject, QC_NEXT);
            };/* while(wpObject) */

          if(thisPtr->mfData) {
            thisPtr->mfData->ulTotalTime=ulSecs;
            thisPtr->mfData->ulNumTracks=iNum;
            thisPtr->mfData->ulSize=ulSize;
            thisPtr->mfData->bOpened=TRUE;
          }
          /* Set status line text */
          thisPtr->cwSetStatusLineText(NULLHANDLE);
        }/* if(somIsObj(cwAudioClass)) */
      else
        {
          /* Error */
          /* !!!!!!!!!!! Insert messagebox here. !!!!!!!!!!!!!!!*/
        }
      }
      CATCH(MEDIAFLDR_OPEN_THREAD)
        {
          SysWriteToTrapLog("\nTrap occured in %s, file: %s, near line %d.\n",
                            __FUNCTION__, __FILE__, __LINE__);
        } END_CATCH;

      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }

  thisPtr->wpFreeMem((PBYTE)tPt);
}


BOOL CWMediaFolder::wpAddToContent(WPObject* Object)
{
  WPFileSystem *fsObject;
  char time[200];
  ULONG ulSecs;
  BOOL rc;
  static int iCounter=0;

  rc=CWProgFolder::wpAddToContent(Object);

  if(mfData)
    if(mfData->bOpened)/* Only post notification if the folder was already populated. On first
                          opening a separate thread calculates the contents. */
      WinPostMsg(hwndBottom, WM_APPTERMINATENOTIFY,MPFROMSHORT(NOTIFY_STARTSTATUSTIMER), 0L);

  iCounter++;
  if(iCounter==10) {
    DosSleep(30);/* Yield processor to another thread to prevent WPS blocking */
    iCounter=0;
  }
  return rc;
  /* On first opening a separate thread calculates the playtime. Doing it this way we don't
     update the statusbar for every single object. */
}

BOOL CWMediaFolder::wpDeleteFromContent(WPObject* Object)
{
  WPFileSystem *fsObject;
  char time[200];
  ULONG ulSecs;
  
  if(!methodPtr)
    somhlpQueryCWAudioMethodPtr(this);
  
  if(methodPtr){
    if((fsObject=(WPFileSystem*)cwGetFileSystemObject(Object))!=NULLHANDLE){/* Filesystem object or NULL */
      if(mfData) {
        /* Query playtime in secs */
        mfData->ulTotalTime-=methodPtr((MMAudio*)fsObject, NULLHANDLE, 0, IDINFO_PLAYTIME);
        mfData->ulNumTracks-=1;
        mfData->ulSize-=fsObject->wpQueryFileSize();
        cwSetStatusLineText(NULLHANDLE);
      }
    }
  }  
  return CWProgFolder::wpDeleteFromContent(Object);
}

HWND CWMediaFolder::wpOpen(HWND hwndCnr,ULONG ulView,ULONG ulParam)
{
  HWND hwnd;
  SIZEL sizel={0};
  HWND myFrameCtrlHWND;
  RECTL rectl;
  MENUITEM mi={0};
  HWND hwndMenu;
  int iItemCount=0;
  char text[200];
  HMODULE hResource=queryResModuleHandle();

  /* Reregister wpFolder window class on first opening */
  /* We need this private code to have some private 
     window words */
  if(!g_fRegisteredWindowClass)
    {
      CLASSINFO ci;
      
      if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                           (PSZ)"wpFolder window",
                           &ci))
        {
          g_ulFrameDataOffset=ci.cbWindowData;
          //g_pfnwpOrgFrameProc=ci.pfnWindowProc;
          if (WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                               (PSZ)"wpFolder window",
                               ci.pfnWindowProc,
                               ci.flClassStyle,
                               ci.cbWindowData + MFLDR_WINDOWBYTES))
            {
              /* */
              ulQWP_CONTAINERPROC+=g_ulFrameDataOffset;
              ulQWP_THISPTR+=g_ulFrameDataOffset;
              ulQWL_TOPCTRLHWND+=g_ulFrameDataOffset;
              ulQWL_BOTTOMCTRLHWND+=g_ulFrameDataOffset;
              ulQWL_LEFTCTRLHWND+=g_ulFrameDataOffset;
              ulQWL_RIGHTCTRLHWND+=g_ulFrameDataOffset;
              g_fRegisteredWindowClass=TRUE;
            }
        }
    }

  /* Prevent opening in tree view */
  if(ulView==OPEN_TREE || ulView==OPEN_CONTENTS)
    return NULLHANDLE;

#if 0    
  for(pViewItem=wpFindViewItem(VIEW_ANY, NULLHANDLE); pViewItem; pViewItem=wpFindViewItem(VIEW_ANY, pViewItem))
    iItemCount+=1;
#endif

  /* Only one open window for now */
  if(hwndMFldrFrame && ulView!=OPEN_SETTINGS)
    return NULLHANDLE;

  /* Call parent to build folder window */
  hwnd=CWProgFolder::wpOpen(hwndCnr, ulView, ulParam);

  /* Save this ptr */
  //  WinSetWindowULong( hwnd, ulQWP_THISPTR, (ULONG)this);
    
  /* Only subclass folder frames, not settings notebooks */
  if(ulView!=OPEN_SETTINGS){
    if(!hResource) {
      errorResource();
      return hwnd;
    }

    /* Save this ptr */ 
    WinSetWindowULong( hwnd, ulQWP_THISPTR, (ULONG)this); /* Moved from outside the if clause */

    hwndMFldrFrame=hwnd;

    TRY_LOUD(MEDIAFLDR_OPEN) {
      THREADPARAMS*  pTp;
      ULONG ulErr;
      BOOL bDisabled=FALSE;

      if(!(ulFlags & FLAG_ALREADYSIZED)) {
        /* Make sure on first open the container is visible */
        SWP swp;
        /* Prevent window flickering while resizing */
        WinEnableWindowUpdate(hwnd, FALSE);
        bDisabled=TRUE;
        WinQueryWindowPos(hwnd, &swp);
        WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y-100, swp.cx, swp.cy+100, SWP_SIZE|SWP_MOVE);
        ulFlags|=FLAG_ALREADYSIZED;
      }
      
      /* Build new menu entries. */
      myFrameCtrlHWND=WinWindowFromID(hwnd,FID_MENU);//Get menu hwnd
      if(WinIsWindow(WinQueryAnchorBlock(hwnd), myFrameCtrlHWND)) {
        /* Menu bar found */
        if(WinSendMsg(myFrameCtrlHWND,MM_QUERYITEM,MPFROM2SHORT(ID_MBMENUSKRIPTS,FALSE),MPFROMP(&mi))) {
          /* Script menu is inserted so WPS-Wizard is installed */
          hwndMenu=mi.hwndSubMenu;
          if(hwndMenu){
            //          ULONG ulItemId;
            //   ulItemId=cwInsertUserMenuItems(hwndMenu, SKRIPTFOLDER_MEDIAFOLDER, FIRSTUSERITEM, FALSE);
            /*          sprintf(text, " %x",ulItemId);
                        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
                        1234, MB_OK|MB_MOVEABLE);
                        */

            /* The class specific skript menu is inserted in class CWFolder */
            /* Query the number of items in the menubar */
            iItemCount=(int)WinSendMsg(hwndMenu,MM_QUERYITEMCOUNT,(MPARAM)0,
                                       (MPARAM)0);
            if(iItemCount)
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_MEDIAFOLDER, FIRSTUSERITEM, TRUE);
            else
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_MEDIAFOLDER, FIRSTUSERITEM, FALSE);
          }
        }
#ifndef ECS
        else {
          /* Load the user menu from the resource if WPS-Wizard is not installed. If installed the menu item
             is added by WPS-Wizzard. */
          hwndMenu=WinLoadMenu(myFrameCtrlHWND,hResource, ID_MBMENUSKRIPTS);
          if(hwndMenu){
            //          ULONG ulItemId;
            //   ulItemId=cwInsertUserMenuItems(hwndMenu, SKRIPTFOLDER_MEDIAFOLDER, FIRSTUSERITEM, FALSE);
            /*          sprintf(text, " %x",ulItemId);
                        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "",
                        1234, MB_OK|MB_MOVEABLE);
                        */
            /* Query the number of items in the menu */
            iItemCount=(int)WinSendMsg(hwndMenu,MM_QUERYITEMCOUNT,(MPARAM)0,
                                       (MPARAM)0);

            if(iItemCount)
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_MEDIAFOLDER, FIRSTUSERITEM, TRUE);
            else
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_MEDIAFOLDER, FIRSTUSERITEM, FALSE);

            /* Query the number of items in the menubar */
            iItemCount=(int)WinSendMsg(myFrameCtrlHWND,MM_QUERYITEMCOUNT,(MPARAM)0,
                                       (MPARAM)0);
            
            /* Fill the MENUITEM structure */
            mi.iPosition=iItemCount-1;
            mi.afStyle=MIS_TEXT|MIS_SUBMENU;
            mi.id=ID_MBMENUSKRIPTS;
            mi.afAttribute=NULL;                
            mi.hwndSubMenu=hwndMenu;
            mi.hItem=NULL;
            /* Add the Misc-tools item to the folder menubar */
            /* Text: "Skripts and tools" */
            getMessage(text, IDSTR_MENUSKRIPTS,sizeof(text), hResource, hwnd);
            WinSendMsg(myFrameCtrlHWND,MM_INSERTITEM,MPFROMP(&mi),
                       (MPARAM)text); 
          }/* end of if(hwndMenu) */
        }
#endif
      }/* menu hwnd */

      //#if 0      
      /* Subclass container for tabbing */
      myFrameCtrlHWND=WinWindowFromID(hwnd,FID_CLIENT);//Get container hwnd
      if(myFrameCtrlHWND){
        PFNWP pfnwpContainer;
        pfnwpContainer=WinSubclassWindow(myFrameCtrlHWND, mediaFldrContainerProc);
        /* Save container proc in frame window words */
        WinSetWindowULong( hwnd, ulQWP_CONTAINERPROC, (ULONG)pfnwpContainer);
      }

      //#endif
      /* Now add the framecontrols */

      /* This is the Top-frameCtrl. */
      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd,topCtrlDialogProc,hResource,IDDLG_TOPFRAME ,this);
      /* Save the HWND in the Window words */    
      WinSetWindowULong( hwnd, ulQWL_TOPCTRLHWND, myFrameCtrlHWND);

      if(myFrameCtrlHWND){
        /* Query the size of the dialog */
        WinQueryWindowRect(myFrameCtrlHWND, &rectl);
        /* It's the top dialog so we need the height but not the width. The width
           is always the width of the folder. */
        sizel.cy=allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy;
        /* Call a method of CWProgFolder to add the dialog as a framecontrol. 
           FCTL_TOP means put it at the top of the folder.
           After calling cwUpdateFrame(hwnd) it will be visible */
        cwAddFrameCtl(hwnd, myFrameCtrlHWND, sizel, FCTL_TOP,0);
      }


      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd,leftCtrlDialogProc,hResource,IDDLG_LEFTFRAME ,this);
      /* Save the HWND in the Window words */    
      WinSetWindowULong( hwnd, ulQWL_LEFTCTRLHWND, myFrameCtrlHWND);
            
      if(myFrameCtrlHWND){
        /* Query the size of the dialog */
        WinQueryWindowRect(myFrameCtrlHWND, &rectl);
        /* It's the right dialog so we need the width but not the height. */
        sizel.cx=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx-1-7-2;//rectl.xRight-rectl.xLeft;
        /* Call a method of CWProgFolder to add the dialog as a framecontrol. 
           FCTL_RIGHT means put it at the RIGHT of the folder.
           After calling cwUpdateFrame(hwnd) it will be visible */
        cwAddFrameCtl(hwnd, myFrameCtrlHWND, sizel, FCTL_LEFT, FCTL_POSABOVE|FCTL_POSBELOW);
      }

      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd,rightCtrlDialogProc,hResource,IDDLG_FRAMERIGHT ,this);
      /* Save the HWND in the Window words */    
      WinSetWindowULong( hwnd, ulQWL_RIGHTCTRLHWND, myFrameCtrlHWND);

      if(myFrameCtrlHWND){
        CNRINFO cnrInfo;
        HWND hwndContainer; 
        PVIEWITEM pView;

        /* Query the size of the dialog */
        WinQueryWindowRect(myFrameCtrlHWND, &rectl);
        /* It's the right dialog so we need the width but not the height. */
        sizel.cx=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx-1-7-2;// rectl.xRight-rectl.xLeft;
        /* Call a method of CWProgFolder to add the dialog as a framecontrol. 
           FCTL_RIGHT means put it at the RIGHT of the folder.
           After calling cwUpdateFrame(hwnd) it will be visible */
        cwAddFrameCtl(hwnd, myFrameCtrlHWND, sizel, FCTL_RIGHT, FCTL_POSABOVE|FCTL_POSBELOW);
        
        hwndContainer=WinWindowFromID(myFrameCtrlHWND, IDCNR_PLAYLIST);
        WinSendMsg(hwndContainer,CM_QUERYCNRINFO, MPFROMP(&cnrInfo),
                   MPFROMLONG(sizeof(cnrInfo)));
      }/* if(hwndRight) */


      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd,bottomCtrlDialogProc,hResource,IDDLG_BOTTOMFRAME ,this);
      /* Save the HWND in the instance data */
      hwndBottom=myFrameCtrlHWND;
      /* Save the HWND in the Window words */    
      WinSetWindowULong( hwnd, ulQWL_BOTTOMCTRLHWND, myFrameCtrlHWND);
   
      if(hwndBottom){
        /* Query the size of the dialog */
        WinQueryWindowRect(hwndBottom,&rectl);
        //sizel.cy=20;
        sizel.cy=rectl.yTop-rectl.yBottom-2;
        //    sizel.cy=allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy;
        /* Call a method of CWProgFolder to add the dialog as a framecontrol. 
           FCTL_TOP means put it at the top of the folder.
           After calling cwUpdateFrame(hwnd) it will be visible */
        cwAddFrameCtl(hwnd, hwndBottom, sizel, FCTL_BOTTOM,0);
      }

      if(bDisabled) {
        WinEnableWindowUpdate(hwnd, TRUE);
      }

      /* Refresh frame to show the new framecontrols */
      cwUpdateFrame(hwnd);

      pfnwpFrame=WinSubclassWindow(hwnd, mediaFrameProc);
      pfnwpGenericFrame=pfnwpFrame;

      /* Start play thread handling user input */
      pTp=(THREADPARAMS*)this->wpAllocMem(sizeof(THREADPARAMS), &ulErr);
      if(pTp) {
        pTp->thisPtr=this;
        pTp->hwndFrame=hwnd;
        _beginthread(playThreadFunc,NULL,8192*16,(void*)pTp); //Fehlerbehandlung fehlt
      }
    }
    CATCH(MEDIAFLDR_OPEN)
      {
        SysWriteToTrapLog("\nTrap occured in %s, file: %s above line %d.\n",
                          __FUNCTION__, __FILE__, __LINE__);
        
      } END_CATCH;
  }
  return hwnd;
}

/**************************************************************/
/*                                                            */
/* This thread handles the about dialog.                      */
/*                                                            */
/**************************************************************/
void _Optlink aboutThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  CWMediaFolder *thisPtr;
  
  thisPtr=(CWMediaFolder*)arg;    //Pointer auf CWMediaFolder-Object
  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      /* Call the method from CWProgFolder */
      thisPtr->cwShowAboutDlg(queryResModuleHandle(), IDDLG_ABOUT);
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
}

/**************************************************************/
/*                                                            */
/* Override method:                                           */
/*                                                            */
/**************************************************************/
BOOL CWMediaFolder::wpSetupOnce(PSZ pSetupString)
{
  BOOL rcParent=FALSE;
  char buffer[CCHMAXPATH];
  ULONG bufferSize;
  int iIndex;

  rcParent=CWProgFolder::wpSetupOnce(pSetupString);

  /* Doing our own setup here if not done by the user. */
  /* Set volume */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString, MEDIAFLDR_VOLUME, buffer,&bufferSize))
    { 
      ulVolume=70;
      /* Set position of slider bar */
      winSetVolumeSlider(this);
    }

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
  
  /* Set folder details- and sortclass to CWAudioShadow so track infos are shown */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"DETAILSCLASS",buffer,&bufferSize))
    wpSetup("DETAILSCLASS=CWAudioShadow");/* Fill in defaults */
  
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"SORTCLASS",buffer,&bufferSize))
    wpSetup("SORTCLASS=CWAudioShadow");/* Fill in defaults */
  
  /* This works only on Warp 4 and above */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"DETAILSTODISPLAY",buffer,&bufferSize))
    /* This works only on Warp 4 and above */
    wpSetup("DETAILSTODISPLAY=0,1,4,12,13,14,15,16,17,18,19,20,21");/* Fill in defaults */
  
  /* This works only on Warp 4 and above */
  bufferSize=sizeof(buffer);
  if(!wpScanSetupString(pSetupString,"SORTBYATTR",buffer,&bufferSize))
    /* This works only on Warp 4 and above */
    wpSetup("SORTBYATTR=6,24,25,26,27,28,29,30,31,32,33");/* Fill in defaults */
  
  return rcParent;
}

static HWND _findOpenMediaFldrViewHWND(CWMediaFolder *thisPtr)
{
  PVIEWITEM pViewItem;
  
  /* Check if we close a settings view or a folder view. */
  pViewItem=thisPtr->wpFindViewItem(VIEW_DETAILS|VIEW_CONTENTS, NULLHANDLE);
  if(pViewItem)
    return pViewItem->handle;

  return NULLHANDLE;
}

/**************************************************************/
/*                                                            */
/* Override method:                                           */
/*                                                            */
/**************************************************************/
BOOL CWMediaFolder::wpSetup(PSZ pSetupString)
{
  char buffer[CCHMAXPATH];
  ULONG bufferSize;
  int tid;

  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_PLAY_FIRST, buffer,&bufferSize))
    {
      HWND hwndFrame=_findOpenMediaFldrViewHWND(this);
      if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrame)) {
        HWND hwndCnr=WinWindowFromID(hwndFrame, FID_CLIENT);
        if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndCnr)) {
          PRECORDCORE prc;
          prc=(PRECORDCORE)WinSendMsg(hwndCnr, CM_QUERYRECORD, 0L, MPFROM2SHORT(CMA_FIRST, CMA_ITEMORDER));
          WinSendMsg(hwndCnr, CM_SETRECORDEMPHASIS, MPFROMP(prc), MPFROM2SHORT(TRUE, CRA_SELECTED));
        }
        cwPlayAudioFile( hwndFrame, PLAY_FIRST);
      }
    }
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_PLAY, buffer,&bufferSize))
    {
      HWND hwndFrame=_findOpenMediaFldrViewHWND(this);
      if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrame))
        cwPlayAudioFile( hwndFrame, PLAY_FIRST);
    }
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_STOP, buffer,&bufferSize))
    { 
      HWND hwndFrame=_findOpenMediaFldrViewHWND(this);
      if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrame))
        cwStopAudioFile( hwndFrame);
    }
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_NEXT, buffer,&bufferSize))
    { 
      HWND hwndFrame=_findOpenMediaFldrViewHWND(this);
      if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrame))
        cwPlayAudioFile( hwndFrame, PLAY_NEXT);
    }
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_PREV, buffer,&bufferSize))
    { 
      HWND hwndFrame=_findOpenMediaFldrViewHWND(this);
      if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrame))
        cwPlayAudioFile( hwndFrame, PLAY_PREV);
    }

  /* Set volume */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_VOLUME, buffer,&bufferSize))
    { 
      ulVolume=atol(buffer);
      if(ulVolume<0)
        ulVolume=0;
      else if(ulVolume>100)
        ulVolume=100;
      /* Set position of slider bar */
      winSetVolumeSlider(this);
    }
  /* Set repeat */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, MEDIAFLDR_REPEAT, buffer,&bufferSize))
    { 
      switch(atol(buffer))
        {
        case 0:
          ulFlags&=~FLAG_REPEAT;
          winCheckRepeatButton(this);
          break;
        case 1:
          ulFlags|=FLAG_REPEAT;
          winCheckRepeatButton(this);
          break;
        }
      ulVolume=atol(buffer);
    }

  /* Set object window HWND */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, "M3UOBJECTWINDOW", buffer,&bufferSize))
    {
      hwndObject=atol(buffer);
    }

  /* Set object pointer to M3U-File */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, "M3UOBJECTHANDLE", buffer,&bufferSize))
    {
      WPObject *wpObject;

      wpObjM3UFile=NULLHANDLE;
      wpObject=(WPObject*)atol(buffer);
      if(somIsObj(wpObject))
        wpObjM3UFile=wpObject;
    }

  /* Insert tracks */
  bufferSize=sizeof(buffer);
  if(wpScanSetupString(pSetupString, "INSERTTRACKS", buffer,&bufferSize))
    {
      cwInsertAllTracks(NULLHANDLE);
    }

  /* FIXME: Use immediate parent here !!!! */
  return CWProgFolder::wpSetup(pSetupString);
}


/**************************************************************/
/*                                                            */
/* Added with V0.2.8 (07.09.2003)                             */
/*                                                            */
/* This method is only overriden to set a flag in the instance*/
/* data so the wpInsertSettingsPage() method knows when       */
/* the details page is inserted.                              */
/*                                                            */
/* This is done because the details page needs a major tab    */
/* when the icon page is removed. The necessary style bit     */
/* is set in wpInsertSettingsPage().                          */
/*                                                            */
/**************************************************************/
ULONG CWMediaFolder::wpAddFolderView3Page(HWND hwndNotebook)
{
  ULONG rc;

  /* Mark in the instance data from where we're coming */
  ulPrivFlags|=MFLDR_CURACTION_ADDFOLDERVIEW3;

  rc=CWProgFolder::wpAddFolderView3Page(hwndNotebook);

  ulPrivFlags&= ~MFLDR_CURACTION_ADDFOLDERVIEW3;
  return rc;
}

/**************************************************************/
/*                                                            */
/* Override method: Remove the Icon view settings page.       */
/*                                                            */
/* Added with V0.2.8 (07.09.2003)                             */
/**************************************************************/
ULONG CWMediaFolder::wpAddFolderView1Page(HWND hwndNotebook)
{
  /* 
     Removing this page also means removing the XWorkplace
     view page. It would be possible to call the XWP method
     directly to insert it's page but unfortunately XWP is a moving
     target without frozen interfaces/methods so every new
     XWP release is likely to break any code relying on certain
     XWP features so nothing is done to keep the XWP pages.
     */
  return SETTINGS_PAGE_REMOVED;
}

/**************************************************************/
/*                                                            */
/* Override method: Remove the tree view settings page.       */
/*                                                            */
/**************************************************************/
ULONG CWMediaFolder::wpAddFolderView2Page(HWND hwndNotebook)
{
  return SETTINGS_PAGE_REMOVED;
}

/**************************************************************/
/*                                                            */
/* Added with V0.2.8 (07.09.2003)                             */
/*                                                            */
/* Overriden to give the details view page a major tab        */
/* because the icon view page is removed.                     */
/*                                                            */
/*                                                            */
/**************************************************************/
ULONG CWMediaFolder::wpInsertSettingsPage(HWND hwndNotebook, PPAGEINFO ppageinfo)
{
  somId mySomId;

  /* V0.2.8 (07.09.2003):

     Removing the icon page isn't that easy because if doing so we lose the major tab
     and the details page will become a second background page.
     So in wpAddFolderView3Page() we set a flag in the instance data telling us here
     from where we are coming. The BKA_MAJOR style bit is switched on so
     a tab is displayed.
     This is only part of the solution because normaly there's no name for
     the details view page. So a string must be loaded and inserted into the
     ppageinfo.

   */
  if(ulPrivFlags & MFLDR_CURACTION_ADDFOLDERVIEW3) /* Check if called from wpAddFolderView3Page() */
    {
      char chr[50]="~View";

      /* Get title for the page */
      getMessage(chr, IDSTR_VIEWPAGENAME, sizeof(chr), queryResModuleHandle(), hwndNotebook);

      ppageinfo->usPageStyleFlags|=BKA_MAJOR;  /* Switch on major tab                         */
      ppageinfo->pszName=chr;
    }
  /*
    
    Climb up the list of parents to make sure we call the right
    method if the parent was replaced. Otherwise we may skip a replacement class
    and call CWProgFolder directly.

    */
  if((mySomId=somIdFromString("wpInsertSettingsPage"))!=NULLHANDLE) {
	ULONG rc;
    rc=((somTD_WPObject_wpInsertSettingsPage)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->
                            somGetMethodToken(mySomId))                    
        )(this, hwndNotebook, ppageinfo);  
    SOMFree(mySomId);
    return rc;
  }
  else 
    return CWProgFolder::wpInsertSettingsPage(hwndNotebook, ppageinfo);
}


/**************************************************************/
/* Override function: Add the 'About' and the 'Compact' menu  */
/*                     items.                                 */
/**************************************************************/
BOOL CWMediaFolder::wpMenuItemSelected(HWND hwndFrame,ULONG ulMenuId)
{
  BOOL rcParent;
  int tid;

  switch(ulMenuId)
    {
      //#ifndef ECS
    case ID_ABOUTITEM:
      tid=_beginthread(aboutThreadFunc,NULL,8192*8,this); //Fehlerbehandlung fehlt
      return TRUE;
      //#endif
    case 0x2ce: /* Detail view */
      /* Call parent */
#if 0
      if(cwUnshrinkFolder(hwndFrame))
        ulPrivFlags&=MFLDR_CHANGING_VIEW; /* Mark that we are changing views.
                                             This flag is checked in CWCDFldr::wpRefresh() to know if it's
                                             necessary to reinsert the tracks */
#endif
      rcParent=CWProgFolder::wpMenuItemSelected(hwndFrame, ulMenuId);
      cwUnshrinkFolder(hwndFrame);
      ulPrivFlags&= ~MFLDR_VIEW_COMPACT;
      return rcParent;
    case ID_MENUVIEWCOMPACT: /* Compact view */ 
      if(cwShrinkFolderToCompact(hwndFrame)) {
        /* Mark that we are in compact view. May be removed later when 'Compact'
           is an 'official' view?
           */
        ulPrivFlags|=MFLDR_VIEW_COMPACT;
      }
      return TRUE;
    default:
      break;
    }

  /* Call parent */
  rcParent=CWProgFolder::wpMenuItemSelected(hwndFrame, ulMenuId);
  return rcParent;
}

/************************************************/
/* Override function: Add the 'About' and the   */
/*                    'Compact' menu items.     */
/************************************************/
BOOL CWMediaFolder::wpModifyPopupMenu(HWND hwndMenu, HWND hwndCnr, ULONG ulPosition)
{
  HMODULE hResource=queryResModuleHandle();

  if(hResource) {
    //#ifndef ECS
    /* Insert the 'About'-Item */
     wpInsertPopupMenuItems(hwndMenu,-1,hResource,ID_MENUABOUT,WPMENUID_HELP);
     //#endif

     /* Insert the new 'Compact' view item but only if folder not already in compact view.
        By doing that the 'compact' menu item doesn't show in the Sysmenu of the folder when
        folder is shrinked.
        */
     if(!(cwQueryMFldrFlags() & MFLDR_VIEW_COMPACT))
       wpInsertPopupMenuItems(hwndMenu,1,hResource, ID_MENUVIEWCOMPACT, MENUID_VIEW);
  }
  return CWProgFolder::wpModifyPopupMenu( hwndMenu, hwndCnr, ulPosition);
}

/************************************************/
/* Override function: Remove the tree view open */
/*                    item from the menu.       */
/************************************************/
ULONG CWMediaFolder::wpFilterPopupMenu(ULONG ulFlags, HWND hwndCnr, BOOL fMultiSelect)
{
  somId mySomId;

  /* V0.2.8 (07.09.2003):
     Removed icon view item form menu. Icon view is no longer supported by
     the media folder.

     Changed to climb up the list of parents to make sure we call the right
     method if the parent was replaced. Otherwise we may skip a replacement class
     and call CWProgFolder directly.

     */
  if((mySomId=somIdFromString("wpFilterPopupMenu"))!=NULLHANDLE) {
	ULONG rc;
    rc=((somTD_WPObject_wpFilterPopupMenu)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->
                            somGetMethodToken(mySomId))                    
        )(this, ulFlags, hwndCnr, fMultiSelect);  
    SOMFree(mySomId);
    return rc & ~( CTXT_TREE | CTXT_ICON);
  }
  else 
    return CWProgFolder::wpFilterPopupMenu( ulFlags, hwndCnr, fMultiSelect) & ~( CTXT_TREE | CTXT_ICON);
}

#if 0
BOOL CWMediaFolder::wpRefresh(ULONG ulView, PVOID pReserved)
{
  somId mySomId;

    if((mySomId=somIdFromString("wpAddSettingsPages"))!=NULLHANDLE) {
      rc=((somTD_WPObject_wpAddSettingsPages)
          somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                              1,
                              __ClassObject->
                              somGetMethodToken(mySomId))                    
          )(this,hwndNotebook);  
      SOMFree(mySomId);
    }
    else 
      rc=CWProgFolder::wpRefresh(ulView, pReserved);

}
#endif

BOOL CWMediaFolder::wpAddSettingsPages(HWND hwndNotebook)
{
	ULONG rc;
    somId mySomId;
    
    if((mySomId=somIdFromString("wpAddSettingsPages"))!=NULLHANDLE) {
      rc=((somTD_WPObject_wpAddSettingsPages)
          somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                              1,
                              __ClassObject->
                              somGetMethodToken(mySomId))                    
          )(this,hwndNotebook);  
      SOMFree(mySomId);
    }
    else 
      rc=CWProgFolder::wpAddSettingsPages(hwndNotebook);
    
    if(!queryResModuleHandle()) {
      errorResource();
      return rc;
    }
    return(rc/* && cwAddToolbarOptionPage(hwndNotebook)*/);
}


MRESULT CWMediaFolder::wpDragOver(HWND hwndCnr,PDRAGINFO pDragInfo)
{
  WPObject *wpObject;
  WPFileSystem *wpFSObject;
  ULONG ulNum;
  ULONG a;
  MRESULT mr;
  SOMClass *mmAudioClass=somhlpQueryMMAudioClass();
  SOMClass *mmMIDIClass=somhlpGetSomClass("MMMIDI");
  SOMClass *cwAudioClass=somhlpGetSomClass("CWAudioShadow");


  mr=MRFROM2SHORT( DOR_DROP, DO_LINK);
  ulNum=DrgQueryDragitemCount(pDragInfo);/* Num items */

  if(pDragInfo->usOperation!=DO_DEFAULT && pDragInfo->usOperation!=DO_LINK)
    //    return MRFROM2SHORT( DOR_NEVERDROP, DO_MOVE);/* It's not an audio object */
    {
      /* Only CWAudioShadows may be copied or moved */
      for(a=0;a<ulNum && SHORT1FROMMR(mr)!=DOR_NEVERDROP; a++)
        {
          /* Check every object dragged */
          wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, a)->ulItemID);
          if(!wpObject->somIsA(cwAudioClass))
            {
              return MRFROM2SHORT( DOR_NEVERDROP, DO_MOVE);/* It's not an audio object */
            }
        }
      return MRFROM2SHORT( DOR_DROP, DO_MOVE);/*  */
      return MRFROM2SHORT( DOR_DROP, pDragInfo->usOperation);/*  */
    }


  for(a=0;a<ulNum && SHORT1FROMMR(mr)!=DOR_NEVERDROP; a++)
    {
      /* Check every object dragged */
      wpObject=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, a)->ulItemID);
      /* Get the filesystem object in case it's a shadow */
      wpFSObject=(WPFileSystem*)cwGetFileSystemObject(wpObject);
      if(!somIsObj(wpFSObject))
        return MRFROM2SHORT( DOR_NEVERDROP, DO_MOVE);/* It's not an object */
      if(!wpFSObject->somIsA(cwAudioClass) && !wpFSObject->somIsA(mmAudioClass) && !wpFSObject->somIsA(mmMIDIClass))
        return MRFROM2SHORT( DOR_NEVERDROP, DO_MOVE);/* It's not an audio object */
    }
  /* The following is necessary because for some reason hwndCnr and pDragInfo->hwndSource
     are equal when the dragged object resides in the same folder as the target and the target
     isn't opened yet. Without this check the dragged object would be moved not linked. */
  if(this->wpQueryFolder()==wpObject->wpQueryFolder())
      return MRFROM2SHORT(DOR_DROP,DO_LINK);
  /* Do not create shadows if the source is the current container */
  if(hwndCnr==pDragInfo->hwndSource)
    return MRFROM2SHORT( DOR_DROP, DO_MOVE);

  return mr;
}

void _Optlink dropThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  PDROPTHREADPARAMS threadParams;
  CWMediaFolder *thisPtr;
  MMAudio* cwAudio;
  HWND hwndCnr;

  threadParams=(PDROPTHREADPARAMS) arg;
  if(!threadParams)
    return;
  thisPtr=(CWMediaFolder*)threadParams->thisPtr;    //Pointer auf CWMediaFolder-Object
  
  if(!somIsObj(thisPtr))
    return;
  
  if(thisPtr->mfData)
    thisPtr->mfData->bProcessing=TRUE;
  
  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      WPObject *wpObject;
      somId mySomId;
      M_WPObject *mAudioObject;
      SOMClass*  scCWShadowClass;
      ULONG ulCount;
      ULONG ulNumObjects;
      /* Needed for specifying the drop point */
      POINTL ptl;
      PMINIRECORDCORE pmrc; 
      CNRINFO cnrInfo;
      int ID;
      HWND hwndSource;

      hwndCnr=threadParams->hwndCnr;

      /* The objects are already checked by the wpDragOver() method. */

      TRY_LOUD(MEDIAFLDR_DROP_THREAD) {
        hwndSource=threadParams->hwndSource;
        ulNumObjects=threadParams->ulNumObjects;
        
        if((mySomId=somIdFromString(SHADOW_CLASS_NAME))!=NULLHANDLE) {
          WPFileSystem *wpFSObject;
          /* Get the CWAudioShadow class object */
          scCWShadowClass=(SOMClass*)SOMClassMgrObject->somClassFromId(mySomId);
          SOMFree(mySomId);
          /* Check shadow class */
          if(somIsObj(scCWShadowClass)) {
            if(hwndSource!=hwndCnr){/* Dropped on white space of an open folder */
              QUERYRECFROMRECT qRecRcl;
              ptl.x=threadParams->ptl.x;
              ptl.y=threadParams->ptl.y;
              
              /* Map to CNR */
              WinMapWindowPoints(HWND_DESKTOP, hwndCnr,&ptl, 1);
              ptl.y+=10;          
              /* Window below the mouse ptr. We need it to check if we are over the column title area. */
              ID=WinQueryWindowUShort(WinWindowFromPoint(hwndCnr,&ptl, TRUE),QWS_ID);
              ptl.y-=10;
              
              /* Find the record near the drop point */
              qRecRcl.cb=sizeof(qRecRcl);
              qRecRcl.rect.yBottom=ptl.y-10;
              qRecRcl.rect.yTop=ptl.y;
              qRecRcl.rect.xLeft=ptl.x;
              qRecRcl.rect.xRight=ptl.x+10;
              qRecRcl.fsSearch=CMA_PARTIAL|CMA_ITEMORDER;
              pmrc=(PMINIRECORDCORE)WinSendMsg(hwndCnr, CM_QUERYRECORDFROMRECT,MPFROMLONG(CMA_FIRST),
                                               MPFROMP(&qRecRcl));
              WinSendMsg(hwndCnr, CM_QUERYCNRINFO, MPFROMP(&cnrInfo), MPFROMSHORT(sizeof(cnrInfo)));        
            }/*if(hwndSource!=hwndCnr)*/
            
            /* For all dropped objects */ 
            for(ulCount=0; ulCount<ulNumObjects; ulCount++) {
              if(ulCount%10)/* Redraw window every 10 objects */
                WinEnableWindowUpdate(hwndCnr, FALSE);
              else
                WinEnableWindowUpdate(hwndCnr, TRUE);
              
              wpObject=(WPObject*) threadParams->wpObject[ulCount];
              /* Get the filesystem object in case it's a shadow */
              wpFSObject=(WPFileSystem*)thisPtr->cwGetFileSystemObject(wpObject);
              if(somIsObj(wpFSObject)){
                if(hwndSource==hwndCnr)/* The folder is closed or dropped on the folder icon. */
                  wpFSObject->wpCreateShadowObjectExt(thisPtr, FALSE, "", (M_WPObject*)scCWShadowClass);
                else {
                  /* Dropped on white space of the folder */
                  WPObject *wpo;
                  if(cnrInfo.flWindowAttr& CV_DETAIL)
                    {
                      /* Detailsview */
                      RECORDINSERT recInsert;
                      
                      recInsert.cb=sizeof(recInsert);
                      recInsert.pRecordParent=NULL;
                      recInsert.fInvalidateRecord=TRUE;
                      recInsert.zOrder=CMA_TOP;
                      recInsert.cRecordsInsert=1;
                      
                      //     WinEnableWindowUpdate(hwndCnr, FALSE);
                      /* Create the shadow */
                      //   wpo=
                      //wpFSObject->wpCreateShadowObject(thisPtr, FALSE);
                      wpo=wpFSObject->wpCreateShadowObjectExt(thisPtr, FALSE, "",
                                                              (M_WPObject*) scCWShadowClass);
                      /* Move the record to the right position in the details view */
                      if(somIsObj(wpo)) {
                        wpo->wpCnrRemoveObject(hwndCnr);
                        //  WinEnableWindowUpdate(hwndCnr, TRUE);
                        if(pmrc && ((int)pmrc)!=-1) {
                          recInsert.pRecordOrder=(PRECORDCORE)pmrc;
                          /* Insert at the right place. For some reason wpSetNextIconPos() does not work!? */
                          wpo->wpCnrInsertObject(hwndCnr, &ptl, NULL, &recInsert);
                        }
                        else {
                          /* No record found check if we are at the top or the bottom */
                          if(ID!=0x7ff0 && ID!=0x7ff1)
                            recInsert.pRecordOrder=(PRECORDCORE)CMA_END;
                          else
                            recInsert.pRecordOrder=(PRECORDCORE)CMA_FIRST;
                          /* Insert at the right place. For some reason wpSetNextIconPos() does not work!? */
                          wpo->wpCnrInsertObject(hwndCnr, &ptl, NULL, &recInsert);
                        }
                      }/* if(somIsObj(wpo)) */
                      //          WinEnableWindowUpdate(hwndCnr, TRUE);
                    }/* if(cnrInfo.flWindowAttr& CV_DETAIL) */
                  else
                    /* Icon view */
                    wpo=wpFSObject->wpCreateShadowObjectExt(thisPtr, FALSE, "", (M_WPObject*) scCWShadowClass);
                  if(somIsObj(wpo))
                    wpo->wpCnrRefreshDetails();
                }/* else(hwndSource==hwndCnr) */
              }/* if(somIsObj(wpFSObject)) */
            }/* for(ulCount=0;ulCount<ulNumObjects ; ulCount++) */
            WinEnableWindowUpdate(hwndCnr, TRUE);
          }/* if(somIsObj(scCWShadowClass)) */
        }/* if((mySomId=somIdFromString(SHADOW_CLASS_NAME))!=NULLHANDLE)*/
      }/* Try */
      CATCH(MEDIAFLDR_DROP_THREAD)
        {
          SysWriteToTrapLog("\nTrap occured in %s, file: %s, line %d.\n",
                            __FUNCTION__, __FILE__, __LINE__);
        } END_CATCH;
        
        WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  if(thisPtr->mfData)
    thisPtr->mfData->bProcessing=FALSE;
  
  thisPtr->wpFreeMem((PBYTE)threadParams);
}



MRESULT CWMediaFolder::wpDrop(HWND hwndCnr,PDRAGINFO pDragInfo,PDRAGITEM pDragItem)
{
  WPObject *wpObject;
  somId mySomId;
  M_WPObject *mAudioObject;
  SOMClass*  scCWShadowClass;
  ULONG ulCount;
  ULONG ulNumObjects=DrgQueryDragitemCount(pDragInfo);

  /* Needed for specifying the drop point */
  POINTL ptl;
  PMINIRECORDCORE pmrc; 
  CNRINFO cnrInfo;
  int ID;
 
  /* The objects are already checked by the wpDragOver() method. */
  TRY_LOUD(MEDIAFLDR_DROP) {
    PDROPTHREADPARAMS pThreadParams;
    ULONG ulErr;
    WPObject* wpObj;
    
    /* DROPTHREADPARAMS already contains one WPObject var so only memory for ulNumObjects-1 must be added to the end */
    pThreadParams=(PDROPTHREADPARAMS)wpAllocMem(sizeof(DROPTHREADPARAMS)+(ulNumObjects-1)*sizeof(WPObject*), &ulErr);
    pThreadParams->hwndCnr=hwndCnr;
    pThreadParams->hwndSource=pDragInfo->hwndSource;
    pThreadParams->ulNumObjects=DrgQueryDragitemCount(pDragInfo);
    pThreadParams->thisPtr=this;
    pThreadParams->ptl.x=pDragInfo->xDrop;
    pThreadParams->ptl.y=pDragInfo->yDrop;
              
    /* For all dropped objects */ 
    for(ulCount=0;ulCount<ulNumObjects ; ulCount++) {
      pThreadParams->wpObject[ulCount]=(WPObject*)OBJECT_FROM_PREC(DrgQueryDragitemPtr( pDragInfo, ulCount)->ulItemID);
    }
    _beginthread(dropThreadFunc,NULL,8192*16,(void*)pThreadParams); //Fehlerbehandlung fehlt
  }
  CATCH(MEDIAFLDR_DROP)
    {
      SysWriteToTrapLog("\nTrap occured in %s, file: %s, around line %d.\n",
                        __FUNCTION__, __FILE__, __LINE__);
    } END_CATCH;
    
  DrgFreeDraginfo(pDragInfo);
  return (MRESULT)RC_DROP_DROPCOMPLETE;
}


BOOL CWMediaFolder::wpSaveState()
{

  wpSaveLong("CWMediaFolder", KEY_VOLUME, ulVolume);
  //  wpSaveData("CWMediaFolder", KEY_FLYBACKGROUND, (PBYTE)&rgbTBFlyBackground, sizeof(RGB));
  //  wpSaveData("CWMediaFolder", KEY_FLYFOREGROUND, (PBYTE)&rgbTBFlyForeground,sizeof(RGB));
  //  wpSaveString("CWMediaFolder", KEY_FLYFONT, (PBYTE)&chrTBFlyFontName);
  //  wpSaveLong("CWMediaFolder", KEY_FLYENABLED,bTBFlyOverEnabled);
  //  wpSaveLong("CWMediaFolder", KEY_FLYDELAY,iTBFlyOverDelay);
  wpSaveLong("CWMediaFolder", KEY_FLAGS, ulFlags);
  wpSaveLong("CWMediaFolder", KEY_DISPLAYTYPE, (LONG)usWhichDisplay);
  if(mfData)
    wpSaveString("CWMediaFolder", KEY_PLAYTIMEFONT, (PBYTE)&mfData->chrPlayTimeFont);
  return CWProgFolder::wpSaveState();
}

BOOL CWMediaFolder::wpRestoreState(ULONG ulReserved)
{
  ULONG dataSize;
  ULONG ulActionButtons;


  if(!wpRestoreLong("CWMediaFolder", KEY_VOLUME, &ulVolume))
    ulVolume=75;
#if 0
  if(!wpRestoreLong("CWMediaFolder", KEY_FLYENABLED,&bTBFlyOverEnabled))
    bTBFlyOverEnabled=1;
  if(!wpRestoreLong("CWMediaFolder", KEY_FLYDELAY,(PULONG)&iTBFlyOverDelay))
    iTBFlyOverDelay=250;
#endif
  if(!wpRestoreLong("CWMediaFolder", KEY_FLAGS,(PULONG)&ulFlags))
    ulFlags=0;
  if(!wpRestoreLong("CWMediaFolder", KEY_DISPLAYTYPE,(PULONG)&dataSize))
    usWhichDisplay=0;
  else
    usWhichDisplay=(USHORT)dataSize;

#if 0
  dataSize=sizeof(RGB);
  if(!wpRestoreData("CWMediaFolder", KEY_FLYBACKGROUND,(PBYTE)&rgbTBFlyBackground,&dataSize)) {
    rgbTBFlyBackground.bBlue=180;
    rgbTBFlyBackground.bGreen=255;
    rgbTBFlyBackground.bRed=255;
  }

  dataSize=sizeof(RGB);
  if(!wpRestoreData("CWMediaFolder",KEY_FLYFOREGROUND,(PBYTE)&rgbTBFlyForeground,&dataSize)) {
    rgbTBFlyForeground.bRed=0;
    rgbTBFlyForeground.bGreen=0;
    rgbTBFlyForeground.bBlue=0;
  }

  memset(chrTBFlyFontName,0,sizeof(chrTBFlyFontName));	
  dataSize=sizeof(chrTBFlyFontName);
  if(!wpRestoreString("CWMediaFolder",KEY_FLYFONT,(PBYTE)&chrTBFlyFontName,&dataSize))
    memcpy(&chrTBFlyFontName,"8.Helv",sizeof("8.Helv"));
#endif

  if(mfData)
    {
      dataSize=sizeof(mfData->chrPlayTimeFont);
      wpRestoreString("CWMediaFolder", KEY_PLAYTIMEFONT,(PBYTE)&mfData->chrPlayTimeFont, &dataSize);
    }

  return CWProgFolder::wpRestoreState(ulReserved);
}


BOOL CWMediaFolder::cwClose(HWND hwndFrame)
{
  char text[200];
  char title[50];
  HWND hwndOwner=HWND_DESKTOP;

  /* Check if we currently play a song */
  if(iWave) {
    PVIEWITEM pViewItem;
    int a=0;

#if 0
    /* Check if we close a settings view or a folder view. */
    for(pViewItem=wpFindViewItem(VIEW_ANY, NULLHANDLE); pViewItem; pViewItem=wpFindViewItem(VIEW_ANY, pViewItem))
      a+=1;
    if(a!=1)
      return TRUE;
#endif

    if(hwndFrame!=hwndMFldrFrame)
      return TRUE;

    if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrame))
      hwndOwner=hwndFrame;

    if(messageBox( text, IDSTR_CLOSETEXT, sizeof(text),
                   title, IDSTR_CLOSETITLE, sizeof(title),
                   queryResModuleHandle(), hwndOwner, MB_YESNO|MB_MOVEABLE|MB_ICONQUESTION)==MBID_NO)
      return FALSE;
  }
  cwStopAudioFile(hwndFrame);  
  /* Stop the playing thread */
  WinPostMsg(mfData->hwndPlayObject, WM_QUIT, 0, 0);

  /* We were opened by an M3U file. Notify it that we're closing */
  if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndObject)) {
    WinPostMsg(hwndObject, WM_APPTERMINATENOTIFY, 0, 0);
  }

  ulPrivFlags&= ~MFLDR_VIEW_COMPACT;
  hwndMFldrFrame=NULL;
  wpSaveDeferred();
  return TRUE;
}

void CWMediaFolder::wpInitData()
{
  ULONG ulErr;
  CWProgFolder::wpInitData();
  /* Get storage for private data. */
  if((mfData=(PMEDIAFLDRDATA)wpAllocMem(sizeof(MEDIAFLDRDATA), &ulErr))!=NULLHANDLE)
    memset((void*)mfData, 0, sizeof(MEDIAFLDRDATA));

  ulPrivFlags=0;

}

void  CWMediaFolder::wpUnInitData()
{
  wpFreeMem((PBYTE)mfData);
  CWProgFolder::wpUnInitData();
}

