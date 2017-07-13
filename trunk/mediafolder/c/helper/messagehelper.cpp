/*
 * This file is (C) Chris Wohlgemuth 1999-2004
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

#define INCL_WIN

#include <os2.h>
#include <stdio.h>
#include <string.h>

#define DEBUG

void errorResource2(char * chrTitle);


void errorResource()
{
  /*
    Don't show a message box because on WPS startup this may lead to a deadlock.
    At least I suspect this happens on system where the MM classes don't work.
    Instead there's a new function errorResourceVerbose() which may be called when we
    know, the WPS is up and running. 
   */

  errorResource2("Problem with Media-Folder installation");

}

void errorResource2(char * chrTitle)
{
  /*
    Don't show a message box because on WPS startup this may lead to a deadlock.
    At least I suspect this happens on system where the MM classes don't work.
    Instead there's a new function errorResourceVerbose() which may be called when we
    know, the WPS is up and running. 
   */
}

void errorResourceVerbose(char * chrTitle)
{
#ifdef ECS
  WinMessageBox(HWND_DESKTOP,0,
                "The resource DLL which contains all the dialogs, graphics and messages cannot be loaded. \
Please check your installation. There must be a file MFLDR_xx.DLL in the .\\MMOS2\\MEDIAFLD\\BIN directory of the \
Media-Player package. xx is the language code of your system e.g. DE for Germany. This code is taken from the \
SET LANG=xx_XX environment variable in your config.sys. If there is no \
support for your language there must be at least the file MFLDR_EN.DLL. The Media-Player features \
are not available!",
                chrTitle,12345,
                MB_OK|MB_MOVEABLE|MB_ERROR);
#else
  WinMessageBox(HWND_DESKTOP,0,
                "The resource DLL which contains all the dialogs, graphics and messages cannot be loaded. \
Please check your installation. There must be a file MFLDR_xx.DLL in the BIN directory of the \
Media-Player package. xx is the language code of your system e.g. DE for Germany. This code is taken from the \
SET LANG=xx_XX environment variable in your config.sys. If there is no \
support for your language there must be at least the file MFLDR_EN.DLL. The Media-Player features \
are not available!",
                chrTitle,12345,
                MB_OK|MB_MOVEABLE|MB_ERROR);
#endif
}

ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
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
  return WinMessageBox(  HWND_DESKTOP, hwnd, text, title, 0UL, ulFlags );
}

BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd)
{
  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulID,lSizeText,text)) {
    sprintf(text,"");
    return FALSE;
  }
  return TRUE;
}



