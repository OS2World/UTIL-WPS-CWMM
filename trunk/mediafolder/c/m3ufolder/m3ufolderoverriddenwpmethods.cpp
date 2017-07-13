/*
 * This file is (C) Chris Wohlgemuth 2002-2003
 * 
 * It's part of the Media-Folder distribution
 */

#define INCL_DOSERRORS

#define INCL_REXXSAA

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "m3ufolder.hh"
#include "except.h"
#include <rexxsaa.h>                   /* needed for RexxStart()     */

#include "mediafolderres.h"
#include "cwcdtrack.hh"

#include "sys_funcs.h"
#include "menu_funcs.h"

extern BOOL g_fRegisteredWindowClass;
extern ULONG g_ulFrameDataOffset;

/* The following offsets are calclated in wpOpen() of media folder */
extern ULONG ulQWP_CONTAINERPROC;
extern ULONG ulQWP_THISPTR;
extern ULONG ulQWL_TOPCTRLHWND;
extern ULONG ulQWL_BOTTOMCTRLHWND;
extern ULONG ulQWL_RIGHTCTRLHWND;
extern ULONG ulQWL_LEFTCTRLHWND;

#if 0 
/* For calling the method in the MMAudio class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
typedef ULONG   SOMLINK somTP_MMCDTrack_cwmmQueryTrackInfo(MMCDTrack *somSelf,
		char** chrString,
		ULONG ulSize,
		int iWhich);
typedef somTP_MMCDTrack_cwmmQueryTrackInfo *somTD_MMCDTrack_cwmmQueryTrackInfo;
extern somTD_MMCDTrack_cwmmQueryTrackInfo methodPtrCDTrack;
#endif

extern LOADEDBITMAP allBMPs[];
extern PFNWP pfnwpGenericFrame;
extern char chrInstallDir[];

BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
HMODULE queryResModuleHandle(void);
void errorResource();

MRESULT EXPENTRY topCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY rightCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY leftCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY bottomCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY mediaFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY mediaFldrContainerProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;


void _Optlink rexxThreadFunc (void *arg);

SOMClass* somhlpGetSomClass(char* chrClassName);

//void HlpWriteToTrapLog(const char* chrFormat, ...);

#if 0
void _Optlink rexxThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  CWM3UFolder *thisPtr;
  HWND hwnd;
#if 0
  char chrFileName[CCHMAXPATH];
  char chrFolderName[CCHMAXPATH];
  char chrRexxScript[CCHMAXPATH];
#endif
  char *chrFileName;
  char *chrFolderName;
  char *chrRexxScript;
  ULONG ulSize;

  thisPtr=(CWM3UFolder*)arg;   
  if(!somIsObj(thisPtr))
    return;

  if(!somIsObj(thisPtr->wpObjM3UFile))
    return;

  chrFileName=thisPtr->wpAllocMem( CCHMAXPATH*3+10 , &ulSize);
  if(!chrFileName)
    return;
  
  chrFolderName=chrFileName+CCHMAXPATH+1;
  chrRexxScript=chrFolderName+CCHMAXPATH+1;

  ulSize=CCHMAXPATH;
  if(!((WPFileSystem*)thisPtr->wpObjM3UFile)->wpQueryRealName(chrFileName, &ulSize, TRUE))
    return;
  
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
      
      
      sprintf(chrRexxScript,"%s%s",  chrInstallDir,RX_INSERT_PLAYLISTTRACKS);
      //HlpWriteToTrapLog("chrRexxScript: %s\n", chrRexxScript);
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
                     (PSZ)        chrRexxScript,/* name of REXX file     */
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
  thisPtr->fContentsChanged=FALSE;
  thisPtr->wpFreeMem((PBYTE)chrFileName); 
}

BOOL CWM3UFolder::wpSetup(PSZ pSetupString)
{
  /**************************************************
   *                                                *
   *                                                *
   **************************************************/
  char buffer[25];
  ULONG bufferSize;

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


  /* FIXME: Use immediate parent here !!!! */
  return CWMediaFolder::wpSetup(pSetupString);
}
#endif

