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
//#include "progbars.h"
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

ULONG SysQueryOSRelease();
void pmUsage();
BOOL writeWindowPosToIni(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL restoreWindowPosFromIni(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL writeIntToIni(char * iniFile, char* chrApp, char *chrKey, int theInt);
int queryIntFromIni(char * iniFile, char* chrApp, char *chrKey, int defaultInt);
BOOL writeDataToIni(char * iniFile, char* chrApp, char *chrKey, void* theData, ULONG ulSize);
BOOL queryDataFromIni(char * iniFile, char* chrApp, char *chrKey, void * theData, ULONG* ulMaxSize);
BOOL saveWindowClrsToIni(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL restoreWindowClrsFromIni(char * chrIniFile, char* chrApp , HWND hwnd);


PFNWP  oldButtonProc2;  //place for original button-procedure
PFNWP  oldProc;  
HWND hwndBubbleWindow;// The handle of the help window
HWND hwndShadow;// The handle of the help window
#define  WM_NEWBUBBLE   WM_USER+100 //Use ATOM later
#define xVal  12      //x-distance of Bubble
#define yVal  8      //y-distance of Bubble
char chrTBFlyFontName[CCHMAXPATH]="9.WarpSans";/* Font for toolbar fly over help */
RGB rgbTBFlyForeground={0};
RGB rgbTBFlyBackground={180,255,255};
BOOL bTBFlyOverEnabled=TRUE;
int iTBFlyOverDelay=250;

void SysWriteToTrapLog(const char* chrFormat, ...);

#define EXCEPTION_LOGFILE_NAME "d:\\arbeitsoberflaeche\\tst.log"


HAB globalHab;
HBITMAP hBitmap;
BITMAPINFOHEADER2 bmpInfoHeader2;

#define mmioFOURCC( ch0, ch1, ch2, ch3 )                         \
                  ( (ULONG)(BYTE)(ch0) | ( (ULONG)(BYTE)(ch1) << 8 ) |    \
                  ( (ULONG)(BYTE)(ch2) << 16 ) | ( (ULONG)(BYTE)(ch3) << 24 ) )

HBITMAP loadBitmap ( PSZ pszFileName , PBITMAPINFOHEADER2 pBMPInfoHeader2)
{
    HBITMAP       hbm;
    MMIOINFO      mmioinfo;
        MMFORMATINFO  mmFormatInfo;
    HMMIO         hmmio;
        ULONG         ulImageHeaderLength;
        MMIMAGEHEADER mmImgHdr;
    ULONG         ulBytesRead;
    ULONG         dwNumRowBytes;
    PBYTE         pRowBuffer;
    ULONG         dwRowCount;
    SIZEL         ImageSize;
    ULONG         dwHeight, dwWidth;
    SHORT          wBitCount;
    FOURCC        fccStorageSystem;
    ULONG         dwPadBytes;
    ULONG         dwRowBits;
    ULONG         ulReturnCode;
    ULONG         dwReturnCode;
    HBITMAP       hbReturnCode;
    LONG          lReturnCode;
    FOURCC        fccIOProc;
    HDC           hdc;
    HPS           hps;


    ulReturnCode = mmioIdentifyFile ( pszFileName,
                                      0L,
                                      &mmFormatInfo,
                                      &fccStorageSystem,
                                      0L,
                                      0L);
    /*
     *  If this file was NOT identified, then this function won't
     *  work, so return an error by indicating an empty bitmap.
     */
    if ( ulReturnCode == MMIO_ERROR )
    {
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
      return (0L);
    }
    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */
    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
      return ( 0L );
    }
    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
      return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }

    /* Clear out and initialize mminfo structure */
    memset ( &mmioinfo, 0L, sizeof ( MMIOINFO ) );
    mmioinfo.fccIOProc = fccIOProc;
    mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
    hmmio = mmioOpen ( (PSZ) pszFileName,
                       &mmioinfo,
                       MMIO_READ | MMIO_DENYWRITE | MMIO_NOIDENTIFY );
    if ( ! hmmio )
    {
      // If file could not be opened, return with error
      //    showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_OPENFILEERROR, queryModuleHandle());
      return (0L);
    }


    dwReturnCode = mmioQueryHeaderLength ( hmmio,
                                         (PLONG)&ulImageHeaderLength,
                                           0L,
                                           0L);
    if ( ulImageHeaderLength != sizeof ( MMIMAGEHEADER ) )
    {
      /* We have a problem.....possibly incompatible versions */
      ulReturnCode = mmioClose (hmmio, 0L);
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR);
      return (0L);
    }

    ulReturnCode = mmioGetHeader ( hmmio,
                                   &mmImgHdr,
                                   (LONG) sizeof ( MMIMAGEHEADER ),
                                   (PLONG)&ulBytesRead,
                                   0L,
                                   0L);

    if ( ulReturnCode != MMIO_SUCCESS )
    {
      /* Header unavailable */
      ulReturnCode = mmioClose (hmmio, 0L);
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR);
      return (0L);
    }

    memcpy(pBMPInfoHeader2, &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
           sizeof(BITMAPINFOHEADER2)+256*sizeof(RGB2) );

    /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */
    dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    dwWidth = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
    wBitCount = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwNumRowBytes = dwRowBits >> 3;

#if 0
    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */
    dwHeight = pBMPInfoHeader2->cy;
    dwWidth = pBMPInfoHeader2->cx;
    wBitCount = pBMPInfoHeader2->cBitCount;
    dwRowBits = dwWidth * pBMPInfoHeader2->cBitCount;
    dwNumRowBytes = dwRowBits >> 3;
#endif
    /*
     *  Account for odd bits used in 1bpp or 4bpp images that are
     *  NOT on byte boundaries.
     */
    if ( dwRowBits % 8 )
    {
         dwNumRowBytes++;
    }
    /*
     *  Ensure the row length in bytes accounts for byte padding.
     *  All bitmap data rows must are aligned on LONG/4-BYTE boundaries.
     *  The data FROM an IOProc should always appear in this form.
     */
    dwPadBytes = ( dwNumRowBytes % 4 );
    if ( dwPadBytes )
    {
         dwNumRowBytes += 4 - dwPadBytes;
    }

    /* Allocate space for ONE row of pels */
    if ( DosAllocMem( (PPVOID)&pRowBuffer,
                      (ULONG)dwNumRowBytes,
                      fALLOC))
    {
      ulReturnCode = mmioClose (hmmio, 0L);
      //    showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOMEMERROR, queryModuleHandle());
      return(0L);
    }

    /* Create a device context */
    hdc=DevOpenDC(globalHab, OD_MEMORY,"*",0L, NULL, NULLHANDLE);
    if(hdc==NULLHANDLE)
      {
        DosFreeMem(pRowBuffer);
        mmioClose (hmmio, 0L);
        return(0L);
      }
    

    // ***************************************************
    // Create a memory presentation space that includes
    // the memory device context obtained above.
    // ***************************************************
    
    ImageSize.cx = dwWidth;
    ImageSize.cy = dwHeight;

    hps = GpiCreatePS ( globalHab,
                        hdc,
                        &ImageSize,
                        PU_PELS | GPIT_NORMAL | GPIA_ASSOC );
    //                        PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC );
    if ( !hps )
      {
#ifdef DEBUG
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       "No HPS...",
                       "Open Image File",
                       (HMODULE) NULL,
                       (ULONG) MB_OK | MB_MOVEABLE |
                       MB_ERROR );
#endif   
        DevCloseDC(hdc);
        DosFreeMem(pRowBuffer);
        mmioClose (hmmio, 0L);
        return(0L);
      }

    //    GpiSelectPalette(hps, NULLHANDLE);
    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // will put all of the bits once we read them in.
    // ***************************************************
    hbm = GpiCreateBitmap ( hps,
                            &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
                            0L,
                            NULL,
                            NULL);

#if 0
    hbm = GpiCreateBitmap ( hps,
                            pBMPInfoHeader2,
                            0L,
                            NULL,
                            NULL);
#endif

    if ( !hbm )
    {
#ifdef DEBUG
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                     "No HBITMAP...",
                     "Open Image File",
                     (HMODULE) NULL,
                     (ULONG) MB_OK | MB_MOVEABLE |
                     MB_ERROR );
