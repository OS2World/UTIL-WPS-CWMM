/*
 * Volume.exe (C) Chris Wohlgemuth 2003
 *
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
 * If you need another license for your project/product (commercial,
 * noncommercial, whatever) contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 *
 */

#define INCL_DOS
#define INCL_GPI
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_MCIOS2
#define INCL_SECONDARYWINDOW    
#define INCL_GRAPHICBUTTON 
#include <os2.h>
#include <sw.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "os2me.h"
#include "common.h"
#include "mmres.h"
#include "mmprogs_defaults.h"

#include "sys_funcs.h"

#define ACTION_SETVOLUME     1L
#define ACTION_QUERYVOLUME   2L

//#define INI_NAME_APP        "volume"

#define INI_TBVISIBLE_KEY   "tbVisible"
#define INI_NAME_COLOR        "color"
#define INI_BTNBGCLR_KEY       "btnbg"
#define INI_BTNFGCLR_KEY       "btnfg"

#if 0
#define INI_BGCLR_KEY       "bg"
#define INI_FGCLR_KEY       "fg"
#define INI_ACTIVETBBGCLR_KEY       "activetbbg"
#define INI_INACTIVETBBGCLR_KEY       "inactivetbbg"
#define INI_ACTIVETBFGCLR_KEY       "activetbfg"
#define INI_INACTIVETBFGCLR_KEY       "inactivetbfg"
#endif
char logName[]="volume.log";

BOOL bHaveWindowPos=FALSE;

BOOL bMute=FALSE; 
HMODULE RESSOURCEHANDLE=0;
int bTBVisible=0;
ULONG ulTBCy;

HWND hwndThread;

char* params[5];
char chrIniFile[CCHMAXPATH];

//ULONG cwQueryOSRelease();
void pmUsage();
BOOL IniSaveWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniRestoreWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniSaveInt(char * iniFile, char* chrApp, char *chrKey, int theInt);
int IniRestoreInt(char * iniFile, char* chrApp, char *chrKey, int defaultInt);
BOOL IniSaveData(char * iniFile, char* chrApp, char *chrKey, void* theData, ULONG ulSize);
BOOL IniRestoreData(char * iniFile, char* chrApp, char *chrKey, void * theData, ULONG* ulMaxSize);
BOOL IniSaveWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL IniRestoreWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize);
HMODULE queryResModuleHandle(char *chrExePath);
void freeResHandle();

ULONG queryMasterVolume(void)
{
  char chrCommand[50];
  char retMsg[20];
  ULONG rc;

  strcpy(chrCommand, "MASTERAUDIO QUERY VOLUME WAIT");
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. */
    return 75;
  }
  else
    return atoi(retMsg);
}

void setMasterVolume(HWND hwnd, SHORT sVolumeLevel)
{
  char chrCommand[500];
  char retMsg[20];
  ULONG rc;

  WinPostMsg(hwndThread, WM_APPTERMINATENOTIFY, MPFROMLONG(ACTION_SETVOLUME), MPFROMSHORT(sVolumeLevel));
  return;

  sprintf(chrCommand, "MASTERAUDIO volume %d wait ", sVolumeLevel);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), hwnd, 2);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. */
    mciGetErrorString(rc, chrCommand,sizeof(chrCommand));
    WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrCommand, "", 1234, MB_MOVEABLE);
    //DosBeep(100, 30);
    return ;
  }
}