/*****************************************************/
/*                                                   */
/* New container procedure so tabbing to the play    */
/* control dialog works.                             */
/*                                                   */
/*****************************************************/
MRESULT EXPENTRY m3uFldrContainerProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder* thisPtr;
  HWND hwndDialog;
  PFNWP pfnwpContainer;

  switch(msg)
    {
    case WM_CONTROL:
      if(SHORT2FROMMP(mp1)==CN_DROP) {
        /* Get frame handle */
        if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE)
          {
            CWM3UFolder *thisPtr;
            thisPtr=(CWM3UFolder*) WinQueryWindowULong(hwndDialog,ulQWP_THISPTR);
            if(somIsObj(thisPtr))
              thisPtr->fContentsChanged=TRUE;
          }
      }
      break;
    default:
      break;
    }

  /* Any other message is handled by the folder container procedure */
  if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE) {
    pfnwpContainer=(PFNWP)WinQueryWindowULong( hwndDialog, ulQWP_CONTAINERPROC);
    if(pfnwpContainer)
      return pfnwpContainer(hwnd, msg, mp1, mp2);
  }
#if 0
  if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE)
    {
      if((hwndDialog=WinWindowFromID(hwndDialog,IDDLG_TOPFRAME))!=NULLHANDLE)
        {
          thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
          if(somIsObj(thisPtr))
            if(thisPtr->pfnwpContainer)
              return thisPtr->pfnwpContainer(hwnd, msg, mp1, mp2);
        }
    }
#endif

  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

BOOL CWM3UFolder::cwInsertAllTracks(char* chrM3UFile)
{
  INSERTTRACKPARMS * threadParms;
  ULONG ulError;

  if((threadParms=(INSERTTRACKPARMS*)wpAllocMem(sizeof(INSERTTRACKPARMS), &ulError))==NULLHANDLE)
    return FALSE;
  threadParms->thisPtr=this;
  sprintf(threadParms->chrRexxScript,"%s%s",  chrInstallDir, RX_INSERT_PLAYLISTTRACKS);

  _beginthread(rexxThreadFunc,NULL,8192*16,(void*)threadParms); //Fehlerbehandlung fehlt
  return TRUE;
}