#endif
      GpiDestroyPS(hps);
      DevCloseDC(hdc);
      DosFreeMem(pRowBuffer);
      ulReturnCode = mmioClose (hmmio, 0L);
      return(0L);
    }

    // ***************************************************
    // Select the bitmap into the memory device context.
    // ***************************************************
    hbReturnCode = GpiSetBitmap ( hps,
                                  hbm );

    //***************************************************************
    //  LOAD THE BITMAP DATA FROM THE FILE
    //      One line at a time, starting from the BOTTOM
    //*************************************************************** */

    for ( dwRowCount = 0; dwRowCount < dwHeight; dwRowCount++ )
    {
         ulBytesRead = (ULONG) mmioRead ( hmmio,
                                          pRowBuffer,
                                          dwNumRowBytes );
         if ( !ulBytesRead )
         {
              break;
         }
         /*
          *  Allow context switching while previewing.. Couldn't get
          *  it to work. Perhaps will get to it when time is available...
          */
         lReturnCode = GpiSetBitmapBits ( hps,
                                          (LONG) dwRowCount,
                                          (LONG) 1,
                                          (PBYTE) pRowBuffer,
                                          (PBITMAPINFO2) &mmImgHdr.mmXDIBHeader.BMPInfoHeader2);
    }

    /* Clean up */
    hbReturnCode = GpiSetBitmap ( hps,
                                  NULLHANDLE );
    ulReturnCode = mmioClose (hmmio, 0L);
    DosFreeMem(pRowBuffer);
    GpiDestroyPS(hps);
    DevCloseDC(hdc);

    return(hbm);
}

 
BOOL MyTrackRoutine(HWND hwnd, PRECTL rcl_, ULONG ulFlag)
 {
   TRACKINFO track;
   HAB hab;
   RECTL rcl2;
   RECTL rcl;

   track.cxBorder = 4;
   track.cyBorder = 4;  /* 4 pel wide lines used for rectangle */
   track.cxGrid = 1;
   track.cyGrid = 1;    /* smooth tracking with mouse */
   track.cxKeyboard = 8;
   track.cyKeyboard = 8; /* faster tracking using cursor keys */
 
   hab=WinQueryAnchorBlock(hwnd);

   WinQueryWindowRect(hwnd, &rcl);
   WinQueryWindowRect(WinQueryWindow(hwnd, QW_PARENT), &rcl2);

   WinMapWindowPoints(hwnd, WinQueryWindow(hwnd, QW_PARENT), &rcl, 2);
   rcl.yTop=rcl2.yTop;
   rcl.yBottom=rcl2.yBottom;
   WinCopyRect(hab, &track.rclTrack, &rcl);   /* starting point */

   WinQueryWindowRect(WinQueryWindow(hwnd, QW_PARENT), &track.rclBoundary);

   //   WinSetRect(hab, &track.rclBoundary, 0, 0, 640, 480); /* bounding rectangle */
 
   track.ptlMinTrackSize.x = 2;
   track.ptlMinTrackSize.y = 2;  /* set smallest allowed size of rectangle */
 
   track.ptlMaxTrackSize.x = track.rclBoundary.xRight;
   track.ptlMaxTrackSize.y = track.rclBoundary.yTop; /* set largest allowed size of rectangle */
 
   track.fs = TF_MOVE;
   track.fs|=TF_ALLINBOUNDARY;
   if (WinTrackRect(WinQueryWindow(hwnd, QW_PARENT), NULLHANDLE, &track) )
   {
     SWP swp;
     /* if successful copy final position back */
     //WinCopyRect(hab, &rcl, &track.rclTrack);
     WinQueryWindowPos(hwnd, &swp);
     WinSetWindowPos(hwnd, NULLHANDLE,/* swp.cx+*/track.rclTrack.xLeft+swp.cx/2/*-rcl.xRight*/, swp.y ,0,0,SWP_MOVE);

     return(TRUE);
   }
 }



 
