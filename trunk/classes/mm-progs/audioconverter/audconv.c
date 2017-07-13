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

#include <os2.h>

#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "os2me.h"
#include "common.h"
#include "mmres.h"
#include "mmprogs_defaults.h"

#include "sys_funcs.h"

#define MSG_CONVERTDONE    1L
#define MSG_CONVERTPERCENT 2L
#define MSG_CONVERTERROR   3L

#define ID_TIMER 1
#define TIMER_DELAY        200

#define CONVERTBUFFERSIZE  500000
#define NUMPARAMS  3
/* argv[0]: progname
 * argv[1]: wavefile
 * argv[2]: IO proc name
 */

//#define DEBUG

//#define INI_NAME_APP        "audioconv"
//#define INI_NAME_COLOR        "color"

void HlpSendCommandToObject(char* chrObject, char* command);
BOOL percentRegisterBarClass(void);
BOOL IniSaveWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniRestoreWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniSaveWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL IniRestoreWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize);
HMODULE queryResModuleHandle(char *chrExePath);
void freeResHandle();
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);


char logName[]="convert.log";

extern SWP swpWindow;
BOOL bHaveWindowPos=FALSE;

char chrTargetName[CCHMAXPATH]={0};
char chrSourceName[CCHMAXPATH]={0};
char chrProcName[CCHMAXPATH]={0};
char chrIniFile[CCHMAXPATH];

int numArgs;
char* params[NUMPARAMS];

#define NUMIOPROCS 1000
int iIoProc;     /* Position of IO-Proc in global data area of MMOS2. 0 based */
int iPrivIOProc[NUMIOPROCS]; /* Number of possible IO-Procs. I'm lazy here and use a static array.
                                If there ever will be more than 1000 procs there'll be a problem... */


int iSampleRate;
int iBitsPerSample;
SHORT sChannels;
LONG lSec;
LONG lAudioSize;

TID tidThread=0;
BOOL bBreak=FALSE;
PMMFORMATINFO pMemFormatInfo=NULLHANDLE;
HMODULE RESSOURCEHANDLE=0;
BOOL bNoProcGiven=FALSE;

void pmUsage();

BOOL createTargetName(char *sourceName, char *chrExt)
{
  char *textPtr;

  strcpy(chrTargetName, sourceName);
  if((textPtr=strrchr(chrTargetName, '.'))!=NULLHANDLE)
    *textPtr=0;
  else
    textPtr=chrTargetName;
  strcat(textPtr,".");
  strcat(textPtr, chrExt);
#if 0
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, textPtr,
                       "",
                       0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
#endif
  return TRUE;
}

