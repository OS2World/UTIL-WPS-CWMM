/*
 * pmcddb.cc (C) Chris Wohlgemuth 1999-2002
 *
 * This helper queries the CDDB database 
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
#define INCL_DOS
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN

#include <os2.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <string.h>
#include "pmcddb.h"
#include "pmcddbres.h"

#include "cddb.h"
#include "cddb.hh"

#define CDDB_OK   0
#define CDDB_MORE 1
#define CDDB_ERROR -1

/**********************************************************/
/* CDDB Stuff */

extern int readResponse(int s, char * buffer,int size);
/* Returns the cddb code. */
extern int get_cddb_code( char * chrResponse);
/* Check error codes coming from the server*/
extern int check_cddb_code(char * chrResponse);
/* Performing the handshake procedure with the CDDB server */
/* s: socket handle */
extern int cddb_handshaking(int s);
/* s: socket handle */
extern int cddb_setting_proto(int s);
/* This is called if the CDDB database has got several matches for our
   discid. We take all matches and decide later, which to use. */
extern cddb * query_get_all_matches(int s);
/* Query some data */
/* With the data given we later query the
   tracknames */
extern cddb * cddb_query(int s, CDDBINFO *cddbInfo);
/* Reads the CDDB banner from the server after connecting */
/* We don't really need it so it is only shown on stderr */
extern int cddb_banner(int s);
/* Ask for the information about our CD. Parse it and
   save it in the data file. */
extern int read_and_parse(int s,cddb * Cddb);
extern cddb_read(int s,cddb * Cddb);
extern int cddbConnectToHost(int * skt);

extern "C" LONG CDDBDiscID(char * drive,CDDBINFO *cddbInfo);
void extern printError(char* errorString);
void extern printErrorCR(char* errorString);
void extern printData(char* outString);

extern "C" void removeLog(void);
extern "C" void writeLog(char* logText);
extern  "C" ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
extern  "C" void getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
extern  "C" HMODULE queryResModuleHandle2(char *installDir, char * chrBaseName);
extern  "C" void freeResHandle();
extern  void pmUsage();
extern  "C" void HlpSendCommandToObject(char* chrObject, char* command);
extern "C" ULONG cwQueryOSRelease(void);
void HlpSendCommandToObject(char* chrObject, char* command);

MRESULT EXPENTRY cddbDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;

//#define DEBUG

char chrCDDBDir[CCHMAXPATH];
char chrInstallDir[CCHMAXPATH];
char chrBinDir[CCHMAXPATH];
char chrCDDrive[3]="N:";
char *chrRexxFile="";
char *chrFolder="";

int numArgs;
char* params[4]={"","","",""};

HMODULE RESSOURCEHANDLE=0;

char logName[CCHMAXPATH]="Cddb.log";

#define QUERYCDDB_VERSION "0.1"

int port=888;
char host[1000]="uk.freedb.org";
char username[1000]={0};
char hostname[1000]={0};
char clientname[100]="Audio-CD-Creator";
char version[100];
int s; /* Socket */
#define MAXSERVERS 10
char cddbServer[MAXSERVERS][100];
int NUMSERVERS= 3;

cddb * Cddb;
char cddbDataFile[CCHMAXPATH]="";

HWND hwndError;
HWND hwndDialog;
HWND hwndNotify;

