/*
 * mmplayer.c (C) Chris Wohlgemuth 2002-2003
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
 * If you need another license for your project/product (commercial,
 * noncommercial, whatever) contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 *
 */

#define INCL_DOS
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_WINWORKPLACE
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_MCIOS2
#define INCL_GPI
#define INCL_PM

#include <os2.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "os2me.h"
#include "common.h"
#include "mmplayerres.h"
#include "mmres.h"
#include "mmplayer.h"
#include "mmprogs_defaults.h"
#include "sys_funcs.h"

#ifndef OPEN_DEFAULT 
/*  Open views for the wpOpen() method */

      #define OPEN_UNKNOWN      -1
      #define OPEN_DEFAULT       0
      #define OPEN_CONTENTS      1
      #define OPEN_SETTINGS      2
      #define OPEN_HELP          3
      #define OPEN_RUNNING       4
      #define OPEN_PROMPTDLG     5
      #define OPEN_PALETTE       121                                   /*SPLIT*/
      #define OPEN_USER          0x6500
#endif

LOADEDBITMAP allBMPs[NUM_CTRL_IDX];

/* Format:
   typedef struct
   {
   INT id;
   RECTL rclSource;
   HBITMAP hbmSource;
   RECTL rclDest;
   }CONTROLINFO;
   */
CONTROLINFO ciControls[NUM_CTRL_IDX]={
  {0,{0,0,414,70}, NULLHANDLE,{0}}, /* main */
  /* Achtung destination verwendet fuer position */

  {IDPB_MMPLAYERPLAY, {0, 0, 36, 36},NULLHANDLE, { 108, 24, 144, 60} },/* Play */
  {IDPB_MMPLAYERPLAY, {0, 0, 36,36},NULLHANDLE, { 108, 24, 144, 60} }, /* Playsel */
  {IDPB_MMPLAYERSTOP, {0, 0, 36, 36},NULLHANDLE, { 24, 24, 60, 60} },/* Stop */
  {IDPB_MMPLAYERSTOP, {0, 0, 36,36},NULLHANDLE, { 24, 24, 60, 60} }, /* Stop sel */
  {IDPB_MMPLAYERPAUSE, {0, 0, 36, 36},NULLHANDLE, { 66, 24, 101, 60} },/* Pause */
  {IDPB_MMPLAYERPAUSE, {0, 0, 36,36},NULLHANDLE, { 66, 24, 101, 60} }, /* Pause sel */
  {IDSL_MMPLAYERPOSBAR,{191-5, 19-4, 389-5,32-4}, NULLHANDLE, { 191, 19, 389, 32} },  //NULLHANDLE, { 0, 0, 191,10} },
  {IDPB_MMPLAYERSLIDERARM,{0,1,9,8},NULLHANDLE, {0,0,9,9} },
  {IDST_MMPLAYERPLAYTIME, {191, 32, 391,54},NULLHANDLE, { 0, 0, 190, 25} },
  {IDSL_MMPLAYERVOLUME,{15, 2,141,17},NULLHANDLE, {20, 6, 146, 21} }, //{0, 0, 126, 9} /* {10,18,93,13} */ },
  {IDSL_MMPLAYERVOLUMEARM,{0,0,9,9},NULLHANDLE, {0,0,9,9}},
  {IDSL_MMPLAYERVOLUMEARM,{0,0,9,9},NULLHANDLE, {0,0,9,9}},
};


#define NUMPARAMS  2

/* argv[0]: progname
 * argv[1]: audio file
 * ( argv[2]: class )
 */

//#define DEBUG
#ifdef DEBUG
void HlpWriteToTrapLog(const char* chrFormat, ...);
#endif

BOOL IniSaveWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniRestoreWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
int IniRestoreInt(char * iniFile, char* chrApp, char *chrKey, int defaultInt);
BOOL IniSaveInt(char * iniFile, char* chrApp, char *chrKey, int theInt);
BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize);
void freeResHandle();
HMODULE queryResModuleHandle(char *chrExePath);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
//MRESULT menuInsertMenuSeparator(int iPosition, HWND hwndMenu, HWND hwndSubMenu);
MRESULT menuInsertMenuSeparator(HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition );
//MRESULT menuInsertMenuItem(int iPosition, HWND hwndMenu, HWND hwndSubMenu, int iID, char * chrText);
SHORT menuInsertMenuItem( HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition, int iID, char * chrText);
void pmUsage();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);