POINTS ptsStart={0};
POINTS ptsTemp={0};
BOOL bTrack=FALSE;
BOOL bHaveSelection=FALSE;

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
        ULONG ulWidth, ulHeight, ulWidthWindow, ulHeightWindow, ulFact;

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
        WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_SIZEWE, FALSE));
        return MRFALSE;
      }
    case WM_BUTTON1UP:
      bTrack=FALSE;
      break;
    case WM_BUTTON1MOTIONSTART:
    case WM_BUTTON2MOTIONSTART:
      {
        MyTrackRoutine(hwnd, NULLHANDLE, TF_MOVE);
        break;
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


ULONG _launchPMProg(PSZ pszTitle, PSZ wrapperExe, PSZ parameters,  char *thisPtr, ULONG ulView)
{
  HWND         hwndNotify;
  PROGDETAILS pDetails;
  HAPP         happ;

  SysWriteToTrapLog("Wrapper: %s, Parameters: %s\n", wrapperExe, parameters);

  memset(&pDetails, 0, sizeof(pDetails)); 

  pDetails.Length          = sizeof(PROGDETAILS);
  pDetails.progt.progc     = PROG_DEFAULT;
  pDetails.progt.fbVisible = SHE_VISIBLE;
  pDetails.pszTitle        = pszTitle;
  pDetails.pszParameters   = parameters;
  pDetails.pszStartupDir   = NULL;
  pDetails.pszExecutable   = wrapperExe;
  
  //pDetails->pszParameters   = NULLHANDLE;
  //pDetails->pszExecutable   = "ab.exe";
  //pDetails->pszParameters   = "mmm3u";
  //pDetails->pszStartupDir   = "D:\\Arbeitsoberflaeche";
  //  pDetails->pszTitle        = "Der Titel 2...";
  
      // pDetails->pszEnvironment  = "WORKPLACE\0\0";
      pDetails.pszEnvironment  = NULLHANDLE;
      pDetails.swpInitial.fl   = SWP_ACTIVATE|SWP_ZORDER; /* window positioning */
      pDetails.swpInitial.cy   = 0;    /* width of window */
      pDetails.swpInitial.cx   = 0;    /* height of window */
      pDetails.swpInitial.y    = 0;    /* lower edge of window */
      pDetails.swpInitial.x    = 0;    /* left edge of window */
      pDetails.swpInitial.hwndInsertBehind = HWND_TOP;
      pDetails.swpInitial.hwnd             = 0;
      //  pDetails->swpInitial.hwnd             = hwndNotify;
      pDetails.swpInitial.ulReserved1      = 0;
      pDetails.swpInitial.ulReserved2      = 0;
      
      happ = WinStartApp( NULLHANDLE, &pDetails,NULL, NULL, SAF_INSTALLEDCMDLINE);
      SysWriteToTrapLog("Done. happ: %d %x\n", happ, happ);
      return happ;
      return 1;   
}

#define  MAX_DIGITS     9          /* maximum digits in numeric arg  */
#define  MAX            256        /* temporary buffer length        */
#define  IBUF_LEN       4096       /* Input buffer length            */
#define  AllocFlag      PAG_COMMIT | PAG_WRITE  /* for DosAllocMem   */

/*********************************************************************/
/* Numeric Return calls                                              */
/*********************************************************************/

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */

/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/

#define  ERROR_RETSTR   "ERROR:"

#if 0
typedef struct RxStemData {
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    CHAR ibuf[IBUF_LEN];               /* Input buffer               */
    CHAR varname[MAX];                 /* Buffer for the variable    */
                                       /* name                       */
    CHAR stemname[MAX];                /* Buffer for the variable    */
                                       /* name                       */
    ULONG stemlen;                     /* Length of stem.            */
    ULONG vlen;                        /* Length of variable value   */
    ULONG j;                           /* Temp counter               */
    ULONG tlong;                       /* Temp counter               */
    ULONG count;                       /* Number of elements         */
                                       /* processed                  */
} RXSTEMDATA;



ULONG printVar(RXSTRING args)
{
  char text[1000];
  ULONG ulLen;
  RXSTEMDATA ldp;
  
#if 0
  ulLen=theVar.strlength;
  if(sizeof(text)-1<theVar.strlength)
    ulLen=sizeof(text);
    
  strncpy(text, theVar.strptr, ulLen);
  
  if(text[strlen(text)-1]!=".")
    strcat(text, ".");
#endif

                                       /* Initialize data area       */
  ldp.count = 0;
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  sprintf(ldp.varname+ldp.stemlen, "%d", 0);

  
  ldp.shvb.shvnext = NULL;
  ldp.shvb.shvname.strptr = ldp.varname;
  ldp.shvb.shvname.strlength = strlen(ldp.varname);
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvvalue.strptr = NULL;//ldp.ibuf;
  ldp.shvb.shvvalue.strlength = 0;//sizeof(ldp.ibuf);
  ldp.shvb.shvvaluelen = 0;//sizeof(ldp.ibuf);

  ldp.shvb.shvcode = RXSHV_SYFET;
  ldp.shvb.shvret = 0;
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
    return INVALID_ROUTINE;      /* error on non-zero          */

#if 0
  strncpy(text, ldp.shvb.shvname.strptr, ldp.shvb.shvname.strlength);
  
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Varname", 1234, MB_MOVEABLE);
#endif

  strncpy(text, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
  
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Value", 1234, MB_MOVEABLE);
  
return 0;
  //

}
#endif

typedef struct RxStemData {
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    CHAR varname[MAX];                 /* Buffer for the variable    */
                                       /* name                       */
  //  CHAR stemname[MAX];                /* Buffer for the variable    */
                                       /* name                       */
    ULONG stemlen;                     /* Length of stem.            */
 } RXSTEMDATA;



ULONG printVar(RXSTRING args)
{
  char text[1000];
  RXSTEMDATA ldp={0};  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  sprintf(ldp.varname+ldp.stemlen, "%d", 0);

  
  ldp.shvb.shvnext = NULL;
  ldp.shvb.shvname.strptr = ldp.varname;
  ldp.shvb.shvname.strlength = strlen(ldp.varname);
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvvalue.strptr = NULL;//ldp.ibuf;
  ldp.shvb.shvvalue.strlength = 0;//sizeof(ldp.ibuf);
  ldp.shvb.shvvaluelen = 0;//sizeof(ldp.ibuf);

  ldp.shvb.shvcode = RXSHV_SYFET;
  ldp.shvb.shvret = 0;
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
    return INVALID_ROUTINE;      /* error on non-zero          */

  strncpy(text, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
  
  WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "Value", 1234, MB_MOVEABLE);
  DosFreeMem(ldp.shvb.shvvalue.strptr);
  return 0;
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
  ULONG ulFileSize;
  void * pData;
  FILE *file;
  HPOINTER hPtr=NULLHANDLE;

  if(!fileGetBinFileSize(chrIconFile, &ulFileSize))
    return NULLHANDLE;  

  if((pData=malloc(ulFileSize))==NULLHANDLE)
    return NULLHANDLE;  

  if((file=fopen(chrIconFile, "rb"))==NULLHANDLE)
    {
      free(pData); 
      return NULLHANDLE;
    }

  if(ulFileSize!=fread(pData, sizeof(BYTE), ulFileSize, file))
    {
      free(pData); 
      return NULLHANDLE;
    }

  hPtr=WinBuildPtrHandle(pData);
  
  free(pData);
  fclose(file);
  
  return hPtr;
}

PFNWP oldStatic;

PFNWP g_oldStatic;
ULONG  g_ulStaticDataOffset;
ULONG  ulQWP_WNDDATA;


//char html[]="<1>šberschrift<2><3> 1.Link.<5><0>77878<3>2.Link<0>!111111111! !22! !333333! !44444!<5>!5555555555555555! ";
//<3>Ein Link.<0>Normaler Text  <3>Link <1>šberschrift";
//char html[]="<0>!111111111111111111";
//char html[]="<1>Selected object<2><9 1><0>Text...<3 1>Open... (Link)<4>LoooooongTextAboutFolderAndFileandthrowitinthebitbucketafterwards.<6><3 2>Delete...is another veeeeeryLoooooooongTextNotFittingIntoOneLine";
char html[]="<1 0>Selected object<2><9 1><0>Text...<0>Text...<6><3 2>Delete...is another veeeeeryLoooooooongTextNotFittingIntoOneLine1234567890123456789012345678901234567890\
<4><10><6><3 3>Move...<4><6><3 4>Open settings...<4>\
<7><1>Details<2><0>Size: 123456 Bytes<6><0>Noch mehr Details<6><3 5>Und ein Link...<4>";
char html2[1000]="Parsed: ";

//struct htmlPart;

typedef struct _htmlPart
{
  char* chrText; /* Text pointer                   */
  int   iLength; /* Length in chars                */
  int   iType;   /* Type e.g. link or text         */
  RECTL rcl;     /* Bounding rect in window coords */
  POINTL ptl;    /* Start position for drawing     */
  ULONG ulParam; /* Number of link in the text */
  struct _htmlPart* hpNext; /* Next text part      */
  struct _htmlPart* hpPrev; /* This links to a previous part
                               holding a part of the current link/text. It's
                               used to climb th elist to find the beginning of
                               a link so it may be painted comnpletely */
}htmlPart;

typedef struct _taginfo
{
  ULONG ulMark;
  ULONG ulParam;
}TAGINFO;

/* Styles for paragraphs, headings etc. */
typedef struct _style
{
  LONG      cx;       /* Delta to move the paragraph to the right */
  LONG      foreClr;  /* Not yet used                             */
}STYLE;

/* Styles for sections */
#define MAX_SECTION_STYLE 2
STYLE  sectionStyles[MAX_SECTION_STYLE]={0,0,10, 0};

/* Known tags */
#define MARK_TEXT         0
#define MARK_H1           1   /* Heading 1 */
#define MARK_H1_END       2

#define MARK_LINK         3   /* A link    */
#define MARK_LINK_END     4

#define MARK_BREAK        5   /* Break in the line */
#define MARK_PARAGRAPH    6
#define MARK_RULER        7   /* horizontal ruler  */
#define MARK_IMAGE        8

#define MARK_SECTION      9   /* New section with different properties */
#define MARK_SECTION_END  10


#define QWL_HTMLWNDDATA  QWL_USER   /* FIXME: Only for testing */

typedef struct _formatinfo
{
  ULONG xPos;
  LONG  cyChar;
  LONG  foreClr;
  LONG  backClr;
  int   iFontSize;
  struct _formatinfo * fiNext;
}FORMATINFO;

/* Additional windowdata */
typedef struct _htmlwnddata
{
  LONG      lVertBarWidth;
  ULONG     ulCyText;      /* Necessary y size to draw the text                  */
  ULONG     ulCxText;      /* Not yet used                                       */
  LONG      lYViewPort;   /* Delta for drawing contents when window is scrolled */
  htmlPart* hPartList;     /* List of the html parts containing the text         */
  HMTX      hmtxList;      /* Semaphore to protect html list when text changes   */
  char *    chrText;       /* Pointer to unformatted text                        */
  FORMATINFO* hFormatInfo;
  HPOINTER  hPtr;
}HTMLWNDDATA;


void parseHtml(void)
{

}

HWND hwndMsg;

void printMsg(char *chrText)
{
  WinSendMsg(hwndMsg, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP(chrText));
}
void printLong(LONG l, char * text)
{
  char msg[500];
  sprintf(msg, "%s: %d",text,  l);
  WinSendMsg(hwndMsg, LM_INSERTITEM, MPFROMSHORT(LIT_END), MPFROMP(msg));
}

char* parseMark(int iLength, char *chrPos,   TAGINFO *tagInfo)
{
  char *chrStart=chrPos;

  *chrStart++; /* move to mark */

  while(*chrPos!=0)
    {
      switch(*chrPos)
        {
        case '>':
          /* found end of mark */
          {
            int iType;

            iType=atoi(chrStart++);/* Which mark e.g. link, text... */
            chrPos++;
            switch(iType)
              {
              case MARK_SECTION:
              case MARK_SECTION_END:
              case MARK_LINK:
              case MARK_H1:
                tagInfo->ulMark=iType;
                /* Get param number. Format of link is: <3 x> with x a number */
                tagInfo->ulParam=atoi(chrStart);
                break;
              case MARK_LINK_END:
              case MARK_H1_END:
              case MARK_BREAK:
              case MARK_PARAGRAPH:
              case MARK_RULER:
                tagInfo->ulMark=iType;
                tagInfo->ulParam=0;
                break;
              default:
                tagInfo->ulMark=MARK_TEXT;
                break;
              }
            return chrPos;
          }
        default:
          break;
        }
      chrPos++;
    }
  tagInfo->ulMark=MARK_TEXT;
  tagInfo->ulParam=0;
  return chrPos;
}

char *findNextMark(char *chrPos)
{
  while(*chrPos!=0)
    {
      switch(*chrPos)
        {
        case '<':
          return chrPos;
        default:
          break;
        }
      chrPos++;
    }
  return chrPos;
}

void addToHtmlPartList(HWND hwnd, htmlPart* hTemp)
{
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData->hPartList)
    {
      wndData->hPartList=hTemp;
      return;
    }
  hTemp->hpNext=wndData->hPartList;
  wndData->hPartList=hTemp;  
}

htmlPart* getHeadHtmlPart(htmlPart* hTemp)
{
  while(hTemp->hpPrev)
      hTemp=hTemp->hpPrev;

  return hTemp;
}

void freeHtmlPartList(HWND hwnd)
{
  htmlPart *htmlTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  htmlTemp=wndData->hPartList;
  while(htmlTemp)
    {
      htmlPart* hTemp=htmlTemp->hpNext;
      free(htmlTemp);
      htmlTemp=hTemp;
    }
  wndData->hPartList=NULLHANDLE;
}

BOOL pushFormatInfo(HWND hwnd, FORMATINFO formatInfo)
{
  FORMATINFO* fiTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return FALSE; /* There's a problem*/

  hFormatInfo=wndData->hFormatInfo;

  if((fiTemp=malloc(sizeof(FORMATINFO)))==NULLHANDLE)
    return FALSE;

  //  memset(fiTemp, 0, sizeof(FORMATINFO));

  *fiTemp=formatInfo;

  //  printLong(fiTemp->xPos, "push it");
  if(!hFormatInfo)
    {
      wndData->hFormatInfo=fiTemp;
      return TRUE;
    }

  fiTemp->fiNext=hFormatInfo;
  wndData->hFormatInfo=fiTemp;  

  //htmlWndData.hFormatInfo=hFormatInfo; /* Testing only */
  return TRUE;
}

BOOL popFormatInfo(HWND hwnd, FORMATINFO* formatInfo)
{
  FORMATINFO* fiTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return FALSE; /* There's a problem*/

  hFormatInfo=wndData->hFormatInfo;

  if(!hFormatInfo)
    return FALSE;

  fiTemp=hFormatInfo->fiNext;
  *formatInfo=*hFormatInfo;
  formatInfo->fiNext=NULLHANDLE;

  free(hFormatInfo);

  wndData->hFormatInfo=fiTemp;
  return TRUE;
}

BOOL getFormatInfo(HWND hwnd, FORMATINFO* formatInfo)
{
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return FALSE; /* There's a problem*/

  hFormatInfo=wndData->hFormatInfo;

  if(!hFormatInfo)
    return FALSE;

  *formatInfo=*hFormatInfo;

  return TRUE;
}

void freeFormatInfoList(HWND hwnd)
{
  FORMATINFO* fiTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  FORMATINFO* hFormatInfo;

  if(!wndData)
    return; /* There's a problem*/

  hFormatInfo=wndData->hFormatInfo;

  fiTemp=hFormatInfo;

  while(fiTemp)
    {
      FORMATINFO* hFi=fiTemp->fiNext;
      free(fiTemp);
      fiTemp=hFi;
    }  

  wndData->hFormatInfo=NULLHANDLE;
}


htmlPart * checkForLink(HWND hwnd, PPOINTL ptl)
{
  htmlPart *hTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  hTemp=wndData->hPartList;
  //  printLong(ptl->y, "y");
  //  printLong(hTemp->rcl.yTop, "rcl x");
  while(hTemp)
    {
      RECTL rcl=hTemp->rcl;
      rcl.yTop+=wndData->lYViewPort;
      rcl.yBottom+=wndData->lYViewPort;
      if(WinPtInRect(WinQueryAnchorBlock(HWND_DESKTOP), &rcl, ptl)) {
        if(hTemp->iType==MARK_LINK)
          return hTemp;
      }
      hTemp=hTemp->hpNext;
    }
  return NULLHANDLE;
}

BOOL clickLink(HWND hwnd, PPOINTL ptl)
{
  htmlPart *hTemp;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
  
  hTemp=wndData->hPartList;

  while(hTemp)
    {
      RECTL rcl=hTemp->rcl;
      rcl.yTop+=wndData->lYViewPort;
      rcl.yBottom+=wndData->lYViewPort;

      if(WinPtInRect(WinQueryAnchorBlock(HWND_DESKTOP), &rcl, ptl)) {
        if(hTemp->iType==MARK_LINK) {
          return hTemp->ulParam;
        }
      }
      hTemp=hTemp->hpNext;
    }
  return 0;
}

/* returns the string length in points for the given hps */
LONG gpiStringLength(HPS hps, LONG lLength, char * chrText)
{
  POINTL aptl[TXTBOX_COUNT];

  GpiQueryTextBox(hps, lLength, chrText, TXTBOX_COUNT, aptl);

  return aptl[TXTBOX_BOTTOMRIGHT].x-aptl[TXTBOX_BOTTOMLEFT].x; 
}


/* returns the string length in chars. Accepts ' ' and '<' as
   string part separators */
LONG newStringLength(HPS hps, LONG lMax, char * chrText)
{
  char * chrPtr;
  LONG rc;

  if((chrPtr=strchr(chrText, ' '))!=NULLHANDLE) {
    rc=chrPtr-chrText+1; /* Move behind the space */
    if(rc<=lMax)
      return rc; 
  }

  if((chrPtr=strchr(chrText, '<'))!=NULLHANDLE)
    return chrPtr-chrText; /* Move behind the space */

  return strlen(chrText);
}

/* Find a string which fits into the remaining window space */
LONG getFittingStringLength(HPS hps, LONG lMax, char* chrText, LONG lFreeSpace)
{
  LONG lPtl;
  int a=0;

  /*
    FIXME:
    Very sloooooow
    */
  while((lPtl=gpiStringLength(hps, a+1, chrText))<=lFreeSpace)
    a++;
  return a;
}

void setHeadingFormat(HPS hps, LONG * clrFore)
{
  FATTRS fat;
  memset(&fat, 0, sizeof(fat));
  fat.fsSelection=FATTR_SEL_BOLD;
  fat.usRecordLength=sizeof(fat);
  fat.lMaxBaselineExt=12L;
  fat.lAveCharWidth=12L;
  fat.fsFontUse=FATTR_FONTUSE_NOMIX;
  
  strcpy(fat.szFacename, "WarpSans");
  GpiCreateLogFont(hps, NULL, 1L, &fat);
  GpiSetCharSet(hps, 1L);
  *clrFore=CLR_BLACK;
}


void setLinkFormat(HPS hps, LONG * clrFore)
{
#if 0
  FATTRS fat;
  
  memset(&fat, 0, sizeof(fat));
  fat.fsSelection=FATTR_SEL_UNDERSCORE;
  fat.usRecordLength=sizeof(fat);
  fat.lMaxBaselineExt=10L;
  fat.lAveCharWidth=10L;
  fat.fsFontUse=FATTR_FONTUSE_NOMIX;
  strcpy(fat.szFacename, "WarpSans");
  
  GpiCreateLogFont(hps, NULL, 1L, &fat);

  GpiSetCharSet(hps, 1L);
#endif
  *clrFore=CLR_BLUE;
}

/* format and draw text */
LONG drawText2(HPS hps, HWND hwnd, LONG lLength, char * chrText, RECTL* rcl,   TAGINFO *tagInfo, LONG lCharsDone, ULONG ulFlags)
{
  POINTL aptl[TXTBOX_COUNT];
  POINTL ptl;
  LONG clrFore;
  LONG lTotalLength=lLength;
  htmlPart* hTemp=NULLHANDLE;
  LONG lPtl;
  LONG lMark=tagInfo->ulMark;
  FORMATINFO fi={0};
  htmlPart *hPrev=NULLHANDLE;

  getFormatInfo( hwnd, &fi);

#if 0
  if(lLength)
    GpiQueryTextBox(hps, lLength, chrText, TXTBOX_COUNT, aptl);
  else  
#endif
    
    GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);


  GpiQueryCurrentPosition(hps, &ptl);

  if(!lCharsDone)
    {
      //printMsg("First run...");
      /* First invocation */
      fi.cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;
      fi.xPos=0;

      ptl.x=fi.xPos;
      rcl->yTop-=fi.cyChar;
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);
      /* Save current formatting info */
      pushFormatInfo(hwnd, fi);
    }


  switch(lMark)
    {
    case MARK_H1:
      {
        setHeadingFormat(hps, &clrFore);

        GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);
        fi.cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;
        ptl.x=rcl->xLeft;
        rcl->yTop-=(fi.cyChar*1.5);
        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl);

        break;
      }
    case MARK_H1_END:
      GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);
      fi.cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;

      //      ptl.x=rcl->xLeft;
      ptl.x=fi.xPos;
      rcl->yTop-=(fi.cyChar*1.5);
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);

      GpiDeleteSetId(hps, LCID_ALL);
      return lLength;
    case MARK_SECTION:
      {
        int idx=0;

        if(tagInfo->ulParam<=MAX_SECTION_STYLE)
          idx=tagInfo->ulParam;
        fi.xPos+=sectionStyles[idx].cx;
        ptl.x=fi.xPos;
        GpiMove(hps, &ptl);
        
        pushFormatInfo(hwnd, fi);

        if(!lLength)
          return lLength;
        break;
      }
    case MARK_SECTION_END:

      popFormatInfo(hwnd, &fi);
      
      if(!lLength)
        return lLength;
      break;

    case MARK_LINK:
      {
        setLinkFormat(hps, &clrFore);
        break;
      }

