/*
 * This file is (C) Chris Wohlgemuth 2002-2004
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
#define INCL_GPI
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_SW
#define INCL_REXXSAA

#include "mediafolder.hh"
#include "mediafolderres.h"

#include "sys_funcs.h"
#include "menu_funcs.h"

#include "launchpad.hh"

//#pragma SOMAsDefault(pop)
#include <rexxsaa.h>                   /* needed for RexxStart()     */

#include <stdio.h>
#include <stdlib.h>
//#pragma SOMAsDefault(on)

#include "menufolder.hh"

typedef BOOL   SOMLINK somTP_CWMenuFolder_mfCheckMenuItems(CWMenuFolder *somSelf,
		WPObject* wpObject,
		ULONG ulMenuId);
typedef somTP_CWMenuFolder_mfCheckMenuItems *somTD_CWMenuFolder_mfCheckMenuItems;

typedef BOOL   SOMLINK somTP_CWMenuFolder_mfCheckSkriptMenuItems(CWMenuFolder *somSelf,
		WPObject* wpObject,
		ULONG ulMenuId,
        HWND hwndFrame);
typedef somTP_CWMenuFolder_mfCheckSkriptMenuItems *somTD_CWMenuFolder_mfCheckSkriptMenuItems;


PFNWP  oldProc; //Old proc from subclassed shadow
HWND hwndShadow;// The handle of the help window
extern PFNWP  oldButtonProc2;  //place for original button-procedure
extern HWND hwndBubbleWindow;// The handle of the help window
#define  WM_NEWBUBBLE   WM_USER+100 //Use ATOM later
#define xVal  12      //x-distance of Bubble
#define yVal  8      //y-distance of Bubble
char chrTBFlyFontName[CCHMAXPATH]="9.WarpSans";/* Font for toolbar fly over help */
RGB rgbTBFlyForeground={0};
RGB rgbTBFlyBackground={180,255,255};
BOOL bTBFlyOverEnabled=TRUE;
int iTBFlyOverDelay=250;

/* Pointer into window words */
extern ULONG ulQWP_CONTAINERPROC;
extern ULONG ulQWP_THISPTR;
extern ULONG ulQWL_TOPCTRLHWND;
extern ULONG ulQWL_BOTTOMCTRLHWND;
extern ULONG ulQWL_RIGHTCTRLHWND;
extern ULONG ulQWL_LEFTCTRLHWND;

extern char chrInstallDir[CCHMAXPATH];
extern CONTROLINFO ciControls[];
extern LOADEDBITMAP allBMPs[];
extern PFNWP pfnwpGenericFrame;
extern PFNWP  oldStaticTextProc;
extern LPList* lplAllLPads;

/* Extern */
//extern ULONG cwQueryOSRelease();
extern BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, USHORT usID, USHORT usDelta);
void sendConfig();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

void handleWindowPosChanged(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);/* In dataCddialogProcs.cpp */
void writeLogPrintf(char* logFile, char* format, ...);
void HlpWriteToTrapLog(const char* chrFormat, ...);
HMODULE queryResModuleHandle(void);
void callMediaFolderMethod(CWMediaFolder *thisPtr, ULONG ulAction, ULONG ulData);

MRESULT paintControls(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL paintHilitedControl(HWND hwnd, USHORT usControl, BOOL bHilite);
void adjustDialogControlPos(HWND hwnd);
MRESULT drawSlider(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2, USHORT id);
void setPlayTimeText(CWMediaFolder *thisPtr);
void togglePlayTimeDisplayVisibility(CWMediaFolder *thisPtr);
winSetVolumeSlider(CWMediaFolder *thisPtr);
winCheckRepeatButton(CWMediaFolder *thisPtr);
void contextMenu(HWND hwnd, USHORT usItem, BOOL fCheckItem);

MRESULT EXPENTRY staticTextProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
extern void _Optlink openThreadFunc (void *arg);

extern SOMClass* cwGetSomClass(char* chrClassName);

/* Local */
MRESULT EXPENTRY topCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;

/*****************************************************/
/*                                                   */
/* New container procedure so tabbing to the play    */
/* control dialog works.                             */
/*                                                   */
/*****************************************************/
MRESULT EXPENTRY mediaFldrContainerProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder* thisPtr;
  HWND hwndDialog;
  PFNWP pfnwpContainer;

  switch(msg)
    {
    case WM_CHAR:
      /* handle TAB keys so user may tab from the container to the frame controls */
      if(SHORT1FROMMP(mp1) & KC_VIRTUALKEY)
        {
          if(SHORT2FROMMP(mp2)==VK_TAB && !(SHORT1FROMMP(mp1) & KC_KEYUP)) {
            /* Always forward tab to the top middle framecontrol */
            if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE)/* The frame */
              {
                /* Get HWND of top middle dialog */
                if((hwndDialog=WinWindowFromID(WinWindowFromID(hwndDialog,IDDLG_TOPFRAME),IDDLG_TOPMIDDLE))!=NULLHANDLE)
                  {
                    if((hwndDialog=WinWindowFromID(hwndDialog,IDPB_SKIPBACK))!=NULLHANDLE)
                      if(WinSetFocus(HWND_DESKTOP,hwndDialog))
                        return (MRESULT)TRUE;
                  }
              }
            break;/* error: fall to the window proc */
          }
          else if(SHORT2FROMMP(mp2)==VK_BACKTAB && !(SHORT1FROMMP(mp1) & KC_KEYUP) )
            {
              /* Check for key down to make sure the focus doesn't switch immediately to the next
                 frame control when just coming from another control. Only if the user hits tab 
                 again in the container the focus switches. */
              if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE)
                {
                /* Get HWND of top middle dialog */
                if((hwndDialog=WinWindowFromID(WinWindowFromID(hwndDialog,IDDLG_TOPFRAME),IDDLG_TOPMIDDLE))!=NULLHANDLE)
                  {
                    if((hwndDialog=WinWindowFromID(hwndDialog,IDSL_POSBAR))!=NULLHANDLE)
                      if(WinSetFocus(HWND_DESKTOP,hwndDialog))
                        return (MRESULT)TRUE;
                  }
                }
            }
          break;/* error: fall to the window proc */
        }
      break;

      /*****************************************************/
      /* Show a busy dialog if necessary.                  */
      /*****************************************************/
    case WM_MOUSEMOVE:
      /* Get frame handle */
      if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE)
        {
          thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwndDialog,ulQWP_THISPTR);
          if(somIsObj(thisPtr))
            if(thisPtr->mfData)
              /* A thread for calculating the size is running so show the wait ptr. */
              if(thisPtr->mfData->bProcessing==TRUE) {
                HPOINTER hptr=WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
                if(hptr) {
                  WinSetPointer(HWND_DESKTOP, hptr);
                  return MRTRUE;
                }
              }

#if 0
          if((hwndDialog=WinWindowFromID(hwndDialog,IDDLG_TOPFRAME))!=NULLHANDLE)
            {
              thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
              if(somIsObj(thisPtr))
                if(thisPtr->mfData)
                  /* A thread for calculating the size is running so show the wait ptr. */
                  if(thisPtr->mfData->bProcessing==TRUE) {
                    HPOINTER hptr=WinQuerySysPointer(HWND_DESKTOP, SPTR_WAIT, FALSE);
                    if(hptr) {
                      WinSetPointer(HWND_DESKTOP, hptr);
                      return MRTRUE;
                    }
                  }
            }
#endif
        }
      break;
    default:
      break;
    }
  if((hwndDialog=WinQueryWindow(hwnd,QW_PARENT))!=NULLHANDLE) {
    pfnwpContainer=(PFNWP)WinQueryWindowULong( hwndDialog, ulQWP_CONTAINERPROC);
    if(pfnwpContainer)
      return pfnwpContainer(hwnd, msg, mp1, mp2);
  }

#if 0
  /* Any other message is handled by the folder container procedure */
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

/************************************************************
 *                                                          *
 * This frame proc handles the about menuitem of the        *
 * Warp 4 menu bar and removes the Tree view item.          *
 *                                                          *
************************************************************/
MRESULT EXPENTRY mediaFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder* thisPtr;
  HWND hwndDialog;
  MENUITEM mi;
  char text[100];

  switch (msg)
    {
    case WM_INITMENU:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,ulQWP_THISPTR);
      if(somIsObj(thisPtr)) {
        HMODULE hResource=queryResModuleHandle();
        switch(thisPtr->usLastSelMenuItem)
          {
            //#ifndef ECS
            /* The eCS version doesn't have an 'About menu' */
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
            //#endif

          case 0x68:
            /* Remove change to Tree from menubar menu */ 
            WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(717,TRUE),0);
            /* V0.2.8 (07.09.2003):
               Removed icon view item from menu. Icon view is no longer supported by
               the media folder.*/

            /* Remove change to Icon view from menubar menu */ 
            WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x2cc,TRUE),0);

            break;

          case 0x2d1:
            /* Remove change to Tree from menubar menu */ 
            WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(717,TRUE),0);
            /* V0.2.8 (07.09.2003):
               Removed icon view item from menu. Icon view is no longer supported by
               the media folder.*/
            /* Remove change to Icon view from menubar menu */ 
            WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x2cc,TRUE),0);

            /* Insert the new 'compact' view item */
            if(hResource) {
              SHORT sItem;
              /* Insert 'Compact' item */
              getMessage(text, IDSTR_MENUCOMPACT ,sizeof(text), hResource, hwnd);
              sItem=menuInsertMenuItem( HWNDFROMMP(mp2), NULL, 1, ID_MENUVIEWCOMPACT, text);
              if(sItem!=MIT_MEMERROR && sItem!=MIT_ERROR)
                {
                  /* Disable menu if already in compact view */
                  if(thisPtr->cwQueryMFldrFlags() & MFLDR_VIEW_COMPACT) {
                    WinEnableMenuItem(HWNDFROMMP(mp2), ID_MENUVIEWCOMPACT, FALSE);
                    /* Enable details view item */
                    WinEnableMenuItem(HWNDFROMMP(mp2), 0x2ce, TRUE);
                  }
                }
            }
            break;
          default:
            break;
          }

      }