MRESULT EXPENTRY playTimeTextProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT drawSlider(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2, USHORT id);
MRESULT EXPENTRY privateSliderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL paintHilitedControl(HWND hwnd, USHORT usControl, BOOL bHilite);
MRESULT paintControls(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY bgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY buttonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

/* Every folder has one running play thread to handle play commands */
void _Optlink playThreadFunc (void *arg);
void controlPlaying(HWND hwndDialog, ULONG ulAction, ULONG ulReserved);
BOOL stopAudioFile(HWND hwndFrame);
BOOL seekAudioFile(LONG lPosSec);
void showPlayTimeDisplay(HWND hwndFrame, BOOL bShow);
void togglePlayTimeDisplayVisibility(HWND hwndFrame);
void setPlayTimeText(HWND hwndFrame);

BOOL writeVolumeToIni(char * iniFile, char* chrApp, char *chrKey, ULONG ulVoume);
int readVolumeFromIni(char * iniFile, char* chrApp, char *chrKey, int iDefault);
BOOL writeDisplayTypeToIni(char * iniFile, char* chrApp, char *chrKey, int iDisplay);
int readDisplayTypeFromIni(char * iniFile, char* chrApp, char *chrKey, int iDefault);

char logName[]="mmplayer.log";

extern SWP swpWindow;
BOOL bHaveWindowPos=FALSE;

/* Track to play. Full path */
char chrSourceName[CCHMAXPATH]={0};
char * chrPtrName=NULLHANDLE; /* Pointer to filename */
char chrClassName[100]= {0};

char chrIniFile[CCHMAXPATH];

int numArgs;
char* params[NUMPARAMS];

/* TRUE when track is playing */
BOOL bIsPlaying=FALSE;
/* TRUE when track is paused */
BOOL bPaused=FALSE;
/* TRUE when user drags the position slider */
BOOL bPosDragging=FALSE;
BOOL bIsMidi=FALSE;

/* Which display to show */
/* 0: time, 1: remaining, 2: filename */
int iWhichDisplay=0;
/* Display timer for changing between track info */
ULONG ulDisplayTimer=0;

ULONG ulVolume=75;

PID ulPid;
/* The play time of the track */
ULONG ulTotalLength=0;
/* Current time in track */
ULONG ulPos=0;

HMODULE RESSOURCEHANDLE=0;

HMODULE BMP_RESSOURCEHANDLE=0;

PFNWP  oldPlayTimeProc;
PFNWP orgBgProc=NULLHANDLE; /* Org proc of background */
PFNWP orgButtonProc=NULLHANDLE;

HPOINTER hptrArrowWE=NULLHANDLE;

void HlpContextMenu(HWND hwnd, USHORT usItem, BOOL fCheckItem)
{
  POINTL ptl;
  HWND   hwndPopup;

  //  hwndPopup = WinLoadMenu(hwnd, _queryResModuleHandle(), ID_MENUTIMECONTEXT) ;
  hwndPopup = WinLoadMenu(hwnd, RESSOURCEHANDLE, ID_MENUTIMECONTEXT) ;
  if (hwndPopup == NULLHANDLE) {
    DosBeep(100,1000);
    return;
  }
  if(fCheckItem)
    WinSendMsg(hwndPopup,MM_SETITEMATTR,MPFROM2SHORT(usItem, TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED));

  WinQueryPointerPos(HWND_DESKTOP, &ptl) ;
  WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
  WinPopupMenu(hwnd, hwnd, hwndPopup, 
               ptl.x, ptl.y, 0, PU_HCONSTRAIN | PU_VCONSTRAIN |
               PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE  ); 
  
}

static void _loadBmps()
{
  HPS hps;
  HMODULE hResource;

  if(allBMPs[MAIN_BMP_IDX].hbm)
    return;

  hResource=BMP_RESSOURCEHANDLE; //_queryResModuleHandle();
  hps=WinGetPS(HWND_DESKTOP);
  
  /* The main BMP */
  allBMPs[MAIN_BMP_IDX].hbm=GpiLoadBitmap(hps, hResource, IDBMP_TOPMIDDLE, 0, 0);
  GpiQueryBitmapParameters(allBMPs[MAIN_BMP_IDX].hbm, &allBMPs[MAIN_BMP_IDX].bmpInfoHdr);

  /* Stop */
  allBMPs[CTRLIDX_STOPBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_STOP, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_STOPBUTTON].hbm, &allBMPs[CTRLIDX_STOPBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_STOPBUTTON].hbmSource=allBMPs[CTRLIDX_STOPBUTTON].hbm;

  /* Stop sel */
  allBMPs[CTRLIDX_STOPBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_STOPSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_STOPBUTTONSEL].hbm, &allBMPs[CTRLIDX_STOPBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_STOPBUTTONSEL].hbmSource=allBMPs[CTRLIDX_STOPBUTTONSEL].hbm;

  /* Pause */
  allBMPs[CTRLIDX_PAUSEBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PAUSE, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PAUSEBUTTON].hbm, &allBMPs[CTRLIDX_PAUSEBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_PAUSEBUTTON].hbmSource=allBMPs[CTRLIDX_PAUSEBUTTON].hbm;

  /* Pause sel */
  allBMPs[CTRLIDX_PAUSEBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PAUSESEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PAUSEBUTTONSEL].hbm, &allBMPs[CTRLIDX_PAUSEBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_PAUSEBUTTONSEL].hbmSource=allBMPs[CTRLIDX_PAUSEBUTTONSEL].hbm;

  /* Play */
  allBMPs[CTRLIDX_PLAYBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PLAY, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PLAYBUTTON].hbm, &allBMPs[CTRLIDX_PLAYBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_PLAYBUTTON].hbmSource=allBMPs[CTRLIDX_PLAYBUTTON].hbm;

  /* Play sel */
  allBMPs[CTRLIDX_PLAYBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PLAYSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PLAYBUTTONSEL].hbm, &allBMPs[CTRLIDX_PLAYBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_PLAYBUTTONSEL].hbmSource=allBMPs[CTRLIDX_PLAYBUTTONSEL].hbm;

  /* Positionslider bg */
  ciControls[CTRLIDX_POSSLIDER].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Positionslider arm */
  allBMPs[CTRLIDX_POSSLIDERARM].hbm=GpiLoadBitmap(hps, hResource, IDBMP_POSSLIDERARM, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_POSSLIDERARM].hbm, &allBMPs[CTRLIDX_POSSLIDERARM].bmpInfoHdr);
  ciControls[CTRLIDX_POSSLIDERARM].hbmSource=allBMPs[CTRLIDX_POSSLIDERARM].hbm;

  /* Playtime text field */
  ciControls[CTRLIDX_PLAYTIME].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Volume slider background */
  ciControls[CTRLIDX_VOLSLIDER].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Volume slider arm */
  allBMPs[CTRLIDX_VOLSLIDERARM].hbm=GpiLoadBitmap(hps, hResource, IDBMP_VOLSLIDERARM, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_VOLSLIDERARM].hbm, &allBMPs[CTRLIDX_VOLSLIDERARM].bmpInfoHdr);
  ciControls[CTRLIDX_VOLSLIDERARM].hbmSource=allBMPs[CTRLIDX_VOLSLIDERARM].hbm;

  /* Volume slider arm selected*/
  allBMPs[CTRLIDX_VOLSLIDERARMSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_VOLSLIDERARMSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_VOLSLIDERARMSEL].hbm, &allBMPs[CTRLIDX_VOLSLIDERARMSEL].bmpInfoHdr);
  ciControls[CTRLIDX_VOLSLIDERARMSEL].hbmSource=allBMPs[CTRLIDX_VOLSLIDERARMSEL].hbm;

  WinReleasePS(hps);
}


