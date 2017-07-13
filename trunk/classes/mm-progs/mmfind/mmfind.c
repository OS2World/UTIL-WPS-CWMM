/*
 * mmfind.c (C) Chris Wohlgemuth 2004-2005
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
#define INCL_REXXSAA

/* OS/2 and compiler includes */
#include <os2.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "os2me.h"
/* CW toolkit includes */
#include "sys_funcs.h"
#include "message_funcs.h"
#include "window_funcs.h"
#include "img_funcs.h"
#include "ini_funcs.h"
/* Program includes */
#include "common.h"
#include "mmres.h"                 /* Ressource IDs */
#include "mmprogs_defaults.h"
//#define DEBUG
#include "mmfind.h"                /* mmfind specific defines */

char logName[]="mmfind.log"; /* To keep some (unused) helpers in helper.c happy */

char chrIniFile[CCHMAXPATH];
char chrInstallDir[CCHMAXPATH];

char chrLastAudioDir[CCHMAXPATH];
char chrLastVideoDir[CCHMAXPATH];
char chrLastImageDir[CCHMAXPATH];

/***** For previewing audio tracks *****/
char chrSourceName[CCHMAXPATH]; /* The audio file to be played */
BOOL bIsMidi=FALSE;             /* No MIDI support atm */
BOOL bIsPlaying=FALSE;          /* TRUE if currently playing */
BOOL bPaused=FALSE;             
ULONG ulPos;
ULONG ulTotalLength;
HWND hwndPlayer;                /* HWND of the player object window used for async coommunication */

/***************************************/

#define NUMPARAMS  1
/*
  argv[0]: progname
 */
int numArgs;
char* params[NUMPARAMS];

HMODULE RESSOURCEHANDLE=0;

char chrMediaFileTypes[NUMMEDIATYPES][100];

char chrMediaFileExt[NUMMEDIATYPES][EXT_STRING_LENGTH]= {"*.AIF;*.AU;*.FLA;*.FLAC;*.IFF;*.MID;*.MP3;*.OGG;*.SND;*.VOC;*.WAV;*._AU",
                                          /* The image extensions are queried using the imageext.rx and imageadd.rx scripts */
                                          "*.BMP;*.DIB;*.GIF;*.JPG;*.PCX;*.PNG;*.PCD;*.TIF;*.TGA;*.VID;*._IM",
                                          "*.AVI;*.MPG;*.FLI;*.FLC;*.MOV;*.QT;*.VBS"};
/* Additional audio extensions to be queried during runtime */
char chrAdditionalExt[200];
/* This holds the template used when building the audio information text */
char audioText[255];

int iMediaType=MT_AUDIO;

/* BMP stuff for preview */
BITMAPINFOHEADER2 bmpInfoHeader2;
HBITMAP hBitmap;

/* HBITMAP for the play control buttons */
HBITMAP hbmPlay;
HBITMAP hbmStop;

FILEDLG fd = { 0 };

static void _loadButtonBmps()
{
  HPS hps;
  HMODULE hResource;

  hResource=NULLHANDLE;
  hps=WinGetPS(HWND_DESKTOP);

  /* Play */  
  hbmPlay=GpiLoadBitmap(hps, hResource, IDBMP_PLAY, 0, 0);

  /* Stop */
  hbmStop=GpiLoadBitmap(hps, hResource, IDBMP_STOP, 0, 0);

  WinReleasePS(hps);
}

static void changeButtons(HWND hwnd)
{
  HWND hwndTemp;
  BTNCDATA btCtrl;
  WNDPARAMS wndParams;
  ULONG ulStyle;

  if(hbmPlay) {
    hwndTemp=WinWindowFromID(hwnd, IDPB_MMFINDPLAY);

    /* Change Style to bitmap */
    ulStyle=WinQueryWindowULong(hwndTemp,QWL_STYLE);
    ulStyle|=(BS_BITMAP);
    ulStyle&=~BS_TEXT;
    
    if(WinSetWindowULong(hwndTemp, QWL_STYLE, ulStyle)) {
      memset(&btCtrl, 0, sizeof(btCtrl));
      btCtrl.cb=sizeof(btCtrl);
      btCtrl.hImage=hbmPlay;
      memset(&wndParams, 0, sizeof(wndParams));
      wndParams.fsStatus=WPM_CTLDATA;
      wndParams.cbCtlData=btCtrl.cb;
      wndParams.pCtlData=&btCtrl;
      WinSendMsg(hwndTemp,WM_SETWINDOWPARAMS,
                 MPFROMP(&wndParams),0);
    }
  }

  if(hbmStop) {
    hwndTemp=WinWindowFromID(hwnd, IDPB_MMFINDSTOP);
    /* Change Style to bitmap */
    ulStyle=WinQueryWindowULong(hwndTemp,QWL_STYLE);
    ulStyle|=(BS_BITMAP);
    ulStyle&=~BS_TEXT;
    
    if(WinSetWindowULong(hwndTemp, QWL_STYLE, ulStyle)) {
      memset(&btCtrl, 0, sizeof(btCtrl));
      btCtrl.cb=sizeof(btCtrl);
      btCtrl.hImage=hbmStop;
      memset(&wndParams, 0, sizeof(wndParams));
      wndParams.fsStatus=WPM_CTLDATA;
      wndParams.cbCtlData=btCtrl.cb;
      wndParams.pCtlData=&btCtrl;
      WinSendMsg(hwndTemp,WM_SETWINDOWPARAMS,
                 MPFROMP(&wndParams),0);
    }
  }
}