#if 0
      if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
        {
          thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
          if(somIsObj(thisPtr)) {
            HMODULE hResource=queryResModuleHandle();
            switch(thisPtr->usLastSelMenuItem)
              {
              case 0x2d3:/* Help menu */
                {
                  if(hResource) {
                    /* insert separator */
                    cwInsertMenuSeparator(MIT_END, HWNDFROMMP(mp2), NULL);
                    /* Insert 'About' item */
                    getMessage(text, IDSTR_MENUABOUT ,sizeof(text), hResource, hwnd);
                    cwInsertMenuItem(MIT_END, HWNDFROMMP(mp2), NULL, ID_ABOUTITEM, text);
                    break;
                  }
                  break;
                }
              case 0x2d1:
                /* Remove change to Tree from menubar menu */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(717,TRUE),0);
                /* Insert the new 'compact' view item */
                if(hResource) {
                  /* Insert 'Compact' item */
                  getMessage(text, IDSTR_MENUCOMPACT ,sizeof(text), hResource, hwnd);
                  cwInsertMenuItem( 1, HWNDFROMMP(mp2), NULL, ID_MENUVIEWCOMPACT, text);
                }
                break;
              default:
                break;
              }
          }
        }
#endif
      break;
    case WM_COMMAND:
      if(SHORT1FROMMP(mp2)==CMDSRC_MENU) {
        if(SHORT1FROMMP(mp1)>=FIRSTUSERITEM) { 
          BOOL bHandled=FALSE;

          TRY_LOUD(AF_MENU) {
            thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,ulQWP_THISPTR);
            if(somIsObj(thisPtr)) {
              CWMenuFolder * wpFolder;	
              HOBJECT hObject;
              somId id;

              somTD_CWMenuFolder_mfCheckSkriptMenuItems methodPtr;
              M_WPObject *m_wpObject;
              
              //     bHandled=thisPtr->cwCheckUserMenuItems(SKRIPTFOLDER_MEDIAFOLDER, SHORT1FROMMP(mp1));
              if(!bHandled)
                bHandled=thisPtr->cwCheckUserMenuItems(USERMENUFOLDER_MEDIAFOLDER , SHORT1FROMMP(mp1));
            }/* if(somIsObj(thisPtr)) */
          }
          CATCH(AF_MENU)
            {
            } END_CATCH;
            
#if 0
          TRY_LOUD(AF_MENU) {
            if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
              {
                thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
                if(somIsObj(thisPtr)) {

                  CWMenuFolder * wpFolder;	
                  HOBJECT hObject;
                  somId id;
                  somTD_CWMenuFolder_mfCheckSkriptMenuItems methodPtr;
                  M_WPObject *m_wpObject;
                  
                  //     bHandled=thisPtr->cwCheckUserMenuItems(SKRIPTFOLDER_MEDIAFOLDER, SHORT1FROMMP(mp1));
                  if(!bHandled)
                    bHandled=thisPtr->cwCheckUserMenuItems(USERMENUFOLDER_MEDIAFOLDER , SHORT1FROMMP(mp1));
                }/* if(somIsObj(thisPtr)) */
              }/* Dialog window */
          }
          CATCH(AF_MENU)
            {
            } END_CATCH;
#endif

            if(bHandled)
              return (MRESULT) 0;
        }
      }
      break;

    case WM_MENUSELECT:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,ulQWP_THISPTR);
      if(somIsObj(thisPtr))
        thisPtr->usLastSelMenuItem=SHORT1FROMMP(mp1);
      break;
    default:
      break;
    }
  /* Any other message is handled by the folder frame procedure */
  thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,ulQWP_THISPTR);
  if(somIsObj(thisPtr))
    if(thisPtr->pfnwpFrame)
      return thisPtr->pfnwpFrame(hwnd, msg, mp1, mp2);
  
  return  pfnwpGenericFrame(hwnd, msg, mp1, mp2);
}

/************************************************************
 *                                                          *
 * This dialog proc handles the playcontrol area at the     *
 * top.                                                     *
 *                                                          *
 ************************************************************/
