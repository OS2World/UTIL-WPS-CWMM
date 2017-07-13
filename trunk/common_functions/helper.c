/*
 * This file is (C) Chris Wohlgemuth 2001/2003
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
#define INCL_GPI
#define INCL_DOS
#define INCL_DOSERRORS

#include <os2.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "mmprogs_defaults.h"
#include "sys_funcs.h"

//#define INSTALLDIR 1

#define INI_BGCLR_KEY       "bg"
#define INI_FGCLR_KEY       "fg"
#define INI_BTNBGCLR_KEY       "btnbg"
#define INI_BTNFGCLR_KEY       "btnfg"
#define INI_ACTIVETBBGCLR_KEY       "activetbbg"
#define INI_INACTIVETBBGCLR_KEY       "inactivetbbg"
#define INI_ACTIVETBFGCLR_KEY       "activetbfg"
#define INI_INACTIVETBFGCLR_KEY       "inactivetbfg"

/* The subdirectory in the installation directory where to put the log files */
#define LOGFILE_SUBDIR "Logfiles"
#define EXCEPTION_LOGFILE_NAME "CWMMCls.log"

#define RESDLLNAME "\\mmres_%c%c.dll"
#define DEFRESDLLNAME "\\mmres_en.dll"

extern char* params[];
extern HMODULE RESSOURCEHANDLE;
extern char logName[];

char chrInstallDir[CCHMAXPATH];

SWP swpWindow;

HMODULE _queryResModuleHandle2(char *installDir);
BOOL IniSaveData(char * iniFile, char* chrApp, char *chrKey, void* theData, ULONG ulSize);
BOOL IniRestoreData(char * iniFile, char* chrApp, char *chrKey, void * theData, ULONG* ulMaxSize);
void writeLog(char* logText);
void HlpWriteToTrapLog(const char* chrFormat, ...);


void removeLog(void)
{
  char logNameLocal[CCHMAXPATH];
  
  sprintf(logNameLocal,"%s\\%s\\%s",params[1],LOGFILE_SUBDIR,logName);

  remove(logNameLocal);
}

void removeLog2(char * installDir, char * logName)
{
  char logNameLocal[CCHMAXPATH];

  sprintf(logNameLocal,"%s\\%s\\%s",installDir,LOGFILE_SUBDIR,logName);
  remove(logNameLocal);
}

void writeLog(char* logText)
{
  char logNameLocal[CCHMAXPATH];
  FILE *fHandle;

  sprintf(logNameLocal,"%s\\%s\\%s",params[1],LOGFILE_SUBDIR,logName);
  fHandle=fopen(logNameLocal,"a");
  if(fHandle) {
    fprintf(fHandle,logText);
    fclose(fHandle);
  }
}

BOOL buildLogName( char * outBuf, char * logName,ULONG ulSize)
{
  if(snprintf(outBuf, ulSize,"logfiles\\%s",logName)==EOF)
    return FALSE;
  return TRUE;
}


/* This function launches the wrapper <wrapperExe>         */
/*  with the params given as a PM-session */
/* in PSZ parameter. PSZ folderPath is the path to put the */
/* write.log. HWND hwnd is the window waiting for the      */
/* WM_APPTERMINATE message                                 */
ULONG launchPMWrapper(PSZ chrInstallDir, PSZ parameter, PSZ folderPath, PSZ wrapperExe, PSZ pszTitle)
{
  STARTDATA startData={0};
  APIRET rc;
  PID pid;
  ULONG ulSessionID=0;
  char chrLoadError[CCHMAXPATH];
  char startParams[CCHMAXPATH*4];
  char exename[CCHMAXPATH]={0};
  char chrFolderPath[CCHMAXPATH+10];
  
  memset(&startData,0,sizeof(startData));
  startData.Length=sizeof(startData);
  startData.Related=SSF_RELATED_INDEPENDENT;
  startData.FgBg=SSF_FGBG_FORE;
  startData.TraceOpt=SSF_TRACEOPT_NONE;
  startData.PgmTitle=pszTitle;
    
  /*  sprintf(exename,"%s",buildWrapName(wrapperExe));*/
  sprintf(exename,"%s\\bin\\%s",params[1],wrapperExe);
  /*
    if(!checkHelper(exename))
    return -1;
    */

  startData.PgmName=exename;
  startData.InheritOpt=SSF_INHERTOPT_SHELL;
  startData.SessionType=SSF_TYPE_PM;
  startData.PgmControl=0;
  startData.InitXPos=30;
  startData.InitYPos=30;
  startData.InitXSize=500;
  startData.InitYSize=400;
  startData.ObjectBuffer=chrLoadError;
  startData.ObjectBuffLen=(ULONG)sizeof(chrLoadError);

  /* Put the exe-path between " " to make sure, spaces are handled correctly */
  strcpy(chrFolderPath,folderPath);
  sprintf(startParams,"\"%s\" \"%s\" %s",
          chrInstallDir,chrFolderPath, parameter);
  startData.PgmInputs=startParams;

  rc=DosStartSession(&startData,&ulSessionID,&pid);   
  return 0;   
}


