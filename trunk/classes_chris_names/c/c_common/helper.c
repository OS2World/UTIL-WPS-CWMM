/*
 * This file is (C) Chris Wohlgemuth 2001/2002
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
 * If you need another license for your prject/product contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 */
#define INCL_WIN
#define INCL_DOS
#define INCL_DOSERRORS

#include <os2.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cwimage.ih"
#include "excptLogName.h"

#ifndef BS_NOTEBOOKBUTTON
#define BS_NOTEBOOKBUTTON 8L   /* Warp 4 notebook style */  
#endif

extern char classDLLPath[CCHMAXPATH];

char HlpGetBootDriveLetter(void)
{
  ULONG ulSysValue;

  if(!DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE,&ulSysValue,sizeof(ulSysValue)))
    return 'a'+ulSysValue-1;

  return 'c';
} 

void HlpWriteToTrapLog(const char* chrFormat, ...)
{
  char logNameLocal[CCHMAXPATH];
  FILE *fHandle;

  sprintf(logNameLocal,"%c:\\%s", HlpGetBootDriveLetter(), EXCEPTION_LOGFILE_NAME);
  fHandle=fopen(logNameLocal,"a");
  if(fHandle) {
    va_list arg_ptr;
    void *tb;

    va_start (arg_ptr, chrFormat);
    vfprintf(fHandle, chrFormat, arg_ptr);
    va_end (arg_ptr);
    //    fprintf(fHandle,logText);
    fclose(fHandle);
  }
}


//void writeLog(char* logText)
void writeLog(const char* chrFormat, ...)
{
  char logNameLocal[CCHMAXPATH];
  FILE *fHandle;

  sprintf(logNameLocal,"d:\\cwmm.log");
  fHandle=fopen(logNameLocal,"a");
  if(fHandle) {
    va_list arg_ptr;
    void *tb;

    va_start (arg_ptr, chrFormat);
    vfprintf(fHandle, chrFormat, arg_ptr);
    va_end (arg_ptr);
    //    fprintf(fHandle,logText);
    fclose(fHandle);
  }
}

HMODULE queryModuleHandle(void)
{
  PSZ moduleName;
  static ULONG hModule=0;
  somId mySomId;

  if(!hModule)
    {
      mySomId=somIdFromString("M_CWImage");
      moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
      /* The above function sometimes returns the class name instead of the dll name.
         I suspect it's because with DTS there's no dllname modifier to tell SOM
         the name. But why does it sometimes work??
         To circumvent the problem not having a module handle we try this function for
         the other classnames again hoping that one of the calls is succesful (until now
         it is). */
      SOMFree(mySomId);
      if( DosQueryModuleHandle(moduleName, &hModule))
        {
          mySomId=somIdFromString("CWImage");
          moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
          SOMFree(mySomId);
          if( DosQueryModuleHandle(moduleName, &hModule))
            {
              mySomId=somIdFromString("M_CWAudio");
              moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
              SOMFree(mySomId);
              if( DosQueryModuleHandle(moduleName, &hModule))
                {
                  mySomId=somIdFromString("M_CWAVC");
                  moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
                  SOMFree(mySomId);
                  DosQueryModuleHandle(moduleName, &hModule);
                }
            }
        }
      strncpy(classDLLPath,moduleName, sizeof(classDLLPath));
      classDLLPath[sizeof(classDLLPath)-1]=0;
    }
  return hModule;
}


PSZ queryModuleName(void)
{
  if(queryModuleHandle())
    return classDLLPath;

  return NULLHANDLE;
}

/****************************************************
 *                                                  *
 * This funktion inserts a separator into menu      *
 * <hwndMenu> and submenu <hwndSubMenu> at          *
 * position <iPosition>                             *
 *                                                  *
 ****************************************************/