/* Masteraudio does not support NOTIFY so use a thread to prevent PM freeze */
MRESULT EXPENTRY volumeObjectProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  switch(msg)
    {
    case WM_APPTERMINATENOTIFY:
      {
        char chrCommand[500];
        char retMsg[20];
        ULONG rc;

        /* This queue is blocked until the query is over. This is the reason we've chosen an object window. */
        switch(LONGFROMMP(mp1))
          {
          case ACTION_SETVOLUME:
            {

#if 0
              ULONG mastervolume;              /* Set to master volumepercentage
                                                  percentage by this example      */
              
              USHORT usDeviceID=0;
              MCI_MASTERAUDIO_PARMS masteraudioparms;
              MCI_OPEN_PARMS   openParms;


              memset(&openParms,0, sizeof(openParms));
              openParms.pszDeviceType="Masteraudio";
 
              rc=mciSendCommand(0,       /* Device                          */
                             MCI_OPEN,                /* Master audio message            */
                             MCI_WAIT ,
                             /* Flags for this message          */
                             (PVOID) &openParms,      /* Data structure                  */
                             0);                             /* User parm                       */

              mciGetErrorString(rc,chrCommand, sizeof(chrCommand));
              //sprintf(chrCommand, "%x", rc);
              WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrCommand, "", 1234, MB_MOVEABLE);              
              DosBeep(5000, 10);
              usDeviceID=openParms.usDeviceID;
              memset(&masteraudioparms,0, sizeof(masteraudioparms));
              masteraudioparms.ulMasterVolume=32;
              /* Get current system master
                 volume setting                  */
              
              mciSendCommand(usDeviceID,       /* Device                          */
                             MCI_MASTERAUDIO,                /* Master audio message            */
                             MCI_WAIT |  MCI_MASTERVOL,
                             /* Flags for this message          */
                             (PVOID) &masteraudioparms,      /* Data structure                  */
                             0);                             /* User parm                       */

              /* Get current system master
                 volume setting                  */
              
              mciSendCommand(usDeviceID,       /* Device                          */
                             MCI_MASTERAUDIO,                /* Master audio message            */
                             MCI_WAIT | MCI_QUERYCURRENTSETTING | MCI_MASTERVOL,
                             /* Flags for this message          */
                             (PVOID) &masteraudioparms,      /* Data structure                  */
                             0);                             /* User parm                       */
              
              mastervolume = masteraudioparms.ulReturn;
                          DosBeep(500, 10);
#endif

            sprintf(chrCommand, "MASTERAUDIO VOLUME %d WAIT", SHORT1FROMMP(mp2));
            //            sprintf(chrCommand, "MASTERAUDIO VOLUME 39", SHORT1FROMMP(mp2));
            rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
            //DosBeep(5000, 20);

            if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
              /* Problem with MCI. */
              //    DosBeep(1000, 10);
              break;
            }

            break;
            }
          case ACTION_QUERYVOLUME:
            break;
          default:
            break;
          }
        return FALSE;
      }
    case WM_CLOSE:
      WinPostMsg(hwnd,WM_QUIT,(MPARAM)0,(MPARAM)0);
    default:
      break;
    }
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}

void _Optlink volumeThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {
      
      if((hwndThread=WinCreateWindow(HWND_OBJECT, WC_STATIC, "", 0,0,0,0,0, NULLHANDLE,
                                     HWND_BOTTOM, 1, NULLHANDLE, NULLHANDLE))==NULLHANDLE) {
        WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, "Can't start thread. Aborting...", "", 1234, MB_MOVEABLE);
        exit(1);
      }
      WinSubclassWindow(hwndThread,&volumeObjectProc);
      /* Window created. */             

      while(WinGetMsg(hab,&qmsg,(HWND)NULL,0,0))
        WinDispatchMsg(hab,&qmsg);
      WinDestroyWindow(hwndThread);
      
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }

}

static void contextMenu(HWND hwnd)
{
  POINTL ptl;
  HWND   hwndPopup;
  HWND hwndTitleBar;
  BOOL bVisible;

  if((hwndTitleBar=WinWindowFromID(hwnd, FID_TITLEBAR))==NULLHANDLE)
    return;
      
  hwndPopup = WinLoadMenu(hwnd, RESSOURCEHANDLE, IDM_POPUP);
  if (hwndPopup == NULLHANDLE)
    return;
  
  bVisible=WinIsWindowVisible(hwndTitleBar);

  if(!bVisible)
    WinSendMsg(hwndPopup,MM_SETITEMATTR,MPFROM2SHORT(IDM_ITEMTITLEBAR,FALSE), 
               MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED));

  WinQueryPointerPos(HWND_DESKTOP, &ptl) ;
  WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
  WinPopupMenu(hwnd, hwnd, hwndPopup, 
               ptl.x, ptl.y, IDM_ITEMEXIT, PU_HCONSTRAIN | PU_VCONSTRAIN |
               PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE  );
}