BOOL IniRestoreWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd)
{
  HINI hini=0;
  ULONG ulSize;
  BOOL bError=FALSE;
  SWP swp;

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile);
  do{
    if(!hini) {
#if 0
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
#endif
      break;
    }/* end of if(!hini) */

    ulSize=sizeof(swp); 
    if(!PrfQueryProfileData(hini, chrApp, chrKey, &swp, &ulSize))
      bError=TRUE;

    if(hini)
      PrfCloseProfile(hini);
    
    if(bError)
      break;
    WinSetWindowPos(hwnd, NULLHANDLE, swp.x, swp.y, swp.cx, swp.cy, SWP_MOVE/*|SWP_SIZE*/);
    return TRUE;
  } while(TRUE);

  return FALSE;
}

BOOL IniSaveWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd)
{
  HINI hini=0;
  BOOL bError=FALSE;
  SWP swp;

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile);
  do{

    if(!hini) {
#if 0
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
#endif
      break;
    }/* end of if(!hini) */

    WinQueryWindowPos(hwnd, &swp);
    if(!PrfWriteProfileData(hini, chrApp, chrKey, &swp, sizeof(swp)))
      bError=TRUE;

    if(hini)
      PrfCloseProfile(hini);
    
    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  return FALSE;
}

BOOL IniSaveWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd)
{
  RGB rgb;
  ULONG  attrFound;

  // Query the current background colour
  if(WinQueryPresParam(hwnd,
                       PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(rgb),
                       &rgb,QPF_NOINHERIT))
    {
      IniSaveData(chrIniFile, chrApp, INI_BGCLR_KEY, &rgb, sizeof(RGB));
    }
  // Query the current foreground colour
  if(WinQueryPresParam(hwnd,
                       PP_FOREGROUNDCOLOR,0,&attrFound,sizeof(rgb),
                       &rgb,QPF_NOINHERIT))
    {
      IniSaveData(chrIniFile, chrApp, INI_FGCLR_KEY, &rgb, sizeof(RGB));
    }

  if(WinQueryPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                       PP_ACTIVECOLOR,0,&attrFound,sizeof(rgb),
                       &rgb,QPF_NOINHERIT))
    {
      IniSaveData(chrIniFile, chrApp, INI_ACTIVETBBGCLR_KEY, &rgb, sizeof(RGB));
    }

  if(WinQueryPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                       PP_INACTIVECOLOR,0,&attrFound,sizeof(rgb),
                       &rgb,QPF_NOINHERIT))
    {
      IniSaveData(chrIniFile, chrApp, INI_INACTIVETBBGCLR_KEY, &rgb, sizeof(RGB));
    }
  if(WinQueryPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                       PP_ACTIVETEXTFGNDCOLOR,0,&attrFound,sizeof(rgb),
                       &rgb,QPF_NOINHERIT))
    {
      IniSaveData(chrIniFile, chrApp, INI_ACTIVETBFGCLR_KEY, &rgb, sizeof(RGB));
    }
  if(WinQueryPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                       PP_INACTIVETEXTFGNDCOLOR,0,&attrFound,sizeof(rgb),
                       &rgb,QPF_NOINHERIT))
    {
      IniSaveData(chrIniFile, chrApp, INI_INACTIVETBFGCLR_KEY, &rgb, sizeof(RGB));
    }
  return TRUE;
}

