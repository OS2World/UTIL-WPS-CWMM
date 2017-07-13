/*
 * This file is (C) Chris Wohlgemuth 2001-2005
 */
#define INCL_WIN

#include <os2.h>

/*!**************************************************/
/*                                                  */
/* @@MODULE                                         */
/*                                                  */
/* Profile functions                                */
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
BOOL PrfSaveWindowPos(HINI hini, char* iniFile, char* chrApp, char* chrKey, HWND hwnd)
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
/* @@MODULE                                         */
/*                                                  */
/* Profile functions                                */
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
BOOL PrfRestoreWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd, ULONG fSize)
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

/*!**************************************************/
/*                                                  */
/* @@MODULE                                         */
/*                                                  */
/* Profile functions                                */
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Save a string in an INI file.                    */
/*                                                  */
/*            .                                     */
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
BOOL PrfWriteString(HINI hini, char * iniFile, char* chrApp, char *chrKey, char* chrString)
{
  BOOL bError=FALSE;
  HINI hiniPriv=hini;

  if(!hini && !iniFile)
    return FALSE;

  do {
    if(!hiniPriv) {
      /* Open ini-file */
      if((hiniPriv=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile))
         ==NULLHANDLE)
        break;
    }/* end of if(!hiniPriv) */

    if(!PrfWriteProfileString(hiniPriv, chrApp, chrKey, chrString))
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
/* @@MODULE                                         */
/*                                                  */
/* Profile functions                                */
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Get a string from an INI file.                   */
/*                                                  */
/*            .                                     */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* If hini is NULL the given file name will be used */
/* for opening the INI file. If hini is not NULL    */
/* the file name is ignored.                        */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulLength                                   */
/*                                                  */
/* TRUE: Function succeeded.                        */
/* FALSE: Error occurred                            */
/*                                                  */
/*!!*************************************************/
BOOL PrfQueryString(HINI hini, char * iniFile, char* chrApp, char *chrKey, char* chrDefault, char* chrBuffer, ULONG ulSize)
{
  HINI hiniPriv=hini;
  ULONG ulRC=0;

  do{
    if(!hiniPriv) {
      if((hiniPriv=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile))
         ==NULLHANDLE)
        break;
    }/* end of if(!hini) */

    ulRC=PrfQueryProfileString(hiniPriv, chrApp, chrKey, chrDefault, chrBuffer, ulSize);

    if(hiniPriv && !hini)
      PrfCloseProfile(hiniPriv);
    
      break;
  } while(TRUE);
  return ulRC;
}


