/*
 * This file is (C) Chris Wohlgemuth 2002
 * 
 * It's part of the media folder distribution
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

#include "cdfolder.hh"
#include <stdio.h>
#include <stdlib.h>
 
//extern HMODULE hResource;
extern char chrInstallDir[CCHMAXPATH];

ULONG launchRexx(PSZ rexxFile);
HMODULE queryModuleHandle(PSZ pszClassName);
HMODULE queryResModuleHandle(void);
BOOL _queryHelpLibrary(char * chrBuffer, int iSize);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
BOOL extern CDQueryCDDrives(int *iNumCD, char * cFirstDrive);
SOMClass* somhlpGetSomClass(char* chrClassName);
HMODULE queryModuleHandle(PSZ pszClassName);

/* Overriden function which returns our class name */
PSZ M_CWCDFolder::wpclsQueryTitle()
{
  static char chrTitle[40];

  if(getMessage(chrTitle, IDSTR_TITLECDFOLDER,sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
    return chrTitle;
  /* Fall back in case of error */
  return "CD-Player";
}

ULONG M_CWCDFolder::wpclsQueryStyle()
{
  return (M_CWMediaFolder::wpclsQueryStyle()|CLSSTYLE_NEVERTEMPLATE|CLSSTYLE_DONTTEMPLATE);
}

BOOL M_CWCDFolder::wpclsCreateDefaultTemplates(WPObject * wpObject)
{
  /* We don't wont any templates */
  return TRUE;
}

/* Overriden function which returns the custom icon */
ULONG M_CWCDFolder::wpclsQueryIconData(PICONINFO pIconInfo)
{
  //  HMODULE hResource=queryResModuleHandle();
  HMODULE hResource=queryModuleHandle("M_CWCDFolder");

  if(!hResource)
    return M_CWMediaFolder::wpclsQueryIconData(pIconInfo);
  
  if (pIconInfo)   {
    pIconInfo->fFormat = ICON_RESOURCE;
    pIconInfo->hmod    = hResource;
    pIconInfo->resid   = IDICON_CDFOLDER;
  } /* endif */

  return ( sizeof(ICONINFO) );
}



/* Overriden function which returns the custom icon */
ULONG M_CWCDFolder::wpclsQueryIconDataN(PICONINFO pIconInfo, ULONG ulIconIndex)
{
  HMODULE hResource=queryResModuleHandle();

  if(!hResource)
    return M_CWMediaFolder::wpclsQueryIconData(pIconInfo);

  if (pIconInfo)   {
    pIconInfo->fFormat = ICON_RESOURCE;
    pIconInfo->hmod    = hResource;
    pIconInfo->resid   = IDICON_CDFOLDER;
  } /* endif */

  return ( sizeof(ICONINFO) );
}

#if 0
MRESULT EXPENTRY cdCreateObjWindowProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  switch (msg)
    {
    case WM_APPTERMINATENOTIFY:
      {
DosBeep(5000, 200);  
        WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, 1, 2500);
        break;
      }
    case WM_TIMER:
      if(SHORT1FROMMP(mp1)==1) {
        int iNumCD;
        char cFirst;
        
        WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd,1);
        
        if(CDQueryCDDrives(&iNumCD, &cFirst)) {
          char name[100];
          char setup[100];
          char id[40];
          int a;
          
          DosSleep(1000);
          
          while(!somhlpGetSomClass(CDFLDR_CLASSNAME))
            DosSleep(100);
          
          for(a=0;a<iNumCD;a++) {
            int b;
            /* Build folder name */
            sprintf(name, "CD-Audio^Drive %c:", cFirst+a);
            /* Build object ID */
            sprintf(id, CDFLDR_ID, cFirst+a);
            /* Build setup string */
            sprintf(setup, "%s=%c:;OBJECTID=%s;", CDFLDR_DRIVE, cFirst+a, id);
            for(b=1;b<=5;b++) {
              /* Wait to let WPS load the class if not yet done. Then retry */ 
              if(!WinCreateObject(CDFLDR_CLASSNAME, name, setup,CDFLDR_LOCATION, CO_UPDATEIFEXISTS))
                printf("Can't create %s\n", name);
              else
                break;
              DosSleep(1000);
            }
          }
        }
        return (MRESULT)FALSE;
      }
      break;
    default:
      break;
    }
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}

void _Optlink createCDThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      HWND hwnd;
      int iNumCD;
      char cFirst;
    
      if(CDQueryCDDrives(&iNumCD, &cFirst)) {
        char name[100];
        char setup[100];
        char id[40];
        int a;
  
        DosSleep(1000);

        while(!somhlpGetSomClass(CDFLDR_CLASSNAME))
          DosSleep(100);

        for(a=0;a<iNumCD;a++) {
          int b;
          /* Build folder name */
          sprintf(name, "CD-Audio^Drive %c:", cFirst+a);
          /* Build object ID */
          sprintf(id, CDFLDR_ID, cFirst+a);
          /* Build setup string */
          sprintf(setup, "%s=%c:;OBJECTID=%s;", CDFLDR_DRIVE, cFirst+a, id);
          for(b=1;b<=5;b++) {
            /* Wait to let WPS load the class if not yet done. Then retry */ 
            if(!WinCreateObject(CDFLDR_CLASSNAME, name, setup,CDFLDR_LOCATION, CO_UPDATEIFEXISTS))
              printf("Can't create %s\n", name);
            else
              break;
            DosSleep(1000);
          }
        }
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
}
#endif

void _Optlink createCDThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {

      launchRexx( SCRIPT_CREATEPLAYERS);
      
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
}

void M_CWCDFolder::wpclsInitData()
{
  static BOOL bObjectsCreated=FALSE;

  /* Call parent */
  M_CWMediaFolder::wpclsInitData();

  /* Be sure we have our install dir */
  queryModuleHandle("M_CWCDFolder");

  /*
    V0.2.8 CW 2004/11/01

    Don't start a thread here for creating the cd objects. That will hang the
    system in many cases.
   */
#if 0
  /* Start skript to create cd objects */
  _beginthread(createCDThreadFunc,NULL,8192*16, NULLHANDLE); //Fehlerbehandlung fehlt
#endif

#if 0
  if(!bObjectsCreated) {
    HWND hwnd;

    // _beginthread(createCDThreadFunc,NULL,8192*16, NULLHANDLE); //Fehlerbehandlung fehlt
    //    launchRexx( SCRIPT_CREATEPLAYERS);
    hwnd=WinCreateWindow(HWND_OBJECT,WC_STATIC,"playObj", 0, 0, 0, 0, 0, 
                         NULLHANDLE, HWND_BOTTOM,12343,NULL,NULL);
    if(hwnd) {
      /* Window created. */

      if(WinSubclassWindow(hwnd,cdCreateObjWindowProc))
        DosBeep(1000, 200);
      WinPostMsg(hwnd, WM_APPTERMINATENOTIFY, 0, 0);
    }
  }
  bObjectsCreated=TRUE;
#endif
}

BOOL M_CWCDFolder::wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary)
{
  if(HelpLibrary)
    _queryHelpLibrary(HelpLibrary, CCHMAXPATH);

  if(HelpPanelId)
    *HelpPanelId= IDHLP_CDFOLDERMAIN;
	
  return TRUE;
}