#if 0
    case MARK_LINK:
      {
        FATTRS fat;
        
        memset(&fat, 0, sizeof(fat));
        fat.fsSelection=FATTR_SEL_BOLD;
        fat.usRecordLength=sizeof(fat);
        //fat.lMaxBaselineExt=9L;
        //  fat.lAveCharWidth=9L;
        fat.fsFontUse=FATTR_FONTUSE_NOMIX;
        strcpy(fat.szFacename, "WarpSans");

        GpiCreateLogFont(hps, NULL, 1L, &fat);
        GpiSetCharSet(hps, 1L);

        clrFore=CLR_BLUE;
        break;
      }
#endif

   case MARK_PARAGRAPH:
     //     ptl.x=rcl->xLeft;
     ptl.x=fi.xPos;
     rcl->yTop-=fi.cyChar*1.5;
     ptl.y=rcl->yTop;
     GpiMove(hps, &ptl);
     return lLength;
    case MARK_BREAK:
      ptl.x=rcl->xLeft;
      rcl->yTop-=fi.cyChar;
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);
      /* if lLength!=0 then no starting tag for following text. Only the <Break> tag given. */
      if(!lLength)
        return lLength;
      break;
    case MARK_RULER:
      {
        /* Draw ruler */
        ptl.x=rcl->xRight*0.1;
        rcl->yTop-=fi.cyChar*1.5;
        ptl.y=rcl->yTop;

        //printLong(ptl.x, "ptl.x");
        //printLong(ptl.y, "ptl.y");

        hTemp=malloc(sizeof(htmlPart));
        hTemp->iType=lMark;

        WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
                   ptl.x, ptl.y,
                   rcl->xRight*0.9, ptl.y+2 );

        hTemp->ptl=ptl;
        hTemp->chrText=chrText;
        hTemp->iLength=rcl->xRight*0.8;
        hTemp->ulParam=tagInfo->ulParam;
        hTemp->hpPrev=hPrev;
        hPrev=hTemp;
        addToHtmlPartList(hwnd, hTemp);

        GpiMove(hps, &ptl);
        ptl.x=rcl->xRight*0.9;
        GpiLine(hps, &ptl);


        ptl.x=fi.xPos;
        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl);
        /* if lLength!=0 then no starting tag for following text. Only the <Break> tag given. */
        if(!lLength)
          return lLength;
        break;
      }
    default:
      GpiSetCharSet(hps, LCID_DEFAULT); /* Default font  */
      GpiDeleteSetId(hps, LCID_ALL);
      clrFore=CLR_BLACK;                /* Default color */
      break;
    }

  // ptl.x=fi.xPos;
  //    printLong(fi.xPos, "fi.xPos");
  /***********************/
  //GpiMove(hps, &ptl);

  GpiSetColor(hps, clrFore);
  //  GpiQueryCurrentPosition(hps, &ptl);

  lPtl=gpiStringLength(hps, lLength, chrText );
  if( lPtl <= (rcl->xRight-ptl.x)) {
    /* String fits into line */
    //GpiSetColor(hps, CLR_BLACK);
    //printLong(ptl.x, "ptl.x");
    GpiCharString(hps, lLength, chrText);
    hTemp=malloc(sizeof(htmlPart));
    hTemp->iType=lMark;
    WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
               ptl.x, ptl.y,
               ptl.x+ lPtl, ptl.y+fi.cyChar );
    hTemp->ptl=ptl;
    hTemp->chrText=chrText;
    hTemp->iLength=lLength;
    hTemp->ulParam=tagInfo->ulParam;
    hTemp->hpPrev=hPrev;
    hPrev=hTemp;
    addToHtmlPartList(hwnd, hTemp);
    return lLength;
  }
  else
    {
      LONG lDone=0;
      LONG lPtl;
      /* String is to long. Split in words and try to draw them */
     
      //DosBeep(5000, 200);
      //DosSleep(100);
      /* Get next word */
      while((lLength=newStringLength( hps, lTotalLength-lDone, chrText)) && lDone+lLength<=lTotalLength)
        {
          lPtl=gpiStringLength(hps, lLength, chrText);
          GpiQueryCurrentPosition(hps, &ptl);

          if(lPtl > (rcl->xRight-ptl.x))
            {
              /* Word doesn't fit. Check if it fits into a whole line */
              //              if(lPtl <= (rcl->xRight-rcl->xLeft))
              if(lPtl <= (rcl->xRight-fi.xPos))
                {
                  /* Yes, does fit. Move to next line and print it  */
                  //                  ptl.x=rcl->xLeft;
                  ptl.x=fi.xPos;
                  rcl->yTop-=fi.cyChar;
                  ptl.y=rcl->yTop;  
                  GpiMove(hps, &ptl);

                  //GpiSetColor(hps, CLR_RED);

                  GpiCharString(hps, lLength, chrText);
                }
              else {
                /* No, too long for a whole line so start writing in current line and wrap */
                /* Get the amount of chars which do fit */
                lLength=getFittingStringLength(hps, lTotalLength-lDone, chrText, rcl->xRight-ptl.x);
                GpiSetColor(hps, CLR_BLUE);
                if(lLength)
                  GpiCharString(hps, lLength, chrText);
                else
                  {
                    /* Not enough space for a single char, move to next line */
                    ptl.x=fi.xPos;
                    rcl->yTop-=fi.cyChar;
                    ptl.y=rcl->yTop;  
                    GpiMove(hps, &ptl);
                  }
              }
            }
          else {
            /* The current string fits into the remaining space */
            //GpiSetColor(hps, CLR_GREEN);
            GpiCharString(hps, lLength, chrText);
          }
          /* Crate the part info */
          hTemp=malloc(sizeof(htmlPart));
          hTemp->iType=lMark;
          WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
                     ptl.x, ptl.y,
                     ptl.x+lPtl, ptl.y+fi.cyChar );
          hTemp->ptl=ptl;
          hTemp->chrText=chrText;
          hTemp->iLength=lLength;
          hTemp->ulParam=tagInfo->ulParam;
          hTemp->hpPrev=hPrev;
          hPrev=hTemp;
          addToHtmlPartList(hwnd, hTemp);
          lDone+=lLength;
          // printLong(lDone, "lDone");
          chrText+=lLength; /* Move write mark in string */
          //  printMsg(chrText);
          /* Get next word */
          //     lLength=newStringLength( hps, lTotalLength-lDone, chrText);
          //    printLong(lLength, "lLength");
        }
      //      printMsg("");
      return lDone;
    }
  return 0;
}

