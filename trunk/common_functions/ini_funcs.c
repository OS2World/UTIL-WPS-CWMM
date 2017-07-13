/*
 * This file is (C) Chris Wohlgemuth 2001-2004
 */

#define INCL_WIN

#include <os2.h>

/*
  hini may be NULL. Then the filename is used to open the ini file.
 */
/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion stores a window position in an INI */
/* file.                                            */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* If hini is NULL the given file name will be used */
/* for opening the INI file. If hini is not NULL    */
/* the file name is ignored.                        */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* BOOL fSuccess                                    */
/*                                                  */
/* TRUE: Function succeeded.                        */
/* FALSE: Error occurred                            */
/*                                                  */
/*!!*************************************************/
BOOL IniSaveWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd)
{
  BOOL bError=FALSE;
  SWP swp;
  HINI hiniPriv=hini;

  if(!hini && !iniFile)
    return FALSE;

  do{

    if(!hiniPriv) {
      /* Open ini-file */
      if((hiniPriv=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile))
         ==NULLHANDLE)
        break;
    }/* end of if(!hiniPriv) */

    WinQueryWindowPos(hwnd, &swp);
    if(!PrfWriteProfileData(hiniPriv, chrApp, chrKey, &swp, sizeof(swp)))
      bError=TRUE;

    if(hiniPriv && !hini)
      PrfCloseProfile(hiniPriv);
    
    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  return FALSE;
}

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion gets a saved window position from  */
/* an INI file and resizes and moves the window     */
/* accordingly.                                     */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* If hini is NULL the given file name will be used */
/* for opening the INI file. If hini is not NULL    */
/* the file name is ignored.                        */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* BOOL fSuccess                                    */
/*                                                  */
/* TRUE: Function succeeded.                        */
/* FALSE: Error occurred                            */
/*                                                  */
/*!!*************************************************/
BOOL IniRestoreWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd, ULONG fSize)
{
  HINI hiniPriv=hini;
  ULONG ulSize;
  BOOL bError=FALSE;
  SWP swp;

  do{
    if(!hiniPriv) {
      if((hiniPriv=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile))
         ==NULLHANDLE)
        break;
    }/* end of if(!hini) */

    ulSize=sizeof(swp); 
    if(!PrfQueryProfileData(hiniPriv, chrApp, chrKey, &swp, &ulSize))
      bError=TRUE;

    if(hiniPriv && !hini)
      PrfCloseProfile(hiniPriv);
    
    if(bError)
      break;
    WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y, swp.cx, swp.cy, SWP_MOVE | fSize);
    return TRUE;
  } while(TRUE);

  return FALSE;
}