MRESULT EXPENTRY playControlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder *thisPtr;
  static BOOL bPosDragging=FALSE;
  
  switch(msg)
    {
      /*****************************************************/
      /* Init the dialog                                   */
      /*****************************************************/
    case WM_INITDLG :
      
      WinDefDlgProc(hwnd, msg, mp1, mp2);
      
      WinSetWindowULong(hwnd,QWL_USER,(ULONG)PVOIDFROMMP(mp2));//Save object ptr.
      thisPtr=(CWMediaFolder*) PVOIDFROMMP(mp2);
      adjustDialogControlPos(hwnd);

      if(SysQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }

      if(somIsObj(thisPtr)) {
        WinSendMsg( WinWindowFromID(hwnd, IDSL_VOLUME),
                    SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMLONG( thisPtr->ulVolume));
        /* Set position slider */
        WinSendMsg( WinWindowFromID(hwnd, IDPB_POSBAR),
                    SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMLONG(0));

        /* Set repeat check button if necessary */
#if 0
        /* This Does not work... Probably because hwndTopMiddle isn't set in the
         instance data yet. Left in here so it isn't forgotten... */
        winCheckRepeatButton(thisPtr);
        winSetVolumeSlider(thisPtr);
#endif

        if(thisPtr->ulFlags&FLAG_REPEAT)
          WinCheckButton(hwnd, IDCB_REPEAT,1);
        if(thisPtr->ulFlags&FLAG_RANDOM)
          WinCheckButton(hwnd, IDCB_RANDOM,1);


        if(thisPtr->mfData)
          WinSetPresParam(WinWindowFromID(hwnd, IDST_PLAYTIME),
                          PP_FONTNAMESIZE,(ULONG)sizeof(thisPtr->mfData->chrPlayTimeFont),
                          thisPtr->mfData->chrPlayTimeFont);
      }
      return (MRESULT)FALSE;
      /*****************************************************/
      /* The dialog is about to be destroyed               */
      /*****************************************************/
    case WM_DESTROY:
      {
        ULONG  attrFound;

        thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(thisPtr)) {
          if(thisPtr->mfData)
            // Query the playtime font
            WinQueryPresParam(WinWindowFromID(hwnd, IDST_PLAYTIME),
                              PP_FONTNAMESIZE,0, &attrFound,sizeof(thisPtr->mfData->chrPlayTimeFont),
                              thisPtr->mfData->chrPlayTimeFont, QPF_NOINHERIT);
        }
        break;
      }
      /*****************************************************/
      /* Paint the background                              */
      /*****************************************************/
    case WM_PAINT:
      {
        SWP swp;
        RECTL rcl;
        HPS hps;

        WinQueryWindowPos(hwnd,&swp);
        if(swp.fl & SWP_MINIMIZE)
          break;      
        hps = WinBeginPaint( hwnd, NULLHANDLE, &rcl );
        WinDrawBitmap(hps, allBMPs[MAIN_BMP_IDX].hbm,
                      &rcl, 
                      (PPOINTL)&rcl,
                      0, 0,
                      DBM_IMAGEATTRS);    
        WinQueryWindowRect(hwnd, &rcl);
        WinEndPaint( hps );        
        return FALSE;
      }
      /*****************************************************/
      /* The following two messages are for device sharing */
      /*****************************************************/
    case WM_ACTIVATE:
      if(SHORT1FROMMP(mp1)){
        thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(thisPtr)){
          char chrCommand[100];

          if(thisPtr->mrcPlaying) {
            sprintf(chrCommand,"ACQUIRE wave%d QUEUE wait", thisPtr->iWave);
            mciSendString(chrCommand, NULL, 0, 0, 0);
          }
        }
      }
      break;
    case MM_MCIPASSDEVICE:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        HWND hwndFrame=WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT);
        switch(SHORT1FROMMP(mp2))
          {
          case MCI_GAINING_USE:
              thisPtr->bPaused=FALSE;/* The time display will not be increased */
               paintHilitedControl(WinWindowFromID(hwnd, IDPB_PAUSE), IDPB_PAUSE, FALSE);
             return (MRESULT)0;
          case MCI_LOSING_USE:
              thisPtr->bPaused=TRUE;
              paintHilitedControl(WinWindowFromID(hwnd, IDPB_PLAY), IDPB_PLAY, FALSE);
              return (MRESULT)0;
          default:
            break;
          }
      }
      break;
      /*****************************************************/
      /* Set the time in the palytime display.             */
      /* SETPOSITIONADVICE does not work for some reason,  */
      /* so a timer is used. Yes, it's quit cumbersome...  */
      /*****************************************************/  
    case WM_TIMER:
      if(SHORT1FROMMP(mp1)==IDTIMER_PLAY) {
        thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(thisPtr)){
          char chrCommand[50];
          char retMsg[20];
          ULONG rc;
          
          if(!thisPtr->bPaused) {
            /* Adjust position info if not paused. */
            thisPtr->ulPos++;
            if(thisPtr->ulPos*1000>thisPtr->ulTotalLength)
              thisPtr->ulPos=thisPtr->ulTotalLength/1000;/* Prevent wrong time if timer info isn't accurate.
                                                            I know all this timing stuff is an ugly thing. */

            if(!bPosDragging) {
              ULONG ulInc=thisPtr->ulTotalLength/1000;
              setPlayTimeText(thisPtr); /* Set time display */
              /* Catch potential division by zero trap */
              if(ulInc)
                ulInc=thisPtr->ulPos*100/ulInc;
              else
                ulInc=100;
              /* Set slider position */
              WinPostMsg( WinWindowFromID(hwnd, IDSL_POSBAR), 
                          SLM_SETSLIDERINFO,
                          MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                          MPFROMLONG(ulInc));
            }
            thisPtr->cwPlayTimeCallback(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT), thisPtr->ulPos);
            /* Blink play control */
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PLAY), IDPB_PLAY, thisPtr->ulPos%2);
          }
          else {
            /* Currently pausing */
            togglePlayTimeDisplayVisibility(thisPtr);
            paintHilitedControl(  WinWindowFromID(hwnd, IDPB_PAUSE), IDPB_PAUSE,
                                 WinIsWindowVisible(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME)));
          }
        }
        return (MRESULT)0;
      }
      break;
      /*****************************************************/
      /* Draw the sliders.                                 */
      /*****************************************************/
    case WM_DRAWITEM:
      switch(SHORT1FROMMP(mp1))
        {
        case IDSL_POSBAR:
        case IDSL_VOLUME:
          return drawSlider(hwnd, msg, mp1, mp2, SHORT1FROMMP(mp1));
        default:
          break;
        }              
      break;
      /*****************************************************/
      /* Control messages                                  */
      /*****************************************************/
    case WM_CONTROL:
      {
        SWP swp;
        char chrCommand[50];
        char retMsg[20];
        
        thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(thisPtr)){
          switch(SHORT1FROMMP(mp1))
            {
            case IDCB_REPEAT:
              if(SHORT2FROMMP(mp1)== BN_CLICKED) {
                if(WinQueryButtonCheckstate(hwnd,IDCB_REPEAT) & 1) {
                  thisPtr->ulFlags|=FLAG_REPEAT;
                }
                else {
                  thisPtr->ulFlags&=~FLAG_REPEAT;
                }
              }
              return (MRESULT) FALSE;
              break;
            case IDCB_RANDOM:
              if(SHORT2FROMMP(mp1)== BN_CLICKED) {
                if(WinQueryButtonCheckstate(hwnd,IDCB_RANDOM) & 1) {
                  thisPtr->ulFlags|=FLAG_RANDOM;
                }
                else {
                  thisPtr->ulFlags&=~FLAG_RANDOM;
                }
              }
              return (MRESULT) FALSE;
              break;
            default:
              break;
            }              
          switch(SHORT2FROMMP(mp1))
            {
            case BN_PAINT:
              /* Paint the custom controls (buttons, sliders ) */
              WinQueryWindowPos(hwnd,&swp);
              if(swp.fl & SWP_MINIMIZE)
                break;
              return paintControls(hwnd, msg, mp1, mp2);
              /*****************************************************/
              /* Handle position and volume changes.               */
              /*****************************************************/
            case SLN_SLIDERTRACK:
              {
                switch(SHORT1FROMMP(mp1))
                  {
                  case IDSL_VOLUME:
                    {
                      ULONG ulValue;
                      
                      ulValue=LONGFROMMP(mp2);
                      thisPtr->ulVolume=ulValue;
                      sprintf(chrCommand,"SET wave%d AUDIO VOLUME %d WAIT", thisPtr->iWave, thisPtr->ulVolume);
                      mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
                      break;
                    }
                  case IDSL_POSBAR:
                    {
                      static lLastPos=0;
                      long lPos=LONGFROMMP(mp2);
              
                      bPosDragging=TRUE;
                      lPos*=thisPtr->ulTotalLength/100;
                      if(lPos/1000!=lLastPos) {
                        /* Prevent flickering */
                        sprintf(chrCommand, "%d:%02d", lPos/1000/60, (lPos/1000)%60);
                        WinSetWindowText(WinWindowFromID(hwnd, IDST_PLAYTIME), chrCommand);
                        lLastPos=lPos/1000;
                      }
                      break;
                    }
                  default:
                    break;
                  }/* switch */
                return (MRESULT) 0;
                break;
              }
            case SLN_CHANGE:
              {
                switch(SHORT1FROMMP(mp1))
                  {
                  case IDSL_VOLUME:
                    ULONG ulValue;
                    
                    ulValue=LONGFROMMP(mp2);
                    thisPtr->ulVolume=ulValue;
                    sprintf(chrCommand,"SET wave%d AUDIO VOLUME %d WAIT", thisPtr->iWave, thisPtr->ulVolume);
                    mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
                    thisPtr->wpSaveDeferred();
                    break;
                  case IDSL_POSBAR:
                    {
                      ULONG rc;
                      long lPos=LONGFROMMP(mp2);

                      bPosDragging=FALSE;
                      lPos*=1000;                      
                      lPos*=thisPtr->ulTotalLength/100;
                      lPos/=1000;// Use only full seconds

                      /* Set time display first */

                      thisPtr->cwSeekAudioFile(lPos);
                      /* restart timer */
                      WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, IDTIMER_PLAY, PLAYTIMER_DELAY);  
                      thisPtr->ulPos=lPos/1000;
                      break;
                    }
                  default:
                    break;
                  }/* switch */
                return (MRESULT)TRUE;
              }
            default:
              break;
            }/* switch */
          break;
        }
      }/* WM_CONTROL */

      /*****************************************************/
      /* Handle push button messages.                      */
      /*****************************************************/
    case WM_COMMAND:    
      /* The play control handling for starting, stopping etc. */
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        HWND hwndFrame=WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT);
        switch(SHORT1FROMMP(mp1))
          {
          case IDPB_PLAY:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PAUSE), IDPB_PAUSE, FALSE);
            callMediaFolderMethod(thisPtr, PLAY_FIRST, 0);
            break;
          case IDPB_STOP:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PLAY), IDPB_PLAY, FALSE);
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PAUSE), IDPB_PAUSE, FALSE);
            callMediaFolderMethod(thisPtr, STOP_TRACK, 0);
            break;
          case IDPB_SKIPFORWARD:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PLAY), IDPB_PLAY, FALSE);
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PAUSE), IDPB_PAUSE, FALSE);
            callMediaFolderMethod(thisPtr, PLAY_NEXT, 0);
            break;
          case IDPB_SKIPBACK:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PLAY), IDPB_PLAY, FALSE);
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PAUSE), IDPB_PAUSE, FALSE);
            callMediaFolderMethod(thisPtr, PLAY_PREV, 0);
            break;
          case IDPB_PAUSE:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_PLAY), IDPB_PLAY, FALSE);
            callMediaFolderMethod(thisPtr, PAUSE_TRACK, 0);
            break;
            /* Popup menu over time control */
          case ID_MENUITEMELAPSED:
            thisPtr->usWhichDisplay=DISPLAY_TIMEELAPSED;
            setPlayTimeText(thisPtr);
            break;
          case ID_MENUITEMREMAINING:
            thisPtr->usWhichDisplay=DISPLAY_TIMEREMAINING;
            setPlayTimeText(thisPtr);
            break;
          case ID_MENUITEMID3:
            thisPtr->usWhichDisplay=DISPLAY_TIMEID3;
            WinSetWindowULong(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), QWL_USER, 0);
            setPlayTimeText(thisPtr);
            break;
          default:
            break;
          }/* switch() */
      }/* somIsObj()*/
      return (MRESULT)FALSE;

      /*****************************************************/
      /* Make sure tabbing works.                          */
      /*****************************************************/
    case WM_CHAR:
      if(SHORT1FROMMP(mp1) & KC_ALT) {  // Send keys with ALT modifier to the folder
        HWND hwndTemp;
        // Save control with focus
        hwndTemp=WinQueryFocus(HWND_DESKTOP);
        // Set focus to container so the key will be processed as usual
        WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),FID_CLIENT));
        // Set saved control as old focus owner so it will get the focus back when necessary, not the container
        WinSetWindowULong(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),QWL_HWNDFOCUSSAVE,hwndTemp);
        return WinSendMsg(WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),FID_CLIENT),msg,mp1,mp2);
      }
      else if((SHORT1FROMMP(mp1) & KC_VIRTUALKEY)== KC_VIRTUALKEY)
        {
          if(SHORT2FROMMP(mp2)==VK_TAB ) {
            if(WinQueryFocus(HWND_DESKTOP)==WinWindowFromID(hwnd,IDSL_POSBAR)) {
              // Set focus to container
              WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),
                                                        FID_CLIENT));
              return (MRESULT)TRUE;
            }
          }/* if(SHORT2FROMMP(mp2)==VK_TAB ) */
          else if(SHORT2FROMMP(mp2)==VK_BACKTAB )
            {
              if(WinQueryFocus(HWND_DESKTOP)==WinWindowFromID(hwnd,IDPB_SKIPBACK)) {
                // Set focus to container
                WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),
                                                          FID_CLIENT));
                return (MRESULT)TRUE;
              }
            }
        }/* else if(SHORT1FROMMP(mp1) & KC_VIRTUALKEY) */
      break;
#if 0
    case WM_CONTEXTMENU:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        POINTL ptl;
        HWND hwndClient=WinWindowFromID(WinQueryWindow(WinQueryWindow(hwnd,QW_PARENT),QW_PARENT),FID_CLIENT);
        WinQueryPointerPos(HWND_DESKTOP, &ptl) ;
        WinMapWindowPoints(HWND_DESKTOP, hwndClient, &ptl, 1);

        thisPtr->wpDisplayMenu(hwndClient, NULLHANDLE, &ptl,MENU_OBJECTPOPUP,NULL);
      }
      return MRFALSE;
#endif
      /*****************************************************/
      /* Handle playtime display switching.                */
      /* The messages are coming from the static text.     */
      /*****************************************************/
    case WM_APPTERMINATENOTIFY:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        switch(SHORT1FROMMP(mp1))
          {
          case NOTIFY_CLICK:
            /* Clicked on the playtime display */
            thisPtr->usWhichDisplay++;
            if(thisPtr->usWhichDisplay>2)
              thisPtr->usWhichDisplay=0;
            if(thisPtr->usWhichDisplay==2)
              WinSetWindowULong(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), QWL_USER, 0);
            setPlayTimeText(thisPtr);
            return MRFALSE;
          case NOTIFY_CONTEXT:
            /* Show context menu. */            
            contextMenu(hwnd, thisPtr->usWhichDisplay+ID_MENUITEMELAPSED, TRUE);
            return MRFALSE;
          default:
            break;
          }
      }
      return MRFALSE;

#if 0
    case MM_MCIPOSITIONCHANGE:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        char chrCommand[50];
        char retMsg[20];
        char retMsg2[20];
        int iTime;
        ULONG rc;
        /* Restart check timer */
        WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, IDTIMER_PLAY, PLAYTIMER_DELAY);  
        sprintf(chrCommand,"STATUS wave%d POSITION WAIT", thisPtr->iWave);
        rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
        if((rc & 0x0000ffff)==MCIERR_SUCCESS) {
          iTime=atoi(retMsg);
          /* Set time display */
          sprintf(chrCommand,"%d:%02d  %d:%02d  -%d:%02d",thisPtr->ulTotalLength/1000/60,(thisPtr->ulTotalLength/1000)%60,
                  (iTime+thisPtr->ulStartPosition)/1000/60,((iTime+thisPtr->ulStartPosition)/1000)%60,
                  (thisPtr->ulTotalLength-(iTime+thisPtr->ulStartPosition))/1000/60,
                  ((thisPtr->ulTotalLength-(iTime+thisPtr->ulStartPosition))/1000)%60);
          WinSetWindowText(WinWindowFromID(hwnd, IDST_PLAYTIME), chrCommand);
          WinPostMsg( WinWindowFromID(hwnd, IDPB_POSBAR),
                      SLM_SETSLIDERINFO,
                      MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                      MPFROMLONG( (LONG)((iTime/10)/(thisPtr->ulTotalLength/1000))));
        }
        else {
          char retError[1024];
          mciGetErrorString(rc, retError, 1024);
          sprintf(chrCommand,"%s",  retMsg);
          WinSetWindowText(WinWindowFromID(hwnd, IDST_PLAYTIME), chrCommand);
        }
      }
      break;
#endif
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/************************************************************
 *                                                          *
 * This procedure calculates the positions for the custom   *
 * frame controls.                                          *
 * PM uses the pos to calculate the area which needs        *
 * repainting and the pos of each control. Using this code  *
 * prevents flickering.                                     *
 *                                                          *
 ************************************************************/
