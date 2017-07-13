/*
 * pmmp3dec.c (C) Chris Wohlgemuth 2002
 *
 * This helper decodes an MP3
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
#define INCL_DOS
#define INCL_GPI
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_MCIOS2
#define INCL_SECONDARYWINDOW    
#define INCL_GRAPHICBUTTON 
#define INCL_GPIBITMAPS
#define INCL_REXXSAA
#define INCL_WINTRACKRECT
#define INCL_PM


#include <os2.h>
#include <sw.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
//#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include "os2me.h"
#include "progbars.h"
#include "common.h"
#include "volumeres.h"

#define ACTION_SETVOLUME     1L
#define ACTION_QUERYVOLUME   2L

#define INI_NAME_APP        "volume"
#define INI_WINDOWPOS_KEY   "windowpos"
#define INI_TBVISIBLE_KEY   "tbVivible"
#define INI_NAME_COLOR        "color"
#define INI_BTNBGCLR_KEY       "btnbg"
#define INI_BTNFGCLR_KEY       "btnfg"

#if 0
#define INI_BGCLR_KEY       "bg"
#define INI_FGCLR_KEY       "fg"
#define INI_ACTIVETBBGCLR_KEY       "activetbbg"
#define INI_INACTIVETBBGCLR_KEY       "inactivetbbg"
#define INI_ACTIVETBFGCLR_KEY       "activetbfg"
#define INI_INACTIVETBFGCLR_KEY       "inactivetbfg"
#endif
char logName[]="volume.log";

BOOL bHaveWindowPos=FALSE;

BOOL bMute=FALSE; 
HMODULE RESSOURCEHANDLE=0;
BOOL bTBVisible;
ULONG ulTBCy;

HWND hwndThread;

char* params[5];
char chrIniFile[CCHMAXPATH];

ULONG cwQueryOSRelease();
void pmUsage();
BOOL writeWindowPosToIni(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL restoreWindowPosFromIni(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL writeIntToIni(char * iniFile, char* chrApp, char *chrKey, int theInt);
int queryIntFromIni(char * iniFile, char* chrApp, char *chrKey, int defaultInt);
BOOL writeDataToIni(char * iniFile, char* chrApp, char *chrKey, void* theData, ULONG ulSize);
BOOL queryDataFromIni(char * iniFile, char* chrApp, char *chrKey, void * theData, ULONG* ulMaxSize);
BOOL saveWindowClrsToIni(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL restoreWindowClrsFromIni(char * chrIniFile, char* chrApp , HWND hwnd);

void HlpWriteToTrapLog(const char* chrFormat, ...);

#define EXCEPTION_LOGFILE_NAME "d:\\arbeitsoberflaeche\\tst.log"


HAB globalHab;
HBITMAP hBitmap;
BITMAPINFOHEADER2 bmpInfoHeader2;
 
POINTS ptsStart={0};
POINTS ptsTemp={0};
BOOL bTrack=FALSE;
BOOL bHaveSelection=FALSE;

PFNWP oldButtonProc;

 
BOOL MyTrackRoutine(HWND hwnd, PRECTL rcl, ULONG ulFlag)
 {
   TRACKINFO track;
   HAB hab;
   HPS hps;


   track.cxBorder = 2;
   track.cyBorder = 2;  /* 4 pel wide lines used for rectangle */
   track.cxGrid = 1;
   track.cyGrid = 1;    /* smooth tracking with mouse */
   track.cxKeyboard = 8;
   track.cyKeyboard = 8; /* faster tracking using cursor keys */
 
   hab=WinQueryAnchorBlock(hwnd);
   hps=WinGetPS(hwnd);

   WinCopyRect(hab, &track.rclTrack, rcl);   /* starting point */

   WinQueryWindowRect(hwnd, &track.rclBoundary);
 
   //   WinSetRect(hab, &track.rclBoundary, 0, 0, 640, 480); /* bounding rectangle */
 
   track.ptlMinTrackSize.x = 10;
   track.ptlMinTrackSize.y = 10;  /* set smallest allowed size of rectangle */
   // track.ptlMaxTrackSize.x = 200;
   // track.ptlMaxTrackSize.y = 200; /* set largest allowed size of rectangle */
 
   track.ptlMaxTrackSize.x = track.rclBoundary.xRight;
   track.ptlMaxTrackSize.y = track.rclBoundary.yTop; /* set largest allowed size of rectangle */
 
   track.fs = ulFlag;
   track.fs|=TF_ALLINBOUNDARY;
   if (WinTrackRect(hwnd, hps, &track) )
   {
     /* if successful copy final position back */
     WinCopyRect(hab, rcl, &track.rclTrack);
     WinReleasePS(hps);
     return(TRUE);
   }
   else
   {
     WinReleasePS(hps);
     return(FALSE);
   }
 }


