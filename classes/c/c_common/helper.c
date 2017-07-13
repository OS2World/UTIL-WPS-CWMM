/*
 * This file is (C) Chris Wohlgemuth 2001/2004
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
#include "except.h"
#include "cwmmdataf.h"

#include "cwaudioinc.h"
#include "sys_funcs.h"

#ifndef BS_NOTEBOOKBUTTON
#define BS_NOTEBOOKBUTTON 8L   /* Warp 4 notebook style */  
#endif

#define RESDLLNAME "\\bin\\mmres_%c%c.dll"
#define DEFRESDLLNAME "\\mmres_en.dll" /* bin directory already appended when used */

extern char classDLLPath[CCHMAXPATH];
extern char chrInstallDir[CCHMAXPATH];
extern char resDLLPath[CCHMAXPATH];

HMODULE queryModuleHandle(void);


MRESULT EXPENTRY appTerminateHandlerProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

ULONG launchPMProg(PSZ pszTitle, PSZ wrapperExe, PSZ parameters,  WPObject *thisPtr, ULONG ulView)
{
  HWND         hwndNotify;
  PROGDETAILS pDetails;
  HAPP         happ;
  HWND hwndAppTerminateHandler;
  char startupDir[CCHMAXPATH];
  char *chrPtr;


  memset(&pDetails, 0, sizeof(pDetails)); 
  pDetails.Length          = sizeof(PROGDETAILS);
  pDetails.progt.progc     = PROG_DEFAULT;
  pDetails.progt.fbVisible = SHE_VISIBLE;
  pDetails.pszTitle        = pszTitle;
  pDetails.pszParameters   = parameters;
  /* Set the startup directory to the location of the executable */
  strncpy(startupDir, wrapperExe, sizeof(startupDir));
  //  SysWriteToTrapLog("1: %s %s\n", startupDir, wrapperExe);
  if((chrPtr=strrchr(startupDir,'\\'))!=NULLHANDLE) {
    *chrPtr=0;
    pDetails.pszStartupDir   = startupDir;
    //SysWriteToTrapLog(pDetails.pszStartupDir);
  }
  pDetails.pszExecutable   = wrapperExe;
  
  pDetails.pszEnvironment  = NULLHANDLE;
  pDetails.swpInitial.fl   = SWP_ACTIVATE|SWP_ZORDER; /* window positioning */
  //  pDetails.swpInitial.cy   = 0;    /* width of window */
  //  pDetails.swpInitial.cx   = 0;    /* height of window */
  //  pDetails.swpInitial.y    = 0;    /* lower edge of window */
  //  pDetails.swpInitial.x    = 0;    /* left edge of window */
  pDetails.swpInitial.hwndInsertBehind = HWND_TOP;
  //  pDetails.swpInitial.hwnd             = 0;
  //  pDetails.swpInitial.ulReserved1      = 0;
  //  pDetails.swpInitial.ulReserved2      = 0;
  
  /* Create an object window which will process the WM_APPTERMINATE message. While processing of this
   message the view item (see below) will be removed from the inuse list thus removing the inuse emphasis. */
  hwndAppTerminateHandler=WinCreateWindow(HWND_OBJECT,WC_STATIC,"CWMMAppTerminate",0,0,0,0,0,NULLHANDLE,HWND_BOTTOM,13344,NULL,NULL);
  if(hwndAppTerminateHandler) {
    WinSubclassWindow(hwndAppTerminateHandler,&appTerminateHandlerProc);
    WinSetWindowULong(hwndAppTerminateHandler, QWL_USER,(ULONG)thisPtr);
  }
  /* Start the application */
  happ = WinStartApp( hwndAppTerminateHandler, &pDetails,NULL, NULL, SAF_INSTALLEDCMDLINE);
  // happ = WinStartApp( hwndAppTerminateHandler, &pDetails, parameters, NULL, 0);
  if(happ) {
    /* Application started. Add the object to inuse list. */
    ULONG ulError;
    MMDATAFILEVIEW *pParams=(MMDATAFILEVIEW*)_wpAllocMem(thisPtr,sizeof(MMDATAFILEVIEW), &ulError);

    if(pParams) {
      /* We have memory */
      memset((void*)pParams ,0, sizeof(MMDATAFILEVIEW));
      pParams->pid=0;
      pParams->thisPtr=thisPtr; /* The object */
      pParams->viewItem.view=ulView; /* The open view */
      pParams->useItem.type=USAGE_OPENVIEW; /* We add a view item to the inus list */
      pParams->viewItem.handle=happ; /* a HAPP seems to work here */
      if(somIsObj(thisPtr))
        _wpAddToObjUseList(thisPtr, &pParams->useItem); /* add to inuse list */
    }
  }
  return happ;
}


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