BOOL IniRestoreWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd)
{
  RGB rgb;
  ULONG ulSize;
  
  ulSize=sizeof(RGB);
  if(IniRestoreData(chrIniFile, chrApp, INI_BGCLR_KEY, &rgb, &ulSize))
    {
      // Set the background colour
      WinSetPresParam(hwnd,
                      PP_BACKGROUNDCOLOR,(ULONG)sizeof(rgb), &rgb);
    }
  ulSize=sizeof(RGB);        
  if(IniRestoreData(chrIniFile, chrApp, INI_FGCLR_KEY, &rgb, &ulSize))
    {
      // Set the foreground colour
      WinSetPresParam(hwnd,
                      PP_FOREGROUNDCOLOR,(ULONG)sizeof(rgb), &rgb);
    }
  /* Titlebar */
  ulSize=sizeof(RGB);
  if(IniRestoreData(chrIniFile, chrApp, INI_ACTIVETBBGCLR_KEY, &rgb, &ulSize))
    {
            
      WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                      PP_ACTIVECOLOR,(ULONG)sizeof(rgb), &rgb);
      WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                      PP_ACTIVETEXTBGNDCOLOR,(ULONG)sizeof(rgb), &rgb);
    }
  ulSize=sizeof(RGB);
  if(IniRestoreData(chrIniFile, chrApp, INI_INACTIVETBBGCLR_KEY, &rgb, &ulSize))
    {
      WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                      PP_INACTIVECOLOR,(ULONG)sizeof(rgb), &rgb);
      WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                      PP_INACTIVETEXTBGNDCOLOR,(ULONG)sizeof(rgb), &rgb);
    }

  ulSize=sizeof(RGB);
  if(IniRestoreData(chrIniFile, chrApp, INI_ACTIVETBFGCLR_KEY, &rgb, &ulSize))
    {
            
      //            WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
      //              PP_ACTIVECOLOR,(ULONG)sizeof(rgb), &rgb);
      WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                      PP_ACTIVETEXTFGNDCOLOR,(ULONG)sizeof(rgb), &rgb);
    }
  ulSize=sizeof(RGB);
  if(IniRestoreData(chrIniFile, chrApp, INI_INACTIVETBFGCLR_KEY, &rgb, &ulSize))
    {
      //            WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
      //              PP_INACTIVECOLOR,(ULONG)sizeof(rgb), &rgb);
      WinSetPresParam(WinWindowFromID(hwnd, FID_TITLEBAR),
                      PP_INACTIVETEXTFGNDCOLOR,(ULONG)sizeof(rgb), &rgb);
    }
  return TRUE;
}

BOOL IniSaveInt(char * iniFile, char* chrApp, char *chrKey, int theInt)
{
  HINI hini=0;
  BOOL bError=FALSE;
  char chrIntString[50];

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile);
  do{

    if(!hini) {
#if 0
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
#endif
      break;
    }/* end of if(!hini) */

    sprintf(chrIntString, "%d", theInt);
    if(!PrfWriteProfileString(hini, chrApp, chrKey, chrIntString))
      bError=TRUE;

    if(hini)
      PrfCloseProfile(hini);
    
    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  return FALSE;
}

BOOL IniSaveData(char * iniFile, char* chrApp, char *chrKey, void* theData, ULONG ulSize)
{
  HINI hini=0;
  BOOL bError=FALSE;

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile);
  do{

    if(!hini) {
#if 0
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
#endif
      break;
    }/* end of if(!hini) */

    if(!PrfWriteProfileData(hini, chrApp, chrKey, theData, ulSize))
      bError=TRUE;

    if(hini)
      PrfCloseProfile(hini);
    
    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  return FALSE;
}

BOOL IniRestoreData(char * iniFile, char* chrApp, char *chrKey, void * theData, ULONG* ulMaxSize)
{
  HINI hini=0;
  BOOL bError=FALSE;

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile);
  do{

    if(!hini) {
#if 0
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
#endif
      break;
    }/* end of if(!hini) */

    bError=PrfQueryProfileData(hini, chrApp, chrKey, theData, ulMaxSize);
    
    if(hini)
      PrfCloseProfile(hini);
    
    return bError;
  } while(TRUE);
  return FALSE;
}


int IniRestoreInt(char * iniFile, char* chrApp, char *chrKey, int defaultInt)
{
  HINI hini=0;
  int theInt;

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)iniFile);
  do{

    if(!hini) {
#if 0
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
#endif
      break;
    }/* end of if(!hini) */

    theInt=PrfQueryProfileInt(hini, chrApp, chrKey, defaultInt);
    
    if(hini)
      PrfCloseProfile(hini);
    
    return theInt;
  } while(TRUE);
  return defaultInt;
}