ULONG chkPointerInSelection( HWND hwnd, POINTS ptsStart, POINTS ptsTemp, SHORT xPtr, SHORT yPtr)
{
  RECTL rcl;
  RECTL rclTemp;
  HAB hab;
  POINTL ptl;

  ptl.x=xPtr;
  ptl.y=yPtr;

  if(ptsTemp.x>ptsStart.x)
    {
      rcl.xLeft=ptsStart.x;
      rcl.xRight=ptsTemp.x;
    }
  else
    {
      rcl.xLeft=ptsTemp.x;
      rcl.xRight=ptsStart.x;
    }
  if(ptsTemp.y>ptsStart.y)
    {
      rcl.yBottom=ptsStart.y;
      rcl.yTop=ptsTemp.y;
    }
  else
    {
      rcl.yBottom=ptsTemp.y;
      rcl.yTop=ptsStart.y;
    }

  hab=WinQueryAnchorBlock(hwnd);

  rclTemp=rcl;
  WinInflateRect(hab, &rclTemp, 2, 2);
  if(!WinPtInRect(hab, &rclTemp, &ptl))
    return 0; /* Mouse out of selection */

  rclTemp=rcl;
  WinInflateRect(hab, &rclTemp, -2, -2);
  if(WinPtInRect(hab, &rclTemp, &ptl))
    return 1; /* Mouse somewhere in the center of the selection */

  /* We are over the border */
  if(yPtr>rclTemp.yBottom && yPtr<rclTemp.yTop)
    {
      /* One of the vertical borders */
      WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_SIZEWE, FALSE));
      if(xPtr>rclTemp.xLeft)
        return 2;
      else
        return 3;
    }
  else if((xPtr>rclTemp.xLeft && xPtr<rclTemp.xRight))
    {
      /* One of the horizontal borders */
      WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_SIZENS, FALSE));
      if(yPtr>rclTemp.yBottom)
        return 4;
      else
        return 5;
    }

  return 0;
}

BOOL drawSelection(HWND hwnd, POINTS ptsStart, POINTS ptsTemp)
{
  RECTL rcl;
  HPS hps;

  hps=WinGetPS(hwnd);
  if(!hps)
    return FALSE;

  if(ptsTemp.x>ptsStart.x)
    {
      rcl.xLeft=ptsStart.x;
      rcl.xRight=ptsTemp.x;
    }
  else
    {
      rcl.xLeft=ptsTemp.x;
      rcl.xRight=ptsStart.x;
    }
  if(ptsTemp.y>ptsStart.y)
    {
      rcl.yBottom=ptsStart.y;
      rcl.yTop=ptsTemp.y;
    }
  else
    {
      rcl.yBottom=ptsTemp.y;
      rcl.yTop=ptsStart.y;
    }
  WinDrawBorder(hps, &rcl, 1, 1, 0,0, DB_PATINVERT);
  WinReleasePS(hps);

  return TRUE;
}

BOOL drawSelection2(HPS hps, POINTS ptsStart, POINTS ptsTemp)
{
  RECTL rcl;

  if(ptsTemp.x>ptsStart.x)
    {
      rcl.xLeft=ptsStart.x;
      rcl.xRight=ptsTemp.x;
    }
  else
    {
      rcl.xLeft=ptsTemp.x;
      rcl.xRight=ptsStart.x;
    }
  if(ptsTemp.y>ptsStart.y)
    {
      rcl.yBottom=ptsStart.y;
      rcl.yTop=ptsTemp.y;
    }
  else
    {
      rcl.yBottom=ptsTemp.y;
      rcl.yTop=ptsStart.y;
    }
  WinDrawBorder(hps, &rcl, 1, 1, 0,0, DB_PATINVERT);

  return TRUE;
}

