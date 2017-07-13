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
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_SW

#include "mediafolderres.h"


#include <stdio.h>
#include <stdlib.h>

#include "cdfolder.hh"


extern HMODULE hResource;
extern PFNWP pfnwpGenericCDFrame;

/* Extern */
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
MRESULT cwInsertMenuItem(int iPosition, HWND hwndMenu, HWND hwndSubMenu, int iID, char * chrText);
MRESULT cwInsertMenuSeparator(int iPosition, HWND hwndMenu, HWND hwndSubMenu);

void writeLogPrintf(char* logFile, char* format, ...);


extern SOMClass* cwGetSomClass(char* chrClassName);

/************************************************************
 *                                                          *
 * This frame proc handles the about menuitem of the        *
 * Warp 4 menu bar and removes the Tree view item.          *
 *                                                          *
************************************************************/
MRESULT EXPENTRY cdFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  CWCDFolder* thisPtr;
  HWND hwndDialog;
  MENUITEM mi;
  char text[100];

  switch (msg)
    {
    case WM_INITMENU:
      if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
        {
          thisPtr=(CWCDFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
          if(somIsObj(thisPtr)) {
            switch(thisPtr->usLastSelMenuItem)
              {
              case 0x2d1: /* view menu */
                /* Remove change to icon view from menubar menu */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x2cc,TRUE),0);
                /* Remove sort from menubar. It's already disabled but anyway... */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT( 5,TRUE),0);
                /* Remove arrange from menubar. It's already disabled but anyway... */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT( 0x135,TRUE),0);
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT( 0x136,TRUE),0);

                break;
              case 0x2d0: /* edit menu */
                /* The following are for a content object */
                /* Remove create from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x65,TRUE),0);
                /* Remove move from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x6b,TRUE),0);
                /* Remove copy from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x6c,TRUE),0);
                /* Remove insert from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x2cb,TRUE),0);
                /* Remove delete from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x6d,TRUE),0);
                /* Remove create shadow from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x13c,TRUE),0);
                break;
              case 0x2cf: /* Folder menu */
                /* Remove copy from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x6c,TRUE),0);
                /* Remove create from menubar */ 
                WinSendMsg(HWNDFROMMP(mp2),MM_REMOVEITEM,MPFROM2SHORT(0x65,TRUE),0);
              default:
                break;
              }
          }
        }
      break;
    default:
      break;
    }
  /* Any other message is handled by the folder frame procedure */
  if((hwndDialog=WinWindowFromID(hwnd,IDDLG_TOPFRAME))!=NULLHANDLE)
    {
      thisPtr=(CWCDFolder*) WinQueryWindowULong(hwndDialog,QWL_USER);
      if(somIsObj(thisPtr)) {
        if(thisPtr->pfnwpCDFrame)
          return thisPtr->pfnwpCDFrame(hwnd, msg, mp1, mp2);
      }
    }
  return  pfnwpGenericCDFrame(hwnd, msg, mp1, mp2);

}