BOOL insertIOProcItems( HWND hwndDrop )
{
  //    CHAR          szBuffer[ sizeof( FOURCC ) + CCHMAXPATH + 4 ];
    MMFORMATINFO  mmFormatInfo;
    PMMFORMATINFO pmmFormatInfoArray;
    ULONG         ulReturnCode;
    LONG          lFormatsRead;
    LONG          index;
    LONG          lBytesRead;


    LONG lNumIOProcs;
    int sIdx;
    memset( &mmFormatInfo,
            '\0',
            sizeof(MMFORMATINFO) );
    
    mmFormatInfo.ulMediaType |= MMIO_MEDIATYPE_AUDIO;
    mmFormatInfo.ulFlags|=MMIO_CANWRITETRANSLATED;    
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
    pmmFormatInfoArray = (PMMFORMATINFO) malloc (lNumIOProcs * sizeof( MMFORMATINFO ) );
    pMemFormatInfo=pmmFormatInfoArray;
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


    for ( index = 0, sIdx=0; index <lNumIOProcs; index++ )
      {
        char szName[CCHMAXPATH];

        mmioGetFormatName(pmmFormatInfoArray, szName, &lBytesRead, 0L, 0L);        
        /* Insert NULL string terminator */
        *( szName + lBytesRead ) = (CHAR)NULL;

#ifdef DEBUG
        SysWriteToTrapLog("%d  IO-Proc: %s, %s\n", index, 
                          pmmFormatInfoArray->szDefaultFormatExt,
                          szName);
#endif

        if(pmmFormatInfoArray->ulFlags & MMIO_CANWRITETRANSLATED)
          {
            /* Insert into list box */
            WinSendMsg(hwndDrop, LM_INSERTITEM,(MPARAM)LIT_END,
                       (MPARAM)szName);
            
            
            iPrivIOProc[sIdx]=index;
            
#ifdef DEBUG
            SysWriteToTrapLog("-- %d  %d, sIdx: %d %s\n", index, iPrivIOProc[sIdx], sIdx,
                              pmmFormatInfoArray->szDefaultFormatExt
                              );
#endif
            
            
            /* Set text field */
            if(bNoProcGiven) {
              WinSetWindowText(hwndDrop, szName);
              createTargetName(chrSourceName, pmmFormatInfoArray->szDefaultFormatExt);
              iIoProc=iPrivIOProc[sIdx];
              bNoProcGiven=FALSE;
            }
            else {
              if(!stricmp(szName, chrProcName)) {
                WinSetWindowText(hwndDrop, szName);
                /* Create target name */
                createTargetName(chrSourceName, pmmFormatInfoArray->szDefaultFormatExt);
                iIoProc=iPrivIOProc[sIdx];
              }
              sIdx++;
            }
          }

        /*
         *  advance to next entry in mmFormatInfo array
         */        
        pmmFormatInfoArray++;
      }
    return TRUE;
}

BOOL queryAudioInfo(char *chrFile)
{
  HMMIO hmmio;
  MMAUDIOHEADER mmAudioHeader;
  MMIOINFO mmioinfo;
  LONG lBytesRead=0;
  APIRET rc;
  
  memset(&mmioinfo,0, sizeof(mmioinfo));
  mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER;
  mmioinfo.ulFlags=MMIO_READ|MMIO_DENYNONE;
  /* open source file */
  if((hmmio=mmioOpen(chrFile, &mmioinfo,MMIO_READ))==NULLHANDLE)
    {
      return FALSE;
    }
  memset(&mmAudioHeader,0,sizeof(MMAUDIOHEADER));
  rc = mmioGetHeader(hmmio, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                     &lBytesRead, 0, 0);
  if(rc!=MMIO_SUCCESS) {
    mmioClose(hmmio, 0);
    //    fprintf(stderr,"mmioGetHeader error!\n");
    return FALSE;
  }

  iSampleRate=mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec;
  sChannels=mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels;
  iBitsPerSample=mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample;
  lSec=mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes/
    mmAudioHeader.mmXWAVHeader.WAVEHeader.ulAvgBytesPerSec;
  lAudioSize=mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInBytes;

  /* close source */
  mmioClose(hmmio,0);
  return TRUE;
}

#if 0
BOOL createTargetName(char *sourceName)
{
  char *textPtr;

  strcpy(chrTargetName, sourceName);
  if((textPtr=strrchr(chrTargetName, '.'))!=NULLHANDLE)
    *textPtr=0;
  else
    textPtr=chrTargetName;
  strcat(textPtr, ".wav");

  return TRUE;
}
#endif