static char* translateChars(char * chrString, char chrFrom, char chrTo)
{
  char *chr=chrString;

  if(chrTo == '\0' || chrFrom=='\0'|| chrString==NULLHANDLE)
    return NULLHANDLE;

  while(*chr)
    {
      if(*chr==chrFrom)
        *chr=chrTo;

      chr++;
    }
  return chrString;
}

static void loadStringsFromResourceDLL(HMODULE hResMod)
{
  if(!MsgGetMessage(audioText, IDSTR_AUDIOINFOTEXT, sizeof(audioText), hResMod,HWND_DESKTOP))
    strcpy(audioText, TEXT_INFOTEXT);
  if(!MsgGetMessage(chrMediaFileTypes[MT_AUDIO], IDSTR_TYPEAUDIO, 
                 sizeof(chrMediaFileTypes[MT_AUDIO]), hResMod, HWND_DESKTOP))
    strcpy(chrMediaFileTypes[MT_AUDIO], TEXT_TYPEAUDIO);
  if(!MsgGetMessage(chrMediaFileTypes[MT_IMAGE], IDSTR_TYPEIMAGE, 
                 sizeof(chrMediaFileTypes[MT_IMAGE]), hResMod, HWND_DESKTOP))
    strcpy(chrMediaFileTypes[MT_AUDIO], TEXT_TYPEIMAGE);
  if(!MsgGetMessage(chrMediaFileTypes[MT_VIDEO], IDSTR_TYPEVIDEO, 
                 sizeof(chrMediaFileTypes[MT_VIDEO]), hResMod, HWND_DESKTOP))
    strcpy(chrMediaFileTypes[MT_VIDEO], TEXT_TYPEVIDEO);
}

static void showVideoControls(HWND hwnd, BOOL bShow)
{
  /* Preview area */
  WinShowWindow(WinWindowFromID(hwnd, IDST_NOPREVIEWAVAILABLE), bShow);
  /* Group box */
  WinShowWindow(WinWindowFromID(hwnd, IDGB_PREVIEW), bShow);
}


static void enableAudioPlayControls(HWND hwnd, BOOL bEnable)
{
    WinEnableWindow(WinWindowFromID(hwnd, IDPB_MMFINDPAUSE), bEnable);
    WinEnableWindow(WinWindowFromID(hwnd, IDPB_MMFINDPLAY), bEnable);
    WinEnableWindow(WinWindowFromID(hwnd, IDPB_MMFINDSTOP), bEnable);
}

void showAudioControls(HWND hwnd, BOOL bShow)
{
  /* Preview area */
  WinShowWindow(WinWindowFromID(hwnd, IDST_AUDIOINFO), bShow);
  /* Group box */
  WinShowWindow(WinWindowFromID(hwnd, IDGB_AUDIOINFO), bShow);
  /* Playcontrols */
  WinShowWindow(WinWindowFromID(hwnd, IDPB_MMFINDPLAY), bShow);
  WinShowWindow(WinWindowFromID(hwnd, IDPB_MMFINDSTOP), bShow);
  WinShowWindow(WinWindowFromID(hwnd, IDPB_MMFINDPAUSE), bShow);
  if(bShow) {
    WinSetWindowToTop(WinWindowFromID(hwnd, IDPB_MMFINDPAUSE));
    WinSetWindowToTop(WinWindowFromID(hwnd, IDPB_MMFINDPLAY));
    WinSetWindowToTop(WinWindowFromID(hwnd, IDPB_MMFINDSTOP));
    enableAudioPlayControls(hwnd, FALSE);
  }
}