/****************************************************************************/
/*                                                                          */
/* This procedure handles the frame extension of the folder at the top      */
/*                                                                          */
/* The playcontrol dialog is a child of this 'background' dialog.           */
/*                                                                          */
/****************************************************************************/
MRESULT EXPENTRY m3uTopCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
    
  switch(msg)
    {
      /*****************************************************/
      /* Init the dialog                                   */
      /*****************************************************/
    case WM_INITDLG :
      {
        ULONG ulStyle;
        SWP swp;
        
        WinSetWindowULong(hwnd,QWL_USER,(ULONG)PVOIDFROMMP(mp2));//Save object ptr.
        WinQueryWindowPos(WinQueryWindow(hwnd,QW_PARENT),&swp);
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        return (MRESULT)FALSE;
      }
    case WM_CHAR:
      // Set focus to container so the key will be processed as usual
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT));
      return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT),msg,mp1,mp2);
      
      /*****************************************************/
      /* Paint the background.                             */
      /*****************************************************/
    case WM_PAINT:
      {
        HPS hps;
        RECTL rcl, rclSave, rclSource;
        SWP swp;

        if(allBMPs[CTRLIDX_BOTTOM].hbm) {
          POINTL ptl;
          LONG lTemp;

          hps=WinBeginPaint(hwnd, NULL, &rcl);
          rclSource.xLeft=0;
          rclSource.yBottom=0;
          rclSource.yTop=allBMPs[CTRLIDX_BOTTOM].bmpInfoHdr.cy;
          rclSource.xRight=allBMPs[CTRLIDX_BOTTOM].bmpInfoHdr.cx;
          lTemp=rcl.xLeft/rclSource.xRight;
          ptl.x=lTemp*rclSource.xRight;
          lTemp=rcl.yBottom/rclSource.yTop;
          lTemp*=rclSource.yTop;   
          while(ptl.x<rcl.xRight) {
            ptl.y=lTemp;
            while(ptl.y<rcl.yTop) {/* y direction */
              WinDrawBitmap(hps, allBMPs[CTRLIDX_BOTTOM].hbm,
                            &rclSource, 
                            (PPOINTL)&ptl,
                            0, 0,
                            DBM_IMAGEATTRS);
              ptl.y+=allBMPs[CTRLIDX_BOTTOM].bmpInfoHdr.cy;
            };
            ptl.x+=allBMPs[CTRLIDX_BOTTOM].bmpInfoHdr.cx; 
          };
          WinEndPaint(hps);
          return (MRESULT)0;
        }
#if 0
        hps=WinBeginPaint(hwnd, NULLHANDLE,NULL);
        /* Get Window size */
        WinQueryWindowRect(hwnd, &rcl);
        rclSave=rcl;
        /* Get size and pos of control area */
        WinQueryWindowPos(WinWindowFromID(hwnd, IDDLG_TOPMIDDLE),&swp);
        /* Left area */
        rcl.xRight=swp.x;
        rclSource.xLeft=0;
        rclSource.yBottom=0;
        rclSource.yTop=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cy;
        rclSource.xRight=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cx;
        WinDrawBitmap(hps, allBMPs[CTRLIDX_TOPLEFT].hbm,
                      &rclSource, 
                      (PPOINTL)&rclSave,
                      0, 0,
                      DBM_IMAGEATTRS | DBM_STRETCH);
        //SysWriteToTrapLog("hbm: %x , yTop: %d xRight; %d\n", allBMPs[CTRLIDX_TOPLEFT].hbm, rclSource.yTop,rclSource.xRight);
        //  WinFillRect(hps, &rclSave, CLR_RED);
        /* Right area */
        rclSave.xLeft=swp.x+swp.cx;
        rclSource.xLeft=0;
        rclSource.yBottom=0;
        rclSource.yTop=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cy;
        rclSource.xRight=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx;
        WinDrawBitmap(hps, allBMPs[CTRLIDX_TOPRIGHT].hbm,
                      &rclSource, 
                      (PPOINTL)&rclSave,
                      0, 0,
                      DBM_IMAGEATTRS | DBM_STRETCH);
                WinFillRect(hps, &rclSave, CLR_BLUE);
        WinEndPaint(hps);
        return (MRESULT) 0;
#endif
        break;
      }
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/************************************************************
 *                                                          *
 * This frame proc handles the about menuitem of the        *
 * Warp 4 menu bar and removes some menu items.             *
 *                                                          *
************************************************************/
MRESULT EXPENTRY m3uFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWM3UFolder* thisPtr;
  HWND hwndDialog;
  MENUITEM mi;
  char text[100];

  switch (msg)
    {
    case WM_INITMENU:
      if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
        {
          thisPtr=(CWM3UFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
          if(somIsObj(thisPtr)) {

            HMODULE hResource=queryResModuleHandle();
            switch(thisPtr->usLastSelMenuItem)
              {
#ifndef ECS
              case 0x2d3:/* Help menu */
                {
                  if(hResource) {
                    /* insert separator */
                    menuInsertMenuSeparator(HWNDFROMMP(mp2), NULL, MIT_END);
                    /* Insert 'About' item */
                    getMessage(text, IDSTR_MENUABOUT ,sizeof(text), hResource, hwnd);
                    menuInsertMenuItem(HWNDFROMMP(mp2), NULL, MIT_END, ID_ABOUTITEM, text);
                    break;
                  }
                  break;
                }
#endif
              case 0x2d1: /* View menu */
                /* Remove change to Tree from menubar menu */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(717,TRUE),0);
                /* Icon view */
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x2cc,TRUE),0);
                /* Arrange */
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x135,TRUE),0);
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x136,TRUE),0);
                //     WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x,TRUE),0);

                break;
              case 0x2d0: /* Edit menu */
                /* Move */
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x6b,TRUE),0);
                /* Insert */
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x2cb,TRUE),0);
                /* Create shadow */
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x13c,TRUE),0);
                /* Create */
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x65,TRUE),0);
                break;
              default:
                break;
              }
          }
        }
      break;
      /* Prevent title changing by XWP */
    case WM_SETWINDOWPARAMS:
      {
        PWNDPARAMS pwp=(PWNDPARAMS)PVOIDFROMMP(mp1);
        if(!pwp)
          break;
        if(pwp->fsStatus!=WPM_TEXT)
          break;

        return MRFALSE;
      }
    case WM_COMMAND:
      if(SHORT1FROMMP(mp2)==CMDSRC_MENU) {
        switch(SHORT1FROMMP(mp1))
          {
          case ID_MENUITEMEXIT:
            WinPostMsg(hwnd, WM_CLOSE, 0, 0);
            return MRFALSE; 
          case ID_MENUITEMSAVE:
            {
              if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
                {
                  thisPtr=(CWM3UFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
                  if(somIsObj(thisPtr)) {
                    thisPtr->cwSavePlaylist(NULLHANDLE, FALSE);
                  }
                }
              return MRFALSE; 
            }
          default:
            if(SHORT1FROMMP(mp1)>=FIRSTUSERITEM) { 
              BOOL bHandled=FALSE;
              
              TRY_LOUD(AF_MENU) {
#if 0
                if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
                  {
                    thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
                    if(somIsObj(thisPtr)) {
                      
                      CWMenuFolder * wpFolder;	
                      HOBJECT hObject;
                      somId id;
                      somTD_CWMenuFolder_mfCheckSkriptMenuItems methodPtr;
                      M_WPObject *m_wpObject;
                      
                      if(!bHandled)
                        bHandled=thisPtr->cwCheckUserMenuItems(USERMENUFOLDER_MEDIAFOLDER , SHORT1FROMMP(mp1));
                    }/* if(somIsObj(thisPtr)) */
                  }/* Dialog window */
#endif
              }
              CATCH(AF_MENU)
                {
                } END_CATCH;
                if(bHandled)
                  return (MRESULT) 0;
            }
            break;
          }/* switch() */
      }
      break;
    case WM_MENUSELECT:
      if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
        {
          thisPtr=(CWM3UFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
          if(somIsObj(thisPtr)) {
            thisPtr->usLastSelMenuItem=SHORT1FROMMP(mp1);
          }
        }
      break;
    case WM_CONTROL:
      if(SHORT2FROMMP(mp1)==CN_DROP) {
        if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
          {
            thisPtr=(CWM3UFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
            if(somIsObj(thisPtr)) {
              thisPtr->fContentsChanged=TRUE;
            }
          }
      }
      break;
    default:
      break;
    }
  /* Any other message is handled by the folder frame procedure */
  if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
    {
      thisPtr=(CWM3UFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
      if(somIsObj(thisPtr)) {
        if(thisPtr->pfnwpFrame)
          return thisPtr->pfnwpFrame(hwnd, msg, mp1, mp2);
      }
    }
  return  pfnwpGenericFrame(hwnd, msg, mp1, mp2);
}


HWND CWM3UFolder::wpOpen(HWND hwndCnr,ULONG ulView,ULONG ulParam)
{
  HWND hwnd;
  HMODULE hResource=queryResModuleHandle();
  SIZEL sizel={0};
  HWND myFrameCtrlHWND;
  RECTL rectl;
  MENUITEM mi={0};
  HWND hwndMenu;
  int iItemCount=0;
  char text[200];
  SWP swp;

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

  /* Skip media folder. Because of this skip we had to register the window words here (see above).
     It's not asured a Mediafolder was already opended. Normaly the window words are added in
     wpOpen() of the Mediafolder. 
     FIXME: This only works if CWProgFolder->CWMediaFolder->CWM3UFolder is the class hierarchy */
  hwnd=CWProgFolder::wpOpen(hwndCnr, ulView, ulParam);

  /* Only subclass folder frames, not settings notebooks */
  if(ulView!=OPEN_SETTINGS){    
    if(!hResource) {
      errorResource();
      return hwnd;
    }

    TRY_LOUD(M3UFLDR_OPEN) {
      THREADPARAMS*  pTp;
      ULONG ulErr;

      /* Remove the title added by XWP */
      WinSetWindowText(hwnd, wpObjM3UFile->wpQueryTitle());
      if(!getMessage(text, IDSTR_PLAYLISTVIEWTITLE, sizeof(text), queryResModuleHandle(), HWND_DESKTOP))
        sprintf(text, "Playlist Editor");
      /* Set view title. By doing this the frame title isn't 'Details' anymore */
      wpRegisterView(hwnd, text);

      if(!(ulFlags & FLAG_ALREADYSIZED)) {
        /* Make sure on first open the container is visible */
        SWP swp;
        WinQueryWindowPos(hwnd, &swp);
        WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y-100, swp.cx, swp.cy+100, SWP_SIZE|SWP_MOVE);
        ulFlags|=FLAG_ALREADYSIZED;
      }

      /* Build new menu entries. */
      myFrameCtrlHWND=WinWindowFromID(hwnd,FID_MENU);//Get menu hwnd
      if(WinIsWindow(WinQueryAnchorBlock(hwnd), myFrameCtrlHWND)) {
        /* Menu bar found */

        /* Remove folder item from menubar menu */ 
        WinSendMsg( myFrameCtrlHWND, MM_REMOVEITEM, MPFROM2SHORT(0x2cf,TRUE),0);

        /* Insert file menu for the folder */
        hwndMenu=WinLoadMenu(myFrameCtrlHWND, hResource, ID_MENUFILE);
        /* Fill the MENUITEM structure */
        mi.iPosition=0;
        mi.afStyle=MIS_TEXT|MIS_SUBMENU;
        mi.id=ID_MENUFILE;
        mi.afAttribute=NULL;                
        mi.hwndSubMenu=hwndMenu;
        mi.hItem=NULL;
        /* Add the item to the folder menubar */
        /* Text: "~File" */
        getMessage(text, IDSTR_MENUFILE,sizeof(text), hResource, hwnd);
        WinSendMsg(myFrameCtrlHWND,MM_INSERTITEM,MPFROMP(&mi),
                   (MPARAM)text); 

        /* Insert skript and user menu item if necessary */
        if(WinSendMsg(myFrameCtrlHWND,MM_QUERYITEM,MPFROM2SHORT(ID_MBMENUSKRIPTS,FALSE),MPFROMP(&mi))) {
          /* Script menu is inserted so WPS-Wizard is installed */
          hwndMenu=mi.hwndSubMenu;
          if(hwndMenu){
            /* The class specific skript menu is inserted in class CWFolder */
            /* Query the number of items in the menubar */
            iItemCount=(int)WinSendMsg(hwndMenu,MM_QUERYITEMCOUNT,(MPARAM)0,
                                       (MPARAM)0);
            if(iItemCount)
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_M3UFOLDER, FIRSTUSERITEM, TRUE);
            else
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_M3UFOLDER, FIRSTUSERITEM, FALSE);
          }
        }
#if 0
        else {
          /* Load the user menu from the resource if WPS-Wizard is not installed. If installed the menu item
             is added by WPS-Wizzard. */
          hwndMenu=WinLoadMenu(myFrameCtrlHWND,hResource, ID_MBMENUSKRIPTS);
          if(hwndMenu){
            /* Query the number of items in the menu */
            iItemCount=(int)WinSendMsg(hwndMenu,MM_QUERYITEMCOUNT,(MPARAM)0,
                                       (MPARAM)0);

            if(iItemCount)
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_M3UFOLDER, FIRSTUSERITEM, TRUE);
            else
              cwInsertUserMenuItems(hwndMenu, USERMENUFOLDER_M3UFOLDER, FIRSTUSERITEM, FALSE);

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
      }

      /* Now add the framecontrols */

      /* This is the Top-frameCtrl. */
      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd, m3uTopCtrlDialogProc,hResource,IDDLG_TOPFRAME ,this);
      /* Save the HWND in the instance data */    
      //      hwndTop=myFrameCtrlHWND;
      /* Save the HWND in the Window words */    
      WinSetWindowULong( hwnd, ulQWL_TOPCTRLHWND, myFrameCtrlHWND);  
    
      if(myFrameCtrlHWND){
        /* Query the size of the dialog */
        WinQueryWindowRect(myFrameCtrlHWND,&rectl);
        /* It's the top dialog so we need the height but not the width. The width
           is always the width of the folder. */
        //        sizel.cy=rectl.yTop-rectl.yBottom;
        sizel.cy=12;
        /* Call a method of CWProgFolder to add the dialog as a framecontrol. 
           FCTL_TOP means put it at the top of the folder.
           After calling cwUpdateFrame(hwnd) it will be visible */
        cwAddFrameCtl(hwnd, myFrameCtrlHWND, sizel, FCTL_TOP,0);
      }

      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd,leftCtrlDialogProc,hResource,IDDLG_LEFTFRAME ,this);
      /* Save the HWND in the instance data */    
      //    hwndLeft=myFrameCtrlHWND;
      /* Save the HWND in the Window words */    
      WinSetWindowULong( hwnd, ulQWL_LEFTCTRLHWND, myFrameCtrlHWND);
        
    
      if(myFrameCtrlHWND){
        /* Query the size of the dialog */
        WinQueryWindowRect(myFrameCtrlHWND, &rectl);
        /* It's the right dialog so we need the width but not the height. */
        sizel.cx=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx-1-7-2;//rectl.xRight-rectl.xLeft;
        //  sizel.cx=rectl.xRight-rectl.xLeft;
        /* Call a method of CWProgFolder to add the dialog as a framecontrol. 
           FCTL_RIGHT means put it at the RIGHT of the folder.
           After calling cwUpdateFrame(hwnd) it will be visible */
        cwAddFrameCtl(hwnd, myFrameCtrlHWND, sizel, FCTL_LEFT, FCTL_POSABOVE|FCTL_POSBELOW);
      }

      /* Load the dialog from the resource. The object pointer of this
         folder is given to the dialog procedure (this). In WM_INITDLG
         the procedure saves this pointer for later use. */
      myFrameCtrlHWND=WinLoadDlg(hwnd,hwnd,rightCtrlDialogProc,hResource,IDDLG_FRAMERIGHT ,this);
      /* Save the HWND in the instance data */    
      //      hwndRight=myFrameCtrlHWND;
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
        
        //        hwndContainer=WinWindowFromID(myFrameCtrlHWND, IDCNR_PLAYLIST);
        //  WinSendMsg(hwndContainer,CM_QUERYCNRINFO, MPFROMP(&cnrInfo),
        //         MPFROMLONG(sizeof(cnrInfo)));
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

      /* Refresh frame to show the new framecontrols */
      cwUpdateFrame(hwnd);

      /* Subclass frame */
      pfnwpFrame=WinSubclassWindow(hwnd, m3uFrameProc);
      pfnwpGenericFrame=pfnwpFrame;

      /* Subclass container */
      myFrameCtrlHWND=WinWindowFromID(hwnd,FID_CLIENT);//Get container hwnd
      if(myFrameCtrlHWND){
        PFNWP pfnwpContainer;
        pfnwpContainer=WinSubclassWindow(myFrameCtrlHWND, m3uFldrContainerProc);
        /* Save container proc in frame window words */
        WinSetWindowULong( hwnd, ulQWP_CONTAINERPROC, (ULONG)pfnwpContainer);
      }
      cwInsertAllTracks(NULLHANDLE);
    }
    CATCH(M3UFLDR_OPEN)
      {
        SysWriteToTrapLog("\nTrap occured in %s, file: %s above line %d.\n",
                          __FUNCTION__, __FILE__, __LINE__);
        
      } END_CATCH;    

      /* Make sure on open the container is visible */
      WinQueryWindowPos(hwnd, &swp);
      WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y-200, swp.cx, swp.cy+200, SWP_SIZE|SWP_MOVE);
  }

  return hwnd;
}