/***********************************************************/
/*                                                         */
/* Move the controls to the position defined by the        */
/* background bitmap.                                      */
/*                                                         */
/***********************************************************/
void adjustDialogControlPos(HWND hwnd)
{
  CONTROLINFO ci;
  ULONG ulStyle;

  /* STOP button */
  ci=ciControls[CTRLIDX_STOPBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id), HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);
  /* Pause button */
  ci=ciControls[CTRLIDX_PAUSEBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);
  /* Play button */
  ci=ciControls[CTRLIDX_PLAYBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);

  /* Playtime text */
  ci=ciControls[CTRLIDX_PLAYTIME];
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclSource.xLeft,ci.rclSource.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE|SWP_SIZE); 
  oldPlayTimeProc=WinSubclassWindow(WinWindowFromID(hwnd, IDST_MMPLAYERPLAYTIME), playTimeTextProc);

  /* Pos slider */
  ci=ciControls[CTRLIDX_POSSLIDER];//ciPosSlider;
  WinCreateWindow(hwnd, SLIDER_CLASS, "", WS_VISIBLE | WS_TABSTOP, 0,0, 10, 10, 
                  hwnd, HWND_TOP, ci.id, NULLHANDLE, NULLHANDLE);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft, ci.rclDest.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE | SWP_SIZE);
  
  WinSendMsg(WinWindowFromID(hwnd, ci.id),SLM_SETSLIDERINFO,
             MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,0),
             MPFROM2SHORT( 9, 9)); 

  /* Vol slider */
  ci=ciControls[CTRLIDX_VOLSLIDER];//ciVolSlider;
  WinCreateWindow(hwnd, SLIDER_CLASS, "", WS_VISIBLE | WS_TABSTOP, 0,0, 10, 10, 
                  hwnd, HWND_TOP, ci.id, NULLHANDLE, NULLHANDLE);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft, ci.rclDest.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE | SWP_SIZE);
  WinSendMsg(WinWindowFromID(hwnd, ci.id),SLM_SETSLIDERINFO,
             MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,0),
             MPFROM2SHORT( 9, 9)); 

}


void playTimeCallback(HWND hwndFrame, LONG lPosSec)
{
  char chrCommand[50];
  char retMsg[20];
  ULONG rc;
  int iWavePriv;

  iWavePriv=ulPid;

  /* Timeout. Check if wave is still playing */
  sprintf(chrCommand,"STATUS wave%d MODE WAIT", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. Clean up. The Timer will be stopped in the method */
    stopAudioFile(hwndFrame); 
  }
  else {
    if(!stricmp(retMsg, "stopped")) {
      /* Audio file played. */
      stopAudioFile(hwndFrame); 
    }
    showPlayTimeDisplay(hwndFrame, TRUE);/* Ensure display is visible */
  }
}

USHORT DrgGetNumObjects(PDRAGINFO pDragInfo)
{
  if(DrgAccessDraginfo(pDragInfo))
    {
      USHORT usItems=DrgQueryDragitemCount(pDragInfo);


      DrgFreeDraginfo(pDragInfo);
      return usItems;
    }
  
  return 0;
}