MRESULT handleCalcValidRects(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PRECTL pRectl, pRectlNew;
  PSWP pswp;
  POINTL ptl;
  
  pswp=(PSWP)mp2; 
  if(pswp->fl&SWP_SIZE) {
    pRectl=(PRECTL)mp1;        
    pRectlNew=pRectl;
    pRectlNew++;

    if(pRectlNew->xRight!=pRectl->xRight) { 
      if(WinSendMsg(hwnd,WM_QUERYBORDERSIZE,(MPARAM)&ptl,0)) {
        pRectl->xRight-=ptl.x;
      }else {
        /* Ok, then use some defaults. Better than nothing... */
        pRectl->xRight-=4;
      }
      pRectlNew->xRight=pRectl->xRight;
      return (MRESULT)0;
    }
    else if(pRectlNew->xLeft<pRectl->xLeft) {
      if(WinSendMsg(hwnd,WM_QUERYBORDERSIZE,(MPARAM)&ptl,0)) {
        pRectl->xLeft+=ptl.x;
      }else {
        /* Ok, then use some defaults. Better than nothing... */
        pRectl->xLeft+=4;
      }
      pRectlNew->xLeft=pRectl->xLeft;
      return (MRESULT)0;
    }
    else if(pRectlNew->xLeft<pRectl->xLeft) {
      pRectlNew->xLeft=pRectl->xLeft;
            
      return (MRESULT)0;
    } else if(pRectlNew->yBottom!=pRectl->yBottom || pRectlNew->yTop!=pRectl->yTop) {            
      pRectlNew->yTop=pRectl->yTop;
      pRectlNew->yBottom=pRectl->yBottom;
      return (MRESULT)0;
    }
  }
  return (MRESULT)0;
}


/************************************************************
 *                                                          *
 * When sizing and moving the folder the custom frame       *
 * controls must be moved too so they  keep their visual    *
 * position with respect to the left border of the folder.  *
 *                                                          *
 ************************************************************/
void handleWindowPosChanged(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PSWP pswp=(PSWP)mp1;

  if((pswp->fl & SWP_SIZE)&&!(pswp->fl & SWP_SHOW)) {
    SWP swp;
    HENUM hEnum;
    HWND hwndClient;
    PSWP pswpOld;
    int xPos, yPos;
    ULONG ulStyle;

    pswpOld=pswp;
    pswpOld++;

    ulStyle=WinQueryWindowULong(WinQueryWindow(hwnd,QW_PARENT),QWL_STYLE);
    if(!(ulStyle & WS_MINIMIZED) && !(ulStyle & WS_MAXIMIZED)  ) {
      
      if(pswpOld->x != pswp->x || 1) {
        if((pswpOld->cx!=0)) {
          xPos=(pswpOld->x > pswp->x ? pswpOld->x-pswp->x : 0);
          //  yPos=(pswpOld->y > pswp->y  ? pswpOld->y-pswp->y : 0);
        }
        else {
          xPos=0;
        }
        if(pswp->cx<=allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx)
          WinSetWindowPos(WinWindowFromID(hwnd, IDDLG_TOPMIDDLE), NULLHANDLE, 0, 0, 0, 0, SWP_MOVE);
        else
          WinSetWindowPos(WinWindowFromID(hwnd, IDDLG_TOPMIDDLE), NULLHANDLE, (pswp->cx-allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx)/2, 
                          0, 0, 0, SWP_MOVE);
#if 0
        /* Move all client windows so they keep their distance to the left border */
        hEnum=WinBeginEnumWindows(hwnd);
        while((hwndClient=WinGetNextWindow(hEnum))!=NULLHANDLE) {
          if(WinQueryWindowULong())
          WinQueryWindowPos(hwndClient,&swp);
          WinSetWindowPos(hwndClient, NULLHANDLE, swp.x-xPos, (pswp->cy-swp.cy)/2/*swp.y-yPos*/, 0, 0, SWP_MOVE);
        }/* while */
        WinEndEnumWindows(hEnum);
#endif
      }
    }
  }
}


/****************************************************************************/
/*                                                                          */
/* This procedure handles the frame extension of the folder at the top      */
/*                                                                          */
/* The playcontrol dialog is a child of this 'background' dialog.           */
/*                                                                          */
/****************************************************************************/
MRESULT EXPENTRY topCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder *thisPtr;
  char name[CCHMAXPATH];
  ULONG ulNameSize;
  SIZEL sizel;
  ULONG ulFlags;
  HWND hwndTemp;
  POINTL ptl;
  SHORT a,b;
  HOBJECT hObject;
    
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
        thisPtr=(CWMediaFolder*) PVOIDFROMMP(mp2);
        
        hwndTemp=WinLoadDlg(hwnd, hwnd, playControlDialogProc, queryResModuleHandle(),
                            IDDLG_TOPMIDDLE ,thisPtr);
        WinQueryWindowPos(WinQueryWindow(hwnd,QW_PARENT),&swp);
        /**!!!!!!!!!!    BORDERSIZE    !!!!!!!!!! **/
        WinSetWindowPos(hwndTemp, NULL, (swp.cx-allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx)/2, 0, 
                        allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx, allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy, SWP_SIZE|SWP_MOVE|SWP_SHOW);
        thisPtr->hwndTopMiddle=hwndTemp;
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        return (MRESULT)FALSE;
      }
    case WM_CALCVALIDRECTS:
      {
        if(WinIsWindowVisible(hwnd))
          return handleCalcValidRects(hwnd,  msg,  mp1,  mp2);
        break;
      }
    case WM_WINDOWPOSCHANGED:
      {
        /* Move the clients of the framecontrol so they keep their distance to the left */
        handleWindowPosChanged(hwnd, msg, mp1, mp2);
        break;
      }
    case WM_CHAR:
      // Set focus to container so the key will be processed as usual
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT));
      return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT),msg,mp1,mp2);

      /*****************************************************/
      /* Paint the background.                             */
      /*****************************************************/
    case WM_PAINT:
      HPS hps;
      RECTL rcl, rclSave, rclSource;
      SWP swp;

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

      WinEndPaint(hps);
      return (MRESULT) 0;
    case WM_COMMAND:    
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        switch(SHORT1FROMMP(mp1))
          {
          case IDPB_PAUSE:
            {
#if 0
              CNRINFO cnrInfo;
              HWND hwndContainer; 
              FIELDINFO fi;
              PFIELDINFO pfi, pfiFirst;
              char text[200];
              int numLeft=0;
              int numRight=0;
              int numVisLeft=0;
              int numVisRight=0;
              int a=0;
              LONG lWidth[100];
              HPS hps;
              FONTMETRICS fm={0};
              RECTL rcl2;
              ULONG  attrFound;
              char chrFontName[50];
              char *chrPtr;

              THREADPARAMS* pTp;
              ULONG ulErr;

              return (MRESULT)0;

              pTp=(THREADPARAMS*)thisPtr->wpAllocMem(sizeof(THREADPARAMS), &ulErr);
              pTp->thisPtr=thisPtr;
              pTp->hwndFrame=WinQueryWindow(hwnd, QW_PARENT);

              //  _beginthread(rexxThreadFunc,NULL,8192*16,(void*)pTp); //Fehlerbehandlung fehlt
              hwndContainer=WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT), FID_CLIENT);
              return (MRESULT)0;
              WinSendMsg(hwndContainer,CM_QUERYCNRINFO, MPFROMP(&cnrInfo),
                                              MPFROMLONG(sizeof(cnrInfo)));

              pfi=(PFIELDINFO)WinSendMsg(hwndContainer,CM_QUERYDETAILFIELDINFO, NULLHANDLE,
                                              MPFROMSHORT(CMA_FIRST));
              if(!pfi)
                return (MRESULT)0;
              lWidth[a]=(LONG)WinSendMsg(hwndContainer,CM_QUERYDETAILFIELDINFO, MPFROMP(pfi),
                                              MPFROMLONG(0x200));
              pfiFirst=pfi;
              numRight++;
              a++;
              if(!(pfi->flData & CFA_INVISIBLE))
                numVisRight++;
              while(pfi){
                if(pfi==cnrInfo.pFieldInfoLast) {
                  numLeft=numRight;
                  numRight=0;
                  numVisLeft=numVisRight;
                  numVisRight=0;
                }
                pfi=(PFIELDINFO)WinSendMsg(hwndContainer,CM_QUERYDETAILFIELDINFO, MPFROMP(pfi),
                                           MPFROMSHORT(CMA_NEXT));
                if(pfi) {
                  lWidth[a]=(LONG)WinSendMsg(hwndContainer,CM_QUERYDETAILFIELDINFO, MPFROMP(pfi),
                                       MPFROMLONG(0x200));
                  a++;
                  numRight++;
                  if(!(pfi->flData & CFA_INVISIBLE))
                    numVisRight++;
                }
              }

              pfi=pfiFirst;
              // Query the current font
              WinQueryPresParam(hwndContainer,
                                PP_FONTNAMESIZE,0,&attrFound,sizeof(chrFontName),
                                chrFontName,QPF_NOINHERIT);
              /* 0 means error */
              fm.lEmHeight=atol(chrFontName);
              if((chrPtr=strchr(chrFontName, '.'))!=NULLHANDLE)
                chrPtr++;

              hps=WinGetPS(hwndContainer);
              strcpy(fm.szFacename, chrPtr);

              GpiQueryFontMetrics(hps,sizeof(fm),&fm);

              WinReleasePS(hps);
              sprintf(text,"%s num: %d CFA_OWNER: %s numLeft: %d numRight: %d numVisLeft: %d numVisRight: %d",
                      pfi->pTitleData,
                      cnrInfo.cFields, 
                      (pfi->flData & CFA_OWNER ? "Yes": "No"), 
                      numLeft, numRight,numVisLeft, numVisRight);
              WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_OK|MB_MOVEABLE);

              WinSendMsg(hwndContainer, CM_QUERYVIEWPORTRECT, MPFROMP(&rcl) , MPFROM2SHORT(CMA_WORKSPACE, FALSE));
              WinSendMsg(hwndContainer, CM_QUERYVIEWPORTRECT, MPFROMP(&rcl2) , MPFROM2SHORT(CMA_WINDOW, FALSE));

              sprintf(text,"%d %d %d %d %d %d %d average: %d average*3 %d rcl.xLeft: %d rcl.xRight %d %d rcl2.xLeft: %d rcl2.xRight %d %d", 
                      lWidth[0], lWidth[1], lWidth[2], lWidth[3], lWidth[4], 
                      lWidth[5], lWidth[6],
                      fm.lAveCharWidth, fm.lAveCharWidth*3 , rcl.xLeft,rcl.xRight, rcl.xRight-rcl.xLeft
                      , rcl2.xLeft,rcl2.xRight, rcl2.xRight-rcl2.xLeft);

              WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_OK|MB_MOVEABLE);
              //  DosBeep(5000,300);
              return (MRESULT)0;

              // WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Noch nicht implementiert..", "", 1234, MB_OK|MB_MOVEABLE);
              char chrCommand[200];
              PUSEITEM pUseItem;
              PVIEWITEM pViewItem;
              PVIEWITEM pView;
              WPObject *wpObj;
              WPObject *wpObject;

              PMINIRECORDCORE mrc;
              M_WPObject *mWPObject;

              hwndContainer=WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT), FID_CLIENT);
              mWPObject=thisPtr->wpQueryFldrDetailsClass();

              /* Selected object */
              /* Get first selected container item of our folder */
              mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, MPFROMSHORT(CMA_FIRST),
                                              MPFROMSHORT(CRA_SELECTED));

              while(mrc) {
                wpObj=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
                wpObject=thisPtr->cwGetFileSystemObject(wpObj);
                if(wpObject!=wpObj && wpObject) {
                  if(somIsObj(wpObject)) {
                    if(wpObject->somIsA(mWPObject)) {
                      ULONG ulFieldSize=0;
                      /* Fieldinfosize */
                      ULONG ulCols=mWPObject->wpclsQueryDetailsInfo(NULL, &ulFieldSize);
                      ULONG ulDataSize=0;
                      PBYTE memFieldPtr, memFieldPtrSave;
                      static PBYTE memPtr, memPtrSave;
                      ULONG ulRC;
                      PCLASSFIELDINFO pcfi;
                      PFIELDINFO pfi;
                      int a;
                      PSZ *theStrings;
                      PBYTE  bPtr;
                      
                      /* Detailsdata size */
                      wpObject->wpQueryDetailsData(NULL, &ulDataSize);
                      
                      /* For now some safety */
                      ulDataSize*=4;
                      
                      if((memFieldPtr=thisPtr->wpAllocMem(ulFieldSize, &ulRC))!=NULLHANDLE) {
                        memFieldPtrSave=memFieldPtr;
                        mWPObject->wpclsQueryDetailsInfo( (PCLASSFIELDINFO*)&memFieldPtr, &ulFieldSize);
                        if((memPtr=thisPtr->wpAllocMem(ulDataSize, &ulRC))!=NULLHANDLE) {
                          memPtrSave=memPtr;
                          memset(memPtr, 0, ulDataSize);
                          
                          /* Query details data */
                          wpObject->wpQueryDetailsData( (PPVOID)&memPtr, &ulDataSize);
                          pcfi=(PCLASSFIELDINFO)memFieldPtr;
                          pcfi=pcfi->pNextFieldInfo;
                          
                          /* Get first fieldinfo */
                          pfi=(PFIELDINFO)WinSendMsg(hwndContainer,CM_QUERYDETAILFIELDINFO, MPFROMLONG(0L),
                                                     MPFROMSHORT(CMA_FIRST));
                          memPtr=memPtrSave;
                          PSZ* ptr;
                          PSZ* mrcPtr;
                          for(a=1, pcfi=(PCLASSFIELDINFO)memFieldPtr,ptr=(PSZ*)memPtr; 
                              a<=ulCols && pcfi && pfi; 
                              pfi=pfi->pNextFieldInfo, pcfi=pcfi->pNextFieldInfo,a++)
                            {
                              PFIELDINFO pfi2=NULL;
                              PBYTE temp;
                              PBYTE temp2;
                              temp=(PBYTE)mrc;
                              temp2=temp;
                              temp+=pfi->offStruct;
                              mrcPtr=(PSZ*)temp;

                              if(pfi->offStruct >= mrc->cb) {
                                /* do something here */
                                if(a==7) {
                                  *mrcPtr=*ptr;
                                  //  pfi->flData&=~(CFA_OWNER|CFA_IGNORE);
                                  //  pfi->flData&=~(CFA_OWNER);
                                }                                
                                mrcPtr++;
                                ptr++;
                              }/* offStruct*/
                            }/* for pcfi */
                          DosBeep(5000, 50);
                          thisPtr->wpFreeMem(memPtrSave);
                        }
                        thisPtr->wpFreeMem(memFieldPtrSave);
                      }
                    }/* somIsA */
                  }/* somIsObj */
                }
                /* Get next container item of our folder */
                mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORD,mrc,
                                                MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
              };/* mrc */


              WinSendMsg(hwndContainer,CM_INVALIDATERECORD, NULL,
                         MPFROM2SHORT(0, CMA_TEXTCHANGED));
              DosBeep(500,300);