ULONG launchRexx( char* pszTitle, char* rexxFile, char* parameters)
{
  STARTDATA startData={0};
  APIRET rc;
  PID pid;
  ULONG ulSessionID=0;
  char chrLoadError[CCHMAXPATH];
  char startParams[CCHMAXPATH*4];
  char tempText[CCHMAXPATH]= {0};
  char *charPtr;
  char trackname[CCHMAXPATH];
    
  memset(&startData,0,sizeof(startData));
  startData.Length=sizeof(startData);
  startData.Related=SSF_RELATED_INDEPENDENT;
  startData.FgBg=SSF_FGBG_BACK;
  startData.TraceOpt=SSF_TRACEOPT_NONE;
  startData.PgmTitle=(UCHAR*)pszTitle;
  
  startData.PgmName=(UCHAR*)"cmd.exe";
  startData.InheritOpt=SSF_INHERTOPT_SHELL;
  startData.SessionType=SSF_TYPE_WINDOWABLEVIO;
  startData.PgmControl|=SSF_CONTROL_INVISIBLE;//|SSF_CONTROL_MAXIMIZE|SSF_CONTROL_NOAUTOCLOSE;
  startData.InitXPos=30;
  startData.InitYPos=30;
  startData.InitXSize=500;
  startData.InitYSize=400;
  startData.ObjectBuffer=(UCHAR*)chrLoadError;
  startData.ObjectBuffLen=(ULONG)sizeof(chrLoadError);

  sprintf(startParams," /C \" \"%s\" %s \"", rexxFile, parameters);
  startData.PgmInputs=(UCHAR*)startParams;

  writeLog("Trying to start ");
  writeLog(rexxFile);
  writeLog("\n");
  rc=DosStartSession(&startData,&ulSessionID,&pid);   
  return rc;   
}

BOOL readCDDBIni()
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

  /* Insert message in Logfile */
  writeLog("Reading values from cddb.ini\n");

  /* Build full path for cddb.ini file */
  sprintf(profileName,"%s\\cddb.ini", chrInstallDir);       
  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)profileName);
  do{
    if(!hini) {
      writeLog("Can't open INI file.\n");
      /* profileName: "Warning! Cannot open Ini-file!"
         moduleName: "Data-CD-Creator"
         */
      messageBox( profileName, IDSTR_INIFILEOPENWARNING , sizeof(profileName),
                  moduleName, IDSTR_PMCDDBTITLE, sizeof(moduleName),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);
      break;
    }/* end of if(!hini) */

    NUMSERVERS=PrfQueryProfileInt(hini,(UCHAR*)"cddb",(UCHAR*)"numservers",0);
    PrfQueryProfileString(hini,(UCHAR*)"cddb",(UCHAR*)"cddbuser",(UCHAR*) "guest",username,sizeof(username));
    PrfQueryProfileString(hini,(UCHAR*)"cddb",(UCHAR*)"cddbuserhost",(UCHAR*) "",hostname,sizeof(hostname));

    strncpy(version,QUERYCDDB_VERSION,sizeof(version));

    if(NUMSERVERS==0) {
      strncpy(cddbServer[0],"de.freedb.org:888",sizeof(cddbServer[0]));
      strncpy(cddbServer[1],"at.freedb.org:888",sizeof(cddbServer[0]));
      strncpy(cddbServer[2],"ca.freedb.org:888",sizeof(cddbServer[0]));
      strncpy(cddbServer[3],"uk.freedb.org:888",sizeof(cddbServer[0]));
      strncpy(cddbServer[4],"freedb.freedb.org:888",sizeof(cddbServer[0]));
      strncpy(cddbServer[5],"cz.freedb.org:888",sizeof(cddbServer[0]));
      NUMSERVERS=6;
    }
    else {
      if(NUMSERVERS>MAXSERVERS) 
        NUMSERVERS=MAXSERVERS;
      /* Read the servers from ini */
      for(a=0;a<NUMSERVERS;a++) {
        sprintf(text,"cddbserver%d",a+1);
        PrfQueryProfileString(hini,(UCHAR*)"cddb",(UCHAR*)text,(UCHAR*)"",cddbServer[a],sizeof(cddbServer[0]));
      }
    }
    if(hini)
      PrfCloseProfile(hini);

    if(strlen(username)==0)
      break;    
    if(strlen(hostname)==0)
      break;    
    
    return TRUE;
  }while(TRUE);
  writeLog("\nError while reading values from cddb.ini.\n");
  writeLog("Make sure your username and your host are set.\n");

  messageBox( profileName, IDSTR_SETUPERROR , sizeof(profileName),
              moduleName, IDSTR_PMCDDBTITLE, sizeof(moduleName),
              RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE);

  HlpSendCommandToObject("<CWFREEDB_CLIENT_CONFIG>", "OPEN=DEFAULT;");
  
  return FALSE;
}