MRESULT HlpInsertMenuSeparator(int iPosition, HWND hwndMenu, HWND hwndSubMenu)
{
  MENUITEM mi;

  /* Fill the MENUITEM structure */
  mi.iPosition=iPosition;
  mi.afStyle=MIS_SEPARATOR;
  if(hwndSubMenu)
    mi.afStyle|=MIS_SUBMENU;
  mi.id=0;
  mi.afAttribute=NULLHANDLE;                
  mi.hwndSubMenu=hwndSubMenu;
  mi.hItem=NULLHANDLE;

  return WinSendMsg(hwndMenu,MM_INSERTITEM,(MPARAM)&mi,
                    (MPARAM)NULL);                 
}


/****************************************************
 *                                                  *
 * This funktion inserts an item into menu          *
 * <hwndMenu> and submenu <hwndSubMenu> at          *
 * position <iPosition>                             *
 *                                                  *
 ****************************************************/
MRESULT HlpInsertMenuItem(int iPosition, HWND hwndMenu, HWND hwndSubMenu, int iID, char * chrText)
{
  MENUITEM mi;

  /* Fill the MENUITEM structure */
  mi.iPosition=iPosition;
  mi.afStyle=MIS_TEXT;
  if(hwndSubMenu)
    mi.afStyle|=MIS_SUBMENU;
  mi.id=iID;
  mi.afAttribute=NULLHANDLE;                
  mi.hwndSubMenu=hwndSubMenu;
  mi.hItem=NULLHANDLE;
 
  return WinSendMsg(hwndMenu,MM_INSERTITEM,(MPARAM)&mi,
                    (MPARAM)chrText);                 
}


/****************************************************
 *                                                  *
 * This funktion returns the running OS version:    *
 *                                                  *
 * 30: Warp 3, 40 Warp 4                            *
 *                                                  *
 ****************************************************/
ULONG cwQueryOSRelease(void)
{
  static ULONG ulVersionMinor=0;

  if(!ulVersionMinor)
    if(DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ulVersionMinor, sizeof(ulVersionMinor)))
      ulVersionMinor=30;/* Default Warp 3 */

  return ulVersionMinor;

}

BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, USHORT usID, USHORT usDelta)
{
  if(cwQueryOSRelease()>=40) {
    HENUM henum;
    HWND hwnd;
    
    /* Move the default notebook buttons */
    if((henum=WinBeginEnumWindows(hwndDlg))!=NULLHANDLE) {
      while((hwnd=WinGetNextWindow(henum))!=NULLHANDLE) {
        if(WinQueryWindowUShort(hwnd,QWS_ID)<=usID)
          WinSetWindowBits(hwnd, QWL_STYLE,
                           BS_NOTEBOOKBUTTON, BS_NOTEBOOKBUTTON);
        else {
          SWP swp;
          POINTL ptl= {0};
          ptl.y=usDelta;

          WinMapDlgPoints(hwndDlg, &ptl, 1, TRUE);
          /* Move all other controls */
          if(WinQueryWindowPos(hwnd, &swp))
            WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y-ptl.y,0,0, SWP_MOVE);
        }
      }
      WinEndEnumWindows(henum);
    }
  }
  return TRUE;
}

void HlpSendCommandToObject(char* chrObject, char* command)
{
  HOBJECT hObject;

  hObject=WinQueryObject(chrObject);
  if(hObject!=NULLHANDLE) {
    WinSetObjectData(hObject, command);
  }
}

#if 0
void HlpOpenWebPage(char* chrUrl)
{
  HOBJECT hObject;
  char tempDir[CCHMAXPATH];
  char setup[CCHMAXPATH*2];

  if(HlpQueryTempDir(chrInstallDir, tempDir, sizeof(tempDir)))
    {
      snprintf(setup, sizeof(setup), "LOCATOR=%s;%s", chrUrl, "OPEN=DEFAULT");
      WinCreateObject("WPUrl","tempUrl", setup, tempDir, 
                      CO_UPDATEIFEXISTS);
        /* WinDestroyObject(hObject); Don't use it, blocks PM */
    }
}
#endif




