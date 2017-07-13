/*
 * This file is (C) Chris Wohlgemuth 2001-2005
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
#define INCL_DOS
#define INCL_DOSERRORS

#include <os2.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
//#include "audiofolder.h"
//#include "audiofolderres.h"

#define INSTALLDIR 1

/* The subdirectory in the installation directory where to put the log files */
#define LOGFILE_SUBDIR "Logfiles"

extern char* params[];
extern HMODULE RESSOURCEHANDLE;
extern char logName[];

char chrInstallDir[CCHMAXPATH];
char chrCDRecord[CCHMAXPATH];/* Path to cdrecord */
char chrCDROptions[CCHMAXPATH];
char chrAudioCDROptions[CCHMAXPATH];
LONG  lCDROptions=0;

char chrMkisofs[CCHMAXPATH];/* Path to mkisofs */
char chrMkisofsOptions[CCHMAXPATH];
LONG lMKOptions;

char chrGrabberPath[CCHMAXPATH];
char chrGrabberOptions[CCHMAXPATH];
int bTrackNumbers;
int iGrabberID;
char chosenCD[3];

char chrMpg123Path[CCHMAXPATH];
BOOL bMpg123SwabBytes;
int iMp3Decoder;

char chrMntIsoFS[CCHMAXPATH];
char chrUmntIso[CCHMAXPATH];

char chrCdrdaoPath[CCHMAXPATH]={0};
char chrCdrdaoDriver[100]={0};
int iBus=0;
int iTarget=0;
int iLun=0;
int iSpeed=1;
int iFifo=4;

BOOL bUseCDDB;
BOOL bTipsEnabled;

SWP swpWindow;

HMODULE queryResModuleHandle2(char *installDir, char * chrBaseName);


BOOL HlpQueryTempDir(char * chrInstallationDir, char * chrBuffer, int iBufferSize)
{
  int iLeft=iBufferSize;

  strncpy(chrBuffer, chrInstallationDir, iBufferSize);
  chrBuffer[iBufferSize-1]=0;
  iLeft=iBufferSize-strlen(chrBuffer);
  strncat(chrBuffer, "\\temp", iLeft);
  chrBuffer[iBufferSize-1]=0;

  return TRUE;
}

/****************************************************
 *                                                  *
 * This funktion returns the running OS version:    *
 *                                                  *
 * 30: Warp 3, 40 Warp 4                            *
 *                                                  *
 ****************************************************/
