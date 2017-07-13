
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
 * If you need another license for your project/product contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 */
#define INCL_PM

#include <os2.h>
#include <stdio.h>

void errorResource(void)
{

  WinMessageBox(HWND_DESKTOP,0,
                "The resource DLL which contains all the dialogs, graphics and messages cannot be loaded. \
Please check your installation.",
"Problem with multimedia class installation",12345,
                MB_OK|MB_MOVEABLE|MB_ERROR);

};

BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd)
{
  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulID,lSizeText,text)) {
    sprintf(text,"");
    return FALSE;
  }
  return TRUE;
};

static ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags)
{

  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulTextID,lSizeText,text)) {
    errorResource();
    return MBID_ERROR;
  }
  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulTitleID,lSizeTitle,title)) {
    errorResource();
    return MBID_ERROR;
  }
  return WinMessageBox(  hwnd, hwnd, text, title, 0UL, ulFlags );
};


/* Show a messagebox with the text ulIDText and title ulIDTitle loaded from
   the resource DLL */
ULONG showMsgBox(ULONG ulIDTitle, ULONG ulIDText, HMODULE hModule)
{
  char text[256];
  char title[100];
  
  return messageBox(  text, ulIDText , sizeof(text),
               title, ulIDTitle, sizeof(title),
               hModule, HWND_DESKTOP, MB_OK | MB_MOVEABLE | MB_ERROR);
};

ULONG showMsgBox2(ULONG ulIDTitle, ULONG ulIDText, HMODULE hModule, ULONG ulFlag)
{
  char text[256];
  char title[100];
  
  return messageBox(  text, ulIDText , sizeof(text),
               title, ulIDTitle, sizeof(title),
               hModule, HWND_DESKTOP, ulFlag);
};