void _Optlink convertThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  HWND hwnd=(HWND)arg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      HMMIO hmmio, hmmioTarget;
      MMIOINFO mmioinfo;
      MMAUDIOHEADER mmAudioHeader;
      LONG lBytesRead=0;
      LONG lBytesWritten=0;
      ULONG rc;
      PBYTE buffer;

      do {
        /*******************************/
        /* Set up/open the SOURCE file */
        /*******************************/
        memset(&mmioinfo,0, sizeof(mmioinfo));
        mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
        mmioinfo.ulFlags=MMIO_READ|MMIO_DENYNONE;

        if((hmmio=mmioOpen(chrSourceName, &mmioinfo,MMIO_READ))==NULLHANDLE)
          {
            char text[500];
            snprintf(text, sizeof(text)-1,"mmioOpen error with file %s\n", chrSourceName);
            WinMessageBox(hwnd, hwnd, "",
                          text,
                          0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
            break;
          }
        memset(&mmAudioHeader,0,sizeof(MMAUDIOHEADER));
        rc = mmioGetHeader(hmmio, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                           &lBytesRead, 0, 0);
        if(rc!=MMIO_SUCCESS) {
          char text[500];
          mmioClose(hmmio, 0);
          snprintf(text, sizeof(text)-1,"mmioGetHeader error!");
          WinMessageBox(hwnd, hwnd, "",
                        text,
                        0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
          break;
        }

        /*******************************/
        /* Set up/open the TARGET file */
        /*******************************/
        memset(&mmioinfo,0, sizeof(mmioinfo));
        mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
        mmioinfo.ulFlags=MMIO_WRITE|MMIO_CREATE;

        //  mmioinfo.fccIOProc=mmioStringToFOURCC("WAVE", MMIO_TOUPPER);
        mmioinfo.fccIOProc=pMemFormatInfo[iIoProc].fccIOProc;
        if((hmmioTarget=mmioOpen(chrTargetName, &mmioinfo, MMIO_WRITE|MMIO_CREATE))==NULLHANDLE)
          {
            char chrError[CCHMAXPATH];
            char text[500];
            rc=mmioGetLastError(hmmioTarget);
            mciGetErrorString(mmioinfo.ulErrorRet ,chrError, sizeof(chrError));
            snprintf(text, sizeof(text)-1,"mmioOpen error with file %s:\n%s\n%d", chrTargetName, chrError,mmioinfo.ulErrorRet);
            mmioClose(hmmio,0);
            WinMessageBox(hwnd, hwnd, "",
                          text,
                          0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
            break;
          }
        /* Set parameters */
        rc = mmioSetHeader(hmmioTarget, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                           &lBytesWritten, 0, 0);
        
        /* Get buffer */
        if((buffer=(void*)malloc(CONVERTBUFFERSIZE))==NULLHANDLE) {
          char text[500];
          mmioClose(hmmio, 0);
          mmioClose(hmmioTarget,0);
          snprintf(text, sizeof(text)-1,"Can't get memory buffer!");
          WinMessageBox(hwnd, hwnd, "",
                        text,
                        0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
          
          break;
        }

        /* convert */
        lBytesWritten=0;
        do{
          rc=mmioRead(hmmio, buffer, CONVERTBUFFERSIZE);
          if(bBreak)
            break;
          if(rc!=0 && rc!=MMIO_ERROR ) {
            mmioWrite(hmmioTarget, buffer, rc);
            lBytesWritten+=rc;
            WinSendMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTPERCENT), MPFROMLONG((lBytesWritten)/(lAudioSize/100)));
          }
        }while(rc!=0 && rc!=MMIO_ERROR && !bBreak/*&& lBytesWritten <1000000*/);
        if(rc==MMIO_ERROR) {
          char text[500];
          snprintf(text, sizeof(text)-1,"Error while reading audio data!");
          WinMessageBox(hwnd, hwnd, "",
                        text,
                        0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
        }
        if(!bBreak) {
          char chrCommand[CCHMAXPATH*2];
          WinSendMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTPERCENT), MPFROMLONG(100));
          /* Refresh details view */
          HlpSendCommandToObject(chrTargetName, "CWAUDIOREFRESH=1");
          sprintf(chrCommand,"CWAUDIOCOPYTAGTO=%s;", chrTargetName);
          HlpSendCommandToObject(chrSourceName, chrCommand);
        }
        free(buffer);
        /* close source */
        mmioClose(hmmio,0);
        /* close target */
        mmioClose(hmmioTarget,0);
        break;
      }
      while(TRUE);
      WinSendMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTDONE), 0);      
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  tidThread=0;
  bBreak=FALSE;
}