ULONG cwQueryOSRelease()
{
  static ULONG ulVersionMinor=0;

  if(!ulVersionMinor)
    if(DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ulVersionMinor, sizeof(ulVersionMinor)))
      ulVersionMinor=30;/* Default Warp 3 */

  return ulVersionMinor;

}
void writeLog(char* logText);
void removeLog(void)
{
  char logNameLocal[CCHMAXPATH];
  
  sprintf(logNameLocal,"%s\\%s\\%s",params[1],LOGFILE_SUBDIR,logName);
  //WinMessageBox(HWND_DESKTOP,HWND_DESKTOP, logNameLocal,"",1234,MB_MOVEABLE|MB_OK);
  //writeLog(logNameLocal);

  remove(logNameLocal);
  //DosBeep(500,100);
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

void writeLog2(char * installDir, char * logName, char* logText)
{
  char logNameLocal[CCHMAXPATH];
  FILE *fHandle;

  sprintf(logNameLocal,"%s\\%s\\%s",installDir, LOGFILE_SUBDIR, logName);
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


ULONG launchCmdExe( PSZ wrapperExe, PSZ parameter, PSZ folderPath, PSZ pszTitle)
{
  STARTDATA startData={0};
  APIRET rc;
  PID pid;
  ULONG ulSessionID=0;
  char chrLoadError[CCHMAXPATH];
  char startParams[CCHMAXPATH*4];
  char exename[CCHMAXPATH]={0};
  char chrCDRexe[CCHMAXPATH+10];
  struct stat statBuf;

  strcpy(exename,wrapperExe);
  
  writeLog("\nStarting '");
  writeLog(exename);
  writeLog("' with the following parameters:\n");
 

  memset(&startData,0,sizeof(startData));
  startData.Length=sizeof(startData);
  startData.Related=SSF_RELATED_INDEPENDENT;
  startData.FgBg=SSF_FGBG_BACK;
  startData.TraceOpt=SSF_TRACEOPT_NONE;
  startData.PgmTitle=pszTitle;
  startData.PgmName=exename;
  startData.InheritOpt=SSF_INHERTOPT_SHELL;
  startData.SessionType=SSF_TYPE_WINDOWABLEVIO;
  startData.PgmControl=0;
  //  if(lCDROptions&IDCDR_HIDEWINDOW)
  startData.PgmControl|=SSF_CONTROL_INVISIBLE;//|SSF_CONTROL_MAXIMIZE|SSF_CONTROL_NOAUTOCLOSE;
  //  if(!(lCDROptions&IDCDR_CLOSEWINDOW))
  //startData.PgmControl|=SSF_CONTROL_NOAUTOCLOSE;
  startData.InitXPos=30;
  startData.InitYPos=30;
  startData.InitXSize=500;
  startData.InitYSize=400;
  startData.ObjectBuffer=chrLoadError;
  startData.ObjectBuffLen=(ULONG)sizeof(chrLoadError);

  startData.PgmInputs=parameter;

  writeLog("\n\n");
 
  rc=DosStartSession(&startData,&ulSessionID,&pid);   
  if(rc!=NO_ERROR) {
    writeLog("Error while starting: ");
    sprintf(chrLoadError,"%d\n",rc);
    writeLog(chrLoadError);
    return -1;
  }
  return 0;   
}

#if 0
BOOL readWindowPosFromIni(char * installDir, char *chrKey)
{
  char profileName[CCHMAXPATH];
  char moduleName[CCHMAXPATH];
  HINI hini=0;
  ULONG ulSize;
  BOOL bError=FALSE;
  HWND hwnd;

  hwnd=HWND_DESKTOP;

  strncpy(chrInstallDir, installDir,sizeof(chrInstallDir));
  /* Build full path for cdrecord.ini file */
  sprintf(profileName,"%s\\cdrecord.ini", installDir);       
  /* Insert message in Logfile */
  writeLog("Reading Window position from ");
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

    ulSize=sizeof(swpWindow); 
    if(!PrfQueryProfileData(hini,"windowposition", chrKey, &swpWindow, &ulSize))
      bError=TRUE;

    if(hini)
      PrfCloseProfile(hini);
    
    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  writeLog("Error while reading cdrecord.ini\n");
  return FALSE;
}

BOOL writeWindowPosToIni(char * installDir, char *chrKey)
{
  char profileName[CCHMAXPATH];
  char moduleName[CCHMAXPATH];
  HINI hini=0;
  ULONG ulSize;
  BOOL bError=FALSE;
  HWND hwnd;

  hwnd=HWND_DESKTOP;

  strncpy(chrInstallDir, installDir,sizeof(chrInstallDir));
  /* Build full path for cdrecord.ini file */
  sprintf(profileName,"%s\\cdrecord.ini", installDir);       
  /* Insert message in Logfile */
  writeLog("Writing Window position to ");
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

    if(!PrfWriteProfileData(hini,"windowposition", chrKey, &swpWindow, sizeof(swpWindow)))
      bError=TRUE;

    if(hini)
      PrfCloseProfile(hini);
    
    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  writeLog("Error while writing cdrecord.ini\n");
  return FALSE;
}


BOOL readIni2(char * installDir)
{
  ULONG keyLength;
  char profileName[CCHMAXPATH];
  char moduleName[CCHMAXPATH];
  char *chrPtr;
  char *chrPtr2;
  HINI hini=0;
  char    text[200];
  ULONG ulSize;
  int a;
  char chrCD[4];
  BOOL bError=FALSE;
  HWND hwnd;

  hwnd=HWND_DESKTOP;

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

    keyLength=PrfQueryProfileString(hini,"CDWriter","cdrecord","",chrCDRecord,sizeof(chrCDRecord));
    if(keyLength==1){
      /* Text: "No CDRecord/2 path in cdrecord.ini found!\n" */
      getMessage(text, IDSTRLOG_NOCDRECORD, sizeof(text), RESSOURCEHANDLE , hwnd);      
      writeLog(text);
      writeLog("\n");
      bError=TRUE;
      //  break;/* First opening. We havn't got entries yet */
    }

    ulSize=sizeof(lCDROptions); 
    PrfQueryProfileData(hini,"CDWriter","options",&lCDROptions,&ulSize);

    PrfQueryProfileString(hini,"CDWriter","audiocdroptions","",chrAudioCDROptions,sizeof(chrAudioCDROptions));    
    PrfQueryProfileString(hini,"CDWriter","cdroptions","",chrCDROptions,sizeof(chrCDROptions));    

    ulSize=sizeof(lCDROptions); 
    PrfQueryProfileData(hini,"CDWriter","options",&lCDROptions,&ulSize);
    keyLength=PrfQueryProfileString(hini,"Mkisofs","mkisofs","",chrMkisofs,sizeof(chrMkisofs));
    if(keyLength==1){
      /* Text: "No mkisofs path in cdrecord.ini found!\n" */
      getMessage(text, IDSTRLOG_NOMKISOFS, sizeof(text), RESSOURCEHANDLE , hwnd);      
      writeLog(text);
      writeLog("\n");
      bError=TRUE;
      //break;/* First opening. We havn't got entries yet */
    }

    PrfQueryProfileString(hini,"Mkisofs","mkisofsoptions","",chrMkisofsOptions,sizeof(chrMkisofsOptions));
    //lMKOptions=PrfQueryProfileInt(hini,"Mkisofs","options",IDMK_HIDEWINDOW|IDMK_CLOSEWINDOW);
    ulSize=sizeof(lMKOptions);
    PrfQueryProfileData(hini,"Mkisofs","options",&lMKOptions,&ulSize);   

    keyLength=PrfQueryProfileString(hini,"CDGrabber","grabber","",chrGrabberPath,sizeof(chrGrabberPath));
    if(keyLength==1){
      /* Text: "No grabber path in cdrecord.ini found!\n" */
      getMessage(text, IDSTRLOG_NOGRABBER, sizeof(text), RESSOURCEHANDLE , hwnd);      
      writeLog(text);
      writeLog("\n");
      bError=TRUE;
      //break;/* We havn't got entries yet */
    }
    PrfQueryProfileString(hini,"CDGrabber","graboptions","",chrGrabberOptions,sizeof(chrGrabberOptions));
    PrfQueryProfileString(hini,"CDGrabber","grabdrive","",chosenCD,sizeof(chosenCD));
    bTrackNumbers=PrfQueryProfileInt(hini,"CDGrabber","tracknumbers",1);
    iGrabberID=PrfQueryProfileInt(hini,"CDGrabber","ID", IDGRABBER_UNKNOWN);/* 99: unknown */

    keyLength=PrfQueryProfileString(hini,"mpg123","path","",chrMpg123Path,sizeof(chrMpg123Path));
    if(keyLength==1){
      /* Text: "No mpg123 path in cdrecord.ini found!\n" */
      getMessage(text, IDSTRLOG_NOMPG123, sizeof(text), RESSOURCEHANDLE , hwnd);      
      writeLog(text);
      writeLog("\n");
      bError=TRUE;
      //      break;/* We havn't got entries yet */
    }
    bMpg123SwabBytes=PrfQueryProfileInt(hini,"mpg123","swabbytes",1);
    iMp3Decoder=PrfQueryProfileInt(hini,"mpg123","decoder",IDKEY_USEMPG123);/* Which decoder to use */

    keyLength=PrfQueryProfileString(hini,"cdrdao","path","",chrCdrdaoPath,sizeof(chrCdrdaoPath));
    if(keyLength==1){
      /* Text: "No cdrdao/2 path in cdrecord.ini found!\n" */
      getMessage(text, IDSTRLOG_NOCDRDAO, sizeof(text), RESSOURCEHANDLE , hwnd);      
      writeLog(text);
      writeLog("\n");
      bError=TRUE;
      //      break; /* We havn't got entries yet */
    }

    keyLength=PrfQueryProfileString(hini,"cdrdao","driver","",chrCdrdaoDriver,sizeof(chrCdrdaoDriver));
    if(keyLength==1){
      /* Text: "No driver for cdrdao/2 found in cdrecord.ini!\n" */
      getMessage(text, IDSTRLOG_NOCDRDAODRIVER, sizeof(text), RESSOURCEHANDLE , hwnd);      
      writeLog(text);
      writeLog("\n");
      bError=TRUE;
     /* We havn't got entries yet */
      //break;
    }

    PrfQueryProfileString(hini,"isofs","mountpath","",chrMntIsoFS,sizeof(chrMntIsoFS));
    PrfQueryProfileString(hini,"isofs","unmountpath","",chrUmntIso, sizeof(chrUmntIso));
    
    iBus=PrfQueryProfileInt(hini,"device","bus",0);
    iTarget=PrfQueryProfileInt(hini,"device","target",0);
    iLun=PrfQueryProfileInt(hini,"device","lun",0);
    iSpeed=PrfQueryProfileInt(hini,"device","speed",1);
    iFifo=PrfQueryProfileInt(hini,"device","fifo",4);

    bUseCDDB=PrfQueryProfileInt(hini,"cddb","usecddb",0);
    bTipsEnabled=PrfQueryProfileInt(hini,"tips","enable",1);

    if(hini)
      PrfCloseProfile(hini);

    if(bError)
      break;
    return TRUE;
  } while(TRUE);
  writeLog("Error while reading cdrecord.ini\n");
  return FALSE;
}


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

/* This function returns the module handle of our ressource dll */
HMODULE queryResModuleHandle2(char *installDir, char * chrBaseName)
{
  COUNTRYCODE country= {0};
  COUNTRYINFO countryInfo= {0};
  char path[CCHMAXPATH];  
  char buf[CCHMAXPATH];
  char* found;
  APIRET rc;
  ULONG ulInfoLen;
    
  if(!RESSOURCEHANDLE) {
    writeLog("Trying to load ressource DLL.\n");

    /* Get the country code of our system and load the  
       resource DLL with the right language */
    do {
      rc=DosQueryCtryInfo(sizeof(countryInfo),&country,&countryInfo,&ulInfoLen);
      if(rc!=NO_ERROR) {
        /* Insert message in Logfile */
        writeLog(__FUNCTION__);
        writeLog(": Can't get country info. Ressource-DLL will not be loaded.\n");
        break;
      }
      sprintf(path,"%s", installDir);
      
      sprintf(buf,"\\%s%03d.dll", chrBaseName, countryInfo.country);
      strcat(path,buf);
      /* Insert message in Logfile */
      writeLog("Using the following DLL path: ");
      writeLog(path);
      writeLog("\n");

      //WinMessageBox(  HWND_DESKTOP,0, path, "Search DLL...", 0UL, MB_YESNO | MB_ICONEXCLAMATION|MB_MOVEABLE );
      rc=DosLoadModule(buf,sizeof(buf),path, &RESSOURCEHANDLE);
      if(rc==NO_ERROR)
        break;

      /* Insert message in Logfile */
      writeLog("Ressource-DLL for the current countrycode not found. Trying to load default one (nnnnn001.DLL).\n");
      
      /* NLS DLL not found. Try to load default */
      found=strrchr(path,'\\');
      if(!found)
        break;

      *found=0;
      sprintf(buf,"\\%s001.dll", chrBaseName);
      strcat(path,buf);
      writeLog("Using the following DLL path: ");
      writeLog(path);
      writeLog("\n");

      rc=DosLoadModule(buf,sizeof(buf),path, &RESSOURCEHANDLE);
      if(rc!=NO_ERROR) {
        writeLog("Can't load DLL!\n");
        RESSOURCEHANDLE=NULLHANDLE;
      }
      else {
        writeLog("Ressource DLL loaded.\n");
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

#if 0
void sendCommand(char* command)
{
  HOBJECT hObject;
  char chrCommand[CCHMAXPATH];

  hObject=WinQueryObject(params[2]);
  sprintf(chrCommand,"Querying object pointer for: %s\n",params[2]);
  writeLog(chrCommand);
  if(hObject!=NULLHANDLE) {
    strcpy(chrCommand,command);
    WinSetObjectData(hObject,chrCommand);
    writeLog("Sending command to folder: ");
    writeLog(command);
    writeLog("\n");
  }
  else {
    sprintf(chrCommand,"Can't query object pointer for: %s\n",params[2]);
    writeLog(chrCommand);
  }
}
#endif

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