#if 0
              sprintf(chrCommand, "a: %d: Title: %s ptr: %lx *ptr: %lx ,*bPtr: %lx, offset: %x cb: %x", 
                      a, pcfi->pTitleData, ptr, *ptr, *ptrMrc , pfi->offStruct, mrc->cb  
                      );
              WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrCommand, "", 1234, MB_OK|MB_MOVEABLE);

              if(a==6) {                                
                sprintf(chrCommand,
                        "a: %d: Title: %s ptr: %lx *ptr: %lx ,*bPtr: %lx, %s offset: %x flData: %x %x", 
                        a, pcfi->pTitleData, ptr, *ptr, *ptrMrc , *ptr, pfi->offStruct  
                        ,pfi->flData ,mrc->cb);
                //WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrCommand, "6", 1234, MB_OK|MB_MOVEABLE);
                //  *ptrMrc="00:00";
                /* Make column visible */
                //                                  pfi->flData |=(CFA_STRING | CFA_FIREADONLY);
              }/* a */
              if(a==26) {
                PSZ* ptr;
                PSZ* ptrMrc;

                DosBeep(500, 20);
                bPtr=(BYTE*) mrc;
                bPtr+=pfi->offStruct;
                ptr=(PSZ*)memPtr;
                ptrMrc=(PSZ*)bPtr;
                *ptrMrc=*ptr;
                *ptrMrc="00:00";
              }/* a */
#endif
#if 0
              /* Get first selected container item of our folder */
              mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, MPFROMSHORT(CMA_FIRST),
                                              MPFROMSHORT(CRA_SELECTED));
              if(mrc) {
                wpObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
                if(somIsObj(wpObject)) {
                  DosBeep(5000,300);
                  for(pView=wpObject->wpFindViewItem( 0xFFFFFFFF, NULL );pView;
                      pView=NULLHANDLE/*wpObject->wpFindViewItem( 0xFFFFFFFF, pView )*/)
                    {

                    }
                  DosBeep(500,300);
                }
              }
#endif
              break;
#endif
            }
      default:
        break;
      }
    }
  return (MRESULT) TRUE;
default:
  break;
}
return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


MRESULT EXPENTRY leftCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder *thisPtr;
  
  char name[CCHMAXPATH];
  ULONG ulNameSize;
  SIZEL sizel;
  ULONG ulFlags;
  HWND hwndTemp;
  POINTL ptl;
  SHORT a,b;
  HOBJECT hObject;
  static  PMINIRECORDCORE pmrcInserted=NULL;
    
  switch(msg)
    {
    case WM_INITDLG :   
      WinSetWindowULong(hwnd,QWL_USER,(ULONG)PVOIDFROMMP(mp2));//Save object ptr.
      thisPtr=(CWMediaFolder*) PVOIDFROMMP(mp2);
      if(somIsObj(thisPtr)) {
        WinSendMsg( WinWindowFromID(hwnd, IDSL_VOLUME),
                    SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMLONG( thisPtr->ulVolume));
      }
      /* Set dialog font to WarpSans for Warp 4 and above */
      if(SysQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }      
      return (MRESULT)FALSE;
    case WM_CHAR:
      // Set focus to container so the key will be processed as usual
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT));
      return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT),msg,mp1,mp2);

    case WM_PAINT:
      {
        HPS hps;
        RECTL rcl, rclSource;
        POINTL ptl;
        LONG lTemp;

        if(allBMPs[CTRLIDX_TOPLEFT].hbm) {
          hps=WinBeginPaint(hwnd, NULL, &rcl);
          rclSource.xLeft=0;
          rclSource.yBottom=0;
          rclSource.yTop=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cy;
          rclSource.xRight=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cx;
          lTemp=rcl.xLeft/rclSource.xRight;
          ptl.x=lTemp*rclSource.xRight;
          lTemp=rcl.yBottom/rclSource.yTop;
          lTemp*=rclSource.yTop;   
          //WinFillRect(hps, &rcl, CLR_RED);
          while(ptl.x<rcl.xRight) {
            ptl.y=lTemp;
            while(ptl.y<rcl.yTop) {/* y direction */
              //DosBeep(5000,100);
              WinDrawBitmap(hps, allBMPs[CTRLIDX_TOPLEFT].hbm,
                            &rclSource, 
                            (PPOINTL)&ptl,
                            0, 0,
                            DBM_IMAGEATTRS);
              ptl.y+=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cy;
              //DosSleep(200);
            };
            ptl.x+=allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr.cx; 
          };
          WinEndPaint(hps);
          return (MRESULT)0;
        }
        break;
      }
    case WM_CONTROL:
      switch(SHORT1FROMMP(mp1))
        {
          return (MRESULT)0;
        default:
          break;
        }
      break;
    case WM_COMMAND:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        switch(SHORT1FROMMP(mp1))
          {
          case IDPB_ADD:
            {
              HWND hwndContainer;
              HWND hwndInsert;
              PMINIRECORDCORE mrc;

              WPObject *wpObject;

              hwndContainer=WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT), FID_CLIENT);
              /* Get first selected container item of our folder */
              mrc=(PMINIRECORDCORE)WinSendMsg(hwndContainer,CM_QUERYRECORDEMPHASIS, MPFROMSHORT(CMA_FIRST),
                                              MPFROMSHORT(CRA_SELECTED));
              if(mrc)
                {
                  DosBeep(5000, 500);
                  wpObject=(WPObject*)OBJECT_FROM_PREC(mrc);//Get object
                  if(somIsObj(wpObject)) {
                    POINTL ptl= {40,40};

                    DosBeep(500, 500);
                    hwndInsert=WinWindowFromID(hwnd, IDCNR_PLAYLIST);
                    if(!pmrcInserted)
                      DosBeep(5000, 500);
                      pmrcInserted=wpObject->wpCnrInsertObject(hwndInsert, &ptl,NULL, NULL );                 
                  }
                }
            break;
            }
          case IDPB_REMOVE:
            {
              HWND hwndInserted;
              WPObject *wpObject;

              hwndInserted=WinWindowFromID(hwnd, IDCNR_PLAYLIST);
              if(pmrcInserted) {
                DosBeep(5000, 500);
                wpObject=(WPObject*)OBJECT_FROM_PREC(pmrcInserted);//Get object
                if(wpObject->wpCnrRemoveObject(hwndInserted))
                  pmrcInserted=NULL;
              }
              break;
            }
          default:
            break;
          }
      }
      return (MRESULT) TRUE;
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