BOOL DrgVerifyTypeCW(PDRAGITEM pDragItem, char * chrType)
{
  /* DrgVerifyType() seems not to work when several types are in the list?? 
     Dokumentation says, it works... */
  /* Only check for a single type for now */
  char text[CCHMAXPATH];

  DrgQueryStrName(pDragItem->hstrType,sizeof(text), text); /* May be MP3, WAV etc... */

  if(strstr(text, chrType))
    return TRUE;

  return FALSE;
}

/* Check if we know the file type dragged */
BOOL DrgCheckForKnownType(PDRAGINFO pDragInfo  , HWND hwnd )
{
  BOOL rc=FALSE;

  if(DrgAccessDraginfo(pDragInfo))
    {
      PDRAGITEM pDragItem;
      //      char text[100];

      pDragItem=DrgQueryDragitemPtr(pDragInfo, 0);
      //    DrgQueryStrName(pDragItem->hstrType,sizeof(text), text); /* May be MP3, WAV etc... */

      //  WinSetWindowText(hwnd, text);
      bIsMidi=FALSE;
      if(DrgVerifyTypeCW(pDragItem, "Digital Audio")) /* This should work for audio files added using the MM configuration */
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "WAV"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "MP3"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "MIDI")) {
        rc=TRUE;
        bIsMidi=TRUE;
      }
      else if(DrgVerifyTypeCW(pDragItem, "OGGS"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "VOC"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "IFF"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "AIF"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "AU"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "SND"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "_AU"))
        rc=TRUE;
      else if(DrgVerifyTypeCW(pDragItem, "FLAC"))
        rc=TRUE;
      DrgFreeDraginfo(pDragInfo);
    }

  return rc;
}

#if 0
#define HASHSIZE 101
/* This is poor mans HASH function... */
USHORT calculateHash(char * theString)
{
  ULONG ulHash=0;

  if(theString)
    for(ulHash=0;*theString!='\0'; theString++)
      ulHash=*theString+37*ulHash;
  
  return ulHash%HASHSIZE;
}

/*
  Hashes:

  AIF:  0x30
  AU:   0x42
  WAV:  0x5b
  MP3:  0x33
  MIDI: 0x4c
  OGGS: 0x58
  IFF:  0x52
  SND:  0x1b
  VOC:  0x1d
  _AU:  0x20
 */
BOOL DrgCheckForKnownType(PDRAGINFO pDragInfo /* , HWND hwnd */)
{
  BOOL rc=FALSE;

return FALSE;
  if(DrgAccessDraginfo(pDragInfo))
    {
      PDRAGITEM pDragItem;
      char text[100];
      USHORT usHash;

      pDragItem=DrgQueryDragitemPtr(pDragInfo, 0);
      DrgQueryStrName(pDragItem->hstrType,sizeof(text), text); /* May be MP3, WAV etc... */

      usHash=calculateHash(text);
      //sprintf(text,"%s, %x", text, usHash);
      //   WinSetWindowText(hwnd, text);
      switch(usHash)
        {
        case  0x30:
        case  0x42:
        case  0x5b:
        case  0x33:
        case  0x4c:
        case  0x58:
        case  0x52:
        case  0x1b:
        case  0x1d:
        case  0x21:
          rc=TRUE;
        default:
          break;
        }

      /* FIXME: Ugly Hack for MP3 files.
         Some versions of the MP3 IO proc scan the whole file during identifying.
         This is bad during dragging. */
      /* Check if type contains MP3 type. There may be different types in the string */
      if(!rc)
        if(strstr(strupr(text), "MP3"))
          rc=TRUE;

      DrgFreeDraginfo(pDragInfo);
    }


  return rc;
}
#endif


MRESULT handleDrag0ver(PDRAGINFO pdi, HWND hwnd)
{
  
  /* Only accept one file */
  if(DrgGetNumObjects(pdi)!=1) {
    return MRFROM2SHORT(DOR_NODROP, DO_LINK);
  }
  /* Check if the file is a known type. that's way faster than using mmioIdentifyFile() to check */
  if(DrgCheckForKnownType(pdi, hwnd))
    return MRFROM2SHORT(DOR_DROP, DO_COPY);
  
  return MRFROM2SHORT(DOR_NEVERDROP, DO_COPY);  
}

MRESULT handleDrop(PDRAGINFO pdi, HWND hwnd)
{

  /* Stop current track (if any) */
  paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY), IDPB_MMPLAYERPLAY, FALSE);
  paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPAUSE), IDPB_MMPLAYERPAUSE, FALSE);
  controlPlaying(hwnd, STOP_TRACK, 0);
  /* Get file name */
  if(DrgAccessDraginfo(pdi))
    {
      char chrFile[CCHMAXPATH];
      PDRAGITEM pDragItem;
      
      pDragItem=DrgQueryDragitemPtr(pdi, 0);
      DrgQueryStrName(pDragItem->hstrSourceName,sizeof(chrFile), chrFile); 
      DrgQueryStrName(pDragItem->hstrContainerName,sizeof(chrSourceName), chrSourceName); 
      strcat(chrSourceName, chrFile);
      
      /* Start new file */
      controlPlaying(hwnd, PLAY_FIRST, 0);
      DrgFreeDraginfo(pdi);
    }
  return MRFALSE;
}