BOOL rectlFrom2Points(RECTL *rcl, POINTS ptsStart, POINTS ptsTemp)
{
  HPS hps;

  if(ptsTemp.x>ptsStart.x)
    {
      rcl->xLeft=ptsStart.x;
      rcl->xRight=ptsTemp.x;
    }
  else
    {
      rcl->xLeft=ptsTemp.x;
      rcl->xRight=ptsStart.x;
    }
  if(ptsTemp.y>ptsStart.y)
    {
      rcl->yBottom=ptsStart.y;
      rcl->yTop=ptsTemp.y;
    }
  else
    {
      rcl->yBottom=ptsTemp.y;
      rcl->yTop=ptsStart.y;
    }

  return TRUE;
}

MRESULT EXPENTRY staticProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{
  switch (msg)
    {
    case WM_PAINT:
      {
        HPS hps;
        RECTL rectl, rcl;
        ULONG ulWidth, ulHeight, ulWidthWindow, ulHeightWindow, ulTemp, ulFact;
        ULONG ulBG;

        WinQueryUpdateRect(hwnd, &rcl);

        hps=WinBeginPaint(hwnd, NULLHANDLE, NULLHANDLE/*&rectl*/);
        ulHeight = bmpInfoHeader2.cy;
        ulWidth = bmpInfoHeader2.cx;

        WinFillRect(hps, &rcl, CLR_WHITE);

        WinQueryWindowRect(hwnd, &rectl);

        ulWidthWindow=rectl.xRight;
        ulHeightWindow=rectl.yTop;

        ulFact=ulWidth/ulWidthWindow>ulHeight/ulHeightWindow ? ulWidth*1000/ulWidthWindow : ulHeight*1000/ulHeightWindow;
        ulWidth=ulWidth*1000/ulFact;
        ulHeight=ulHeight*1000/ulFact;
        rectl.xRight=ulWidth;
        rectl.yTop=ulHeight;
        WinDrawBitmap(hps, hBitmap, NULLHANDLE, (PPOINTL) &rectl, 0, 0, DBM_STRETCH);
        drawSelection2(hps, ptsStart, ptsTemp);
        WinEndPaint(hps);
        return MRFALSE;
      }
    case WM_MOUSEMOVE:
      {
        HPS hps;
        RECTL rcl;
        SHORT x, y;
DosBeep(5000, 10);
        if(bTrack) {          
          hps=WinGetPS(hwnd);
          if(hps)
            {
              POINTS pts;

              drawSelection(hwnd, ptsStart, ptsTemp);

              pts.x=SHORT1FROMMP(mp1);
              pts.y=SHORT2FROMMP(mp1);
              drawSelection(hwnd, ptsStart, pts);

              ptsTemp.x=pts.x;
              ptsTemp.y=pts.y;

              bHaveSelection=TRUE;

            }
          WinReleasePS(hps);
        }
        else if(bHaveSelection) {
          /* Check if mouse is over selection */
          RECTL rcl;
                  
          switch(chkPointerInSelection( hwnd, ptsStart, ptsTemp, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)))
            {
            case 2:
            case 3:
            case 4:
            case 5:
              return MRFALSE;
            default:
              break;
            }
        }
        break;
      }
    case WM_BUTTON1UP:
      bTrack=FALSE;
      break;
    case WM_BUTTON2MOTIONSTART:
      {
        if(bHaveSelection) {
          RECTL rcl;
          
          switch(chkPointerInSelection( hwnd, ptsStart, ptsTemp, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)))
            {
            case 1:
              rectlFrom2Points(&rcl, ptsStart, ptsTemp);
              //              HlpWriteToTrapLog("  rcl->yTop: %d yBottom: %d\n", rcl.yTop, rcl.yBottom );
              //              MyTrackRoutine(hwnd, &rcl, TF_MOVE);
              ptsStart.x=rcl.xLeft;
              ptsStart.y=rcl.yBottom;
              ptsTemp.x=rcl.xRight;
              ptsTemp.y=rcl.yTop;
              break;
            case 2:
              break;
            default:
              break;
            }
          //          drawSelection(hwnd, ptsStart, ptsTemp);
        }
        break;
      }
    case WM_BUTTON1MOTIONSTART:
      {
        if(bHaveSelection) {
          RECTL rcl;

          rectlFrom2Points(&rcl, ptsStart, ptsTemp);

          switch(chkPointerInSelection( hwnd, ptsStart, ptsTemp, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)))
            {
            case 2:
              MyTrackRoutine(hwnd, &rcl, TF_RIGHT);
              break;
            case 3:
              MyTrackRoutine(hwnd, &rcl, TF_LEFT);
              break;
            case 4:
              MyTrackRoutine(hwnd, &rcl, TF_TOP);
              break;
            case 5:
              MyTrackRoutine(hwnd, &rcl, TF_BOTTOM);
              break;
            default:
              /* Clear previous selection*/
              drawSelection(hwnd, ptsStart, ptsTemp);
              ptsStart.x=SHORT1FROMMP(mp1);
              ptsStart.y=SHORT2FROMMP(mp1);
              
              ptsTemp=ptsStart;
              bTrack=TRUE;
              return MRFALSE;
            }

          ptsStart.x=rcl.xLeft;
          ptsStart.y=rcl.yBottom;
          ptsTemp.x=rcl.xRight;
          ptsTemp.y=rcl.yTop;
          
          return MRFALSE;
        }
        ptsStart.x=SHORT1FROMMP(mp1);
        ptsStart.y=SHORT2FROMMP(mp1);
        
        ptsTemp=ptsStart;
        bTrack=TRUE;
        return MRFALSE;
      }
    default:
      break;
    }
  return WinDefWindowProc(hwnd,msg,mp1,mp2);	

    }

