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
#include "mmplayer.h"

/* The object window controling the trac playing */
HWND hwndPlayObject;
extern PID ulPid;
/* TRUE when track is playing */
extern BOOL bIsPlaying;
/* TRUE when track is paused */
extern BOOL bPaused;
/* Current time in track */
extern ULONG ulPos;
extern ULONG ulTotalLength;
extern BOOL bIsMidi;
extern ULONG ulVolume;

extern char chrSourceName[CCHMAXPATH];

void showPlayTimeDisplay(HWND hwndFrame, BOOL bShow);
void _resetDisplay(HWND hwndFrame);
void setPlayTimeText2( HWND hwndDialog, char * theText);

BOOL pauseAudioFile(HWND hwndFrame)
{
  int iWavePriv;
  char chrCommand[50];
  char retMsg[100];
  ULONG rc;

  iWavePriv=ulPid;

  if(bPaused) {
    sprintf(chrCommand,"RESUME wave%d wait", iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    bPaused=FALSE;
    showPlayTimeDisplay( hwndFrame, TRUE);
  }
  else {
    sprintf(chrCommand,"PAUSE wave%d wait", iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    bPaused=TRUE;
  }
  return TRUE;
}

BOOL stopAudioFile(HWND hwndFrame)
{
  int iWavePriv;
  char chrCommand[50];
  char retMsg[100];
  ULONG rc;

  iWavePriv=ulPid;
  
  if(bIsPlaying) {
    sprintf(chrCommand,"stop wave%d wait", iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    
    sprintf(chrCommand,"close wave%d wait", iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    
    _resetDisplay(hwndFrame);
    showPlayTimeDisplay( hwndFrame, TRUE);
    WinStopTimer(WinQueryAnchorBlock(hwndFrame), hwndFrame, IDTIMER_PLAY);
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
  int iWavePriv;
  int iTime2;
  HWND hwndNotify;


  if(bPaused)
    {
      pauseAudioFile(hwndFrame);
      return 1;
    }


  iWavePriv=++ulPid;
  ulPos=0;

  stopAudioFile(hwndFrame);

#if 0
#endif

#if 0
  if(!getMessage(chrCommand, IDSTR_STARTINGTRACK, sizeof(chrCommand), queryResModuleHandle(),  hwndFrame))
#endif
    sprintf(chrCommand, "Starting track...");
  setPlayTimeText2(hwndFrame, chrCommand);

  if(bIsMidi)
    strncpy(chrDevice,"SEQUENCER", sizeof(chrDevice));
  else
    strncpy(chrDevice,"WAVEAUDIO", sizeof(chrDevice));

  hwndNotify=hwndFrame;//WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE);

  /* Start audio file */  
  sprintf(chrCommand,"open \"%s\"  type %s alias wave%d SHAREABLE wait", chrSourceName, chrDevice, iWavePriv);
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
  sprintf(chrCommand,"STATUS wave%d LENGTH WAIT", iWavePriv);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)==MCIERR_SUCCESS) {
    iTime2=atoi(retMsg);
    sprintf(chrCommand,"%d:%02d  %d:%02d  -%d:%02d",iTime2/1000/60,(iTime2/1000)%60,
            0, 0,
            iTime2/1000/60,(iTime2/1000)%60);
  }
  ulTotalLength=iTime2;


  /* Set volume */
  sprintf(chrCommand,"SET wave%d AUDIO VOLUME %ld wait", iWavePriv, ulVolume);
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

  WinStartTimer(WinQueryAnchorBlock(hwndFrame), hwndFrame, IDTIMER_PLAY, PLAYTIMER_DELAY);

  bIsPlaying=TRUE;
  return 1;
}

BOOL seekAudioFile(LONG lPosSec)
{
  char chrCommand[50];
  char retMsg[20];
  APIRET rc;

  sprintf(chrCommand,"PLAY wave%ld from %ld", ulPid, lPosSec);
  rc=mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  //  ulStartPosition=lPosSec;
  bPaused=FALSE;

  return TRUE;
}

void controlPlaying(HWND hwndDialog, ULONG ulAction, ULONG ulReserved)
{
  WinPostMsg(hwndPlayObject, WM_APPTERMINATENOTIFY, MPFROMP(hwndDialog), MPFROMLONG(ulAction));
}

MRESULT EXPENTRY playObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch (msg)
    {
    case WM_APPTERMINATENOTIFY:
      {
        HWND hwndFrame=HWNDFROMMP(mp1);


        switch(LONGFROMMP(mp2))
          {
          case PLAY_FIRST:
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

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {

      hwndPlayObject=WinCreateWindow(HWND_OBJECT,WC_STATIC,"MFObj",0,0,0,0,0,NULLHANDLE,HWND_BOTTOM,13343,NULL,NULL);
      if(hwndPlayObject) {
        WinSubclassWindow(hwndPlayObject,&playObjectProc);
        /* Window created. */
        while(WinGetMsg(hab,&qmsg,(HWND)NULL,0,0))
          WinDispatchMsg(hab,&qmsg);

        WinDestroyWindow(hwndPlayObject);
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
}