BOOL CWM3UFolder::wpDeleteFromContent(WPObject* Object)
{
  fContentsChanged=TRUE;
  return CWMediaFolder::wpDeleteFromContent(Object);
}

MRESULT CWM3UFolder::wpDrop(HWND hwndCnr,PDRAGINFO pDragInfo,PDRAGITEM pDragItem)
{
  return CWMediaFolder::wpDrop(hwndCnr, pDragInfo, pDragItem);
}

void _Optlink saveThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  CWM3UFolder *thisPtr;
  HWND hwnd;
  char chrFileName[CCHMAXPATH];
  ULONG ulSize;
  M3UTHREADPARMS *parms=(M3UTHREADPARMS*)arg;


  if(!parms)
    return;

  thisPtr=(CWM3UFolder*)parms->thisPtr;   
  if(!somIsObj(thisPtr))
    return;

  if(!somIsObj(thisPtr->wpObjM3UFile))
    return;

  ulSize=sizeof(chrFileName);
  if(!((WPFileSystem*)thisPtr->wpObjM3UFile)->wpQueryRealName(chrFileName, &ulSize, TRUE))
    return;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      FILE* fStream;      

      if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), parms->hwndCnr))
        {
          /* Container handle is valid */
          if((fStream=fopen(chrFileName, "w"))!=NULLHANDLE) /* Open the playlist file */
            {
              HWND hwndContainer=parms->hwndCnr;

              for(;;)
                {
                  PMINIRECORDCORE mrc;
                  WPObject * contentObject;
                  /* Get first container item of our folder */
                  mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, MPFROMSHORT(CMA_FIRST),
                                                  MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
                  if(!mrc || (int)mrc==-1)
                    break;

                  do {
                    contentObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
                    if(somIsObj(contentObject)) {
                      /* Get file system object or NULL */
                      if((contentObject=thisPtr->cwGetFileSystemObject(contentObject))==NULLHANDLE) {
                        /* Try next object */
                        mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                                        MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
                        continue;
                      }
                    }
                    else
                      break;/* Error */
                                        
                    /* Get the name of the audio file */
                    ulSize=sizeof(chrFileName);
                    ((WPFileSystem*)contentObject)->wpQueryRealName(chrFileName,&ulSize,TRUE);
                    /* Try next object */
                    fputs(chrFileName,fStream);
                    fputs("\n",fStream);
                    mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD, mrc,
                                                    MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
                  }while(mrc && (int)mrc!=-1);
                  break;
                }/* for() */
            }/* fopen() */   
          fclose(fStream);
        }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  /* Mark as saved */
  thisPtr->fContentsChanged=FALSE;
  if(!parms->fSaveOnly) {
    /* Close and destroy the folder if requested */
    WinPostMsg(parms->hwndFrame, WM_CLOSE, 0, 0);
    if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), thisPtr->hwndObject)) 
      WinPostMsg(thisPtr->hwndObject, WM_APPTERMINATENOTIFY, 0, 0);
  }
  thisPtr->wpFreeMem((PBYTE)parms);
}