/*
  FIXME: Use position info for optimized drawing instead of relying on
  system clipping in HPS.
 */
void drawItNow(HWND hwnd, HPS hps, RECTL *rcl)
{
  htmlPart *hTemp;
  LONG clrFore;
  POINTL ptl;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  hTemp=wndData->hPartList;
  //  printLong(ptl->y, "y");
  //  printLong(hTemp->rcl.yTop, "rcl x");
  while(hTemp)
    {
      switch(hTemp->iType)
        {
        case MARK_H1:
          {
            setHeadingFormat(hps, &clrFore);
            break;
          }
          GpiDeleteSetId(hps, LCID_ALL);
          return;
        case MARK_LINK:
          {
            setLinkFormat(hps, &clrFore);
            break;
          }
        case MARK_RULER:
          {
            /* Draw ruler */
            GpiSetColor(hps, clrFore);
            
            ptl=hTemp->ptl;
            ptl.y+=wndData->lYViewPort;

            GpiMove(hps, &ptl);
            ptl.x+=hTemp->iLength;
            GpiLine(hps, &ptl);

            hTemp=hTemp->hpNext;
            continue;
          }
        default:
          GpiSetCharSet(hps, LCID_DEFAULT);
          GpiDeleteSetId(hps, LCID_ALL);
          clrFore=CLR_BLACK;
          break;
        }

      GpiSetColor(hps, clrFore);

      ptl=hTemp->ptl;
      ptl.y+=wndData->lYViewPort;
      GpiMove(hps, &ptl);    
      GpiCharString(hps, hTemp->iLength, hTemp->chrText);
  
      hTemp=hTemp->hpNext;
    }
  return;
}