static void contextMenu(HWND hwnd)
{
  POINTL ptl;
  HWND   hwndPopup;
  HWND hwndTitleBar;
  BOOL bVisible;

  if((hwndTitleBar=WinWindowFromID(hwnd, FID_TITLEBAR))==NULLHANDLE)
    return;
      
  hwndPopup = WinLoadMenu(hwnd, NULLHANDLE, IDM_POPUP);
  if (hwndPopup == NULLHANDLE)
    return;
  
  bVisible=WinIsWindowVisible(hwndTitleBar);

  if(!bVisible)
    WinSendMsg(hwndPopup,MM_SETITEMATTR,MPFROM2SHORT(IDM_ITEMTITLEBAR,FALSE), 
               MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED));

  WinQueryPointerPos(HWND_DESKTOP, &ptl) ;
  WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
  WinPopupMenu(hwnd, hwnd, hwndPopup, 
               ptl.x, ptl.y, IDM_ITEMEXIT, PU_HCONSTRAIN | PU_VCONSTRAIN |
               PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE  );
}


static BOOL toggleTitleBar(HWND hwnd)
{
  HWND hwndTitleBar;

  if((hwndTitleBar=WinWindowFromID(hwnd, FID_TITLEBAR))!=NULLHANDLE)
    {
      SWP swp, swpFrame;

      if(!WinQueryWindowPos(hwndTitleBar,&swp))
        return FALSE;
      
      if(!WinQueryWindowPos(hwnd,&swpFrame))
        return FALSE;
      
      /* Shrink or expand the frame */
      if(!bTBVisible) {
        WinSetWindowPos(hwnd, NULLHANDLE, swpFrame.x, swpFrame.y,//-swp.cy, 
                        swpFrame.cx, swpFrame.cy+swp.cy, SWP_SIZE|SWP_MOVE);
        bTBVisible=TRUE;
      }
      else
        {
          WinSetWindowPos(hwnd, NULLHANDLE, swpFrame.x, swpFrame.y,//+swp.cy, 
                          swpFrame.cx, swpFrame.cy-swp.cy, SWP_SIZE|SWP_MOVE);
          bTBVisible=FALSE;
        }
    }
  WinSendMsg(hwnd,WM_UPDATEFRAME,0,0);      
}