void printCddbList(cddb * Cddb)
{
  cddb * tmpCddb;
  char text[400];
  int iNum;

  tmpCddb=Cddb;
  iNum=0;

  printError("\n-------------------\n");
  printError("CDDB database returned the following match(es):\n");

  while (tmpCddb) {/* Print data of every match */
    /* Print our data */
    iNum++;
    /* Print it to log */
    sprintf(text,"%d:   Title: %s   Artist: %s"
            ,iNum,tmpCddb->title,tmpCddb->artist);
    printError(text);
    printError("\n");
    /* Insert into Listbox */
    WinSendMsg(WinWindowFromID(hwndDialog, IDLB_CDDBCHOOSE), LM_INSERTITEM,MPFROMSHORT(LIT_END),text);   
    tmpCddb=tmpCddb->getNextCddb();
  }/* while */
  /* Preselect the first item */
  WinSendMsg(WinWindowFromID(hwndDialog, IDLB_CDDBCHOOSE), LM_SELECTITEM,MPFROMSHORT(LIT_FIRST),MPFROMSHORT(TRUE));   

  printError("-------------------\n");

}

/* Return the chosen cddb data set from the list of cddb data sets */
cddb* choseCddb(cddb* Cddb, SHORT a)
{
  int iNum;
  cddb * tmpCddb;
  char buffer[10]={0};
  char * rc;

  iNum=0;
  tmpCddb=Cddb;
  /* Count the matches */
  while (tmpCddb) {
    iNum++;
    tmpCddb=tmpCddb->getNextCddb();
  }/* while */


  iNum=1;
  tmpCddb=Cddb;
  /* Get the cddb */
  while (iNum!=a) {
    iNum++;
    tmpCddb=tmpCddb->getNextCddb();
  }/* while */

  return tmpCddb;
}

