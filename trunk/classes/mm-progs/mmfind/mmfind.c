/*
 * mmplayer.c (C) Chris Wohlgemuth 2004
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
#include "mmres.h"
#include "mmprogs_defaults.h"
#include "sys_funcs.h"
#include "message_funcs.h"

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

#define TIMERID_SELECTION 1

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
MRESULT menuInsertMenuSeparator(HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition );
SHORT menuInsertMenuItem( HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition, int iID, char * chrText);
void pmUsage();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);

HBITMAP loadBitmap ( PSZ pszFileName, PBITMAPINFOHEADER2 pBMPInfoHeader2);
VOID DrawBitmap ( HWND hwnd );
MRESULT EXPENTRY bmpPreviewProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
void showImageControls(HWND hwnd, BOOL bShow);

char logName[]="mmfind.log";

char chrIniFile[CCHMAXPATH];

#define NUMPARAMS  1
/*
  argv[0]: progname
 */
int numArgs;
char* params[NUMPARAMS];

HMODULE RESSOURCEHANDLE=0;

#define numMediaType 3
#define MT_AUDIO 0
#define MT_IMAGE 1
#define MT_VIDEO 2
#define MT_UNKNOWN 100

char chrMediaFileTypes[numMediaType][100]= {"Audio","Image","Video"};
char chrMediaFileExt[numMediaType][300]= {"*.AIF;*.AU;*.FLA;*.FLAC;*.IFF;*.MID;*.MP3;*.OGG;*.SND;*.VOC;*.WAV;*._AU",
                                          "*.BMP;*.DIB;*.GIF;*.JPG;*.PCX;*.PNG;*.PCD;*.TIF;*.TGA;*.VID;*._IM",
                                          "*.AVI;*.MPG;*.FLI;*.FLC;*.MOV;*.QT;*.VBS"};
int iMediaType=MT_AUDIO;

char audioText[1000]={"Play time:\t%02d:%02d\n\nSamplerate:\t%d\n\nBits per sample:\t%d\n\nChannels:\t%d"};

HAB globalHab;

/* BMP info for preview */
BITMAPINFOHEADER2 bmpInfoHeader2;
HBITMAP hBitmap;

FILEDLG fd = { 0 };

void loadStrings(void)
{

}

void showVideoControls(HWND hwnd, BOOL bShow)
{
  /* Preview area */
  WinShowWindow(WinWindowFromID(hwnd, IDST_NOPREVIEWAVAILABLE), bShow);
  /* Group box */
  WinShowWindow(WinWindowFromID(hwnd, IDGB_PREVIEW), bShow);
}

void showAudioControls(HWND hwnd, BOOL bShow)
{
  /* Preview area */
  WinShowWindow(WinWindowFromID(hwnd, IDST_AUDIOINFO), bShow);
  /* Group box */
  WinShowWindow(WinWindowFromID(hwnd, IDGB_AUDIOINFO), bShow);
}