/* This function is undocumented */
#pragma import(WinBuildPtrHandle,,"PMMERGE", 5117)

HPOINTER WinBuildPtrHandle(void* pIconData);

BOOL fileGetBinFileSize(char *chrFileName, ULONG* ulFileSize)
{
  FILE *file;
  struct stat buf;

  if((file=fopen(chrFileName, "rb"))==NULLHANDLE)
    return FALSE;

  if(fstat(fileno(file), &buf)!=0) {
    fclose(file);
    return FALSE;
  }

  fclose(file);
  *ulFileSize=buf.st_size;

  return TRUE;

}

HPOINTER createPointerFromFile(char *chrIconFile)
{
}
#if 0
BOOL MyTrackRoutine(HWND hwnd, PRECTL rcl, ULONG ulFlag)
{
  TRACKINFO track;
  HAB hab;
  HPS hps;
  
  
  track.cxBorder = 2;
  track.cyBorder = 2;  /* 4 pel wide lines used for rectangle */
  track.cxGrid = 1;
  track.cyGrid = 1;    /* smooth tracking with mouse */
  track.cxKeyboard = 8;
  track.cyKeyboard = 8; /* faster tracking using cursor keys */
  
  hab=WinQueryAnchorBlock(hwnd);
  hps=WinGetPS(WinWindowFromID(hwnd, QW_PARENT));
  
  WinCopyRect(hab, &track.rclTrack, rcl);   /* starting point */
  
  WinQueryWindowRect(WinWindowFromID(hwnd, QW_PARENT), &track.rclBoundary);
  
  //   WinSetRect(hab, &track.rclBoundary, 0, 0, 640, 480); /* bounding rectangle */
  
  track.ptlMinTrackSize.x = 2;
  track.ptlMinTrackSize.y = 2;  /* set smallest allowed size of rectangle */
  // track.ptlMaxTrackSize.x = 200;
  // track.ptlMaxTrackSize.y = 200; /* set largest allowed size of rectangle */
  
  track.ptlMaxTrackSize.x = track.rclBoundary.xRight;
  track.ptlMaxTrackSize.y = track.rclBoundary.yTop; /* set largest allowed size of rectangle */
  
  track.fs = ulFlag;
  //track.fs|=TF_ALLINBOUNDARY;

  if (WinTrackRect(hwnd, hps, &track) )
    {
DosBeep(5000,20);
      /* if successful copy final position back */
      WinCopyRect(hab, rcl, &track.rclTrack);
      WinReleasePS(hps);
      return(TRUE);
    }
  else
    {
      WinReleasePS(hps);
      return(FALSE);
    }
}
#endif
          
BOOL bDrag=FALSE;