/****************************************************************
 *  FixSysMenu procedure
 *--------------------------------------------------------------
 *
 *  Name:    FixSysMenu(HWND hwndDlg)
 *
 *  Purpose: This routine removes the Restore, Size, Minimize, and
 *           Maximize options from the system menu of a dialog.
 *
 *  Usage
 *
 *  Method:  Called during the WM_INITDLG of a dialog procedure.
 *
 *  Returns: VOID
 *
 ****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
  HWND hwndMenu;                      /* Handle to system menu */
  MENUITEM mi={0};

  hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);


  WinSendMsg(hwndMenu,  /* Delete Restore from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_RESTORE, TRUE),
             MPFROMP(NULL));

  WinSendMsg(hwndMenu,  /* Delete Size from the system menu    */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_SIZE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu, /* Delete Minimize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_MINIMIZE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_MAXIMIZE, TRUE),
             MPFROMP(NULL));

  WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_HIDE, TRUE),
             MPFROMP(NULL));

  if(WinSendMsg(hwndMenu, /* Get sys menu menuitem  */
             MM_QUERYITEM,
             MPFROM2SHORT(SC_SYSMENU, FALSE),
             MPFROMP(&mi)))
    {
      char chrText[100];
      menuInsertMenuSeparator( mi.hwndSubMenu, NULLHANDLE, MIT_END);
      if(!getMessage(chrText,  IDSTR_MASTERVOLUMEITEM, sizeof(chrText), RESSOURCEHANDLE, hwndMenu))
         strcpy(chrText, "Master ~Volume");
      menuInsertMenuItem( mi.hwndSubMenu, NULLHANDLE, MIT_END, ID_MENUITEMMASTERVOLUME, chrText);
    }
  return;
}