BOOL readIni2(char * installDir)
{
  HWND hwnd;

  hwnd=HWND_DESKTOP;
#if 0
  strncpy(chrInstallDir, installDir,sizeof(chrInstallDir));
  /* Build full path for cdrecord.ini file */
  sprintf(profileName,"%s\\cdrecord.ini", installDir);       
  /* Insert message in Logfile */
  writeLog("Reading values from ");
  writeLog(profileName);
  writeLog("\n");

  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)profileName);
  do{
    if(!hini) {
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTRD_DATACDCREATOR, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
      break;
    }/* end of if(!hini) */

    if(hini)
      PrfCloseProfile(hini);

    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  writeLog("Error while reading cdrecord.ini\n");
#endif
  return FALSE;
}

BOOL readIni()
{
  return readIni2(params[1]);
}
#if 0
static void _HlpBuildProfileName(char* chrBuffer, int iBufferSize)
{
  /* Build full path for cdrecord.ini file */
  snprintf(chrBuffer, iBufferSize, "%s\\cdrecord.ini", chrInstallDir);       
  chrBuffer[iBufferSize-1]=0; /* Always terminate with zero */
}


HINI HlpOpenIni()
{
  char profileName[CCHMAXPATH];

  /* Build full path for cdrecord.ini file */
  _HlpBuildProfileName(profileName, sizeof(profileName));

  /* Open ini-file */
  return PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)profileName);
}

void HlpCloseIni(HINI hini)
{
    if(hini)
      PrfCloseProfile(hini);
}
#endif
#if 0
if(SysQueryHomeDir(chrHomeDir, sizeof(chrHomeDir))) {
    strcpy(chrPath, chrHomeDir);
    strncat(chrPath, "\\WPSWIZZ.INI", size-strlen(chrPath)-1);/* Default is INI file in users home dir */
    return;
  }

  strcpy(chrPath, queryHelpPath());
  if((ptrChar=strrchr(chrPath, '\\'))!=NULLHANDLE) {
    *ptrChar=0;
    if((ptrChar=strrchr(chrPath, '\\'))!=NULLHANDLE)
      *ptrChar=0;
  }
  strncat(chrPath, "\\WPSWIZZ.INI", size-strlen(chrPath)-1);
  chrPath[size-1]=0;
#endif

/*
  Query the path to the INI file used by the MM progs to store the private data.
  If a home dir exists on the users system the INI file will be create there otherwise
  in the directory where the calling EXE is located.
 */
BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize)
{
  char * chrPtr;  

  /* Default is INI file in users home dir */
  if(SysQueryHomeDir(chrBuffer, ulBufferSize)) {
    strlcat(chrBuffer, MMCLS_INI_FILE_NAME, ulBufferSize); /* MMCLS_INI_FILE_NAME = "\\CWMM.INI" */
    return TRUE;
  }

  /* No HOME dir for some reason */
  strlcpy(chrBuffer, chrProgname, ulBufferSize);

  if((chrPtr=strrchr(chrBuffer,'\\'))==NULLHANDLE)
    return FALSE; // This shouldn't happen!

  *chrPtr=0;
  strlcat(chrBuffer, MMCLS_INI_FILE_NAME, ulBufferSize);

  return TRUE;
}


#if 0
/* This function returns the module handle of our ressource dll */
HMODULE queryResModuleHandle(char *chrExePath)
{
  COUNTRYCODE country= {0};
  COUNTRYINFO countryInfo= {0};
  char path[CCHMAXPATH];  
  char buf[CCHMAXPATH];
  char* found;
  APIRET rc;
  ULONG ulInfoLen;
    
  if(!RESSOURCEHANDLE) {

    //writeLog("Trying to load ressource DLL.\n");

    /* Get the country code of our system and load the  
       resource DLL with the right language */
    do {
      rc=DosQueryCtryInfo(sizeof(countryInfo),&country,&countryInfo,&ulInfoLen);
      if(rc!=NO_ERROR) {
        /* Insert message in Logfile */
        //  writeLog(__FUNCTION__);
        //writeLog(": Can't get country info. Ressource-DLL will not be loaded.\n");
        break;
      }
      sprintf(path,"%s", chrExePath);
      if((found=strrchr(path, '\\'))!=NULLHANDLE)
        *found=0;

      sprintf(buf, RESDLLNAME ,countryInfo.country);
      strcat(path,buf);
      /* Insert message in Logfile */
      //HlpWriteToTrapLog("Using the following DLL path: ");
      //HlpWriteToTrapLog(path);
      //HlpWriteToTrapLog("\n");

      rc=DosLoadModule(buf,sizeof(buf),path, &RESSOURCEHANDLE);
      if(rc==NO_ERROR)
        break;

      /* Insert message in Logfile */
      //      writeLog("Ressource-DLL for the current countrycode not found. Trying to load default one (CDFLD001.DLL).\n");
      
      /* NLS DLL not found. Try to load default */
      found=strrchr(path,'\\');
      if(!found)
        break;

      *found=0;
      sprintf(buf, DEFRESDLLNAME);
      strcat(path,buf);
      //HlpWriteToTrapLog("Using the following DLL path: ");
      //HlpWriteToTrapLog(path);
      //HlpWriteToTrapLog("\n");
      

      rc=DosLoadModule(buf,sizeof(buf),path, &RESSOURCEHANDLE);
      if(rc!=NO_ERROR) {
        //        writeLog("Can't load DLL!\n");
        RESSOURCEHANDLE=NULLHANDLE;
      }
      else {
        //writeLog("Ressource DLL loaded.\n");
      }
      break;
    }while(TRUE);
  }
  return RESSOURCEHANDLE;
}
#endif