MRESULT EXPENTRY buttonProc(HWND hwnd, ULONG msg,MPARAM mp1,MPARAM mp2 )
{

  switch (msg)
    {
#if 0
    case WM_MOUSEMOVE:
      {
        HPS hps;
        RECTL rcl;
        SHORT x, y;

DosBeep(5000, 20);
        if(bDrag) {          
          hps=WinGetPS(hwnd);
          if(hps)
            {
              POINTS pts;

              //  drawSelection(hwnd, ptsStart, ptsTemp);

              pts.x=SHORT1FROMMP(mp1);
              pts.y=SHORT2FROMMP(mp1);
              //     drawSelection(hwnd, ptsStart, pts);

              ptsTemp.x=pts.x;
              ptsTemp.y=pts.y;

              bHaveSelection=TRUE;

            }
          WinReleasePS(hps);
        }


        break;
      }

    case WM_BUTTON1UP:
      bDrag=FALSE;
      break;
#endif
#if 0
    case WM_BUTTON2MOTIONSTART:
      {
        if(bHaveSelection) {
          RECTL rcl;
          
          switch(chkPointerInSelection( hwnd, ptsStart, ptsTemp, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)))
            {
            case 1:
              rectlFrom2Points(&rcl, ptsStart, ptsTemp);
              //              HlpWriteToTrapLog("  rcl->yTop: %d yBottom: %d\n", rcl.yTop, rcl.yBottom );
              //              MyTrackRoutine(hwnd, &rcl, TF_MOVE);
              ptsStart.x=rcl.xLeft;
              ptsStart.y=rcl.yBottom;
              ptsTemp.x=rcl.xRight;
              ptsTemp.y=rcl.yTop;
              break;
            case 2:
              break;
            default:
              break;
            }
          //          drawSelection(hwnd, ptsStart, ptsTemp);
        }
        break;
      }
#endif

#if 0
    case WM_BUTTON1MOTIONSTART:
      {
        if(1) {
          RECTL rcl;
          rectlFrom2Points(&rcl, ptsStart, ptsTemp);
          WinQueryWindowRect(hwnd, &rcl);
          MyTrackRoutine(hwnd, &rcl, TF_MOVE);
#if 0
          switch(chkPointerInSelection( hwnd, ptsStart, ptsTemp, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1)))
            {
            case 2:
              MyTrackRoutine(hwnd, &rcl, TF_RIGHT);
              break;
            case 3:
              MyTrackRoutine(hwnd, &rcl, TF_LEFT);
              break;
            case 4:
              MyTrackRoutine(hwnd, &rcl, TF_TOP);
              break;
            case 5:
              MyTrackRoutine(hwnd, &rcl, TF_BOTTOM);
              break;
            default:
              /* Clear previous selection*/
              drawSelection(hwnd, ptsStart, ptsTemp);
              ptsStart.x=SHORT1FROMMP(mp1);
              ptsStart.y=SHORT2FROMMP(mp1);
              
              ptsTemp=ptsStart;
              bTrack=TRUE;
              return MRFALSE;
            }
#endif
          return MRFALSE;
        }
        return MRFALSE;
      }
#endif
    default:
      break;
    }
  return (*oldButtonProc)(hwnd,msg,mp1,mp2);	
  
}