int queryServer()
{
  CDDBINFO cddbInfo;
  char text[100];
  SHORT a;
  int b;
  char *chrPtr;
  struct stat statBuf;

  Cddb=NULL;

  /* Get track data for CDDB query.
     This function calculates the diskID and the
     length of the tracks of the inserted CD.
     We use the data to perform the database query. */

  if(!CDDBDiscID(chrCDDrive, &cddbInfo)) {
    printError("Cannot query track data for calculating diskID.\nMake sure an audio CD is inserted." );
    writeLog("Cannot query track data for calculating diskID.\nMake sure an audio CD is inserted.\n");
    /*    WinPostMsg(hwndNotify,WM_APPTERMINATENOTIFY,MPFROMLONG(ACKEY_CDDBQUERY),MPFROMLONG(0)); */
    return -1;
  }

  /* Tell the caller, that we start a CDDB query. The caller may go on in the meantime */
  /*WinPostMsg(hwndNotify, WM_APPTERMINATENOTIFY,MPFROMLONG(ACKEY_CDDBQUERY),MPFROMLONG(0));*/
  sprintf(text,"%08x",cddbInfo.discid);
  strcat(cddbDataFile,text);/* Build the name of the cddb data file */
  writeLog("CDDB datafile name is: ");
  writeLog(cddbDataFile);
  writeLog("\n");
  if(stat(cddbDataFile , &statBuf)!=-1) {
    writeLog("Found CDDB data in cache directory. DB query finished.\n");
    /*    WinPostMsg(hwndNotify,WM_APPTERMINATENOTIFY,MPFROMLONG(ACKEY_CDDBQUERY),MPFROMLONG(1)); */
    return 0;/* We have already a cddb data file from a previous query, so we are ready. */
  }

  for(b=0;b<NUMSERVERS;b++) {/* Query the whole list of servers if necessary */
    strncpy(host,cddbServer[b],sizeof(host));
    chrPtr=strrchr(host,':');
    if(chrPtr) {
      *chrPtr=0;
      chrPtr++;
      port=atoi(chrPtr);
    }
    /* Report what we're doing */
    sprintf(text,"SETSTATUSTEXT=Trying %s:%d...",host,port);
    /* Set statusline of folder */
    HlpSendCommandToObject(chrFolder, text);
    sprintf(text,"\nTrying %s:%d...",host,port);
    printErrorCR(text);

    /* Connect to cddb-host */
    if(!cddbConnectToHost(&s)) {    
      /* Query the database */
      do {
        /* Getting CDDB banner */
        if(cddb_banner(s))
          break; /* Error */
    
        /* Setting protocol level */
        if(cddb_setting_proto(s))
          break;
    
        /* Handshaking */
        if(cddb_handshaking(s))
          break;
    
        /* Query disc */
        /* If there're several matches this call returns 
           a linked list of all matches. We let the user
           decide later, which match to use. */ 
        Cddb=cddb_query(s, &cddbInfo);

        if(!Cddb)
          break; /* no list, so break */

        /* Let the user decide which match to use */
        /* If the head of the list has iFuzzy set
           to one we have to choose. */
        if(Cddb->getFuzzyOrN()) {
          /* Show the dialog with the list of matches */
          a=(SHORT)WinDlgBox( HWND_DESKTOP, NULLHANDLE, cddbDialogProc, RESSOURCEHANDLE, IDDLG_CDDBCHOOSE , 0 );        
          sprintf(text,"Selection: %d",a);
          /* WinMessageBox(  HWND_DESKTOP,   HWND_DESKTOP, text, "Debug", 0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE ); */
          if(a==LIT_NONE) {
            /* User pressed cancel */
            if(Cddb)
              delete(Cddb);
            write(s,"QUIT\r\n",6);
            close(s);
            return -1; 
          }
          Cddb=choseCddb(Cddb,a);
        }    
        
        if(!Cddb)
          break; /* no choice, so break */
      
        /* Query tracks of match 'Cddb' */
        /* The track names are put into a linked
           list in Cddb. */
        if(cddb_read(s,Cddb)==CDDB_ERROR)
          break;
      
        /* Print the queried data */
        /* The data is already printed in cddb.cpp to the given datafile */
        //    printQueriedData(Cddb, &cddbInfo);
        

        /* everything's ok */
        write(s,"QUIT\r\n",6);
        close(s);
        delete(Cddb);
        /*        WinPostMsg(hwndNotify,WM_APPTERMINATENOTIFY,MPFROMLONG(ACKEY_CDDBQUERY),MPFROMLONG(1)); */
        sprintf(text,"SETSTATUSTEXT=CDDB data found.");
        return 0;  
      } while(TRUE);        
      if(Cddb)
        delete(Cddb);
      /* Error */
      write(s,"QUIT\r\n",6);
      close(s);
    }
  }/* for */
  /*  WinPostMsg(hwndNotify,WM_APPTERMINATENOTIFY,MPFROMLONG(ACKEY_CDDBQUERY),MPFROMLONG(1)); */
  printError("Can't get data...");
  HlpSendCommandToObject(chrFolder, "SETSTATUSTEXT=Can't get CDDB data...");
  return -1;
}