BOOL pauseAudioFile(HWND hwndNotify)
{
#if 0
  char chrCommand[50];
  char retMsg[100];
  ULONG rc;

  iWavePriv=ulPid;

  if(bPaused) {
    sprintf(chrCommand,"RESUME wave%d wait", hwndNotify);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    bPaused=FALSE;
    showPlayTimeDisplay( hwndFrame, TRUE);
  }
  else {
    sprintf(chrCommand,"PAUSE wave%d wait", iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    bPaused=TRUE;
  }
#endif
  return TRUE;
}

BOOL stopAudioFile(HWND hwndNotify)
{
  char chrCommand[50];
  char retMsg[100];
  ULONG rc;

  if(bIsPlaying) {
    sprintf(chrCommand,"stop wave%ld wait", hwndNotify);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    
    sprintf(chrCommand,"close wave%ld wait", hwndNotify);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);

#if 0    
    _resetDisplay(hwndFrame);
    showPlayTimeDisplay( hwndFrame, TRUE);
    WinStopTimer(WinQueryAnchorBlock(hwndFrame), hwndFrame, IDTIMER_PLAY);
#endif
    bIsPlaying=FALSE;
    bPaused=FALSE;  
  }

  return TRUE;
}

ULONG playAudioFile(HWND hwndFrame)
{
  char chrCommand[CCHMAXPATH*2];
  char chrDevice[30];
  char retMsg[20];
  ULONG rc;
  int iTime2;
  HWND hwndNotify;

  hwndNotify=hwndFrame;//WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE);

  if(bPaused)
    {
      pauseAudioFile(hwndFrame);
      return 1;
    }

  ulPos=0;

  stopAudioFile(hwndFrame);

  if(bIsMidi)
    strncpy(chrDevice,"SEQUENCER", sizeof(chrDevice));
  else
    strncpy(chrDevice,"WAVEAUDIO", sizeof(chrDevice));

  /* Start audio file */  
  sprintf(chrCommand,"open \"%s\"  type %s alias wave%ld SHAREABLE wait", chrSourceName, chrDevice, hwndNotify);


  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), hwndNotify, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS)
    return 0;
  
  /* Set time format */
  sprintf(chrCommand,"SET wave%ld TIME FORMAT MILLISECONDS wait", hwndNotify);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%ld wait", hwndNotify);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }      

#if 0
  sprintf(chrCommand,"SETPOSITIONADVISE wave%d ON EVERY 1000 notify", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg),hwndNotify, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d wait",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }
#endif

  /* Get wave length in ms */
  iTime2=0;
  sprintf(chrCommand,"STATUS wave%ld LENGTH WAIT", hwndNotify);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)==MCIERR_SUCCESS) {
    iTime2=atoi(retMsg);
    sprintf(chrCommand,"%d:%02d  %d:%02d  -%d:%02d",iTime2/1000/60,(iTime2/1000)%60,
            0, 0,
            iTime2/1000/60,(iTime2/1000)%60);
  }
  ulTotalLength=iTime2;

#if 0
  /* Set volume */
  sprintf(chrCommand,"SET wave%ld AUDIO VOLUME %ld wait", hwndFrame, ulVolume);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }
#endif

  sprintf(chrCommand,"play wave%ld FROM 0", hwndNotify);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%ld", hwndNotify);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }

#if 0
  WinStartTimer(WinQueryAnchorBlock(hwndFrame), hwndFrame, IDTIMER_PLAY, PLAYTIMER_DELAY);
#endif

  bIsPlaying=TRUE;

  return 1;
}