MRESULT EXPENTRY volumeDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  char text[CCHMAXPATH*4 +10];
  char title[CCHMAXPATH*4];
  SWCNTRL swctl;
  PID pid;
  int a;
  int iPercent;
  HPOINTER          hpProgramIcon;           /* handle to program's icon     */

  switch (msg)
    {      
    case WM_INITDLG:
      {
        RGB rgb;
        ULONG ulSize;

        /*
         * The slider control cannot be completely set from the dialog
         * template so some aspects must be set here.  We will set the
         * volume range to 0-100, increment to 1-10, and the initial
         * volume level to 75.
         */
        WinSendMsg( WinWindowFromID(hwnd, IDCS_VOLUME),
                    CSM_SETRANGE,
                    (MPARAM) 0L,
                    (MPARAM) 100L);
        
        WinSendMsg( WinWindowFromID(hwnd, IDCS_VOLUME),
                    CSM_SETINCREMENT,
                    (MPARAM) 10L,
                    (MPARAM) 1L);
        
        

        /* Set dialog font to WarpSans for Warp 4 and above */
        if(cwQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        
        hpProgramIcon =
          WinLoadPointer(
                         HWND_DESKTOP,
                         (HMODULE) NULL,              /* Resource is kept in .Exe file. */
                         ID_ICONVOLUME );                   /* Which icon to use.             */
        
        WinSendMsg(
                   hwnd,                    /* Dialog window handle.                 */
                   WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                   (MPARAM) hpProgramIcon,
                   (MPARAM) 0 );            /* mp2 no value.                         */
#if 0
        WinDefSecondaryWindowProc(
                                  hwnd,                    /* Dialog window handle.                 */
                                  WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                                  (MPARAM) hpProgramIcon,
                                  (MPARAM) 0 );            /* mp2 no value.                         */
#endif
        bTBVisible=queryIntFromIni(chrIniFile, INI_NAME_APP, INI_TBVISIBLE_KEY, 1);
        //      sprintf(text, ": %d", bTBVisible);
        //   WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_MOVEABLE);
        restoreWindowPosFromIni(chrIniFile, INI_NAME_APP, INI_WINDOWPOS_KEY, hwnd);
        restoreWindowClrsFromIni(chrIniFile, INI_NAME_COLOR , hwnd);

        ulSize=sizeof(RGB);
        if(queryDataFromIni(chrIniFile, INI_NAME_COLOR, INI_BTNBGCLR_KEY, &rgb, &ulSize))
          {
            // Set the background colour
            WinSetPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                            PP_BACKGROUNDCOLOR,(ULONG)sizeof(rgb), &rgb);
          }
        ulSize=sizeof(RGB);        
        if(queryDataFromIni(chrIniFile, INI_NAME_COLOR, INI_BTNFGCLR_KEY, &rgb, &ulSize))
          {
            // Set the foreground colour
            WinSetPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                            PP_FOREGROUNDCOLOR,(ULONG)sizeof(rgb), &rgb);
          }
        
        WinShowWindow( hwnd, TRUE );

        //        oldButtonProc=WinSubclassWindow(WinWindowFromID(hwnd, IDPB_DRAGBUTTON), buttonProc);
        WinSubclassWindow(WinWindowFromID(hwnd, IDPB_DRAGBUTTON), staticProc);
        globalHab=WinQueryAnchorBlock(hwnd);
       return (MRESULT) FALSE;
      }
    case WM_FORMATFRAME:
      {
        /* Remove top frame controls if titlebar is hidden */
        SHORT countSwp;
        SWP  *sav;
        
        countSwp = (int) WinDefDlgProc(hwnd, msg, mp1, mp2);
        if(countSwp) {
          if(!bTBVisible) { 
            /* Hide frame controls */        
            /* countSWP:  number of standard framecontrols
               pswp:      Array of SWP describing the framecontrols.
               */
            int a;
            PSWP pswpTB=NULLHANDLE;
            int y;            
            PSWP pswp=(PSWP)mp1;
            for (a = 0; a < countSwp; a++)
              {
                /* Find the titlebar window */
                if ( WinQueryWindowUShort( pswp[a].hwnd, QWS_ID ) == FID_TITLEBAR )
                  {
                    pswpTB=&pswp[a];
                    y=pswp[a].y;
                    break;
                  }
              }
            if(pswpTB) {
              /* Hide all frame controls at the top */
              for (a = 0; a < countSwp; a++)
                {
                  if ( pswp[a].y == y )
                    {
                      pswp[a].fl|=SWP_HIDE;
                      pswp[a].fl&=~SWP_SHOW;
                    }
                }/* for() */
            }
          }
        }
        return MRFROMSHORT(countSwp);
      }
    case WM_CONTEXTMENU:
      contextMenu(hwnd);
      return (MRESULT) FALSE;
    case WM_BUTTON1DBLCLK:
      {
        /* Remove frame controls (titlebar) when double clicking MB 1 */
        toggleTitleBar(hwnd);
        return (MRESULT) FALSE;
      }
    case WM_BUTTON2MOTIONSTART:
    case WM_BUTTON1MOTIONSTART:
      /* Drag the window without titlebar */
      WinSendMsg(hwnd, WM_TRACKFRAME, 
                 MPFROMSHORT(TF_MOVE   /*| TF_SETPOINTERPOS */), NULL) ;
      return (MRESULT) 0 ;
      case WM_CONTROL:
         if (SHORT1FROMMP(mp1)==IDCS_VOLUME)
         {

         }
         else if (SHORT1FROMMP(mp1)==IDPB_DRAGBUTTON)
           {
             DosBeep(1000, 10);

           }

         return( (MRESULT) 0);
    case WM_CLOSE:
      {
        MCI_MASTERAUDIO_PARMS masteraudioparms;
        ULONG  attrFound;
        RGB rgb;

        /* Save window position */
        writeWindowPosToIni(chrIniFile, INI_NAME_APP, INI_WINDOWPOS_KEY, hwnd);
        /* Save titlebar state */
        bTBVisible=WinIsWindowVisible(WinWindowFromID(hwnd, FID_TITLEBAR));
        writeIntToIni(chrIniFile, INI_NAME_APP, INI_TBVISIBLE_KEY, bTBVisible);
        /* Save colors */
        // Query the current button background colour
        if(WinQueryPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                             PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(rgb),
                             &rgb,QPF_NOINHERIT))
          {
            writeDataToIni(chrIniFile, INI_NAME_COLOR, INI_BTNBGCLR_KEY, &rgb, sizeof(RGB));
          }
        // Query the current button foreground colour
        if(WinQueryPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                             PP_FOREGROUNDCOLOR,0,&attrFound,sizeof(rgb),
                             &rgb,QPF_NOINHERIT))
          {
            writeDataToIni(chrIniFile, INI_NAME_COLOR, INI_BTNFGCLR_KEY, &rgb, sizeof(RGB));
          }
        saveWindowClrsToIni(chrIniFile, INI_NAME_COLOR , hwnd);
        WinPostMsg(hwnd, WM_QUIT, 0, 0);
        return (MRESULT )FALSE;
      }
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDPB_DRAGBUTTON:

          break;
        case IDM_ITEMTITLEBAR:

          break;
        case IDM_ITEMEXIT:
          WinPostMsg(hwnd, WM_CLOSE, 0, 0);/* Timer error so do a hard quit */
          break;
        default:
          break;
        }
      return (MRESULT) FALSE;
    case WM_PAINT:
      {
        HPS hps;
        MRESULT mrc;
        HPOINTER hPtr;

        mrc=WinDefDlgProc( hwnd, msg, mp1, mp2);
        hPtr=WinLoadPointer(HWND_DESKTOP, NULLHANDLE, ID_ICONVOLUME);
        hPtr=createPointerFromFile("H:\\WPS-Wizard\\res\\MWDAT.ICO");


        hps=WinGetPS(hwnd);

        WinDrawPointer(hps,20, 120, hPtr, DP_NORMAL);

        WinReleasePS(hps);
        return mrc;
      }
    default:
      break;
    }/* switch */
  //return (MRESULT) WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) ;
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}

