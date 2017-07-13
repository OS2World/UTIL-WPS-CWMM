/*
 * pmmp3dec.c (C) Chris Wohlgemuth 2002
 *
 * This helper decodes an MP3
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
#include "progbars.h"
#include "common.h"
#include "volumeres.h"

char logName[]="volume.log";

BOOL bHaveWindowPos=FALSE;

BOOL bMute=FALSE; 

HMODULE RESSOURCEHANDLE=0;
HWND    hwndMainDialogBox;         /* Handle to the dialog window.            */
HWND hwndFrame;
char* params[5];

ULONG cwQueryOSRelease();
void pmUsage();

ULONG queryMasterVolume(void)
{
  char chrCommand[50];
  char retMsg[20];
  ULONG rc;

   DosBeep(2000, 200);
    return 75;

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
  char chrCommand[50];
  char retMsg[20];
  ULONG rc;

  sprintf(chrCommand, "MASTERAUDIO VOLUME %d WAIT", sVolumeLevel);
  rc = mciSendString(chrCommand, retMsg, sizeof(retMsg), hwnd, 0);
  if((rc & 0x0000ffff)!=MCIERR_SUCCESS) {
    /* Problem with MCI. */
    
    return ;
  }

}

/* This Proc handles the on-the-fly data CD writing */
MRESULT EXPENTRY decodeStatusDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  char text[CCHMAXPATH*4 +10];
  char title[CCHMAXPATH*4];
  SWCNTRL swctl;
  PID pid;
  int a;
  int iPercent;
  HPOINTER          hpProgramIcon;           /* handle to program's icon     */

  switch (msg)
    {      
    case WM_INITDLG:
      DosBeep(5000, 200);
#if 0
      /* Add switch entry */
      memset(&swctl,0,sizeof(swctl));
      WinQueryWindowProcess(hwndMainDialogBox,&pid,NULL);
      swctl.hwnd=hwndMainDialogBox;//hwndFrame;
      swctl.uchVisibility=SWL_VISIBLE;
      swctl.idProcess=pid;
      swctl.bProgType=PROG_DEFAULT;
      swctl.fbJump=SWL_JUMPABLE;
      
      strcpy(swctl.szSwtitle,"Volume!!");
      WinAddSwitchEntry(&swctl);
#endif

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
#if 0
      WinSendMsg( WinWindowFromID(hwnd, IDCS_VOLUME),
                  CSM_SETVALUE,
                  (MPARAM) queryMasterVolume(),
                  (MPARAM) NULL);
#endif             
      DosBeep(5000, 200);
      // getMessage(title, IDSTR_CONVERTCHANNELS, sizeof(title), RESSOURCEHANDLE, hwnd);
      /* Set dialog font to WarpSans for Warp 4 and above */
      if(cwQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }
      DosBeep(500, 200);
#if 0
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

      WinDefSecondaryWindowProc(
                                hwnd,                    /* Dialog window handle.                 */
                                WM_SETICON,              /* Message to the dialog.  Set it's icon.*/
                                (MPARAM) hpProgramIcon,
                                (MPARAM) 0 );            /* mp2 no value.                         */
#endif
      WinShowWindow( hwndFrame, TRUE );

      return (MRESULT) FALSE;
      /* WM_APPTERMINATENOTIFY messages are sent from the helper programs e.g. format checker. */
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

      WinPostMsg(hwnd, WM_QUIT, 0, 0);
      return (MRESULT )FALSE;
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDPB_CLOSE:
          WinPostMsg(hwnd, WM_CLOSE, 0, 0);/* Timer error so do a hard quit */
          break;
        default:
          break;
        }
      return (MRESULT) FALSE;
    default:
      break;
    }/* switch */
  return (MRESULT) WinDefSecondaryWindowProc( hwnd, msg, mp1, mp2 ) ;
  //return WinDefDlgProc( hwnd, msg, mp1, mp2);
}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  QMSG qmsg;
  char text[CCHMAXPATH];
  char title[CCHMAXPATH];
  short a;
  HWND hwndClient;
  HMMIO hmmio;
  ULONG result;
  LONG lBytesRead=0;
  ULONG rc;
   HPOINTER          hpProgramIcon;           /* handle to program's icon     */

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      /* Check if user started prog by hand */   
      if(argc!=1) {/* Not the actual num of params */
        //pmUsage();
      }
      else {
        RESSOURCEHANDLE=0;
        

        hwndFrame=WinLoadSecondaryWindow(
                                         HWND_DESKTOP,           /* Parent of the dialog box.     */
                                         HWND_DESKTOP,           /* Owner of the dialog box.      */
                                         (PFNWP) decodeStatusDialogProc,   /* */
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
        
      }
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}