static MRESULT EXPENTRY playObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch (msg)
    {
    case WM_APPTERMINATENOTIFY:
      {

        HWND hwndFrame=HWNDFROMMP(mp2);
        switch(LONGFROMMP(mp1))
          {
          case PLAY_FILE:
            playAudioFile(hwndFrame);
            break;
          case STOP_TRACK:
            stopAudioFile(hwndFrame);
            break;
          case PAUSE_TRACK:
            pauseAudioFile(hwndFrame);
            break;
          default:
            break;
          }
        return (MRESULT)0;
      }
    case WM_CLOSE:

      break;
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
  HWND hwndNotify=(HWND)arg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      HWND hwndPlayObject;
      hwndPlayObject=WinCreateWindow(HWND_OBJECT,WC_STATIC,"MMAudioPlayObj",
                                     0,0,0,0,0,NULLHANDLE,HWND_BOTTOM,13343,NULL,NULL);
      if(hwndPlayObject) {
        WinSubclassWindow(hwndPlayObject,&playObjectProc);
        /* Window created. */
        WinPostMsg(hwndNotify, WM_APPTERMINATENOTIFY, MPFROMSHORT(MMAUDIO_PLAYER_CREATED),
                   MPFROMHWND(hwndPlayObject));
        while(WinGetMsg(hab,&qmsg,(HWND)NULL,0,0))
          WinDispatchMsg(hab,&qmsg);

        WinDestroyWindow(hwndPlayObject);
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
}


void MMAudioPlayFile(HWND hwndPlayer, HWND hwndNotify, char* chrFile)
{
  WinPostMsg(hwndPlayer, WM_APPTERMINATENOTIFY, MPFROMSHORT(PLAY_FILE), MPFROMHWND(hwndNotify));
}

void MMAudioStop(HWND hwndPlayer, HWND hwndNotify)
{
  WinPostMsg(hwndPlayer, WM_APPTERMINATENOTIFY, MPFROMSHORT(STOP_TRACK), MPFROMHWND(hwndNotify));
}

void MMAudioPause(HWND hwndPlayer, HWND hwndNotify)
{
  WinPostMsg(hwndPlayer, WM_APPTERMINATENOTIFY, MPFROMSHORT(PAUSE_TRACK), MPFROMHWND(hwndNotify));
}

BOOL MMAudioOpenPlayer(HWND hwndNotify)
{
  if(-1==_beginthread(playThreadFunc,NULL,8192*16, (void*)hwndNotify))
    return FALSE; /* Error occured */

  return TRUE;
}

void MMAudioClosePlayer(HWND hwndPlayer)
{
  WinSendMsg(hwndPlayer, WM_QUIT, 0, 0);
  return;
}

static ULONG mmIdentifyFile(char* chrFile)
{
  MMIOINFO mmioInfo={0};
  MMFORMATINFO mmFormatInfo={0};
  FOURCC fourCC;

  if(MMIO_SUCCESS!=mmioIdentifyFile(chrFile, &mmioInfo, &mmFormatInfo, &fourCC, 0,0))
    {
      return MT_UNKNOWN; 
    }

  switch(mmFormatInfo.ulMediaType)
    {
    case MMIO_MEDIATYPE_MIDI:  /* Midi */
    case MMIO_MEDIATYPE_AUDIO: /* Any other audio file */
      return MT_AUDIO;
    case MMIO_MEDIATYPE_IMAGE:
      return MT_IMAGE;
    case MMIO_MEDIATYPE_DIGITALVIDEO:
    case MMIO_MEDIATYPE_MOVIE:
      return MT_VIDEO;
    default:
      return MT_UNKNOWN;
    }

  return 0;
}

static PSZ queryExeDir(const char *chrEXE)
{
  if(chrInstallDir[0]==0)
    {
      char * chr;
      strlcpy(chrInstallDir, chrEXE, sizeof(chrInstallDir));
      if((chr=strrchr(chrInstallDir, '\\'))!=NULLHANDLE)
        *chr='\0';
    }
  return chrInstallDir;
}


MRESULT findFileDlgProc(HWND hwnd, ULONG  msg, MPARAM mp1, MPARAM mp2)
{

  switch(msg)
    {
    case WM_INITDLG:
      {
        int a;
        SWCNTRL swctl;
        PID pid;

        /* Set dialog font */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        /* Fill combo box with media types (audio, image, video) */
        for(a=0;a<NUMMEDIATYPES;a++) 
          WinSendMsg(WinWindowFromID(hwnd, IDCB_TYPEOFFILE), LM_INSERTITEM, MRFROMSHORT(LIT_SORTASCENDING), 
                     MPFROMP(chrMediaFileTypes[a]));
        WinSetWindowText(WinWindowFromID(hwnd, IDCB_TYPEOFFILE), chrMediaFileTypes[0]);

        /* Subclass preview area */
        WinSubclassWindow(WinWindowFromID(hwnd, IDSR_IMGPREVIEW), bmpPreviewProc);

        /* Add switch entry */
        memset(&swctl,0,sizeof(swctl));
        WinQueryWindowProcess(hwnd,&pid,NULL);
        swctl.hwnd=hwnd;
        swctl.uchVisibility=SWL_VISIBLE;
        swctl.idProcess=pid;
        swctl.bProgType=PROG_DEFAULT;
        swctl.fbJump=SWL_JUMPABLE;
        WinAddSwitchEntry(&swctl);

        /* Add bitmap to play control buttons */
        changeButtons(hwnd);

        /* Start an audio player thread */
        MMAudioOpenPlayer(hwnd);
        break;
      }
    case WM_APPTERMINATENOTIFY:
      {
        /* Posted from player object window */
        switch(SHORT1FROMMP(mp1))
          {
          case MMAUDIO_PLAYER_CREATED:
            /* Player object window */
            hwndPlayer=HWNDFROMMP(mp2);
            break;
          default:
            break;
          }
        return MRFALSE;
      }
    case WM_CONTROL:
      {
        switch(SHORT1FROMMP(mp1))
          {
          case IDCB_TYPEOFFILE:
            {
              if(SHORT2FROMMP(mp1)==CBN_EFCHANGE) {
                char chrText[100];
                int a;
                WinQueryWindowText(HWNDFROMMP(mp2), sizeof(chrText), chrText);
                /* Find Index */
                for(a=0; a<NUMMEDIATYPES;a++) {
                  if(!stricmp(chrMediaFileTypes[a], chrText))
                    break;
                }
                /* Error!??!*/
                switch(a)
                  {
                  case 0: /* Audio */
                    {
                      showVideoControls(hwnd, FALSE);
                      showImageControls(hwnd, FALSE);
                      showAudioControls(hwnd, TRUE);
                      iMediaType=MT_AUDIO;
                      break;
                    }
                  case 1: /* Image */
                    showAudioControls(hwnd, FALSE);
                    showVideoControls(hwnd, FALSE);
                    showImageControls(hwnd, TRUE);
                    iMediaType=MT_IMAGE;
                    break;
                  case 2:
                    showAudioControls(hwnd, FALSE);
                    showImageControls(hwnd, FALSE);
                    showVideoControls(hwnd, TRUE);
                    iMediaType=MT_VIDEO;
                    break;
                  default:
                    return WinDefFileDlgProc(hwnd, msg, mp1, mp2);
                  }
                WinSetWindowText(WinWindowFromID(hwnd, 258), chrMediaFileExt[a]);
                WinSetFocus(HWND_DESKTOP, WinWindowFromID(hwnd, 258));
                WinSendMsg(WinWindowFromID(hwnd, 258), WM_CHAR,
                           MPFROM2SHORT(KC_VIRTUALKEY , 0), MPFROM2SHORT(0, VK_NEWLINE));
              }
            break;
            }
          case 258:
            {
              if(SHORT2FROMMP(mp1)==EN_CHANGE)
                WinStartTimer(WinQueryAnchorBlock(HWND_DESKTOP), hwnd, TIMERID_SELECTION, 100);
              break;
            }
          default:
            break;
          }
        break;
      }
    case WM_TIMER:
      switch(SHORT1FROMMP(mp1))
        {
        case TIMERID_SELECTION:
          {
            char chrName[CCHMAXPATH];
            char chrFullName[CCHMAXPATH*2];

            WinStopTimer(WinQueryAnchorBlock(HWND_DESKTOP), hwnd, TIMERID_SELECTION);
            WinQueryWindowText(WinWindowFromID(hwnd, 258), sizeof(chrName), chrName);
            strcpy(chrFullName, fd.szFullFile);
            switch(iMediaType)
              {
              case MT_AUDIO:
                strlcpy(chrLastAudioDir, fd.szFullFile, sizeof(chrLastAudioDir));
                break;
              case MT_IMAGE:
                strlcpy(chrLastImageDir, fd.szFullFile, sizeof(chrLastImageDir));
                break;
              case MT_VIDEO:
                strlcpy(chrLastImageDir, fd.szFullFile, sizeof(chrLastImageDir));
                break;
              default:
                break;
              }
            strcat(chrFullName, chrName);
            strlcpy(chrSourceName, chrFullName, sizeof(chrSourceName));
            switch(iMediaType)
              {
              case MT_AUDIO:
                {
                  char text[500];
                  MMIOINFO mmio;
                  HMMIO hmmio;
                  MMAUDIOHEADER mmAudioHeader={0};
                  LONG lBytesRead=0;
                  ULONG rc;
                  ULONG ulPlaySecs;

                  MMAudioStop(hwndPlayer, hwnd);

                  memset(&mmio,0, sizeof(mmio));
                  mmio.ulFlags=MMIO_READ;
                  mmio.ulTranslate=MMIO_TRANSLATEHEADER;
                  
                  hmmio = mmioOpen(chrFullName, &mmio, MMIO_READ);
                  if(!hmmio)
                    break;

                  memset(&mmAudioHeader,0,sizeof(MMAUDIOHEADER));
                  rc = mmioGetHeader(hmmio, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                                     &lBytesRead, 0, 0);
                  
                  mmioClose(hmmio, 0);
                  if(rc!=MMIO_SUCCESS) {
                    WinSetWindowText(WinWindowFromID(hwnd, IDST_AUDIOINFO), "");
                    enableAudioPlayControls(hwnd, FALSE);
                    break;
                  }

                  ulPlaySecs=mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInMS/1000;
                  sprintf(text, audioText,  ulPlaySecs/60, ulPlaySecs%60,
                          mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec,
                          mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample,
                          mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels);
                  WinSetWindowText(WinWindowFromID(hwnd, IDST_AUDIOINFO), text);
                  enableAudioPlayControls(hwnd, TRUE);
                  break;
                }
              case MT_IMAGE:
                if(hBitmap) {
                  HBITMAP hBitmapTemp;
                  //                  hBitmapTemp=loadBitmap (chrFullName, &bmpInfoHeader2);
                  hBitmapTemp=ImgLoadImageFileAndHeader(chrFullName, &bmpInfoHeader2);
                  GpiDeleteBitmap(hBitmap);
                  hBitmap=hBitmapTemp;
                }
                else
                  hBitmap=ImgLoadImageFileAndHeader(chrFullName, &bmpInfoHeader2);
                  //                  hBitmap=loadBitmap (chrFullName, &bmpInfoHeader2);
                /* Force redraw of preview area */
                WinInvalidateRect(WinWindowFromID(hwnd, IDSR_IMGPREVIEW), NULLHANDLE, FALSE);
                break;
              case MT_VIDEO:
                break;
              default:
                break;
              }
            return MRFALSE;
          }
        default:
          break;
        }
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDPB_MMFINDPLAY:
          {
          MMAudioPlayFile(hwndPlayer, hwnd, NULLHANDLE);
          return MRFALSE;
          }
        case IDPB_MMFINDSTOP:
          {
          MMAudioStop(hwndPlayer, hwnd);
          return MRFALSE;
          }
        case IDPB_MMFINDPAUSE:
          {
          MMAudioPause(hwndPlayer, hwnd);
          return MRFALSE;
          }
        case IDPB_NEWFILE: /* New */
          {
            HOBJECT hObject;

            MMAudioStop(hwndPlayer, hwnd);
            MMAudioClosePlayer(hwndPlayer);

            /* Open the MM templates folder */
            if((hObject=WinQueryObject("<MMPM2_MMTEMPLATEFOLDER>"))!=NULLHANDLE) {
              WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
              WinDismissDlg(hwnd, DID_CANCEL);
            }
            else
              MsgShowMessageBox(hwnd, IDSTR_NOMMTEMPLATEFLDRTITLE, IDSTR_NOMMTEMPLATEFLDR,
                                RESSOURCEHANDLE, MB_MOVEABLE|MB_OK);
            return MRFALSE;
          }
        default:
          //          MMAudioStop(hwndPlayer, hwnd);
          stopAudioFile(hwnd);
          MMAudioClosePlayer(hwndPlayer);
          // DosSleep(1000);
          break;
        }
    default:
      break;
    }
  return WinDefFileDlgProc(hwnd, msg, mp1, mp2);
}