/* This function returns the module handle of our ressource dll */
HMODULE queryResModuleHandle(char *chrExePath)
{
  char path[CCHMAXPATH];  
  char buf[CCHMAXPATH];
  char* found;
  APIRET rc;
    
  if(!RESSOURCEHANDLE) {
    
    //writeLog("Trying to load ressource DLL.\n");
    
    /* Get the language code of our system and load the  
       resource DLL with the right language */
    do 
      {
        static char chrLang[]="en_EN";
        PSZ pszLang="";
        char *chrPtr;
        
        /* Get Language var */
        if(NO_ERROR!=DosScanEnv("LANG", &pszLang)) {
         pszLang=chrLang;
        }        
        /* Skip leading spaces */
        chrPtr=pszLang;
        while(*chrPtr==' ')
          chrPtr++;
        
        /* Check if value seems to be valid. The var must be something like xx_XX thus length is 5 */
        if(strlen(chrPtr)<5)
          break;
        
        sprintf(path,"%s", chrExePath);
        //    SysWriteToTrapLog(path);
        if((found=strrchr(path, '\\'))!=NULLHANDLE)
          *found=0;

        /* Extract the first two chars */                
        sprintf(buf, RESDLLNAME, chrPtr[0], chrPtr[1]);
        strcat(path,buf);
        /* Insert message in Logfile */
        // SysWriteToTrapLog("Using the following DLL path: %s\n", path);
        
        rc=DosLoadModule(buf,sizeof(buf),path, &RESSOURCEHANDLE);
        if(rc==NO_ERROR)
          break;
        
        /* Insert message in Logfile */
        SysWriteToTrapLog("Ressource-DLL for the current country not found. Trying to load default one.\n");
        
        /* NLS DLL not found. Try to load default */
        found=strrchr(path,'\\');
        if(!found)
          break;
        
        *found=0;
        sprintf(buf, DEFRESDLLNAME);
        strcat(path,buf);
        //HlpWriteToTrapLog("Using the following DLL path: ");
        //HlpWriteToTrapLog(path);
        //HlpWriteToTrapLog("\n");
        
        
        rc=DosLoadModule(buf,sizeof(buf),path, &RESSOURCEHANDLE);
        if(rc!=NO_ERROR) {
          //        writeLog("Can't load DLL!\n");
          RESSOURCEHANDLE=NULLHANDLE;
        }
        else {
          //writeLog("Ressource DLL loaded.\n");
        }
        break;
      }while(TRUE);
  }
  return RESSOURCEHANDLE;
}

void freeResHandle()
{
  if(RESSOURCEHANDLE)
    DosFreeModule(RESSOURCEHANDLE);
}

void HlpSendCommandToObject(char* chrObject, char* command)
{
  HOBJECT hObject;
  char chrCommand[CCHMAXPATH];

  hObject=WinQueryObject(chrObject);
  sprintf(chrCommand,"Querying object pointer for: %s\n", chrObject);
  writeLog(chrCommand);
  if(hObject!=NULLHANDLE) {
    strcpy(chrCommand,command);
    WinSetObjectData(hObject,chrCommand);
    writeLog("Sending command to object: ");
    writeLog(command);
    writeLog("\n");
  }
  else {
    sprintf(chrCommand,"Can't query object pointer for: %s\n", chrObject);
    writeLog(chrCommand);
  }
}