void formatText(HWND hwnd, char *text, RECTL *rcl)
{
  LONG lLength;
  LONG  lTotalLength=strlen(text);
  char *chrPos=text;
  char *chrEnd;
  LONG lDone=0;
  TAGINFO tagInfo;
  RECTL rclTemp;
  HPS hps;
  HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

  if(!wndData)
    return ;

  freeHtmlPartList(hwnd);

  rclTemp=*rcl;
  //  rclTemp.xRight-=10;//wndData->lVertBarWidth;
  hps=WinGetPS(hwnd);
  tagInfo.ulMark=MARK_TEXT;

  while(lDone<=lTotalLength && *chrPos!=0)
    {
      /* Check if starting with a mark */
      if(*chrPos=='<')
        chrPos=parseMark(lLength, chrPos, &tagInfo); /* Start of real text in chrPos */

      /* Get next mark */
      chrEnd=findNextMark(chrPos);
      lLength=chrEnd-chrPos; /* Length of text to be drawn */

      lDone+=drawText2(hps, hwnd, lLength, chrPos, &rclTemp, &tagInfo, lDone, DT_WORDBREAK);
      chrPos=chrEnd;
    } /* while */
  //  printLong( rcl->yTop, "cy window ");
  //  printLong( rcl->yTop-rclTemp.yTop, "cy text ");

  wndData->ulCyText=rcl->yTop-rclTemp.yTop;
  if(wndData->ulCyText>rcl->yTop) {
    /* Set scrollbar range and position */
    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR, MPFROMSHORT(1),
               MPFROM2SHORT( 1,wndData->ulCyText-rcl->yTop));

    /* Set size of thumb */
    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETTHUMBSIZE,
               MPFROM2SHORT( rcl->yTop, wndData->ulCyText), 0L);

    WinShowWindow(WinWindowFromID(hwnd, FID_VERTSCROLL), TRUE);
  }
  else
    WinShowWindow(WinWindowFromID(hwnd, FID_VERTSCROLL), FALSE);
  WinReleasePS(hps);

  freeFormatInfoList(hwnd);
}