BOOL getStringFromRexxScript(PSZ pszBinDir, PSZ rexxFile, char* chrResult, ULONG ulSize)
{

  RXSTRING arg[1];                    /* argument string for REXX  */
  RXSTRING rexxretval;                /* return value from REXX    */
  APIRET   rc;                        /* return code from REXX     */
  SHORT    rexxrc = 0;                /* return code from function */
  char theScript[CCHMAXPATH];  
  /* By setting the strlength of the output RXSTRING to zero, we   */
  /* force the interpreter to allocate memory and return it to us. */
  /* We could provide a buffer for the interpreter to use instead. */
  rexxretval.strlength = 0L;          /* initialize return to empty*/
    
  sprintf(theScript, "%s\\%s", pszBinDir, rexxFile);

  /* Here we call the interpreter.  We don't really need to use    */
  /* all the casts in this call; they just help illustrate         */
  /* the data types used.                                          */
  rc=RexxStart((LONG)       0,             /* number of arguments   */
               (PRXSTRING)  &arg,          /* array of arguments    */
               (PSZ)        theScript,     /* name of REXX file     */
               (PRXSTRING)  0,             /* No INSTORE used       */
               (PSZ)        "CWRXX",       /* Command env. name     */
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
  if(!rc)
    return TRUE;
  else
    return FALSE;
}
/*
  This function checks the installed image IO procs and adds extensions which are not yet handled
  to the list of known extensions.
 */
static BOOL getMissingExtFromImageIOProc(char *chrMMImageKnownExt, ULONG ulSize)
{
    MMFORMATINFO  mmFormatInfo;
    PMMFORMATINFO pmmFormatInfoArray;
    void * memPtr;
    ULONG         ulReturnCode;
    LONG          lFormatsRead;
    LONG          index;
    LONG lNumIOProcs;

    memset( &mmFormatInfo,
            '\0',
            sizeof(MMFORMATINFO) );
    
    mmFormatInfo.ulMediaType |= MMIO_MEDIATYPE_IMAGE;
    mmFormatInfo.ulFlags|=MMIO_CANREADTRANSLATED;/* Read !!!*/    
    ulReturnCode = mmioQueryFormatCount ( &mmFormatInfo,
                                          &lNumIOProcs,
                                          0,
                                          0 );
    
    if( ulReturnCode != MMIO_SUCCESS )
      {
        /*
         * Error - mmioQueryFormatCount failed.
         */
        return FALSE;
      }

    /*
     * Allocate enough memory for n number of FormatInfo blocks
     */
    pmmFormatInfoArray = malloc (lNumIOProcs * sizeof( MMFORMATINFO ) );
    memPtr=pmmFormatInfoArray;
    if( pmmFormatInfoArray == NULL )
      {
        /*
         * Could not allocate enough memory for mmFormatInfo array.
         */
        return FALSE;
      }
    
    /*
     * call mmioGetFormats to get info on the formats supported.
     */
    ulReturnCode = mmioGetFormats( &mmFormatInfo,
                                   lNumIOProcs,
                                   pmmFormatInfoArray,
                                   &lFormatsRead,
                                   0,
                                   0 );
    if( ulReturnCode != MMIO_SUCCESS )
      {
        /*
         *  mmioGetFormats failed.
         */
        free(pmmFormatInfoArray);
        return FALSE;
      }
    
    if( lFormatsRead != lNumIOProcs )
      {
        /*
         * Error in MMIO - number of formats read in by
         * mmioGetFormats is not equal to number of formats
         * found by mmioQueryFormatCount.
         */
        free(pmmFormatInfoArray);
        return FALSE;
      }

    for ( index = 0; index <lNumIOProcs; index++ )
      {
        /* Now scan the whole image proc array */
        if(!strstr(chrMMImageKnownExt, pmmFormatInfoArray->szDefaultFormatExt)) {
          /* This extension is yet unknown */
          if(chrMMImageKnownExt[0]==0) {
            /* It's the first one */
            sprintf(chrMMImageKnownExt,"*.%s", pmmFormatInfoArray->szDefaultFormatExt);
          }
          else {
            /* Check if the extension is not already added (e.g. TIF has a lot of subformats) */
            if(!strstr(chrMMImageKnownExt, pmmFormatInfoArray->szDefaultFormatExt)) {
              strlcat(chrMMImageKnownExt,";*.", ulSize);
              strlcat(chrMMImageKnownExt, pmmFormatInfoArray->szDefaultFormatExt, ulSize);
            }
          }
        }  
        /*
         *  advance to next entry in mmFormatInfo array
         */        
        pmmFormatInfoArray++;
      }
    free(memPtr);
    return FALSE;
}


int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  char text[CCHMAXPATH];
  short a;

  /* Create a copy of the args */
  /* argv[0]: progname
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
        HWND hwndDlg;       

        /* Get our ressource dll */  
        RESSOURCEHANDLE=queryResModuleHandle(argv[0]);
        /* Load NLV strings */
        loadStringsFromResourceDLL(RESSOURCEHANDLE);

        /* Get data from INI file */
        HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));

        /* Get additional audio extensions from mmpm2.ini */
        getStringFromRexxScript(queryExeDir(argv[0]), "audioext.rx",
                                chrAdditionalExt, sizeof(chrAdditionalExt));
        if(strlen(chrAdditionalExt)!=0) {
          translateChars(chrAdditionalExt, ',', ';');
          strlcat(chrMediaFileExt[MT_AUDIO], ";", sizeof(chrMediaFileExt[MT_AUDIO]));
          strlcat(chrMediaFileExt[MT_AUDIO], chrAdditionalExt, sizeof(chrMediaFileExt[MT_AUDIO]));
        }

        /* Get image extensions */
        getStringFromRexxScript(queryExeDir(argv[0]), "imageext.rx",
                                chrMediaFileExt[MT_IMAGE], sizeof(chrMediaFileExt[MT_IMAGE]));
        if(strlen(chrMediaFileExt[MT_IMAGE])!=0)
          translateChars(chrMediaFileExt[MT_IMAGE], ',', ';');
        /* Get additional image extensions supplied by the user in imageadd.rx */
        chrAdditionalExt[0]=0;
        getStringFromRexxScript(queryExeDir(argv[0]), "imageadd.rx",
                                chrAdditionalExt, sizeof(chrAdditionalExt));
        if(strlen(chrAdditionalExt)!=0) {
          translateChars(chrAdditionalExt, ',', ';');
          strlcat(chrMediaFileExt[MT_IMAGE], ";", sizeof(chrMediaFileExt[MT_IMAGE]));
          strlcat(chrMediaFileExt[MT_IMAGE], chrAdditionalExt, sizeof(chrMediaFileExt[MT_IMAGE]));
        }
        /* Get extensions handled by installed IO procs */
        getMissingExtFromImageIOProc(chrMediaFileExt[MT_IMAGE], sizeof(chrMediaFileExt[MT_IMAGE]));