ULONG modulo(ULONG start,ULONG* delta, ULONG max)
{   
  while(start>max) {/* Probably slow for small bitmaps and wide screens. Anyone volunteering to write better code? */
    start-=max;
  };
  *delta=max-start;
  return start;
}

/****************************************************************************/
/*                                                                          */
/* This procedure handles the frame extension of the folder on the right    */
/*                                                                          */
/****************************************************************************/
MRESULT EXPENTRY rightCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder *thisPtr;

  switch(msg)
    {
    case WM_INITDLG :   
      WinSetWindowULong(hwnd,QWL_USER,(ULONG)PVOIDFROMMP(mp2));//Save object ptr.
      thisPtr=(CWMediaFolder*) PVOIDFROMMP(mp2);
      if(somIsObj(thisPtr)) {
        WinSendMsg( WinWindowFromID(hwnd, IDSL_VOLUME),
                    SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMLONG( thisPtr->ulVolume));
      }
      /* Set dialog font to WarpSans for Warp 4 and above */
      if(SysQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }

      return (MRESULT)FALSE;
    case WM_CHAR:
      // Set focus to container so the key will be processed as usual
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT));
      return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT),msg,mp1,mp2);
    case WM_PAINT:
      {
        HPS hps;
        RECTL rcl, rclSource;
        POINTL ptl;
        LONG lTemp;

        if(allBMPs[CTRLIDX_TOPRIGHT].hbm) {
          hps=WinBeginPaint(hwnd, NULL, &rcl);
          rclSource.xLeft=0;
          rclSource.yBottom=0;
          rclSource.yTop=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cy;
          rclSource.xRight=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx;
          lTemp=rcl.xLeft/rclSource.xRight;
          ptl.x=lTemp*rclSource.xRight;
          lTemp=rcl.yBottom/rclSource.yTop;
          lTemp*=rclSource.yTop;   
          //WinFillRect(hps, &rcl, CLR_RED);
          while(ptl.x<rcl.xRight) {
            ptl.y=lTemp;
            while(ptl.y<rcl.yTop) {/* y direction */
              //DosBeep(5000,100);
              WinDrawBitmap(hps, allBMPs[CTRLIDX_TOPRIGHT].hbm,
                            &rclSource, 
                            (PPOINTL)&ptl,
                            0, 0,
                            DBM_IMAGEATTRS);
              ptl.y+=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cy;
              //DosSleep(200);
            };
            ptl.x+=allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr.cx; 
          };
          WinEndPaint(hps);
          return (MRESULT)0;
        }
        break;
      }
    case WM_CONTROL:
      switch(SHORT1FROMMP(mp1))
        {

          return (MRESULT)0;
        default:
          break;
        }
      break;
    case WM_COMMAND:    
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
      if(somIsObj(thisPtr)){
        switch(SHORT1FROMMP(mp1))
          {
          default:
            break;
          }
      }
      return (MRESULT) TRUE;
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/****************************************************************************/
/*                                                                          */
/* This procedure is used to position the controls on the bottom frame ctrl */
/* when the folder is sized or a new item is added to the launchpad.        */
/*                                                                          */
/****************************************************************************/
static void _setBottomCtrlClientsPos(HWND hwnd)
{
  HWND hwndClient;
  SWP swp, swp2;
  
  WinQueryWindowPos(hwnd, &swp2);
  hwndClient=WinWindowFromID(hwnd, 1234);
  WinQueryWindowPos(hwndClient,&swp);
  WinSetWindowPos(hwndClient,NULLHANDLE,  20, 0,0,0,SWP_MOVE);
  
  hwndClient=WinWindowFromID(hwnd, IDST_STATUSTEXT);
  WinSetWindowPos(hwndClient,NULLHANDLE,  swp.cx+20+20, 0,  swp2.cx-4,
                  swp2.cy-2, SWP_MOVE|SWP_SIZE|SWP_SHOW);
}

/****************************************************************************/
/*                                                                          */
/* This procedure handles the frame extension of the folder at the bottom   */
/*                                                                          */
/****************************************************************************/
MRESULT EXPENTRY bottomCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  CWMediaFolder *thisPtr;

  switch(msg)
    {
    case WM_INITDLG :
      {
        WinSetWindowULong(hwnd,QWL_USER,(ULONG)PVOIDFROMMP(mp2));//Save object ptr.
        thisPtr=(CWMediaFolder*) PVOIDFROMMP(mp2);
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        oldStaticTextProc=WinSubclassWindow(WinWindowFromID(hwnd, IDST_STATUSTEXT), staticTextProc);

        /* Build launchpad control */
        if(somIsObj(thisPtr)) {
          SWP swp, swp2;
          char text[200];          
          thisPtr->lPad=new launchPad(hwnd, hwnd, TRUE, (WPFolder*) PVOIDFROMMP(mp2), &lplAllLPads, NULLHANDLE, 0);
          WinQueryWindowPos(hwnd,&swp);
          if(thisPtr->lPad) {
            thisPtr->lPad->lpSetConfiguration(chrInstallDir, MEDIAFLDR_TBID);
            thisPtr->lPad->lpSetLaunchPadPos(NULLHANDLE,  20, 0,
                                             30 , swp.cy-2, SWP_SIZE | SWP_MOVE);
            thisPtr->lPad->lpFillPad();
            if(!getMessage(text, IDSTR_LAUNCHPADFLYOVER, sizeof(text), queryResModuleHandle(), hwnd))
              strncpy(text, "Launchpad: Drop objects here", sizeof(text));
            text[sizeof(text)-1]=0;
            thisPtr->lPad->lpSetFlyOverText(text);
          }
        }
        /* Start timer which triggers time calculation */
        WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd, IDTIMER_STATUS, 300);
        return (MRESULT)FALSE;
      }
    case WM_TIMER:
      if(SHORT1FROMMP(mp1)==IDTIMER_STATUS) {
        thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd,QWL_USER);
        if(somIsObj(thisPtr)){
          THREADPARAMS* pTp;
          ULONG ulErr;
          SWP swp;
          char text[100];

          _setBottomCtrlClientsPos(hwnd);
          getMessage(text, IDSTR_STATUSCOLLECTING, sizeof(text), queryResModuleHandle(),hwnd);
          WinSetWindowText(WinWindowFromID(hwnd, IDST_STATUSTEXT), text);
          if(!(thisPtr->wpQueryFldrFlags() & FOI_POPULATEDWITHALL)) {
            WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd, IDTIMER_STATUS, 300);
            return MRFALSE;
          }
          pTp=(THREADPARAMS*)thisPtr->wpAllocMem(sizeof(THREADPARAMS), &ulErr);
          pTp->thisPtr=thisPtr;
          pTp->hwndFrame=WinQueryWindow(hwnd, QW_PARENT);
          _beginthread(openThreadFunc,NULL,8192*16,(void*)pTp); //Fehlerbehandlung fehlt
        };
        WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, IDTIMER_STATUS);
        return MRFALSE;
      }
      break;
    case WM_CHAR:
      // Set focus to container so the key will be processed as usual
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT));
      return WinSendMsg(WinWindowFromID(WinQueryWindow(hwnd,QW_PARENT),FID_CLIENT),msg,mp1,mp2);
    case WM_CALCVALIDRECTS:
      {
        if(WinIsWindowVisible(hwnd))
          return handleCalcValidRects(hwnd,  msg,  mp1,  mp2);
        break;
      }
    case WM_WINDOWPOSCHANGED:
      {
        /* Move the clients of the framecontrol so they keep their distance to the left */
        PSWP pswp=(PSWP)mp1;
        if((pswp->fl & SWP_SIZE)&&!(pswp->fl & SWP_SHOW)) {
          PSWP pswpOld;
          ULONG ulStyle;
          
          pswpOld=pswp;
          pswpOld++;

          if(!(ulStyle & WS_MINIMIZED) && !(ulStyle & WS_MAXIMIZED) ) {
            HWND hwndClient;
            SWP swp, swp2;
            
            hwndClient=WinWindowFromID(hwnd, 1234);
            WinQueryWindowPos(hwndClient,&swp);
            WinSetWindowPos(hwndClient,NULLHANDLE,  20, 0,0,0,SWP_MOVE);
            
            hwndClient=WinWindowFromID(hwnd, IDST_STATUSTEXT);
            WinSetWindowPos(hwndClient,NULLHANDLE,  swp.cx+20+20, 0, pswp->cx-4-20-swp.cx,
                            pswp->cy-2, SWP_MOVE|SWP_SIZE);
          }
        }
        break;
      }
    case WM_APPTERMINATENOTIFY:
      if(SHORT1FROMMP(mp1)==NOTIFY_STARTSTATUSTIMER)
        {
          WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd, IDTIMER_STATUS, 300);
          return MRFALSE;
        }
      break;
    case WM_PAINT:
      {
        HPS hps;
        RECTL rcl, rclSource;
        POINTL ptl;
        LONG lTemp;

        if(allBMPs[CTRLIDX_BOTTOM].hbm) {
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
        break;
      }
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

/*************************************************************/
/* This dialog procedure handles the toolbar page            */
/*************************************************************/			
MRESULT EXPENTRY settingsToolbarOptionDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) {
  HINI hini;
  HOBJECT  hObject;
  ULONG  attrFound;
  ULONG  len;
  char text[250];
  char title[250];
  CWMediaFolder* thisPtr;

  switch(msg) {
#if 0
  case WM_INITDLG :	
    WinSetWindowULong(hwnd,QWL_USER,LONGFROMMP(mp2));
    thisPtr=(CWMediaFolder*)LONGFROMMP(mp2);
    /* We have to initialize the dialog controls with the approbiate values */
    
    if(somIsObj(thisPtr)) {
      // Set the focus on the demo area
      WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD));
      
      // Set the background colour of the demo area
      WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                      PP_BACKGROUNDCOLOR,(ULONG)sizeof(RGB), &(thisPtr->rgbTBFlyBackground) );
      // Set the foreground colour of the demo area
      WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                      PP_FOREGROUNDCOLOR,(ULONG)sizeof(RGB), &(thisPtr->rgbTBFlyForeground) );
      
      // Set the font of the demo area
      WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                      PP_FONTNAMESIZE,(ULONG)sizeof(thisPtr->chrTBFlyFontName),
                      thisPtr->chrTBFlyFontName );
      
      if(thisPtr->bTBFlyOverEnabled) {
        WinCheckButton(hwnd, IDCB_ENABLETBFLYOVER, 1);
        WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),TRUE);
      }
      else {
        WinCheckButton(hwnd, IDCB_ENABLETBFLYOVER, 0);
        WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),FALSE);
      }
      
      // Set the linits for the delay spin button
      WinSendMsg(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),
                 SPBM_SETLIMITS,(MPARAM)MAXDELAY,(MPARAM)0);
      // Set the current value for delay
      WinSendMsg(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),
                 SPBM_SETCURRENTVALUE,
                 (MPARAM)thisPtr->iTBFlyOverDelay,
                 (MPARAM)NULL);
    }
    /* Move default buttons on Warp 4 */
    cwMoveNotebookButtonsWarp4(hwnd, IDPB_COLORHELP, 20);
    WinSetWindowULong(WinQueryWindow(hwnd, QW_PARENT), QWL_HWNDFOCUSSAVE, hwnd);
    return (MRESULT) TRUE;
  case WM_DESTROY:
    WinSetFocus(WinQueryWindow(hwnd, QW_PARENT), TRUE);
    thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd , QWL_USER);    
    if(somIsObj(thisPtr)){

      /* The notebook closes and gets destroyed */
      // Query the current background colour
      len=WinQueryPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                            PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(thisPtr->rgbTBFlyBackground),
                            &(thisPtr->rgbTBFlyBackground),QPF_NOINHERIT);
      // Query the current font
      len=WinQueryPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                            PP_FONTNAMESIZE,0,&attrFound,sizeof(thisPtr->chrTBFlyFontName),
                            thisPtr->chrTBFlyFontName,QPF_NOINHERIT);
      // Query the current foreground colour
      len=WinQueryPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                            PP_FOREGROUNDCOLOR,0,&attrFound,sizeof(thisPtr->rgbTBFlyForeground),
                            &(thisPtr->rgbTBFlyForeground),QPF_NOINHERIT);
      // Query the enable state
      if(WinQueryButtonCheckstate(hwnd,IDCB_ENABLETBFLYOVER) & 1)
        thisPtr->bTBFlyOverEnabled=1;
      else
        thisPtr->bTBFlyOverEnabled=0;
      
      // Query delay value
      WinSendMsg(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),
                 SPBM_QUERYVALUE,(MPARAM)&(thisPtr->iTBFlyOverDelay),
                 MPFROM2SHORT(0,SPBQ_ALWAYSUPDATE));				
      
      // Save data
      thisPtr->wpSaveImmediate();
    }
    break;
