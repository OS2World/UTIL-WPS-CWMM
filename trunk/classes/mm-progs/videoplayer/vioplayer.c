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

#ifdef __EMX__
//#define USE_OS2_TOOLKIT_HEADERS
#endif
#include <os2.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <os2me.h>
#include "common.h"
#include "mmplayerres.h"
#include "mmres.h"
#include "mmplayer.h"
#include "mmprogs_defaults.h"
#include "sys_funcs.h"


/* Track to play. Full path */
char chrSourceName[CCHMAXPATH]={0};

BOOL bPaused=FALSE;
ULONG ulPid;
BOOL bIsPlaying=FALSE;
ULONG ulPos=0;
ULONG ulTotalLength;

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
    //    showPlayTimeDisplay( hwndFrame, TRUE);
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
    
    //    _resetDisplay(hwndFrame);
    // showPlayTimeDisplay( hwndFrame, TRUE);
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
  int iTime2=0;
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
  if(!getMessage(chrCommand, IDSTR_STARTINGTRACK, sizeof(chrCommand), queryResModuleHandle(),  hwndFrame))
#endif
    sprintf(chrCommand, "Starting track...");
  //  setPlayTimeText2(hwndFrame, chrCommand);

#if 0
  if(bIsMidi)
    strncpy(chrDevice,"SEQUENCER", sizeof(chrDevice));
  else
    strncpy(chrDevice,"WAVEAUDIO", sizeof(chrDevice));
#endif
  strncpy(chrDevice,"DIGITALVIDEO02", sizeof(chrDevice));
  //  strncpy(chrDevice,"xine", sizeof(chrDevice));

  hwndNotify=hwndFrame;//WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE);



    /* Start audio file */
  //    sprintf(chrCommand,"open \"%s\"  type %s PARENT %lx alias wave%d SHAREABLE wait",
  //        chrSourceName, chrDevice, hwndVideo, iWavePriv);
    SysWriteToTrapLog("Here %s\n %s\n", chrSourceName, chrCommand);
    
    sprintf(chrCommand,"open digitalvideo01 alias wave%d  wait", iWavePriv);
    //  sprintf(chrCommand,"open \"%s\"  alias wave%d SHAREABLE wait", chrSourceName, iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), hwndNotify, 0);

  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    SysWriteToTrapLog("Open error %d!\n", (rc & 0x0000ffff));
    DosBeep(5000, 400);
    return 0;
  }  

  
#if 0
  sprintf(chrCommand,"open %s alias wave%d SHAREABLE wait",
          chrDevice, iWavePriv);
  sprintf(chrCommand,"open %s alias wave%d SHAREABLE wait",
          "digitalvideo", iWavePriv);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    SysWriteToTrapLog("Open error %d!\n", (rc & 0x0000ffff));
    DosBeep(5000, 400);
    return 0;
  }

  sprintf(chrCommand,"window %s HANDLE %ld wait",
          chrDevice, hwndVideo);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    SysWriteToTrapLog("window error %d!\n", (rc & 0x0000ffff));
    DosBeep(3000, 400);
    return 0;
  }
  sprintf(chrCommand,"LOAD %s %s  wait",
          chrDevice, chrSourceName);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    SysWriteToTrapLog("window error %d!\n", (rc & 0x0000ffff));
    DosBeep(3000, 400);
    return 0;
  }
#endif

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

#if 0
  /* Set volume */
  sprintf(chrCommand,"SET wave%d AUDIO VOLUME %ld wait", iWavePriv, 70);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    sprintf(chrCommand,"close wave%d",iWavePriv);
    rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
    return 0;
  }
#endif

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

int main(int argc, char *argv[])
{
  PTIB ptib;
  PPIB ppib;

  /* Save source name */
  if(argc==2)
    strcpy(chrSourceName, argv[1]);
  printf("File: %s\n", chrSourceName);

  if(DosGetInfoBlocks(&ptib, &ppib)==NO_ERROR)
    {
      HAB  hab;
      HMQ  hmq;


      printf("Got info block...\n");
      ulPid=ppib->pib_ulpid;
      ppib->pib_ultype=3; /* Morph into PM */
      if((hab=WinInitialize(0))==NULLHANDLE)
        {
          printf("Can't get hab.\n");
          return -1;
        }
      hmq=WinCreateMsgQueue(hab,0);
      if(!hmq) {  
        printf("Can't get hmq.\n");
      }
      /*  WinCreateWindow(HWND_DESKTOP, WC_BUTTON, "Test", WS_VISIBLE, 0, 0, 100, 100, 
                      HWND_DESKTOP, HWND_TOP, 1234, NULLHANDLE, NULLHANDLE);*/

      playAudioFile(HWND_DESKTOP);
  printf("!!!!!!!!!!!!!!! Here! %s\n", "!!!!!!!!!!!!");  
DosSleep(5000);
      WinDestroyMsgQueue(hmq);
      WinTerminate(hab);
    }
  return 0;

}