/* This Proc handles the main dialog */
MRESULT EXPENTRY decodeStatusDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  //  char text[CCHMAXPATH*4 +10];
  //  char title[CCHMAXPATH*4];
  SWCNTRL swctl;
  PID pid;

  switch (msg)
    {      
    case DM_DRAGOVER:
      {
      PDRAGINFO pdi=PVOIDFROMMP(mp1);

      return handleDrag0ver( pdi, hwnd);

      break;
      }
    case DM_DROP:
      {
        PDRAGINFO pdi=PVOIDFROMMP(mp1);

        return handleDrop( pdi, hwnd);
      }
    case WM_INITDLG:
      {
        IniRestoreWindowPos(chrIniFile, INI_MMPLAYER_APP, INI_WINDOWPOS_KEY, hwnd);

        /* Add switch entry */
        memset(&swctl,0,sizeof(swctl));
        WinQueryWindowProcess(hwnd,&pid,NULL);
        swctl.hwnd=hwnd;
        swctl.uchVisibility=SWL_VISIBLE;
        swctl.idProcess=pid;
        swctl.bProgType=PROG_DEFAULT;
        swctl.fbJump=SWL_JUMPABLE;
        WinAddSwitchEntry(&swctl);
        
        adjustDialogControlPos(hwnd);
        
        /* Set volume slider*/
        WinSendMsg( WinWindowFromID(hwnd, IDSL_MMPLAYERVOLUME),
                    SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMLONG( ulVolume));
        
        /* Subclass buttons for DnD */
        orgButtonProc=WinSubclassWindow(WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY), buttonProc);
        WinSubclassWindow(WinWindowFromID(hwnd, IDPB_MMPLAYERPAUSE), buttonProc);
        WinSubclassWindow(WinWindowFromID(hwnd, IDPB_MMPLAYERSTOP), buttonProc);
                
        FixSysMenu(hwnd);

        /* Size dialog */
        if(allBMPs[MAIN_BMP_IDX].hbm) {
          SWP swp;
          
          WinQueryWindowPos(WinWindowFromID(hwnd, FID_TITLEBAR), &swp);         
          WinSetWindowPos(WinWindowFromID(hwnd, ID_CLIENT), NULLHANDLE, 
                          WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME), 
                          WinQuerySysValue(HWND_DESKTOP, SV_CYDLGFRAME), 
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx,
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy,
                          SWP_SIZE|SWP_MOVE);
          orgBgProc=WinSubclassWindow(WinWindowFromID(hwnd, ID_CLIENT), bgProc);
          
          WinSetWindowPos(hwnd, NULLHANDLE, 0, 0,
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx+WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME)*2,
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy+swp.cy+WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME)*2
                          , SWP_SIZE);
        }
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(WinWindowFromID(hwnd, IDST_MMPLAYERPLAYTIME),
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY));
        WinStartTimer(WinQueryAnchorBlock(HWND_DESKTOP), hwnd, IDTIMER_START_PLAYING, START_TIMEOUT);
        return (MRESULT) TRUE;
      }
    case WM_CLOSE:
      controlPlaying(hwnd, STOP_TRACK, 0);
      IniSaveWindowPos(chrIniFile, INI_MMPLAYER_APP, INI_WINDOWPOS_KEY, hwnd);
      writeVolumeToIni(chrIniFile, INI_MMPLAYER_APP, INI_VOLUME_KEY, ulVolume);
      writeDisplayTypeToIni(chrIniFile, INI_MMPLAYER_APP, INI_DISPLAY_KEY, iWhichDisplay);
      //      WinSendMsg(hwndPlayObject, WM_CLOSE, 0, 0);
      break;
      /*****************************************************/
      /* Handle playtime display switching.                */
      /* The messages are coming from the static text.     */
      /*****************************************************/
    case WM_APPTERMINATENOTIFY:
      
      switch(SHORT1FROMMP(mp1))
        {
        case NOTIFY_CLICK:
          /* Clicked on the playtime display */
          iWhichDisplay++;
          if(iWhichDisplay>2)
            iWhichDisplay=0;
          if(iWhichDisplay==2)
            ulDisplayTimer=0;
          setPlayTimeText(hwnd);
          return MRFALSE;
        case NOTIFY_CONTEXT:
          /* Show context menu. */            
          HlpContextMenu(hwnd, iWhichDisplay+ID_MENUITEMELAPSED, TRUE);
          return MRFALSE;
        default:
          break;
        }
      return MRFALSE;
      
      /*****************************************************/
      /* The following two messages are for device sharing */
      /*****************************************************/
    case WM_ACTIVATE:
      if(SHORT1FROMMP(mp1)){
        char chrCommand[100];
        int iWavePriv;
    
        iWavePriv=ulPid;

        if(bIsPlaying) {
          sprintf(chrCommand,"ACQUIRE wave%d QUEUE wait", iWavePriv);
          mciSendString(chrCommand, NULL, 0, 0, 0);
        }
      }
      break;

    case MM_MCIPASSDEVICE:
      {
        switch(SHORT1FROMMP(mp2))
          {
          case MCI_GAINING_USE:
            bPaused=FALSE;/* The time display will not be increased */
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPAUSE), IDPB_MMPLAYERPAUSE, FALSE);
            return (MRESULT)0;
          case MCI_LOSING_USE:
            bPaused=TRUE;
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY), IDPB_MMPLAYERPLAY, FALSE);
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
        if(!bPaused) {
          /* Adjust position info */
          ulPos++;
          if(ulPos*1000>ulTotalLength)
            ulPos=ulTotalLength/1000;/* Prevent wrong time if timer info isn't accurate.
                                                          I know all this timing stuff is an ugly thing. */


          if(!bPosDragging) {
            ULONG ulInc=ulTotalLength/1000;
            setPlayTimeText(hwnd); /* Set time display */
            /* Catch potential division by zero trap */
            if(ulInc)
              ulInc=ulPos*100/ulInc;
            else
              ulInc=100;
            /* Set slider position */

            WinPostMsg( WinWindowFromID(hwnd, IDSL_MMPLAYERPOSBAR), 
                        SLM_SETSLIDERINFO,
                        MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                        MPFROMLONG(ulInc));

          }

          playTimeCallback(hwnd, ulPos);
          /* Blink play control */
          paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY), IDPB_MMPLAYERPLAY, ulPos%2);
        }
        else {
          /* Currently pausing */
          togglePlayTimeDisplayVisibility(hwnd);
          paintHilitedControl(  WinWindowFromID(hwnd, IDPB_MMPLAYERPAUSE), IDPB_MMPLAYERPAUSE,
                                WinIsWindowVisible(WinWindowFromID(hwnd, IDST_MMPLAYERPLAYTIME)));
        }
        return (MRESULT)0;
      }
      else if(SHORT1FROMMP(mp1)=IDTIMER_START_PLAYING)
        {
          WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, IDTIMER_START_PLAYING);
          controlPlaying(hwnd, PLAY_FIRST, 0);
          return (MRESULT)0;
        }
      break;

      /* The play control handling for starting, stopping etc. */
    case WM_COMMAND:
      {
        switch(SHORT1FROMMP(mp1))
          {
          case IDPB_MMPLAYERPLAY:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPAUSE), IDPB_MMPLAYERPAUSE, FALSE);
            controlPlaying(hwnd, PLAY_FIRST, 0);
            break;
          case IDPB_MMPLAYERSTOP:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY), IDPB_MMPLAYERPLAY, FALSE);
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPAUSE), IDPB_MMPLAYERPAUSE, FALSE);
            controlPlaying(hwnd, STOP_TRACK, 0);
            break;
          case IDPB_MMPLAYERPAUSE:
            paintHilitedControl(WinWindowFromID(hwnd, IDPB_MMPLAYERPLAY), IDPB_MMPLAYERPLAY, FALSE);
            controlPlaying(hwnd, PAUSE_TRACK, 0);
            break;
            /* Popup menu over time control */
          case ID_MENUITEMELAPSED:
            iWhichDisplay=DISPLAY_TIMEELAPSED;
            setPlayTimeText(hwnd);
            break;
          case ID_MENUITEMREMAINING:
            iWhichDisplay=DISPLAY_TIMEREMAINING;
            setPlayTimeText(hwnd);
            break;
          case ID_MENUITEMID3:
            iWhichDisplay=DISPLAY_TIMEID3;
            ulDisplayTimer=0;
            setPlayTimeText(hwnd);
            break;
          case ID_MENUITEMMASTERVOLUME:
            {
              HOBJECT hObject;
              /* User selected master volume item from sys menu */
              if((hObject=WinQueryObject("<MMPM2_MMVOLUME>"))!=NULLHANDLE) {
                WinOpenObject(hObject,OPEN_DEFAULT,TRUE);
              }
              break;
            }
          default:
            break;
          }/* switch() */

      return (MRESULT)FALSE;
      }/* case */

      /*****************************************************/
      /* Draw the sliders.                                 */
      /*****************************************************/
    case WM_DRAWITEM:
      switch(SHORT1FROMMP(mp1))
        {
        case IDSL_MMPLAYERPOSBAR:
        case IDSL_MMPLAYERVOLUME:
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
                  case IDSL_MMPLAYERVOLUME:
                    {
                      ULONG ulValue;

                      ulValue=LONGFROMMP(mp2);
                      ulVolume=ulValue;
                      sprintf(chrCommand,"SET wave%ld AUDIO VOLUME %ld WAIT", ulPid, ulVolume);
                      mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
                      break;
                    }
                  case IDSL_MMPLAYERPOSBAR:
                    {
                      static long lLastPos=0;
                      long lPos=LONGFROMMP(mp2);
              
                      bPosDragging=TRUE;
                      lPos*=ulTotalLength/100;
                      if(lPos/1000!=lLastPos) {
                        /* Prevent flickering */
                        sprintf(chrCommand, "%d:%02d", lPos/1000/60, (lPos/1000)%60);
                        WinSetWindowText(WinWindowFromID(hwnd, IDST_MMPLAYERPLAYTIME), chrCommand);
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
                  case IDSL_MMPLAYERVOLUME:
                    {
                      ULONG ulValue;
                      
                      ulValue=LONGFROMMP(mp2);
                      ulVolume=ulValue;
                      sprintf(chrCommand,"SET wave%ld AUDIO VOLUME %ld WAIT", ulPid, ulVolume);
                      mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
                      break;
                    }
                  case IDSL_MMPLAYERPOSBAR:
                    {
                      long lPos=LONGFROMMP(mp2);

                      bPosDragging=FALSE;
                      lPos*=1000;                      
                      lPos*=ulTotalLength/100;
                      lPos/=1000;// Use only full seconds

                      /* Set time display first */
                      seekAudioFile(lPos);
                      /* restart timer */
                      WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, IDTIMER_PLAY, PLAYTIMER_DELAY);  
                      ulPos=lPos/1000;
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
      }/* WM_CONTROL */
    default:
      break;
    }/* switch */
  
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}