void convertAudioFile(HWND hwnd)
{
  tidThread=_beginthread(convertThreadFunc,NULL,  8192*16, (void*)hwnd); //Fehlerbehandlung fehlt
  if(tidThread==-1) {
    DosBeep(100, 500);
    tidThread=0;
  }
}

/* This Proc handles the on-the-fly data CD writing */
MRESULT EXPENTRY decodeStatusDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  char text[CCHMAXPATH*4 +10];
  char title[CCHMAXPATH*4];
  SWCNTRL swctl;
  PID pid;
  int iPercent;

  switch (msg)
    {      
    case WM_INITDLG:

      /* Add switch entry */
      memset(&swctl,0,sizeof(swctl));
      WinQueryWindowProcess(hwnd,&pid,NULL);
      swctl.hwnd=hwnd;
      swctl.uchVisibility=SWL_VISIBLE;
      swctl.idProcess=pid;
      swctl.bProgType=PROG_DEFAULT;
      swctl.fbJump=SWL_JUMPABLE;
      WinAddSwitchEntry(&swctl);

#if 0
      sprintf(text,"1: %s, 2: %s, 3: %s 4: %s 5: %s 6: %s",params[1],params[2],params[3],
              params[4], params[4],params[4]);
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, text,
                     params[4],
                     0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
#endif

      WinSendMsg(WinWindowFromID(hwnd,IDST_CONVERTNAME),EM_SETTEXTLIMIT,MPFROMSHORT((SHORT)CCHMAXPATH),0);
      
      WinSetWindowText(WinWindowFromID(hwnd,IDST_CONVERTCDBITS), "");
      
      /* Filename */
      WinSetWindowText(WinWindowFromID(hwnd,IDST_CONVERTNAME), chrSourceName);
      
      /* Set dialog font to WarpSans for Warp 4 and above */
      if(SysQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }

      /* Set percent bars to 0. */
      WinSetWindowText(WinWindowFromID(hwnd,IDBAR_CONVERTPROGRESS),"0#0%");

      WinSendMsg(WinWindowFromID(hwnd,IDEF_CONVERTTARGETNAME), EM_SETTEXTLIMIT,MPFROMSHORT((SHORT)CCHMAXPATH),0);
      insertIOProcItems( WinWindowFromID(hwnd, IDDD_IOPROC) );
      WinSetWindowText( WinWindowFromID(hwnd,IDEF_CONVERTTARGETNAME), chrTargetName );
      WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTABORT), FALSE);

      /* Channels */
      getMessage(title, IDSTR_CONVERTCHANNELS, sizeof(title), RESSOURCEHANDLE, hwnd);
      sprintf(text, title, sChannels);
      WinSetWindowText(WinWindowFromID(hwnd,IDST_CONVERTCHANNELS),text);

      /* Samplerate */
      getMessage(title, IDSTR_CONVERTSAMPLERATE, sizeof(title), RESSOURCEHANDLE, hwnd);	
      sprintf(text, title, iSampleRate);
      WinSetWindowText(WinWindowFromID(hwnd,IDST_CONVERTSAMPLERATE),text);

      /* Playtime */
      getMessage(title, IDSTR_CONVERTPLAYTIME, sizeof(title), RESSOURCEHANDLE, hwnd);	
      sprintf(text, title, lSec/60, lSec%60);
      WinSetWindowText(WinWindowFromID(hwnd,IDST_CONVERTPLAYTIME),text);

      IniRestoreWindowPos(chrIniFile, INI_AUDIOCONV_APP, INI_WINDOWPOS_KEY, hwnd);
      IniRestoreWindowClrs(chrIniFile, INI_AUDIOCONV_APP , hwnd);

      WinSetFocus(HWND_DESKTOP, hwnd);
      return (MRESULT) TRUE;

      /* WM_APPTERMINATENOTIFY messages are sent from the helper programs e.g. format checker. */
    case WM_APPTERMINATENOTIFY:
      switch(LONGFROMMP(mp1))
        {
        case MSG_CONVERTERROR:
        case MSG_CONVERTDONE:
          WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTOK), TRUE);
          WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTABORT), FALSE);
          break;
        case MPFROMLONG(MSG_CONVERTPERCENT):
          iPercent=LONGFROMMP(mp2);
          if(iPercent>100)
            iPercent=100;
          if(iPercent<0)
            iPercent=0;

          /* Update track percent bar value. The helper prog sends us the actual decoded %. */         
          sprintf(text,"%d#%d%%", iPercent, iPercent);
          WinSetWindowText(WinWindowFromID(hwnd,IDBAR_CONVERTPROGRESS), text);
          break;
        default:
          break;
        }
      return FALSE;

    case WM_CLOSE:
      if(tidThread) {
        bBreak=TRUE;
        if(!WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, ID_TIMER, TIMER_DELAY))
          {
            /* Save window position */
            IniSaveWindowPos(chrIniFile, INI_AUDIOCONV_APP, INI_WINDOWPOS_KEY, hwnd);
            IniSaveWindowClrs(chrIniFile, INI_AUDIOCONV_APP , hwnd);
            WinDismissDlg(hwnd,0);
          }
        else
          return FALSE;
      }
      /* Save window position */
      IniSaveWindowPos(chrIniFile, INI_AUDIOCONV_APP, INI_WINDOWPOS_KEY, hwnd);
      IniSaveWindowClrs(chrIniFile, INI_AUDIOCONV_APP , hwnd);
      WinDismissDlg(hwnd,0);
      return FALSE;
    case WM_CONTROL:
      if(SHORT1FROMMP(mp1)==IDDD_IOPROC)
        {
         if(SHORT2FROMMP(mp1)==EN_CHANGE) {
           SHORT sIdx;
           sIdx=SHORT1FROMMR(WinSendMsg(HWNDFROMMP(mp2),LM_QUERYSELECTION, MPFROMLONG(LIT_FIRST),0));
           iIoProc=iPrivIOProc[sIdx];

#ifdef DEBUG
        SysWriteToTrapLog("sIdx: %d  IO-Proc Idx: %d %s\n", sIdx, iPrivIOProc[sIdx],
                          pMemFormatInfo[iIoProc].szDefaultFormatExt
                          );
#endif

           createTargetName(chrSourceName, pMemFormatInfo[iIoProc].szDefaultFormatExt);
           WinSetWindowText( WinWindowFromID(hwnd,IDEF_CONVERTTARGETNAME), chrTargetName );           
         }
        }
      break;
    case WM_TIMER:
      if(SHORT1FROMMP(mp1)==ID_TIMER)
        {
          if(!tidThread) {
            /* Convert thread ended. Quit. */
            WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, ID_TIMER);
            WinPostMsg(hwnd, WM_CLOSE, 0, 0);
          }
          return MRFALSE;
        }
      break;
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDPB_CONVERTBROWSE:
          {
            char chrTitle[200];
            FILEDLG fd = { 0 };
            /* User pressed the browse button */
            fd.cbSize = sizeof( fd );
            /* It's an centered 'Open'-dialog */
            fd.fl = FDS_OPEN_DIALOG|FDS_CENTER;
            /* Set the title of the file dialog */
            fd.pszTitle = chrTitle;
            if(!getMessage(chrTitle, IDSTR_AUDIOBROWSETITLE, sizeof(chrTitle), RESSOURCEHANDLE, hwnd))
              fd.pszTitle = "Wave name";
            /* Only show * files */
            //sprintf(fd.szFullFile,"%s","*");
            strcpy(fd.szFullFile, chrTargetName);
            if( WinFileDlg( HWND_DESKTOP, hwnd, &fd ) == NULLHANDLE )
              {
                /* WinFileDlg failed */
                break;
              }
            if( fd.lReturn == DID_OK )
              {
                WinSetWindowText( WinWindowFromID(hwnd,IDEF_CONVERTTARGETNAME), fd.szFullFile );
              }
            break;
          }
        case IDPB_CONVERTCLOSE:
          bBreak=TRUE;
          if(!WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, ID_TIMER, TIMER_DELAY))
            WinPostMsg(hwnd, WM_CLOSE, 0, 0);/* Timer error so do a hard quit */
          break;
        case IDPB_CONVERTABORT:
          bBreak=TRUE;
          WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTABORT), FALSE);
          break;
        case IDPB_CONVERTOK:
          {
            FSALLOCATE fsAlloc;
            long long lFreeSpace;
            ULONG ulDiskNum;
            char cLetter;
            
            WinQueryWindowText( WinWindowFromID(hwnd,IDEF_CONVERTTARGETNAME), sizeof(chrTargetName), chrTargetName );

            /* Check if diskspace is sufficient. Only for *.wav */
            if(!stricmp(pMemFormatInfo[iIoProc].szDefaultFormatExt, "WAV")) {
              cLetter=tolower(chrTargetName[0]);
              ulDiskNum=cLetter-'a'+1;
              if(DosQueryFSInfo(ulDiskNum, FSIL_ALLOC,&fsAlloc,sizeof(fsAlloc)))
                lFreeSpace=0;
              else
                lFreeSpace=fsAlloc.cUnitAvail*fsAlloc.cbSector*fsAlloc.cSectorUnit;
            }
            else
              lFreeSpace=lAudioSize; /* Fake sufficient space */

            if(lFreeSpace<lAudioSize) {
              /*
                Text:   
                Title: 
                */       
              getMessage(title, IDSTR_CONVERTNOSPACETEXT,sizeof(title), RESSOURCEHANDLE, hwnd);
              sprintf(text,title,lAudioSize/1000000 );
              getMessage(title, IDSTR_CONVERTNOSPACETITLE,sizeof(title), RESSOURCEHANDLE, hwnd);
              WinMessageBox( HWND_DESKTOP, hwnd, text,
                             title,
                             1234UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
            }
            else {
              /* Start decoding an audio file */
              WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTOK), FALSE);            
              WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTABORT), TRUE);
              convertAudioFile(hwnd);
            }
            
            break;
          }
        default:
          break;
        }
      return (MRESULT) FALSE;
    default:
      break;
    }/* switch */
  
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}


int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  char text[CCHMAXPATH];
  char title[CCHMAXPATH];
  short a;

  /* Create a copy of the args */
  /* argv[0]: progname
   * argv[1]: wavefile
   * argv[2]: IO proc name
   */


  numArgs=argc;

  for(a=0;a<argc;a++)
    {
      params[a]=argv[a];
    }


  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      /* Check if user started prog by hand */   
      if(argc<NUMPARAMS-1) {/* Not the actual num of params */
        pmUsage();
      }
      else {
        /* Save source name */
        strcpy(chrSourceName,params[1]);
        if(argc==NUMPARAMS)
          strcpy(chrProcName,params[2]);
        else {
          bNoProcGiven=TRUE;
          strcpy(chrProcName, "");
        }
        /* Get our ressource dll */  
        //   RESSOURCEHANDLE=0;
        RESSOURCEHANDLE=queryResModuleHandle(argv[0]);
        HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));
        /* Register the percent bar window class */
        percentRegisterBarClass();
        /* Get audio file characteristics */
        if(queryAudioInfo(chrSourceName)) {
          if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, decodeStatusDialogProc, RESSOURCEHANDLE, IDDLG_CONVERT, 0) == DID_ERROR )
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
      }
      freeResHandle();
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}