ULONG mmIdentifyFile(char* chrFile)
{
  MMIOINFO mmioInfo={0};
  MMFORMATINFO mmFormatInfo={0};
  FOURCC fourCC;

  //  SysWriteToTrapLog("%s\n", chrFile);
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


MRESULT findFileDlgProc(HWND hwnd, ULONG  msg, MPARAM mp1, MPARAM mp2)
{

  switch(msg)
    {
    case WM_INITDLG:
      {
        int a;

        /* Set dialog font */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        /* Fill combo box with media types (audio, image, video) */
        for(a=0;a<numMediaType;a++) 
          WinSendMsg(WinWindowFromID(hwnd, IDCB_TYPEOFFILE), LM_INSERTITEM, MRFROMSHORT(LIT_SORTASCENDING), 
                     MPFROMP(chrMediaFileTypes[a]));
        WinSetWindowText(WinWindowFromID(hwnd, IDCB_TYPEOFFILE), chrMediaFileTypes[0]);

        /* Subclass preview area */
        WinSubclassWindow(WinWindowFromID(hwnd, IDSR_IMGPREVIEW), bmpPreviewProc);
        break;
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
                for(a=0; a<numMediaType;a++) {
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
                WinStartTimer(globalHab, hwnd, TIMERID_SELECTION, 100);
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

            WinStopTimer(globalHab, hwnd, TIMERID_SELECTION);
            WinQueryWindowText(WinWindowFromID(hwnd, 258), sizeof(chrName), chrName);
            strcpy(chrFullName, fd.szFullFile);
            strcat(chrFullName, chrName);

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
                    break;
                  }

                  ulPlaySecs=mmAudioHeader.mmXWAVHeader.XWAVHeaderInfo.ulAudioLengthInMS/1000;
                  sprintf(text, audioText,
                          ulPlaySecs/60, ulPlaySecs%60,
                          mmAudioHeader.mmXWAVHeader.WAVEHeader.ulSamplesPerSec,
                          mmAudioHeader.mmXWAVHeader.WAVEHeader.usBitsPerSample,
                          mmAudioHeader.mmXWAVHeader.WAVEHeader.usChannels);
                  WinSetWindowText(WinWindowFromID(hwnd, IDST_AUDIOINFO), text);
                  
                  break;
                }
              case MT_IMAGE:
                if(hBitmap) {
                  HBITMAP hBitmapTemp;
                  hBitmapTemp=loadBitmap (chrFullName, &bmpInfoHeader2);            
                  GpiDeleteBitmap(hBitmap);
                  hBitmap=hBitmapTemp;
                }
                else
                  hBitmap=loadBitmap (chrFullName, &bmpInfoHeader2);
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
        case IDPB_NEWFILE: /* New */
          {
            HOBJECT hObject;
            if((hObject=WinQueryObject("<MMPM2_MMTEMPLATEFOLDER>"))!=NULLHANDLE) {
              WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
              WinDismissDlg(hwnd, DID_CANCEL);
            }
            else
              showMessageBox2(hwnd, IDSTR_NOMMTEMPLATEFLDRTITLE, IDSTR_NOMMTEMPLATEFLDR, RESSOURCEHANDLE, MB_MOVEABLE|MB_OK);

            return MRFALSE;
          }
        default:
          break;
        }
    default:
      break;
    }
  return WinDefFileDlgProc(hwnd, msg, mp1, mp2);
}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  char text[CCHMAXPATH];
  short a;

  //  FILEDLG fd = { 0 };

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
        HWND hwndDlg;

        /* Get our ressource dll */  
        RESSOURCEHANDLE=queryResModuleHandle(argv[0]);
        /* Get data from INI file */
        HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));

        globalHab=WinQueryAnchorBlock(HWND_DESKTOP);

        fd.cbSize = sizeof( fd );
        /* It's an centered 'Open'-dialog */
        fd.fl = FDS_OPEN_DIALOG|FDS_CENTER | FDS_CUSTOM;
        /* Title: "Search CDRecord/2" */
        //getMessage(text,IDSTR_FDLGSEARCHCDRDAOTITLE,sizeof(text), hSettingsResource,hwnd);
        /* Set the title of the file dialog */
        //fd.pszTitle = text;
        //        fd.pszTitle = "Find file";
        /* Only show *.wav files */
        sprintf(fd.szFullFile,"%s","*.wav");
        fd.hMod=RESSOURCEHANDLE;
        fd.usDlgId=IDDLG_MMFIND;
        fd.pfnDlgProc=findFileDlgProc;
        //SysWriteToTrapLog("%x\n", fd.hMod);
        if( (hwndDlg=WinFileDlg( HWND_DESKTOP, HWND_DESKTOP, &fd )) != NULLHANDLE )
          {
            /* Set dialog font to WarpSans for Warp 4 and above */
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
                      char *chr;
                      if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                        WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
                      if((chr=strrchr(fd.szFullFile, '\\'))!=NULLHANDLE) {
                        *chr=0;
                        if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                          WinOpenObject(hObject, OPEN_CONTENTS, TRUE);
                      }
                      break;
                    }
                  case MT_VIDEO:
                    {
                      HOBJECT hObject;
                      char *chr;
                      if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                        WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
                      if((chr=strrchr(fd.szFullFile, '\\'))!=NULLHANDLE) {
                        *chr=0;
                        if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                          WinOpenObject(hObject, OPEN_CONTENTS, TRUE);
                      }
                      break;
                    }
                  default:
                    {
                      HOBJECT hObject;
                      char *chr;
                      //if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                      // WinOpenObject(hObject, OPEN_DEFAULT, TRUE);
                      if((chr=strrchr(fd.szFullFile, '\\'))!=NULLHANDLE) {
                        *chr=0;
                        if((hObject=WinQueryObject(fd.szFullFile))!=NULLHANDLE)
                          WinOpenObject(hObject, OPEN_CONTENTS, TRUE);
                      }
                      break;
                    }
                  }/* switch()*/
              }/* DID_OK */
          }
        //        freeResHandle();
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}