MRESULT EXPENTRY htmlProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_MOUSEMOVE:
      {
        POINTL ptl;
        htmlPart *hTemp;

        ptl.x=SHORT1FROMMP(mp1);
        ptl.y=SHORT2FROMMP(mp1);
        if((hTemp=checkForLink(hwnd,  &ptl))!=NULLHANDLE) {
          HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);
          /* Mouse is over a link */
          if(wndData->hPtr)
          WinSetPointer(HWND_DESKTOP, wndData->hPtr);
          //          hTemp=getHeadHtmlPart(hTemp);
          // drawLinkSelected
          return MRTRUE;
        }
      break;
      }
    case WM_BUTTON1CLICK:
      {
        POINTL ptl;
        ULONG ulLink;
        ptl.x=SHORT1FROMMP(mp1);
        ptl.y=SHORT2FROMMP(mp1);
        
        if((ulLink=clickLink(hwnd,  &ptl))!=0) {
          printLong(ulLink, "Link nr.");
          return MRTRUE;
        }

        return MRFALSE;
      }
    case WM_PAINT:
      {
        RECTL rcl;
        HPS hps;

        hps=WinBeginPaint(hwnd, NULLHANDLE, &rcl);

        WinQueryWindowRect(hwnd, &rcl);
        WinFillRect(hps, &rcl, CLR_WHITE);

        drawItNow(hwnd, hps, &rcl);

        WinEndPaint(hps);
        return MRFALSE;
      }
      /* Vertical slider */
    case WM_VSCROLL:
      {
        switch(SHORT1FROMMP(mp1))
          {
          case FID_VERTSCROLL:
            {
              HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

              switch(SHORT2FROMMP(mp2))
                {
                case SB_LINEDOWN:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort+=10;//wndData->ulCyText;
                    if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                  break;
                  }
                case SB_LINEUP:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort-=10;//wndData->ulCyText;
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                  break;
                  }
                case SB_PAGEDOWN:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort+=rcl.yTop;//wndData->ulCyText;
                    if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                  break;
                  }
                case SB_PAGEUP:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort-=rcl.yTop;//wndData->ulCyText;
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                    break;
                  }
                  /* User moved slider and released the button */
                case SB_SLIDERPOSITION:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort=SHORT1FROMMP(mp2);
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    else if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;

                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                    break;
                  }
                case SB_SLIDERTRACK:
                  {
                    RECTL rcl;
                    WinQueryWindowRect(hwnd, &rcl);
                    wndData->lYViewPort=SHORT1FROMMP(mp2);
                    if(wndData->lYViewPort<0)
                      wndData->lYViewPort=0;
                    else if(wndData->lYViewPort+rcl.yTop>wndData->ulCyText)
                      wndData->lYViewPort=wndData->ulCyText-rcl.yTop;

                    rcl.xRight-=wndData->lVertBarWidth;
                    WinInvalidateRect(hwnd, &rcl, TRUE);
                    /* Position slider */
                    WinSendMsg(WinWindowFromID(hwnd, FID_VERTSCROLL), SBM_SETSCROLLBAR,
                               MPFROMSHORT(1+wndData->lYViewPort),
                               MPFROM2SHORT( 1,wndData->ulCyText-rcl.yTop));
                    break;
                  }
                default:
                  break;
                }
              break;
            }
          default:
            break;
          }

      break;
      }
    case WM_SETWINDOWPARAMS:
      {
      PWNDPARAMS pwp=PVOIDFROMMP(mp1);
      RECTL rcl;
      HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

      if(!pwp || !(pwp->fsStatus & WPM_TEXT))
        break;

      WinQueryWindowRect(hwnd, &rcl);

      if(wndData) {
        if(wndData->chrText)
          free(wndData->chrText);

        wndData->chrText=malloc(pwp->cchText+1); /* Don't know if terminating 0 is included... */
        if(wndData->chrText)
          strncpy(wndData->chrText, pwp->pszText, pwp->cchText);
        rcl.xRight-= wndData->lVertBarWidth;
        formatText( hwnd, wndData->chrText,  &rcl);
      }
      break;
      }
    case WM_WINDOWPOSCHANGED:
      {
        PSWP pswp=PVOIDFROMMP(mp1);
        
        if(!pswp)
          break;
        if(pswp->fl & SWP_SIZE) {
          HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

          if(wndData) {
            RECTL rcl={0};

            WinQueryWindowRect(hwnd, &rcl);
            rcl.xRight=pswp->cx-wndData->lVertBarWidth;
            rcl.yTop=pswp->cy;
            formatText(hwnd, html,  &rcl);
          }
        }
        break;
      }
    case WM_CREATE:
      {
        FRAMECDATA frameCtrl={0};
        RECTL rcl;
        HTMLWNDDATA* wndData;
        MRESULT mr;

        ULONG ulVBar=WinQuerySysValue(HWND_DESKTOP, SV_CXVSCROLL);
        mr=g_oldStatic( hwnd, msg, mp1, mp2);


        /* Create scrollbar */
        frameCtrl.cb=sizeof(frameCtrl);
        frameCtrl.flCreateFlags=FCF_VERTSCROLL;
        WinCreateFrameControls(hwnd, &frameCtrl, "");
        
        /* Position Framecontrol */        
        WinQueryWindowRect( hwnd, &rcl);
        WinSetWindowPos(WinWindowFromID(hwnd, FID_VERTSCROLL),
                        NULLHANDLE, rcl.xRight-ulVBar, 0, ulVBar, rcl.yTop, SWP_MOVE|SWP_SIZE);
        
        wndData=malloc(sizeof(HTMLWNDDATA));
        if(wndData) {
          wndData->lVertBarWidth=ulVBar;
          WinSetWindowULong(hwnd, ulQWP_WNDDATA, (ULONG)wndData);
          //     WinSetWindowULong(hwnd, QWL_USER, (ULONG)wndData);
          //printLong(ulQWP_WNDDATA,"wnddata");
          //      DosBeep(500, 400);
        }
        
        return mr;
      break;
      }
    case WM_DESTROY:
      {
        HTMLWNDDATA* wndData=(HTMLWNDDATA*)WinQueryWindowULong(hwnd, ulQWP_WNDDATA);

        if(wndData)
          {
            freeHtmlPartList(hwnd);
            free(wndData);
          }
        break;
      }
    default:
      break;
  }
  //  return WinDefWindowProc( hwnd, msg, mp1, mp2);
  return g_oldStatic( hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY separatorProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_PAINT:
      {
        MRESULT mr;
        HPS hps;
        RECTL rcl;

        WinQueryWindowRect(hwnd, &rcl);
        if(rcl.yTop-rcl.yBottom<2)
          break;

        mr=WinDefWindowProc(hwnd, msg, mp1, mp2);
        if((hps=WinGetPS(hwnd))!=NULLHANDLE) {
          POINTL ptl;

          ptl.y=(rcl.yTop-rcl.yBottom)/2;
          ptl.x=0;
          GpiMove(hps, &ptl);
          ptl.x=rcl.xRight;
          GpiSetColor(hps, CLR_DARKGRAY);
          GpiLine(hps,&ptl);
          GpiSetColor(hps, CLR_WHITE);
          ptl.y-=1;
          GpiMove(hps, &ptl);
          ptl.x=0;
          GpiLine(hps,&ptl);
          WinReleasePS(hps);
        }
        return mr;
        break;
      }
    default:
      break;
    }
  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY volumeDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  //  static bShowParsed=FALSE;

  switch (msg)
    {      
    case WM_BUTTON1DBLCLK:
#if 0
      if(bShowParsed) {
        WinSetWindowText(WinWindowFromID(hwnd, IDST_TEXT), html);
        bShowParsed=FALSE;
      }
      else {
        WinSetWindowText(WinWindowFromID(hwnd, IDST_TEXT), html2);
        bShowParsed=TRUE;
      }
#endif
      return MRTRUE;
    case WM_INITDLG:
      {
        RECTL rcl;
        HPS hps;
        SWP swp;

        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        
        //   WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_MOVEABLE);

        hwndMsg=WinWindowFromID(hwnd, IDLB_MSG);
        WinShowWindow( hwnd, TRUE );

        // WinSubclassWindow(WinWindowFromID(hwnd, IDPB_DRAGBUTTON), staticProc);
        globalHab=WinQueryAnchorBlock(hwnd);
        //        hBitmap=loadBitmap("H:\\cwmm\\classes\\mm-progs\\pmtst\\synthesis027.BMP", &bmpInfoHeader2);


        WinQueryWindowPos(WinWindowFromID(hwnd, IDST_TEXT), &swp);

        WinDestroyWindow(WinWindowFromID(hwnd, IDST_TEXT));
        WinCreateWindow(hwnd, "WC_FORMATEDTEXT", "", WS_VISIBLE, swp.x, swp.y, swp.cx, swp.cy, hwnd, HWND_TOP,IDST_TEXT,
                      NULLHANDLE, NULLHANDLE);


                WinSetWindowText(WinWindowFromID(hwnd, IDST_TEXT), html);


        return (MRESULT) FALSE;
        WinQueryWindowRect(WinWindowFromID(hwnd, IDST_TEXT), &rcl);
        hps=WinGetPS(WinWindowFromID(hwnd, IDST_TEXT));
        //        formatText(hps, html,  &rcl);
        WinReleasePS(hps);

        return (MRESULT) FALSE;
        /*   
             WinSetWindowULong(WinWindowFromID(hwnd, IDST_TEXT), QWL_STYLE,
             WinQueryWindowULong(WinWindowFromID(hwnd, IDST_TEXT), QWL_STYLE)&~WS_PARENTCLIP);
             /*
               
        frameCtrl.cb=sizeof(frameCtrl);
        frameCtrl.flCreateFlags=FCF_VERTSCROLL;
        WinCreateFrameControls( WinWindowFromID(hwnd, IDST_TEXT), &frameCtrl, "");

        /* Position Framecontrol */        
        WinQueryWindowRect( WinWindowFromID(hwnd, IDST_TEXT), &rcl);
        WinSetWindowPos(WinWindowFromID(WinWindowFromID(hwnd, IDST_TEXT), FID_VERTSCROLL),
                        NULLHANDLE, rcl.xRight-10, 0, 10, rcl.yTop, SWP_MOVE|SWP_SIZE|SWP_SHOW);
          oldStatic=WinSubclassWindow(WinWindowFromID(hwnd, IDST_TEXT), htmlProc);
        WinSetWindowText(WinWindowFromID(hwnd, IDST_TEXT), html);
        WinQueryWindowRect(WinWindowFromID(hwnd, IDST_TEXT), &rcl);
        hps=WinGetPS(WinWindowFromID(hwnd, IDST_TEXT));
        //        formatText(hps, html,  &rcl);
        WinReleasePS(hps);
       return (MRESULT) FALSE;
      }
    case WM_CONTEXTMENU:
      contextMenu(hwnd);
      return (MRESULT) FALSE;
    case WM_BUTTON2MOTIONSTART:
    case WM_BUTTON1MOTIONSTART:
      /* Drag the window without titlebar */
      WinSendMsg(hwnd, WM_TRACKFRAME, 
                 MPFROMSHORT(TF_MOVE   /*| TF_SETPOINTERPOS */), NULL) ;
      return (MRESULT) 0 ;
      case WM_CONTROL:
         if (SHORT1FROMMP(mp1)==IDCS_VOLUME)
         {
            if ((SHORT2FROMMP(mp1)==CSN_CHANGED) ||    /* change volume?   */
                (SHORT2FROMMP(mp1)==CSN_TRACKING))     /* tracking volume? */
            {

            }
         }
         return( (MRESULT) 0);
    case WM_CLOSE:
      {
        WinPostMsg(hwnd, WM_QUIT, 0, 0);
        return (MRESULT )FALSE;
      }
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDM_ITEMTITLEBAR:
          //          DosBeep(5000, 500);
          break;
        case IDM_ITEMEXIT:
          WinPostMsg(hwnd, WM_CLOSE, 0, 0);/* Timer error so do a hard quit */
          break;
        default:
          break;
        }
      return (MRESULT) FALSE;
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

  CLASSINFO ci;
           
  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      RESSOURCEHANDLE=0;
      buildIniFileName(argv[0]);

      /* Register the new text control */
 
      if(WinQueryClassInfo(WinQueryAnchorBlock(HWND_DESKTOP),
                           WC_STATIC,
                           &ci))
        {
          g_ulStaticDataOffset=ci.cbWindowData;
          g_oldStatic=ci.pfnWindowProc;
          ulQWP_WNDDATA=g_ulStaticDataOffset;
          

          if(WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                               (PSZ)"WC_FORMATEDTEXT",
                               htmlProc,
                               (ci.flClassStyle) &~(CS_PUBLIC|CS_PARENTCLIP),
                               ci.cbWindowData+sizeof(void*)))
            {
              /* */
            }

          if(WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP),
                              (PSZ)"CW_SEPARATOR",
                              separatorProc,
                              (ci.flClassStyle) &~(CS_PUBLIC | CS_PARENTCLIP),
                              0))
            {
              /* */
              
            }
          
        }    


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
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}









#if 0