static void buildIniFileName(char* chrProgname)
{
  char * chrPtr;  
  strcpy(chrIniFile, chrProgname);

  if((chrPtr=strrchr(chrIniFile,'\\'))==NULLHANDLE)
    return;

  *chrPtr=0;
  strcat(chrIniFile,"\\volume.ini");
  //  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrIniFile, "", 1234, MB_MOVEABLE);
}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  char text[CCHMAXPATH];
  char title[CCHMAXPATH];
  HWND hwndClient;
  ULONG rc;
  //  HPOINTER          hpProgramIcon;           /* handle to program's icon     */

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      RESSOURCEHANDLE=0;
      buildIniFileName(argv[0]);

      //      RexxRegisterFunctionExe("testFunc", rxFunc);

      if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, 
                     volumeDialogProc, RESSOURCEHANDLE, IDDLG_VOLUME, 0) == DID_ERROR )
        {
          /*
            Text:   
            Title:  "Installation problem"                       
            */             
          WinDestroyMsgQueue( hmq );
          WinTerminate( hab );
          DosBeep(100,600);
          return( 1 );
        }
#if 0
      hwndFrame=WinLoadSecondaryWindow(
                                       HWND_DESKTOP,           /* Parent of the dialog box.     */
                                       HWND_DESKTOP,           /* Owner of the dialog box.      */
                                       (PFNWP) volumeDialogProc,   /* */
                                       (HMODULE) RESSOURCEHANDLE,         /* */
                                       IDDLG_VOLUME,            /* Dialog ID.                   */
                                       (PVOID) NULL);          /* Creation      */
      hwndMainDialogBox = WinQuerySecondaryHWND(hwndFrame, QS_DIALOG);
      WinShowWindow( hwndFrame, TRUE );
      hpProgramIcon =
        WinLoadPointer(
                       HWND_DESKTOP,
                       (HMODULE) NULL,              /* Resource is kept in .Exe file. */
                       ID_ICONVOLUME );                   /* Which icon to use.             */
      WinSendMsg(
                 hwndFrame,                    /* Dialog window handle.                 */
                 WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                 (MPARAM) hpProgramIcon,
                 (MPARAM) 0 );            /* mp2 no value.                         */
      WinPostMsg( WinWindowFromID(hwndFrame, IDCS_VOLUME),
                  CSM_SETVALUE,
                  (MPARAM) queryMasterVolume(),
                  (MPARAM) NULL);
      while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
        WinDispatchMsg( hab, (PQMSG) &qmsg );  
#endif       
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}