/* Get the handle of our resource DLL */
HMODULE queryResModuleHandle(void)
{
  static ULONG hModule=0;

  if(!hModule)
    {
      
      if(queryModuleHandle())
        {
          char *chrPtr;
          PSZ pszLang="";
          char* found;
          APIRET rc;

          /* Got class dll */
          /* Get install dir (base dir) */
          strncpy(resDLLPath, chrInstallDir, sizeof(resDLLPath));
          resDLLPath[sizeof(resDLLPath)-1]=0; /* Paranoia */

          /* Now get language */
          do
            {
              char buf[CCHMAXPATH];
              /* Get Language var */
              if(NO_ERROR!=DosScanEnv("LANG", &pszLang))
                break;
              
              /* Skip leading spaces */
              chrPtr=pszLang;
              while(*chrPtr==' ')
                chrPtr++;
              
              /* Check if value seems to be valid. The var must be something like xx_XX thus length is 5 */
              if(strlen(chrPtr)<5)
                break;
              
              /* Extract the first two chars */        
              sprintf(buf, RESDLLNAME, chrPtr[0], chrPtr[1]);
              strcat(resDLLPath,buf);
              /* Insert message in Logfile */
              //  HlpWriteToTrapLog("Using the following DLL path: ");
              //  HlpWriteToTrapLog(resDLLPath);
              //  HlpWriteToTrapLog("\n");
        
              rc=DosLoadModule(buf,sizeof(buf), resDLLPath, &hModule);
              if(rc==NO_ERROR)
                break;
        
              /* Insert message in Logfile */
              //  writeLog("Ressource-DLL for the current countrycode not found. Trying to load default one (CDFLD001.DLL).\n");
        
              /* NLS DLL not found. Try to load default */
              found=strrchr(resDLLPath,'\\');
              if(!found)
                break;
              
              *found=0;

              strcat(resDLLPath, DEFRESDLLNAME);
              // HlpWriteToTrapLog("Using the following DLL path: ");
              //   HlpWriteToTrapLog(resDLLPath);
              //   HlpWriteToTrapLog("\n");
              
              rc=DosLoadModule(buf,sizeof(buf), resDLLPath, &hModule);
              if(rc!=NO_ERROR) {
                //        writeLog("Can't load DLL!\n");
                hModule=NULLHANDLE;
              }
              else {
                //writeLog("Ressource DLL loaded.\n");
              }
              
              break;
            }while(TRUE);
        }
    }
   return hModule;
 }

APIRET APIENTRY DosQueryModFromEIP( HMODULE *phMod,
                                    ULONG *pObjNum,
                                    ULONG BuffLen,
                                    PCHAR pBuff,
                                    ULONG *pOffset,
                                    PVOID Address );

 /* Get the HMODULE of the MM class DLL */
HMODULE queryModuleHandle(void)
{
  PSZ moduleName;
  static ULONG hModule=0;
  somId mySomId;

  if(!hModule)
    {
      char *chrPtr;
      ULONG ulObj, ulBufLen, ulOffset;
      char thePath[CCHMAXPATH];
      
      if(DosQueryModFromEIP( &hModule,
                             &ulObj,
                             CCHMAXPATH,
                             thePath,
                             &ulOffset,
                             (PVOID)queryModuleHandle )!=NO_ERROR) {
        SysWriteToTrapLog("Can't get module handle for CWMM class DLL!\n");
        hModule=0;
        return NULLHANDLE; /* Error */
      }
#if 0
    /*
      Sometimes the returned name isn't correct. To prevent loading a bunch of classes just
      to get the module handle the following is replaced by the undocumented DosQueryModFromEIP()
      call. Maybe this even fixes some deadlocks people had during WPS startup with the classes
      because now the WPS isn't urged to load all classes at once.
     */

      mySomId=somIdFromString("M_MMImage");
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
          mySomId=somIdFromString("MMImage");
          moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
          SOMFree(mySomId);
          if( DosQueryModuleHandle(moduleName, &hModule))
            {
              mySomId=somIdFromString("M_MMAudio");
              moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
              SOMFree(mySomId);
              if( DosQueryModuleHandle(moduleName, &hModule))
                {
                  mySomId=somIdFromString("M_MMAVC");
                  moduleName=_somLocateClassFile(SOMClassMgrObject, mySomId, 1, 1);
                  SOMFree(mySomId);
                  DosQueryModuleHandle(moduleName, &hModule);
                }
            }
        }
      strncpy(classDLLPath,moduleName, sizeof(classDLLPath));
      classDLLPath[sizeof(classDLLPath)-1]=0;
      
      /* Get install dir */
      strncpy(chrInstallDir,moduleName, sizeof(chrInstallDir));
      chrInstallDir[sizeof(chrInstallDir)-1]=0;
      if((chrPtr=strrchr(chrInstallDir,'\\'))!=NULLHANDLE)
        *chrPtr=0;
#endif

      /* Get module name. */
      if(!DosQueryModuleName(hModule,sizeof(chrInstallDir),chrInstallDir)) {
        strncpy(classDLLPath,chrInstallDir, sizeof(classDLLPath));
        classDLLPath[sizeof(classDLLPath)-1]=0;
        
        /* Get install dir */
        if((chrPtr=strrchr(chrInstallDir,'\\'))!=NULLHANDLE)
          *chrPtr=0;
      }
    }/* if(!hModule) */
  return hModule;
}


PSZ queryModuleName(void)
{
  if(queryModuleHandle())
    return classDLLPath;

  return NULLHANDLE;
}

PSZ queryInstallDir(void)
{
  if(queryModuleHandle())
    return chrInstallDir;

  return NULLHANDLE;
}

PSZ _queryMMClassIniFile(void)
{
  static char iniName[CCHMAXPATH]= {0};

  if(iniName[0]==0)
    sprintf(iniName, "%s\\bin\\CWMM.INI", queryInstallDir());

  return iniName;
}

BOOL cwMoveNotebookButtonsWarp4(HWND hwndDlg, USHORT usID, USHORT usDelta)
{
  if(SysQueryOSRelease()>=40) {
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

/* This function checks if the given file exists */ 
ULONG checkFileSize(char* chrFileName)
{
  struct stat statBuf;

  /* Check file path */
  if(stat(chrFileName , &statBuf)==-1)
    return 0;
 
  return statBuf.st_size;
}
#endif