static BOOL toggleTitleBar(HWND hwnd)
{
  HWND hwndTitleBar;

  if((hwndTitleBar=WinWindowFromID(hwnd, FID_TITLEBAR))!=NULLHANDLE)
    {
      SWP swp, swpFrame;

      if(!WinQueryWindowPos(hwndTitleBar,&swp))
        return FALSE;
      
      if(!WinQueryWindowPos(hwnd,&swpFrame))
        return FALSE;
      
      /* Shrink or expand the frame */
      if(!bTBVisible) {
        WinSetWindowPos(hwnd, NULLHANDLE, swpFrame.x, swpFrame.y,//-swp.cy, 
                        swpFrame.cx, swpFrame.cy+swp.cy, SWP_SIZE|SWP_MOVE);
        bTBVisible=TRUE;
      }
      else
        {
          WinSetWindowPos(hwnd, NULLHANDLE, swpFrame.x, swpFrame.y,//+swp.cy, 
                          swpFrame.cx, swpFrame.cy-swp.cy, SWP_SIZE|SWP_MOVE);
          bTBVisible=FALSE;
        }
    }
  WinSendMsg(hwnd,WM_UPDATEFRAME,0,0);      
  return TRUE;
}


/* This Proc handles the on-the-fly data CD writing */
MRESULT EXPENTRY volumeDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  //char text[CCHMAXPATH*4 +10];
  //char title[CCHMAXPATH*4];
  HPOINTER          hpProgramIcon;           /* handle to program's icon     */

  switch (msg)
    {      
    case WM_INITDLG:
      {
        RGB rgb;
        ULONG ulSize;
        SWP swp;
        HWND hwndTB;
        ULONG cy;

        /*
         * The slider control cannot be completely set from the dialog
         * template so some aspects must be set here.  We will set the
         * volume range to 0-100, increment to 1-10, and the initial
         * volume level to 75.
         */
        WinSendMsg( WinWindowFromID(hwnd, IDCS_VOLUME),
                    CSM_SETRANGE,
                    (MPARAM) 0L,
                    (MPARAM) 100L);
        
        WinSendMsg( WinWindowFromID(hwnd, IDCS_VOLUME),
                    CSM_SETINCREMENT,
                    (MPARAM) 10L,
                    (MPARAM) 1L);
        
        
        WinSendMsg( WinWindowFromID(hwnd, IDCS_VOLUME),
                    CSM_SETVALUE,
                    (MPARAM) queryMasterVolume(),
                    (MPARAM) NULL);
        
        
        // getMessage(title, IDSTR_CONVERTCHANNELS, sizeof(title), RESSOURCEHANDLE, hwnd);
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(hwnd,
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        
        hpProgramIcon =
          WinLoadPointer(
                         HWND_DESKTOP,
                         (HMODULE) NULL,              /* Resource is kept in .Exe file. */
                         ID_ICONVOLUME );                   /* Which icon to use.             */
        
        WinSendMsg(
                   hwnd,                    /* Dialog window handle.                 */
                   WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                   (MPARAM) hpProgramIcon,
                   (MPARAM) 0 );            /* mp2 no value.                         */
#if 0
        WinDefSecondaryWindowProc(
                                  hwnd,                    /* Dialog window handle.                 */
                                  WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                                  (MPARAM) hpProgramIcon,
                                  (MPARAM) 0 );            /* mp2 no value.                         */
#endif

        bTBVisible=IniRestoreInt(chrIniFile, INI_VOLUME_APP, INI_TBVISIBLE_KEY, 1);
        //      sprintf(text, ": %d", bTBVisible);
        //   WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, text, "", 1234, MB_MOVEABLE);

        IniRestoreWindowPos(chrIniFile, INI_VOLUME_APP, INI_WINDOWPOS_KEY, hwnd);
        IniRestoreWindowClrs(chrIniFile, INI_VOLUME_APP , hwnd);

        ulSize=sizeof(RGB);
        if(IniRestoreData(chrIniFile, INI_VOLUME_APP, INI_BTNBGCLR_KEY, &rgb, &ulSize))
          {
            // Set the background colour
            WinSetPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                            PP_BACKGROUNDCOLOR,(ULONG)sizeof(rgb), &rgb);
          }
        ulSize=sizeof(RGB);        
        if(IniRestoreData(chrIniFile, INI_VOLUME_APP, INI_BTNFGCLR_KEY, &rgb, &ulSize))
          {
            // Set the foreground colour
            WinSetPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                            PP_FOREGROUNDCOLOR,(ULONG)sizeof(rgb), &rgb);
          }
        //#if 0
        if(!bTBVisible)
          {
            /* Reduce size of dialog */
            hwndTB=WinWindowFromID(hwnd, FID_TITLEBAR);

            WinQueryWindowPos(hwndTB, &swp);
            cy=swp.cy;
            WinQueryWindowPos(hwnd, &swp);
            WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, swp.cx, swp.cy-cy, SWP_SIZE  );
          }
        //#endif

        WinShowWindow( hwnd, TRUE );
        _beginthread(volumeThreadFunc,NULL,8192*16, NULLHANDLE); //Fehlerbehandlung fehlt
        return (MRESULT) FALSE;
      }

    case WM_FORMATFRAME:
      {
        /* Remove top frame controls if titlebar is hidden */
        SHORT countSwp;
                
        countSwp = (int) WinDefDlgProc(hwnd, msg, mp1, mp2);
        if(countSwp) {
          if(bTBVisible==0) { 
            /* Hide frame controls */        
            /* countSWP:  number of standard framecontrols
               pswp:      Array of SWP describing the framecontrols.
               */
            int a;
            PSWP pswpTB=NULLHANDLE;
            PSWP pswp=(PSWP)mp1;
            int y, cy;

            for (a = 0; a < countSwp; a++)
              {
                /* Find the titlebar window */
                if ( WinQueryWindowUShort( pswp[a].hwnd, QWS_ID ) == FID_TITLEBAR )
                  {
                    pswpTB=&pswp[a];
                    y=pswp[a].y;
                    cy=pswp[a].cy;
                    //    DosBeep(500,600);
                    break;
                  }
              }

            if(pswpTB) {
              /* Hide all frame controls at the top */
              for (a = 0; a < countSwp; a++)
                {
                  if ( pswp[a].y == y )
                    {
                      pswp[a].fl|=SWP_HIDE;
                      pswp[a].fl&=~SWP_SHOW;
                    }
                }/* for() */
              /* Reduce size of dialog */
              //    WinQueryWindowPos(hwnd, &swp);
              /*WinSetWindowPos(hwnd, NULLHANDLE, 0, 0, swp.cx, swp.cy-cy, SWP_SIZE  );*/
              //      DosBeep(5000,600);
            }
          }
        }
        return MRFROMSHORT(countSwp);
      }
    case WM_CONTEXTMENU:
      contextMenu(hwnd);
      return (MRESULT) FALSE;
    case WM_BUTTON1DBLCLK:
      {
        /* Remove frame controls (titlebar) when double clicking MB 1 */
        toggleTitleBar(hwnd);
        return (MRESULT) FALSE;
      }
    case WM_BUTTON2MOTIONSTART:
    case WM_BUTTON1MOTIONSTART:
      /* Drag the window without titlebar */
      WinSendMsg(hwnd, WM_TRACKFRAME, 
                 MPFROMSHORT(TF_MOVE   /*| TF_SETPOINTERPOS */), NULL) ;
      return (MRESULT) 0 ;
      case WM_CONTROL:
         if (SHORT1FROMMP(mp1)==IDCS_VOLUME)
         {
            if ((SHORT2FROMMP(mp1)==CSN_CHANGED) ||    /* change volume?   */
                (SHORT2FROMMP(mp1)==CSN_TRACKING))     /* tracking volume? */
            {
              SHORT sVolumeLevel;

               sVolumeLevel = SHORT1FROMMP (mp2);
               if(!bMute)
                 setMasterVolume(hwnd, sVolumeLevel);
            }
         }
         else if (SHORT1FROMMP(mp1)==IDPB_MUTE)
           {
             if (SHORT2FROMMP(mp1)==GBN_BUTTONDOWN) {
               bMute=TRUE; 
               setMasterVolume(hwnd, 0);
             }
             else if (SHORT2FROMMP(mp1)==GBN_BUTTONUP) {
               SHORT sValue;
               
               bMute=FALSE;
               WinSendMsg(WinWindowFromID(hwnd, IDCS_VOLUME),CSM_QUERYVALUE ,MPFROMP(&sValue) ,0);
               setMasterVolume(hwnd, sValue);
             }
           }
         return( (MRESULT) 0);
    case WM_CLOSE:
      {
        MCI_MASTERAUDIO_PARMS masteraudioparms;
        ULONG  attrFound;
        RGB rgb;

        memset(&masteraudioparms,0, sizeof(masteraudioparms));
        /* Save master audio level */
        mciSendCommand( 0,       
                       MCI_MASTERAUDIO,                /* Master audio message            */
                       MCI_WAIT |  MCI_MASTERVOL | MCI_SAVESETTING,
                       /* Flags for this message          */
                       (PVOID) &masteraudioparms,
                       0);

        WinPostMsg(hwndThread, WM_CLOSE, 0, 0);
        /* Save window position */
        IniSaveWindowPos(chrIniFile, INI_VOLUME_APP, INI_WINDOWPOS_KEY, hwnd);
        /* Save titlebar state */
        bTBVisible=WinIsWindowVisible(WinWindowFromID(hwnd, FID_TITLEBAR));
        IniSaveInt(chrIniFile, INI_VOLUME_APP, INI_TBVISIBLE_KEY, bTBVisible);
        /* Save colors */
        // Query the current button background colour
        if(WinQueryPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                             PP_BACKGROUNDCOLOR,0,&attrFound,sizeof(rgb),
                             &rgb,QPF_NOINHERIT))
          {
            IniSaveData(chrIniFile, INI_VOLUME_APP, INI_BTNBGCLR_KEY, &rgb, sizeof(RGB));
          }
        // Query the current button foreground colour
        if(WinQueryPresParam(WinWindowFromID(hwnd, IDPB_MUTE),
                             PP_FOREGROUNDCOLOR,0,&attrFound,sizeof(rgb),
                             &rgb,QPF_NOINHERIT))
          {
            IniSaveData(chrIniFile, INI_VOLUME_APP, INI_BTNFGCLR_KEY, &rgb, sizeof(RGB));
          }
        IniSaveWindowClrs(chrIniFile, INI_VOLUME_APP , hwnd);
        WinPostMsg(hwnd, WM_QUIT, 0, 0);
        return (MRESULT )FALSE;
      }
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDM_ITEMTITLEBAR:
          toggleTitleBar(hwnd);
          break;
        case IDM_ITEMEXIT:
          WinPostMsg(hwnd, WM_CLOSE, 0, 0);/* Timer error so do a hard quit */
          break;
        default:
          break;
        }
      return (MRESULT) FALSE;
    default:
      break;
    }/* switch */
  //return (MRESULT) WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) ;
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}