BOOL CWM3UFolder::cwSavePlaylist(char* chrM3UFile, BOOL fClose)
{
  M3UTHREADPARMS* parms;
  ULONG ulError;

  if((parms=(M3UTHREADPARMS*)wpAllocMem(sizeof(M3UTHREADPARMS),&ulError))!=NULLHANDLE)
    {
      HWND hwnd;

      memset(parms,0, sizeof(M3UTHREADPARMS));
      parms->thisPtr=this;
      if((hwnd=WinQueryWindow(hwndBottom,QW_PARENT))!=NULLHANDLE)
        {
          parms->hwndFrame=hwnd;
          parms->hwndCnr=WinWindowFromID(hwnd, FID_CLIENT);
        }
      if(fClose)
        fClosing=TRUE;
      else
        parms->fSaveOnly=TRUE;

      _beginthread(saveThreadFunc,NULL,8192*16,(void*)parms); //Fehlerbehandlung fehlt
    }
  else {
    /* FIXME: Error handling!*/
  }
  return TRUE;
}

BOOL CWM3UFolder::cwClose(HWND hwndFrame)
{
  SWP swp;

  if(fClosing)
    return TRUE;

  if(fContentsChanged)
    {
      char text[200];
      char title[50];
      ULONG ulRc;
      if((ulRc=messageBox( text, IDSTR_M3UCLOSETEXT, sizeof(text),
                           title, IDSTR_CLOSETITLE, sizeof(title),
                           queryResModuleHandle(),
                           HWND_DESKTOP, 
                           MB_YESNOCANCEL|MB_MOVEABLE|MB_ICONQUESTION))==MBID_CANCEL)
      return FALSE;
      if(ulRc==MBID_YES) {
        cwSavePlaylist(NULLHANDLE, TRUE);
        return FALSE;
      }
    }
  fClosing=TRUE;

  if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndObject)) {
    WinPostMsg(hwndObject, WM_APPTERMINATENOTIFY, 0, 0);
  }
  return TRUE;
}

ULONG CWM3UFolder::wpFilterPopupMenu(ULONG ulFlags, HWND hwndCnr, BOOL fMultiSelect)
{

  return CTXT_HELP|CTXT_SORT|CTXT_SETTINGS|CTXT_REFRESH|CTXT_FIND;
#if 0
  return CWMediaFolder::wpFilterPopupMenu(ulFlags, hwndCnr, fMultiSelect)&
    ~(CTXT_COPY|CTXT_ICON|CTXT_CHANGETOICON|CTXT_CHANGETOTREE|CTXT_CHANGETODETAILS);
#endif
}

BOOL CWM3UFolder::wpModifyPopupMenu(HWND hwndMenu, HWND hwndCnr, ULONG ulPosition)
{
  WinSendMsg( hwndMenu, MM_REMOVEITEM, MPFROM2SHORT(0x2ca, TRUE),0);
  return TRUE;
}