#if 0
  case WM_CONTROL:
    /* The window controls send WM_CONTROL messages */
    switch(SHORT1FROMMP(mp1))
      {
      case IDCB_ENABLETBFLYOVER:
        if(WinQueryButtonCheckstate(hwnd,IDCB_ENABLETBFLYOVER) & 1)
          WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),1);
        else
          WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),0);
        break;
      default:
        break;
      } // end switch(SHORT1FROMMP(mp1))
    return (MRESULT)0;
    break;
#endif	
  case WM_COMMAND :
    switch (SHORT1FROMMP(mp1)) {
      // Process commands here //
    case IDPB_COLORUNDO:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd , QWL_USER);    
      if(somIsObj(thisPtr)){
        /* The undo button was clicked */
        // Set the background colour of the demo area
        WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                        PP_BACKGROUNDCOLOR,(ULONG)sizeof(thisPtr->rgbTBFlyBackground), &(thisPtr->rgbTBFlyBackground) );
        // Set the foreground colour of the demo area
        WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                        PP_FOREGROUNDCOLOR,(ULONG)sizeof(thisPtr->rgbTBFlyForeground), &(thisPtr->rgbTBFlyForeground) );
        
        // Set the font of the demo area
        WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                        PP_FONTNAMESIZE,(ULONG)sizeof(thisPtr->chrTBFlyFontName),
                        thisPtr->chrTBFlyFontName );
        
        if(thisPtr->bTBFlyOverEnabled) {
          WinCheckButton(hwnd, IDCB_ENABLETBFLYOVER, 1);
          WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),TRUE);
        }
        else {
          WinCheckButton(hwnd, IDCB_ENABLETBFLYOVER, 0);
          WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),FALSE);
        }
        
        // Set the current value for delay
        WinSendMsg(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),
                   SPBM_SETCURRENTVALUE,
                   (MPARAM)thisPtr->iTBFlyOverDelay,
                   (MPARAM)NULL);
      }
      break;
    case IDPB_COLORSTANDARD:
      thisPtr=(CWMediaFolder*) WinQueryWindowULong(hwnd , QWL_USER);    
      if(somIsObj(thisPtr)){
        /* The default button was clicked */
        thisPtr->rgbTBFlyBackground.bBlue=180;  // Set the default colours
        thisPtr->rgbTBFlyBackground.bGreen=255; 
        thisPtr->rgbTBFlyBackground.bRed=255;
        thisPtr->rgbTBFlyForeground.bBlue=0;
        thisPtr->rgbTBFlyForeground.bGreen=0;			
        thisPtr->rgbTBFlyForeground.bRed=0;
        // Set the default font
        if(cwQueryOSRelease()>=40)
          strcpy((thisPtr->chrTBFlyFontName), DEFAULT_DIALOG_FONT);
        else
          strcpy((thisPtr->chrTBFlyFontName), DEFAULT_DIALOG_FONT_WARP3);
        
        // Set the background colour of the demo area
        WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                        PP_BACKGROUNDCOLOR,(ULONG)sizeof(RGB), &thisPtr->rgbTBFlyBackground );
        // Set the foreground colour of the demo area
        WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                        PP_FOREGROUNDCOLOR,(ULONG)sizeof(RGB), &thisPtr->rgbTBFlyForeground );
        // Set the font of the demo area
        WinSetPresParam(WinWindowFromID(hwnd, IDST_TEXTDEMOFIELD),
                        PP_FONTNAMESIZE,(ULONG)sizeof(thisPtr->chrTBFlyFontName),
                        thisPtr->chrTBFlyFontName );
        
        WinCheckButton(hwnd, IDCB_ENABLETBFLYOVER, 1);
        WinEnableWindow(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),TRUE);
        
        // Set the value for delay
        WinSendMsg(WinWindowFromID(hwnd, IDSB_TBFLYOVERDELAY),
                   SPBM_SETCURRENTVALUE,
                   (MPARAM)DEFAULTDELAY,
                   (MPARAM)NULL);
      }
      break;
    case IDPB_COLORPALETTE:
      /* Colour... button was clicked */
      // Open the colorpalette 
      if((hObject=WinQueryObject("<WP_HIRESCLRPAL>"))!=NULL) {
        WinOpenObject(hObject,OPEN_DEFAULT,TRUE);
      }
      else {//Error, can't open the palette
        /*  Show an error msg.						   */
        /*
          Text:   "Can't open the color palette. It may have lost it's ID. Check your INI files."
          Title: "CD-Creator settings"                       
          */             
        messageBox( text, IDSTRS_NOCOLORPALETTE , sizeof(text),
                    title, IDSTRS_MEDIAFLDRSETTINGS , sizeof(title),
                    queryResModuleHandle(), hwnd, MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);
    
      }
      break;
    case IDPB_FONTPALETTE:
      /* Font... button was clicked */
      // Open the fontpalette 
      if((hObject=WinQueryObject("<WP_FNTPAL>"))!=NULL) {
        WinOpenObject(hObject,OPEN_DEFAULT,TRUE);
      }
      else {//Error, can't open the palette
        /*  Show an error msg.						   */
        /*
          Text:   "Can't open the font palette. It may have lost it's ID. Check your INI files."
          Title: "CD-Creator settings"                       
          */             
        messageBox( text, IDSTRS_NOFONTPALETTE , sizeof(text),
                    title, IDSTRS_MEDIAFLDRSETTINGS , sizeof(title),
                    queryResModuleHandle(), hwnd, MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);
      }
      break;
    default:
      break;
    }
    /* Don't call WinDefDlgProc here, or the dialog gets closed */
    return (MRESULT) TRUE;
#endif
  default:
    break;
  }
  // The WinDefDlgProc() handles the rest of the messages
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