BOOL cwGetStringFromEA( char * chrFileName, PSZ pKey, char * chrBuffer, ULONG ulBuffSize)
{
  LONG rc;                             /* Ret code                   */
  UCHAR       geabuff[300];            /* buffer for GEA             */
  PVOID       fealist;                 /* fealist buffer             */
  EAOP2       eaop;                    /* eaop structure             */
  PGEA2       pgea;                    /* pgea structure             */
  PFEA2       pfea;                    /* pfea structure             */
  HFILE       handle;                  /* file handle                */
  ULONG       act;                     /* open action                */
  ULONG ulSize;
  USHORT     *ptrUs;

  if((fealist=malloc(0x00010000L /* Add some space */
                     ))==NULLHANDLE)
    return FALSE;
  
  ulSize=sizeof(chrFileName);
  
  if ((rc = DosOpen(chrFileName, &handle, &act,
                    0L, 0, OPEN_ACTION_OPEN_IF_EXISTS,
                    OPEN_ACCESS_READONLY + OPEN_SHARE_DENYREADWRITE +
                    OPEN_FLAGS_FAIL_ON_ERROR + OPEN_FLAGS_WRITE_THROUGH,NULL))!=NO_ERROR) {
    free(fealist);    /* Free the memory            */
    return FALSE;
  }                                    /* get the file status info   */

                                       /* FEA and GEA lists          */
  eaop.fpGEA2List = (PGEA2LIST)geabuff;
  eaop.fpFEA2List = (PFEA2LIST)fealist;
  eaop.oError = 0;                     /* no error occurred yet      */
  pgea = &eaop.fpGEA2List->list[0];    /* point to first GEA         */
  eaop.fpGEA2List->cbList = sizeof(ULONG) + sizeof(GEA2) +
    strlen(pKey);
  eaop.fpFEA2List->cbList = (ULONG)0xffff;

                                       /* fill in the EA name length */
  pgea->cbName = (BYTE)strlen(pKey);
  strcpy(pgea->szName, pKey);          /* fill in the name           */
  pgea->oNextEntryOffset = 0;          /* fill in the next offset    */
                                       /* read the extended attribute*/
  rc = DosQueryFileInfo(handle, 3, (PSZ)&eaop, sizeof(EAOP2));
  DosClose(handle);                    /* close the file             */
  if (eaop.fpFEA2List->cbList <= sizeof(ULONG))
    rc = ERROR_EAS_NOT_SUPPORTED;      /* this is error also         */

  if (rc) {                            /* failure?                   */
    free(fealist);    /* Free the memory            */
    return FALSE;
  }

  pfea = &(eaop.fpFEA2List->list[0]);  /* point to the first FEA     */

  ptrUs=(USHORT*)((PSZ)pfea->szName+(pfea->cbName+1));
  if(*ptrUs!=0xfffd) {
    /* Not an ASCII EA */
    free(fealist);    /* Free the memory            */
    return FALSE;
  }
  ptrUs++; /* Move to length */
  ulSize=(ULONG)*ptrUs;

  ptrUs++; /* Move to value */
  if(ulSize==0) {
    /* Not an ASCII EA */
    free(fealist);    /* Free the memory            */
    return FALSE;
  }

  memset(chrBuffer,0,ulBuffSize);
  memcpy(chrBuffer, (char*)ptrUs, (ulSize>=ulBuffSize ? ulBuffSize-1 : ulSize));
  free(fealist);    /* Free the memory            */
  return TRUE;
}