#if 0
        sprintf(chrInstallDir, "%d!%s!", strlen(chrMediaFileExt[MT_IMAGE]), chrMediaFileExt[MT_IMAGE]);
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrInstallDir, chrMediaFileExt[MT_IMAGE], 1234, MB_OK);
#endif

        /* Restore recent visited directories */
        PrfQueryString(NULLHANDLE, chrIniFile, "mmfind", "lastaudiodir", "", chrLastAudioDir, sizeof(chrLastAudioDir));
        PrfQueryString(NULLHANDLE, chrIniFile, "mmfind", "lastvideodir", "", chrLastVideoDir, sizeof(chrLastVideoDir));
        PrfQueryString(NULLHANDLE, chrIniFile, "mmfind", "lastimagedir", "", chrLastImageDir, sizeof(chrLastImageDir));

        _loadButtonBmps();
        fd.cbSize = sizeof( fd );
        /* It's an centered 'Open'-dialog */
        fd.fl = FDS_OPEN_DIALOG|FDS_CENTER | FDS_CUSTOM;
        fd.hMod=RESSOURCEHANDLE;
        fd.usDlgId=IDDLG_MMFIND;
        fd.pfnDlgProc=findFileDlgProc;


        switch(iMediaType)
          {
          case MT_AUDIO:
            strlcpy(fd.szFullFile, chrLastAudioDir, sizeof(fd.szFullFile));
            break;
          case MT_IMAGE:
            strlcpy(fd.szFullFile, chrLastImageDir, sizeof(fd.szFullFile));
            break;
          case MT_VIDEO:
            strlcpy(fd.szFullFile, chrLastVideoDir, sizeof(fd.szFullFile));
            break;
          default:
            break;
          }

        if( (hwndDlg=WinFileDlg( HWND_DESKTOP, HWND_DESKTOP, &fd )) != NULLHANDLE )
          {
            if(hBitmap)
              GpiDeleteBitmap(hBitmap);  
            if( fd.lReturn == DID_OK )
              {
                switch(mmIdentifyFile(fd.szFullFile))
                  {
                  case MT_IMAGE:
                    {
                      HOBJECT hObject;
                      if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                        WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
                      break;
                    }
                  case MT_AUDIO:
                    {
                      HOBJECT hObject;

                      /* Open the audio file in the editor */
                      if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                        WinOpenObject(hObject, ID_MENU_EDITOR, TRUE);
                      // WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
                      break;
                    }
                  case MT_VIDEO:
                    {
                      HOBJECT hObject;
                      char *chr;
                      if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                        WinOpenObject(hObject, ID_MENU_PLAYER /* OPEN_DEFAULT */, TRUE);
#if 0
                      /* Open folder */
                      if((chr=strrchr(fd.szFullFile, '\\'))!=NULLHANDLE) {
                        *chr=0;
                        if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                          WinOpenObject(hObject, OPEN_CONTENTS, TRUE);
                      }
#endif
                      break;
                    }
                  default:
                    {
                      HOBJECT hObject;
                      char *chr;
                      if((chr=strrchr(fd.szFullFile, '\\'))!=NULLHANDLE) {
                        *chr=0;
                        if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                          WinOpenObject(hObject, OPEN_CONTENTS, TRUE);
                      }
                      break;
                    }
                  }/* switch()*/
              }/* DID_OK */
            /* Save the last visited directories */
            PrfWriteString(NULLHANDLE, chrIniFile, "mmfind", "lastaudiodir", chrLastAudioDir);
            PrfWriteString(NULLHANDLE, chrIniFile, "mmfind", "lastvideodir", chrLastVideoDir);
            PrfWriteString(NULLHANDLE, chrIniFile, "mmfind", "lastimagedir", chrLastImageDir);
          }
        freeResHandle();
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}