MRESULT EXPENTRY cddbObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  switch(msg)
    {
    case WM_APPTERMINATENOTIFY:
      /* This queue is blocked until the query is over. This is the reason we've chosen an object window. */
      if(!queryServer()) {
        char rexxParams[CCHMAXPATH*3];
        snprintf(rexxParams, sizeof(rexxParams), "\"%s\" \"%s\" \"%s\"", chrCDDrive, cddbDataFile, params[3]);
        launchRexx( "Set tracknames", chrRexxFile, rexxParams);      
      }
      WinPostMsg(hwnd,WM_QUIT,(MPARAM)0,(MPARAM)0);
      return FALSE;
    default:
      break;
    }
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY cddbDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{

  char text[CCHMAXPATH];
  char statusText[CCHMAXPATH];
  short a;
  ULONG rc;
  SWCNTRL swctl;
  PID pid;

  switch(msg)
    {
    case WM_INITDLG:   

#ifdef DEBUG
      sprintf(text,"Params:\n");  
      for(a=0;a<numArgs;a++) {
        strcat(text,params[a]);
        strcat(text,"\n");
      }
      WinMessageBox(  HWND_DESKTOP,   HWND_DESKTOP, text, "Debug", 0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
#endif

      /* Add switch entry */
      memset(&swctl,0,sizeof(swctl));
      WinQueryWindowProcess(hwnd,&pid,NULL);
      swctl.hwnd=hwnd;
      swctl.uchVisibility=SWL_VISIBLE;
      swctl.idProcess=pid;
      swctl.bProgType=PROG_DEFAULT;
      swctl.fbJump=SWL_JUMPABLE;
      WinAddSwitchEntry(&swctl);

      hwndError=WinWindowFromID(hwnd,3102);
      hwndDialog=hwnd;

      /* Set dialog font to WarpSans for Warp 4 and above */
      if(cwQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }

      /*      sprintf(text,"1.Eintrag");
              WinSendMsg(WinWindowFromID(hwndDialog,3101), LM_INSERTITEM,MPFROMSHORT(LIT_END),text);   
              sprintf(text,"2.Eintrag");
              WinSendMsg(WinWindowFromID(hwndDialog,3101), LM_INSERTITEM,MPFROMSHORT(LIT_END),text);   
              WinShowWindow(hwndDialog,TRUE);
              for(a=0;a<4;a++) {
              sprintf(text,cddbServer[a]);
              WinSendMsg(WinWindowFromID(hwndDialog,3101), LM_INSERTITEM,MPFROMSHORT(LIT_END),text);       
              }*/
      printCddbList(Cddb);           
      return (MRESULT) TRUE;
    case WM_CLOSE:
      WinPostMsg(hwnd,WM_QUIT,0,0);
      return 0;
    case WM_DESTROY:
      /* The dialog closes and gets destroyed */     
      break;    
    case WM_COMMAND:    
      switch(SHORT1FROMMP(mp1))
        {
        case IDPB_OK:        
          /* Query selection */
          a=SHORT1FROMMR(WinSendMsg(WinWindowFromID(hwnd, IDLB_CDDBCHOOSE),LM_QUERYSELECTION,MPFROMSHORT(LIT_FIRST),0));
          if(a==LIT_NONE)
            break;
          WinDismissDlg(hwnd,a+1);
          break;             
        case IDPB_CANCEL:
          WinDismissDlg(hwnd, (ULONG)LIT_NONE);
          break;
        default:
          break;
        }
      return (MRESULT) TRUE;
    default:
      break;
    }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);    
}

void config(char * theAddress)
{
  char *chrPtr;
  char profileName[CCHMAXPATH];
  HINI hini=0;

  if((chrPtr=strchr(theAddress,'@'))==NULLHANDLE)
    return;

  *chrPtr=0;
  chrPtr++;
  strncpy(username, theAddress, sizeof(username)-1);
  strncpy(hostname, chrPtr, sizeof(hostname)-1);
  /* Save it to ini */

  /* Insert message in Logfile */
  writeLog("Reading values from cddb.ini\n");

  /* Build full path for cddb.ini file */
  sprintf(profileName,"%s\\cddb.ini", chrInstallDir);       
  /* Open ini-file */
  hini=PrfOpenProfile(WinQueryAnchorBlock(HWND_DESKTOP),(unsigned char *)profileName);
  if(hini) {
    writeLog("INI file opened.\n");
    PrfWriteProfileString(hini,(UCHAR*)"cddb",(UCHAR*)"cddbuserhost", (UCHAR*)hostname);
    PrfWriteProfileString(hini,(UCHAR*)"cddb",(UCHAR*)"cddbuser",(UCHAR*) username);
    PrfCloseProfile(hini);
  }/* end of if(hini) */
  else {
    writeLog("Can't open INI file to save address.\n");
  }
}