/* format and draw text */
LONG drawText2(HPS hps, LONG lLength, char * chrText, RECTL* rcl,   TAGINFO *tagInfo, LONG lCharsDone, ULONG ulFlags)
{
  POINTL aptl[TXTBOX_COUNT];
  POINTL ptl;
  LONG cyChar;
  LONG clrFore;
  LONG lTotalLength=lLength;
  htmlPart* hTemp=NULLHANDLE;
  LONG lPtl;
  LONG lMark=tagInfo->ulMark;
  FORMATINFO fi;

  if(lLength)
    GpiQueryTextBox(hps, lLength, chrText, TXTBOX_COUNT, aptl);
  else  
    GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);

  cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;


  GpiQueryCurrentPosition(hps, &ptl);
  if(!lCharsDone)
    {
      ptl.x=rcl->xLeft;
      rcl->yTop-=cyChar;
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);
    }

  switch(lMark)
    {
    case MARK_H1:
      {
        FATTRS fat;
        memset(&fat, 0, sizeof(fat));
        fat.fsSelection=FATTR_SEL_BOLD;
        fat.usRecordLength=sizeof(fat);
        fat.lMaxBaselineExt=12L;
        fat.lAveCharWidth=12L;
        fat.fsFontUse=FATTR_FONTUSE_NOMIX;
        strcpy(fat.szFacename, "WarpSans");
        GpiCreateLogFont(hps, NULL, 1L, &fat);
        GpiSetCharSet(hps, 1L);
        clrFore=CLR_BLACK;

        GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);
        cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;
        ptl.x=rcl->xLeft;
        rcl->yTop-=(cyChar*1.5);
        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl);

        break;
      }
    case MARK_H1_END:
      GpiQueryTextBox(hps, 1, "*", TXTBOX_COUNT, aptl);
      cyChar=aptl[TXTBOX_TOPLEFT].y-aptl[TXTBOX_BOTTOMLEFT].y;

      ptl.x=rcl->xLeft;
      rcl->yTop-=(cyChar*1.5);
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);

      GpiDeleteSetId(hps, LCID_ALL);
      return lLength;
    case MARK_LINK:
      {
        FATTRS fat;
        
        memset(&fat, 0, sizeof(fat));
        fat.fsSelection=FATTR_SEL_UNDERSCORE;
        fat.usRecordLength=sizeof(fat);
        fat.lMaxBaselineExt=10L;
        fat.lAveCharWidth=10L;
        fat.fsFontUse=FATTR_FONTUSE_NOMIX;
        strcpy(fat.szFacename, "WarpSans");

        GpiCreateLogFont(hps, NULL, 1L, &fat);
        GpiSetCharSet(hps, 1L);

        clrFore=CLR_BLUE;
        break;
      }
   case MARK_PARAGRAPH:
     ptl.x=rcl->xLeft;
     rcl->yTop-=cyChar*1.5;
     ptl.y=rcl->yTop;
     GpiMove(hps, &ptl);
     return lLength;
    case MARK_BREAK:
      ptl.x=rcl->xLeft;
      rcl->yTop-=cyChar;
      ptl.y=rcl->yTop;
      GpiMove(hps, &ptl);
      /* if lLength!=0 then no starting tag for following text. Only the <Break> tag given. */
      if(!lLength)
        return lLength;
      break;
    case MARK_RULER:
      {
        /* Draw ruler */
        ptl.x=rcl->xRight*0.1;
        rcl->yTop-=cyChar*1.5;
        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl);
        ptl.x=rcl->xRight*0.9;
        GpiLine(hps, &ptl);
        ptl.x=rcl->xLeft;
        //  rcl->yTop-=cyChar;
        ptl.y=rcl->yTop;
        GpiMove(hps, &ptl);
        /* if lLength!=0 then no starting tag for following text. Only the <Break> tag given. */
        if(!lLength)
          return lLength;
        break;
      }
    default:
      GpiSetCharSet(hps, LCID_DEFAULT);
      GpiDeleteSetId(hps, LCID_ALL);
      clrFore=CLR_BLACK;
      break;
    }

  GpiSetColor(hps, clrFore);
  GpiQueryCurrentPosition(hps, &ptl);

  lPtl=gpiStringLength(hps, lLength, chrText );
  if( lPtl <= (rcl->xRight-ptl.x)) {
    // DosBeep(500, 200);
    /* String fits into line */
    //    GpiSetColor(hps, CLR_BLACK);
    GpiCharString(hps, lLength, chrText);
    hTemp=malloc(sizeof(htmlPart));
    hTemp->iType=lMark;
    WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
               ptl.x, ptl.y,
               ptl.x+ lPtl, ptl.y+cyChar );
    hTemp->ptl=ptl;
    hTemp->chrText=chrText;
    hTemp->iLength=lLength;
    hTemp->ulParam=tagInfo->ulParam;
    addToHtmlPartList(hTemp);
    return lLength;
  }
  else
    {
      LONG lDone=0;
      LONG lPtl;
      /* String is to long. Split in words and try to draw them */
      /* Get next word */
      lLength=newStringLength( hps, lTotalLength-lDone, chrText);

      //DosBeep(5000, 200);
      //DosSleep(100);
      while(lLength && lDone+lLength<=lTotalLength)
        {
          lPtl=gpiStringLength(hps, lLength, chrText);
          GpiQueryCurrentPosition(hps, &ptl);

          if(lPtl > (rcl->xRight-ptl.x))
            {
              /* Word doesn't fit. Check if it fits into a whole line */
              if(lPtl <= (rcl->xRight-rcl->xLeft))
                {
                  /* Yes, does fit */
                  ptl.x=rcl->xLeft;
                  rcl->yTop-=cyChar;
                  ptl.y=rcl->yTop;  
                  GpiMove(hps, &ptl);

                  //          ptl.y=rcl->yTop-cyChar;
                  //          GpiMove(hps, &ptl);
                  //          GpiSetColor(hps, CLR_RED);

                  GpiCharString(hps, lLength, chrText);

                }
              /* No, too long for a whole line so start writing in current line and wrap */
              /*

                FIXME !!!!!!!!!!!!!!!!!!!!!!!!!!!

               */
            }
          else {
            //            DosBeep(5000, 500);
            //            GpiSetColor(hps, CLR_GREEN);
            GpiCharString(hps, lLength, chrText);
          }
          hTemp=malloc(sizeof(htmlPart));
          hTemp->iType=lMark;
          WinSetRect(WinQueryAnchorBlock(HWND_DESKTOP), &hTemp->rcl,
                     ptl.x, ptl.y,
                     ptl.x+lPtl, ptl.y+cyChar );
          hTemp->ptl=ptl;
          hTemp->chrText=chrText;
          hTemp->iLength=lLength;
          hTemp->ulParam=tagInfo->ulParam;
          addToHtmlPartList(hTemp);
          lDone+=lLength;
          // printLong(lDone, "lDone");
          chrText+=lLength;
          //  printMsg(chrText);
          /* Get next word */
          lLength=newStringLength( hps, lTotalLength-lDone, chrText);
          //    printLong(lLength, "lLength");
        }
      //      printMsg("");
      return lDone;
    }
  return 0;
}


#if 0
void drawText(HPS hps, char *text, RECTL *rcl)
{
  LONG lTotalDrawn;
  LONG cyChar=10;
  LONG lLength;
  LONG lDrawn;
  LONG lMark;
  LONG  lTotalLength=strlen(text);
  char *chrPos=text;
  char *chrEnd;
  LONG lDone=0;
  POINTL ptl;
  char text2[1000];

  lMark=MARK_TEXT;

  while(lDone<=lTotalLength && *chrPos!=0)
    {
      /* Check if starting with a mark */
      if(*chrPos=='<')
        chrPos=parseMark(lLength, chrPos, &lMark); /* Start of real text in chrPos */

      /* Get next mark */
      chrEnd=findNextMark(chrPos);
      //      printMsg(chrEnd);
      lLength=chrEnd-chrPos; /* Length of text to be drawn */
      //printLong(lLength);
      strcpy(text2, chrPos);
      //      text2[lLength-1]=0;
      //      printMsg(text2);

      lDone+=drawText2(hps, lLength, chrPos, rcl, lMark, lDone, DT_WORDBREAK);
      //chrPos+=lLength;
      chrPos=chrEnd;
      //DosSleep(500);

    } /* while */
  printLong(iNum, "Anzahl: ");
}
#endif


#endif
