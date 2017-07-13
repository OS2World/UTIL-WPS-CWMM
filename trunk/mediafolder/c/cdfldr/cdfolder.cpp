/*
 * This file is (C) Chris Wohlgemuth 2002
 * 
 * It's part of the Media-Folder distribution
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

#define INCL_DOSERRORS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mediafolderres.h"
#include "cdfolder.hh"

extern char chrInstallDir[CCHMAXPATH];

extern HFILE  openDrive(char* drive);
extern void   closeDrive(HFILE hfDrive);
extern int    CDQueryAudioCDTracks(HFILE hfDrive);
LONG extern   CDQueryTrackSecs(ULONG numTrack, char * drive);
extern ULONG launchPMWrapper(PSZ pszTitle, PSZ wrapperExe, PSZ parameters);

BOOL CWCDFolder::cwInsertAllTracks(char* chrCDDrive)
{
  HFILE hfDrive;
  ULONG ulSize;
  ULONG flWindowAttr;
  HWND hwndCnr=NULLHANDLE;
  WPObject *wpObject;

  wpDeleteContents(NULL);

  ulNumTracks=0;

  if(WinIsWindow(WinQueryAnchorBlock(HWND_DESKTOP), hwndFrameCDFldr)) {
    if((hwndCnr=WinWindowFromID(hwndFrameCDFldr, FID_CLIENT))!=NULLHANDLE) {
      WinEnableWindowUpdate(hwndCnr, FALSE);
    }
  }

  //  WinMessageBox(HWND_DESKTOP,HWND_DESKTOP, chrCDDrive, "", 12324, MB_MOVEABLE);
  if((hfDrive=openDrive(chrCDDrive))!=NULLHANDLE)
    {
      char chrFreeDBClient[CCHMAXPATH]={0};
      ulNumTracks=CDQueryAudioCDTracks(hfDrive);
      closeDrive(hfDrive);
      if(ulNumTracks>0) {
        int a;
        char chrTarget[CCHMAXPATH];
        char chrTitle[CCHMAXPATH];
        char chrSetup[CCHMAXPATH];
        ulSize=sizeof(chrTarget);
        wpQueryRealName(chrTarget, &ulSize, TRUE);
        for(a=ulNumTracks;a>0;a--) {
          LONG lSecs;
          lSecs=CDQueryTrackSecs(a, chrCDDrive);
          sprintf(chrSetup,"CWAUDIOPLAYTIME=%d;CWCDTTRACKNUM=%d;OBJECTID=<CDTRACK_%c_%d>", lSecs, a, chrDrive[0], a);
          //sprintf(chrSetup,"CWAUDIOPLAYTIME=%d;CWCDTTRACKNUM=%d", lSecs, a);
          //        if(a==1)
          //        WinMessageBox(HWND_DESKTOP,HWND_DESKTOP, chrSetup, "", 12324, MB_MOVEABLE);
          sprintf(chrTitle,"Track %d",a);
          WinCreateObject("MMCDTrack", chrTitle, chrSetup, chrTarget, CO_REPLACEIFEXISTS);
        }

        /* Query FreeDB */
        if(PrfQueryProfileString(HINI_USERPROFILE, APPKEY_CWMM, KEY_FREEDBCLIENTROOT,"", 
                                 chrFreeDBClient, sizeof(chrFreeDBClient)))
          {
            char chrParams[CCHMAXPATH+20];
            strcat(chrFreeDBClient, FREEDB_CLIENT_NAME);
            sprintf(chrParams, "%s \"%s%s\"", chrCDDrive, chrInstallDir, FREEDB_REXXSCRIPT);
            launchPMWrapper("FreeDB", chrFreeDBClient, chrParams);
          }
      }
      else
        ulNumTracks=0;
    }
  if(hwndCnr) {
    WinShowWindow(hwndCnr, TRUE);
  }
#if 0
  /* Set emphasis of first track */
  wpObject=wpQueryContent(NULLHANDLE, (ULONG)QC_LAST);
  if(somIsObj(wpObject))
    wpObject->wpCnrSetEmphasis(CRA_SELECTED, TRUE);
#endif
  return TRUE;
}






