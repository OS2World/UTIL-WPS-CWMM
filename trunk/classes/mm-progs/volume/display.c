/*
 * This file is (C) Chris Wohlgemuth 2002
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
/*
 * If you need another license for your project/product (commercial,
 * noncommercial, whatever) contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 *
 */

#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_GPILOGCOLORTABLE
#define INCL_SW
#define INCL_GPIREGIONS
#define INCL_WIN

#include <os2.h>

#include "common.h"
#include "mmres.h"
#include "mmplayer.h"
#include <stdio.h>
#include <stdlib.h>

/* 0: time, 1: remaining, 2: filename */
extern int iWhichDisplay;
/* Display timer for changing between track info */
extern ULONG ulDisplayTimer;
extern ULONG ulTotalLength;
/* Current time in track */
extern ULONG ulPos;
char * chrPtrName; /* Pointer to filename */

void _resetDisplay(HWND hwndFrame)
{
  WinSetWindowText(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), "0:00");
  //WinSetWindowULong(WinWindowFromID(WinWindowFromID(hwndTop, IDDLG_TOPMIDDLE), IDST_PLAYTIME), QWL_USER, 0);
  WinPostMsg( WinWindowFromID(hwndFrame, IDSL_MMPLAYERPOSBAR),
              SLM_SETSLIDERINFO,
              MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
              MPFROMLONG(0L));
}



/* Set the text in the playtime area to 'theText' */
void setPlayTimeText2( HWND hwndDialog, char * theText)
{
     WinSetWindowText(WinWindowFromID(hwndDialog, IDST_MMPLAYERPLAYTIME), theText);
}


/* Toggle the visibility of the text in the playtime area. Useful for blinking. */
void togglePlayTimeDisplayVisibility(HWND hwndFrame)
{
  if(WinIsWindowVisible(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME)))
    WinShowWindow(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), FALSE);
  else
    WinShowWindow(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), TRUE);
}


/* Show/hide the text in the playtime area. Useful for blinking. */
void showPlayTimeDisplay(HWND hwndFrame, BOOL bShow)
{
  WinShowWindow(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), bShow);
}


/* This funktion sets the playtime display according to the user settings.
   Supported are remaining time, elapsed time and ID3-tag display */
void setPlayTimeText(hwndFrame)
{
  char text[CCHMAXPATH];
  ULONG ulTemp;

  switch(iWhichDisplay)
    {
    case 1:
      /* Remaining */
      ulTemp=ulTotalLength/1000-ulPos;
      sprintf(text,"-%d:%02d",
              (int)ulTemp/60,
              (int)ulTemp%60);
      WinSetWindowText(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), text);
      break;

    case 2:
      {
        ULONG ulTemp=ulDisplayTimer;

        /* Show filename */
        text[0]=0;
        
        if(ulTemp>=3)
          text[0]=0;
        else if(ulTemp>=0)
          strcpy(text, chrPtrName);
        
#if 0
        else
          ulTemp=15; /* Show playtime for midi files */
#endif
     
        if(ulTemp>=3||!strlen(text)) {
          /* Play time */
          sprintf(text,"%d:%02d",
                  (int)ulPos/60,
                  (int)ulPos%60);
        }
        WinSetWindowText(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), text);
        ulTemp++;
        if(ulTemp>=6)
          ulTemp=0;
        ulDisplayTimer=ulTemp;
        break;
        
#if 0
        /* Show ID3 tag */
        cwAudio=thisPtr->cwQueryCurrentlyPlayingObject();
        if(somIsObj(cwAudio)) {
          if(!methodPtr)
            somhlpQueryCWAudioMethodPtr(thisPtr);
          sprintf(text,"");
          if(methodPtr && somIsObj(cwAudioClass) && cwAudio->somIsA(cwAudioClass) ){/* Query playtime in secs */
            char* chrPtr=text;
            text[0]=0;

            if(ulTemp>=15)
              text[0]=0;
            else if(ulTemp>=12) 
              methodPtr(cwAudio, &chrPtr, sizeof(text), IDINFO_GENRE);
            else if(ulTemp>=9)
              methodPtr(cwAudio, &chrPtr, sizeof(text), IDINFO_COMMENT);
            else if(ulTemp>=6)
              methodPtr(cwAudio, &chrPtr, sizeof(text), IDINFO_ALBUM);
            else if(ulTemp>=3)
              methodPtr(cwAudio, &chrPtr, sizeof(text), IDINFO_NAME);
            else if(ulTemp>=0)
              methodPtr(cwAudio, &chrPtr, sizeof(text), IDINFO_ARTIST);
          }
          else
            ulTemp=15; /* Show playtime for midi files */
          
          if(ulTemp>=15||!strlen(text)) {
            /* Play time */
            sprintf(text,"%d:%02d",
                    thisPtr->ulPos/60,
                    thisPtr->ulPos%60);
          }
          WinSetWindowText(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), text);
          ulTemp++;
          if(ulTemp>=18)
            ulTemp=0;
          ulDisplayTimer=ulTemp;
          break;
        }
#endif
      }
      /* Fall through to playtime */
    case 0:
      /* Elapsed time */
      sprintf(text,"%d:%02d",
              (int)ulPos/60,
              (int)ulPos%60);
      WinSetWindowText(WinWindowFromID(hwndFrame, IDST_MMPLAYERPLAYTIME), text);
      break;
    default:
      break;
    }
}