MRESULT EXPENTRY shadowProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{
  switch (msg)
    {
    case WM_PAINT:
      {
        HPS hps;
        HPS hpsMem;
        BITMAPINFOHEADER2 bmpIH2;
        PBITMAPINFO2 pbmp2;
        char * chrBuff;
        PBYTE ptr;
        HDC hdcMem;
        HBITMAP hbm;
        ULONG ulCx, ulCy;
        SWP swp;

        hps=WinBeginPaint(hwnd, NULLHANDLE, NULLHANDLE/*&rectl*/);

        GpiCreateLogColorTable(hps, 0, LCOLF_RGB, 0, 0, NULL);
        WinQueryWindowPos(hwnd, &swp);

        ulCx=swp.cx;
        ulCy=swp.cy;

        bmpIH2.cbFix=sizeof(BITMAPINFOHEADER2);
        bmpIH2.cx=ulCx;
        bmpIH2.cy=ulCy;
        bmpIH2.cPlanes=1;
        bmpIH2.cBitCount=8;
        bmpIH2.cbImage=(((ulCx*(1<<bmpIH2.cPlanes)*(1<<bmpIH2.cBitCount))+31)/32)*bmpIH2.cy;

        chrBuff=(char*)malloc(bmpIH2.cbImage+sizeof(BITMAPINFO2)+256*sizeof(RGB2));
        pbmp2=(PBITMAPINFO2)chrBuff;
        memset(pbmp2, 0, sizeof(BITMAPINFO2)+256*sizeof(RGB2));
        ptr=chrBuff+sizeof(BITMAPINFO2)+256*sizeof(RGB2);

        pbmp2->cbFix=sizeof(BITMAPINFO2);
        pbmp2->cx=ulCx;
        pbmp2->cy=ulCy;
        pbmp2->cPlanes=1;
        pbmp2->cBitCount=8;
        pbmp2->cbImage=((pbmp2->cx+31)/32)*pbmp2->cy;
        pbmp2->ulCompression=BCA_UNCOMP;
        pbmp2->ulColorEncoding=BCE_RGB;

        hdcMem=DevOpenDC(WinQueryAnchorBlock(hwnd),OD_MEMORY,"*", 0L/*4L*/, (PDEVOPENDATA)NULLHANDLE/*pszData*/, NULLHANDLE);
        if(hdcMem) {
          SIZEL sizel= {0,0};

          hpsMem=GpiCreatePS(WinQueryAnchorBlock(hwnd), hdcMem, &sizel, PU_PELS|GPIT_MICRO|GPIA_ASSOC);
          if(hpsMem)
            {                 
              hbm=GpiCreateBitmap(hpsMem, &bmpIH2, FALSE, NULL, pbmp2);
              if(hbm) {
                HPS hpsDesktop;
                POINTL ptl[3]={0};
                RGB2 *prgb2;
                int a, r,g,b;
 
                hpsDesktop=WinGetScreenPS(HWND_DESKTOP);
                GpiSetBitmap(hpsMem, hbm);

                ptl[0].x=0;
                ptl[0].y=0;
                ptl[1].x=0+ulCx;
                ptl[1].y=0+ulCy;

                ptl[2].x=swp.x;
                ptl[2].y=swp.y;

                if(GpiBitBlt(hpsMem, hpsDesktop, 3, ptl , ROP_SRCCOPY, BBO_IGNORE)==GPI_ERROR)
                  {
                  }

                WinReleasePS(hpsDesktop);

                if(GpiQueryBitmapBits(hpsMem, 0, ulCy, ptr, pbmp2)==GPI_ALTERROR)
                  {
                    //   DosBeep(500,300);
                  }

                prgb2=(RGB2*)(++pbmp2);
                for(a=0;a<256; a++, prgb2++) {
                  r=-50;
                  g=-50;
                  b=-50;
                  
                  b+=prgb2->bBlue;
                  g+=prgb2->bGreen;
                  r+=prgb2->bRed;
                  if(r>255)
                    r=255;
                  if(r<0)
                    r=0;
                  prgb2->bRed=r;

                  if(g>255)
                    g=255;
                  if(g<0)
                    g=0;
                  prgb2->bGreen=g;

                  if(b>255)
                    b=255;
                  if(b<0)
                    b=0;
                  prgb2->bBlue=b;        
                }

                if(GpiSetBitmapBits(hpsMem, 0, ulCy, ptr, --pbmp2)!=GPI_ALTERROR)
                  {
                    ptl[0].x=0;
                    ptl[0].y=0;
                    ptl[1].x=ulCx;
                    ptl[1].y=ulCy;
                    ptl[2].x=0;
                    ptl[2].y=0;
                    
                    GpiBitBlt(hps, hpsMem, 3, ptl , ROP_SRCCOPY, BBO_IGNORE);
                  }
                GpiSetBitmap(hpsMem, NULLHANDLE);
                GpiDeleteBitmap(hbm);
              }/* hbm */
              GpiDestroyPS(hpsMem);
            }/* hpsMem */
          DevCloseDC(hdcMem);
        }/* if(hdcMem) */
        WinEndPaint(hps);
        free(chrBuff);
        return MRFALSE;
      }
    default:
      break;
    }
  return (*oldProc)(hwnd,msg,mp1,mp2);	
}


/*****************************************************************************/
/*  New button procedure with fly over help	                                 */
/*****************************************************************************/
MRESULT EXPENTRY newButtonProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{
  static BOOL bBubbleOn=TRUE;
  POINTL ptl;
  HPS  hps;
  FONTMETRICS   fm;
  LONG  ulWinTextLen;
  POINTL aptlPoints[TXTBOX_COUNT];
  RECTL   rcl;
  LONG   deltaX,deltaY;
  RGB    rgb= {200,200,0};
  static USHORT id=0;//Initialisation new in V1.00a 
  RECTL  rclWork;
  HWND hwndStore;
  WPObject * wpObject;

  switch (msg)
    {
    case WM_DESTROY:
      WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd,1);//Stop timer if running
      if(hwndBubbleWindow) WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
      hwndBubbleWindow=0;
      /* Stop delay timer if running */
      WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd,WinQueryWindowUShort(hwnd,QWS_ID));			
      break;
    case WM_NEWBUBBLE:
      ULONG bubbleEnabled;
      HWND hwndStore;
      /*  we have to build a new information window  */
      if(hwndBubbleWindow){// if(){...} new in V1.00a 
        WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
        hwndBubbleWindow=NULL;
      }
      // Query the pointer position
      WinQueryPointerPos(HWND_DESKTOP,&ptl);
      WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
      WinQueryWindowRect(hwnd,&rclWork);				
      if(!hwndBubbleWindow 
         && WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl)
         && bTBFlyOverEnabled) {

        static HWND hwndBubbleClient;
	ULONG style=FCF_BORDER|FCF_NOBYTEALIGN;
	char winText[255];
	
	/* Get window text (It's the object title) for size calculating */
	WinQueryWindowText(hwnd,sizeof(winText),winText);
	ulWinTextLen=(LONG)strlen(winText); // Query text length
	
	/* Delete 'Returns' in object title */
	char *pBuchst;
    char *pRest;
    pRest=winText;
    while((pBuchst=strchr(pRest,13))!=NULL) {
      *pBuchst=' ';
      pBuchst++;
      if(*pBuchst==10)
        *pBuchst=' ';
      pRest=pBuchst;
    }

	/* Create help window */
	hwndBubbleWindow=WinCreateStdWindow(HWND_DESKTOP,
					    0,
					    &style,
					    WC_STATIC,
					    "",
                        SS_TEXT|DT_CENTER|DT_VCENTER,
					    NULLHANDLE,
					    400,
					    &hwndBubbleClient);
    hwndShadow=WinCreateWindow(HWND_DESKTOP,
                               WC_STATIC,
                               "",
                               SS_TEXT|DT_CENTER|DT_VCENTER,
                               0, 0,
                               0, 0,
                               hwndBubbleWindow,
                               hwndBubbleWindow,
                               401,
                               NULLHANDLE,
                               NULLHANDLE);
    oldProc=WinSubclassWindow(hwndShadow, shadowProc);

	// Set the font for the help
	WinSetPresParam(hwndBubbleClient,PP_FONTNAMESIZE,
                    sizeof(chrTBFlyFontName),
                    chrTBFlyFontName);
	/* Calculate text size in pixel */
	hps=WinBeginPaint(hwndBubbleClient,(HPS)NULL,(PRECTL)NULL);
	GpiQueryTextBox(hps,ulWinTextLen,winText,TXTBOX_COUNT,aptlPoints);
	WinEndPaint(hps);
	
	/* Set colors */
    WinSetPresParam(hwndBubbleClient,
                    PP_BACKGROUNDCOLOR,sizeof(rgbTBFlyBackground) ,
                    &rgbTBFlyBackground );
	WinSetPresParam(hwndBubbleClient,
                    PP_FOREGROUNDCOLOR,sizeof(rgbTBFlyForeground) ,
                    &rgbTBFlyForeground );

	/* Calculate bubble positon and show bubble */
	WinQueryPointerPos(HWND_DESKTOP,&ptl);//Query pointer position in the desktop window
	WinQueryWindowRect(HWND_DESKTOP,&rcl);//Query desktop size
	aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+7+xVal+ptl.x 
	  > rcl.xRight 
	    ? deltaX=-aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x-xVal-xVal-7 
	      : deltaX=0 ;

	aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2+yVal+ptl.y 
	  > rcl.yTop 
	    ? deltaY=-aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y-2*yVal-7
	      : deltaY=0 ;		
	WinSetWindowPos(hwndBubbleWindow,
			HWND_TOP,
			ptl.x+xVal+deltaX,ptl.y+yVal+deltaY,  
			aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+8,
			aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2,
			SWP_ZORDER|SWP_SIZE|SWP_MOVE|SWP_SHOW);

    WinSetWindowPos(hwndShadow,
                    hwndBubbleWindow,
                    ptl.x+xVal+deltaX+5
                    ,ptl.y+yVal+deltaY-5,  
                    aptlPoints[TXTBOX_BOTTOMRIGHT].x-aptlPoints[TXTBOX_BOTTOMLEFT].x+8,
                    aptlPoints[TXTBOX_TOPLEFT].y-aptlPoints[TXTBOX_BOTTOMLEFT].y+2,
                    SWP_ZORDER|SWP_SIZE|SWP_MOVE|SWP_SHOW);

	/* Set bubble text */
	WinSetWindowText(hwndBubbleClient,winText);
	WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,1,35); 
      } // end if(!hwndBubbleWindow)
      break;
    case WM_MOUSEMOVE:
      USHORT  tempID;
      ULONG delayValue;
      delayValue=250;
   
      tempID=WinQueryWindowUShort(hwnd,QWS_ID);/*  get the id of the window under the pointer  */  			
      if(id!=tempID) {	// New Button?	
        WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,tempID,(ULONG)iTBFlyOverDelay); // New timer for delay
        id=tempID;  // Save ID 
      }
      else {
        if(!hwndBubbleWindow)WinStartTimer(WinQueryAnchorBlock(hwnd),hwnd,tempID,(ULONG)iTBFlyOverDelay); // New timer for delay	
      }			
      break;
    case WM_TIMER:			
      switch (SHORT1FROMMP(mp1))
        {
        case 1: //Intervall timer
          /* Test pointer position */
          WinQueryPointerPos(HWND_DESKTOP,&ptl);
          WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
          WinQueryWindowRect(hwnd,&rclWork);
          if(!WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl))
            {	// Button has changed				 
              WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd,1);  // stop the running timer
              if(hwndBubbleWindow) WinDestroyWindow(hwndBubbleWindow);/*  close the bubblewindow  */
              hwndBubbleWindow=0;
              id=0;
            }			 			
          break;
        default:// delay over
          if (SHORT1FROMMP(mp1)==WinQueryWindowUShort(hwnd,QWS_ID)) {//our own timer. It has got the window id
            WinStopTimer(WinQueryAnchorBlock(hwnd),hwnd,SHORT1FROMMP(mp1));//Stop the delay timer
            /* Check the pointer position */
            WinQueryPointerPos(HWND_DESKTOP,&ptl);
            WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
            WinQueryWindowRect(hwnd,&rclWork);
            if(WinPtInRect(WinQueryAnchorBlock(hwnd),&rclWork,&ptl))
              WinPostMsg(hwnd,WM_NEWBUBBLE,NULL,NULL);//Request a help window
            return (MRESULT)FALSE;
          }
          break;
        }
      break;

    }
  // call the original button procedure to handle the rest of the messages
  return (*oldButtonProc2)(hwnd,msg,mp1,mp2);	
};