int readVolumeFromIni(char * iniFile, char* chrApp, char *chrKey, int iDefault)
{
  return IniRestoreInt(iniFile, chrApp, chrKey, iDefault);
}

BOOL writeVolumeToIni(char * iniFile, char* chrApp, char *chrKey, ULONG ulVolume)
{
  
  return IniSaveInt(iniFile, chrApp, chrKey, ulVolume);
}

int readDisplayTypeFromIni(char * iniFile, char* chrApp, char *chrKey, int iDefault)
{
  return IniRestoreInt(iniFile, chrApp, chrKey, iDefault);
}

BOOL writeDisplayTypeToIni(char * iniFile, char* chrApp, char *chrKey, int iDisplay)
{
  return IniSaveInt(iniFile, chrApp, chrKey, iDisplay);
}

ULONG mmIdentifyFile(char* chrFile)
{
  MMIOINFO mmioInfo;
  MMFORMATINFO mmFormatInfo;
  FOURCC fourCC;


  if(MMIO_SUCCESS!=mmioIdentifyFile(chrFile, &mmioInfo, &mmFormatInfo, &fourCC, 0,0))
    {
      return 0; 
    }

  if(mmFormatInfo.ulMediaType==MMIO_MEDIATYPE_MIDI)
    return 1; /* Midi */
  
  /* Any other audio file */
  return 2;
}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  char text[CCHMAXPATH];
  char title[CCHMAXPATH];
  short a;
  ULONG rc;

  /* Create a copy of the args */
  /* argv[0]: progname
   * argv[1]: audio file
   */

  numArgs=argc;

  strcpy(text,"");
  for(a=0;a<argc;a++)
    {
      params[a]=argv[a];
    }

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      /* Check if user started prog by hand */   
      if(argc!=NUMPARAMS) {/* Not the right num of params */
        pmUsage();
      }
      else {
        PTIB ptib;
        PPIB ppib;

        /* Save source name */
        strcpy(chrSourceName, params[1]);
        //strcpy(chrSourceName, "H:\\cwmm\\classes\\mm-progs\\MMPlayer\\01 Vicious Circle - Propaganda - 1234.WAV");
        if((chrPtrName=strrchr(chrSourceName, '\\'))!=NULLHANDLE)
          ++chrPtrName;

        if((rc=mmIdentifyFile(chrSourceName))==0)
          {
            /* FIXME: Identify error !!!*/
            
          };
        
        if(rc==1)
          {
            /* It's a MIDI file */
            bIsMidi=TRUE;
          }
        else
          bIsMidi=FALSE;

        /* Class name. We need it to distinguish MIDI and audio files */
        if(argc==3)
          strcpy(chrClassName, params[1]);

        /* Get our ressource dll */  
        //        RESSOURCEHANDLE=0;
        RESSOURCEHANDLE=queryResModuleHandle(argv[0]);
        /* Get data from INI file */
        HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));
        ulVolume=readVolumeFromIni(chrIniFile, INI_MMPLAYER_APP, INI_VOLUME_KEY, DEFAULT_VOLUME);
        iWhichDisplay=readDisplayTypeFromIni(chrIniFile, INI_MMPLAYER_APP, INI_DISPLAY_KEY, 0);

        /* Load pointer for slider */
        hptrArrowWE=WinLoadPointer(HWND_DESKTOP, BMP_RESSOURCEHANDLE /*_queryResModuleHandle()*/, IDPTR_ARROWWE);
        /* Register the slider window class */
        WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP), SLIDER_CLASS, privateSliderProc,0, SLADDBYTES);

        _loadBmps();        
        DosGetInfoBlocks(&ptib, &ppib);
        ulPid=ppib->pib_ulpid;
        _beginthread(playThreadFunc,NULL,8192*16, NULLHANDLE); //Fehlerbehandlung fehlt

        if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, decodeStatusDialogProc, 
                       RESSOURCEHANDLE, IDDLG_MMPLAYER, 0) == DID_ERROR )
          {
            /*
              Text:   
              Title:  "Installation problem"                       
              */             
            messageBox( text, IDSTR_CONVERTDIALOGERROR , sizeof(text),
                        title, IDSTR_INSTALLERRORTITLE , sizeof(title),
                        RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);
            freeResHandle();
            WinDestroyMsgQueue( hmq );
            WinTerminate( hab );
            DosBeep(100,600);
            return( 1 );
          }
      }
      freeResHandle();
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}