void usage(char * chrProgName)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      /* We need a message queue for dialog boxes */
      char title[100];
      char text[500];

      RESSOURCEHANDLE=queryResModuleHandle2(chrInstallDir, RESDLL_BASENAME);

      messageBox( text, IDSTR_USAGE , sizeof(text),
                  title, IDSTR_PMCDDBTITLE, sizeof(title),
                  RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_MOVEABLE);

      freeResHandle();
      
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }

}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  char *chrPtr;
  short a;

  ULONG fl;
  HWND hwndClient;

  /* Create a copy of the args */
  /* argv[0]: progname
   * argv[1]: cd drive or /C for configuring
   * argv[2]: REXX file
   * argv[3]: folder path
   */

  numArgs=argc;


  for(a=0;a<argc;a++)
    {
      params[a]=argv[a];
    }

  strncpy(chrBinDir, argv[0], sizeof(chrBinDir)-1);
  if((chrPtr=strrchr(chrBinDir, '\\'))!=NULLHANDLE)
    *chrPtr=0;
  strncpy(chrInstallDir, chrBinDir, sizeof(chrInstallDir)-1);
  /*  if((chrPtr=strrchr(chrInstallDir, '\\'))!=NULLHANDLE)
   *chrPtr=0;*/

  if(numArgs<3) {
    usage(argv[0]);
    exit(1);
  }

  /* Build the cache directory location */
  strncpy(chrCDDBDir, chrInstallDir, sizeof(chrCDDBDir)-1);
  sprintf(chrCDDBDir, "%s\\cddbdata",chrInstallDir);
  sprintf(cddbDataFile, "%s\\",chrCDDBDir);

  strncpy(chrCDDrive, argv[1],sizeof(chrCDDrive)-1);
  params[1]=chrInstallDir; /* For writeLog */
  if(!stricmp(argv[1],"/c")) /* Configure E-Mail address */
    {
      config(argv[2]);
      exit(0);
    }
  chrRexxFile=argv[2];
  chrFolder=argv[3];

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      removeLog();

      /*WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrInstallDir, "", 123, MB_MOVEABLE);*/
      writeLog(argv[0]);
      writeLog(" started with the following parameters:\n\n");
      for(a=0;a<argc;a++)
        {
          writeLog(argv[a]);
          writeLog("\n");
        }
      writeLog("\n");

      /* Check if user started prog by hand */   
      if(!(argc<5))
        {/*pmUsage();*/
        }
      else {
        /* The window we notify after getting the cddb data */
        /*        hwndNotify=atol(params[3]);*/
        /* Get our ressource dll */  
        RESSOURCEHANDLE=queryResModuleHandle2(chrInstallDir, RESDLL_BASENAME);
        /* Load options from cdrecord.ini */
        if(readCDDBIni()) {
          /*        sprintf(cddbDataFile,"%s\\cddbdata\\",params[1]); */
          writeLog("\nCache directory is: ");
          writeLog(cddbDataFile);
          writeLog("\n\n");
          /* Object window creation */
          hwndClient=WinCreateWindow(HWND_OBJECT,WC_STATIC,(UCHAR*)"cddbObject",0,0,0,0,0,NULL,HWND_BOTTOM,12343,NULL,NULL);
          if(hwndClient) {
            WinSubclassWindow(hwndClient,&cddbObjectProc);
            /* Window created. */             
            WinPostMsg(hwndClient,WM_APPTERMINATENOTIFY,NULL,NULL); /* Let's start */
            while(WinGetMsg(hab,&qmsg,(HWND)NULL,0,0))
              WinDispatchMsg(hab,&qmsg);
            WinDestroyWindow(hwndClient);
          }
          freeResHandle();
        }
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}