int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  //char text[CCHMAXPATH];
  //char title[CCHMAXPATH];

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {

      RESSOURCEHANDLE=queryResModuleHandle(argv[0]);

      HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));
      bTBVisible=IniRestoreInt(chrIniFile, INI_VOLUME_APP, INI_TBVISIBLE_KEY, 1);

      if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, 
                     volumeDialogProc, RESSOURCEHANDLE, IDDLG_VOLUME, 0) == DID_ERROR )
        {
          /*
            Text:   
            Title:  "Installation problem"                       
            */
#if 0
          messageBox( text, IDSTR_CONVERTDIALOGERROR , sizeof(text),
                      title, IDSTR_INSTALLERRORTITLE , sizeof(title),
                      RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);
#endif       
          WinDestroyMsgQueue( hmq );
          WinTerminate( hab );
          DosBeep(100,600);
          freeResHandle();
          return( 1 );
        }
#if 0
      hwndFrame=WinLoadSecondaryWindow(
                                       HWND_DESKTOP,           /* Parent of the dialog box.     */
                                       HWND_DESKTOP,           /* Owner of the dialog box.      */
                                       (PFNWP) volumeDialogProc,   /* */
                                       (HMODULE) RESSOURCEHANDLE,         /* */
                                       IDDLG_VOLUME,            /* Dialog ID.                   */
                                       (PVOID) NULL);          /* Creation      */
      hwndMainDialogBox = WinQuerySecondaryHWND(hwndFrame, QS_DIALOG);
      WinShowWindow( hwndFrame, TRUE );
      hpProgramIcon =
        WinLoadPointer(
                       HWND_DESKTOP,
                       (HMODULE) NULL,              /* Resource is kept in .Exe file. */
                       ID_ICONVOLUME );                   /* Which icon to use.             */
      WinSendMsg(
                 hwndFrame,                    /* Dialog window handle.                 */
                 WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                 (MPARAM) hpProgramIcon,
                 (MPARAM) 0 );            /* mp2 no value.                         */
      WinPostMsg( WinWindowFromID(hwndFrame, IDCS_VOLUME),
                  CSM_SETVALUE,
                  (MPARAM) queryMasterVolume(),
                  (MPARAM) NULL);
      while ( WinGetMsg( hab, (PQMSG) &qmsg, (HWND) NULL, 0, 0) )
        WinDispatchMsg( hab, (PQMSG) &qmsg );  
#endif
      freeResHandle();
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}












