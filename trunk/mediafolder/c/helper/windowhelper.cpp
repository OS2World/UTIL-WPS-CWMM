#define INCL_DOSERRORS
#include "mediafolder.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mediafolderres.h"
#include <sys_funcs.h>

/* typedef for somTP_MMAudio_cwmmQueryTrackInfo */
/* For calling the method in the CWAudio class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
#include "cwmmquerytrackinfo.h"

#define IDINFO_NAME              1
#define IDINFO_ARTIST            2
#define IDINFO_ALBUM             3
#define IDINFO_YEAR              4
#define IDINFO_COMMENT           5
#define IDINFO_GENRE             6

#define IDINFO_PLAYTIME          7

/* Pointer into window words */
extern ULONG ulQWP_CONTAINERPROC;
extern ULONG ulQWP_THISPTR;
extern ULONG ulQWL_TOPCTRLHWND;
extern ULONG ulQWL_BOTTOMCTRLHWND;
extern ULONG ulQWL_RIGHTCTRLHWND;
extern ULONG ulQWL_LEFTCTRLHWND;

extern BOOL somhlpQueryCWAudioMethodPtr(CWMediaFolder *thisPtr);
extern SOMClass* somhlpGetSomClass(char* chrClassName);

extern HMODULE queryResModuleHandle(void);

/*

  Functions to get the frame controls from the folder frame HWND.
  If anytime the window hierarchy changes only these functions must be
  changed but not dozens of places all over the source.

  The HWNDs of the controls are saved in the window words of the reregistered frame for
  easy access. Strivtly spoken no need to have the HWND in the instance data. Only the
  frame hwnd must be available.

 */

/* Get the HWND of the frame control at the top. That's not the control
   with the buttons and the time display. The play controls are located on
   another frame (use playControlDialogFromFrameHWND() to get it). */
HWND topControlFromFrameHWND(HWND hwndFrame)
{
  return WinQueryWindowULong(hwndFrame, ulQWL_TOPCTRLHWND);
}

/* Get the HWND of the frame control at the bottom (status bar) */
HWND bottomControlFromFrameHWND(HWND hwndFrame)
{
  return WinQueryWindowULong(hwndFrame, ulQWL_BOTTOMCTRLHWND);
}

/* Get the HWND of the play control dialog. This Dialog is on top of
   the top frame control. */
HWND playControlDialogFromFrameHWND(HWND hwndFrame)
{
  return WinWindowFromID(topControlFromFrameHWND(hwndFrame), IDDLG_TOPMIDDLE);
}

/* Get the HWND of the time display static text control. */
HWND playTimeControlFromFrameHWND(HWND hwndFrame)
{
  return WinWindowFromID(playControlDialogFromFrameHWND(hwndFrame), IDST_PLAYTIME);
}


/* Show/hide the text in the playtime area. Useful for blinking. */
void showPlayTimeDisplay(CWMediaFolder *thisPtr, BOOL bShow)
{
  WinShowWindow(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), bShow);
}

/* Toggle the visibility of the text in the playtime area. Useful for blinking. */
void togglePlayTimeDisplayVisibility(CWMediaFolder *thisPtr)
{
  if(WinIsWindowVisible(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame)))
    WinShowWindow(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), FALSE);
  else
    WinShowWindow(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), TRUE);
}

static HWND _findOpenMediaFldrView(CWMediaFolder *thisPtr)
{
  PVIEWITEM pViewItem;

  pViewItem=thisPtr->wpFindViewItem(VIEW_DETAILS|VIEW_CONTENTS, NULLHANDLE);
  if(pViewItem)
    return pViewItem->handle;

  return NULLHANDLE;
}

/*

  V0.2.8 (08.09.2003)
  This function sets the window list entry text, the folder frame title and the playtime text.

  */
static void setAllTitles(CWMediaFolder *thisPtr, char * chrText)
{
  HSWITCH hSwitch;
  char chrBuff[100]={0};
  SWP swp;

  my_strlcpy(chrBuff, thisPtr->wpQueryTitle(), sizeof(chrBuff));
  if(chrText && *chrText)
    {
      strncat(chrBuff," - ", sizeof(chrBuff)-1-strlen(chrBuff));
      strncat(chrBuff, chrText, sizeof(chrBuff)-1-strlen(chrBuff));
    }
  
  /* Set folder playtimer control */
  WinSetWindowText(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), chrText);
  /* 
     Register as a view so the 'Details view' in folder titles is changed to
     the given text.
     Changing the window title is not sufficient because the WPS only displays the number
     of characters in the switch list the object title had during first registering of a view.
     If the window title (or the object title) is changed later it will be truncated and the
     view is appended to the then truncated title. This is even true if the switch list title
     is directly changed using WinChangeSwitchEntry(). Probably the window list window is
     subclassed by the WPS to append the current view.
     */
  thisPtr->wpRegisterView(thisPtr->hwndMFldrFrame, chrText);

  /*
    If the window is minimized change also the window text so the info is shown
    in the minimized view. This will not change the length of the title in the window list entry.
    The string to be shown is built starting with the object title (as the WPS does) so it
    doesn't matter the switch list entry length is limited.
    */
  WinQueryWindowPos(thisPtr->hwndMFldrFrame, &swp);
  if(swp.fl & SWP_MINIMIZE)
    WinSetWindowText(thisPtr->hwndMFldrFrame, chrBuff);
 
  /* The switch list must also be changed because wpRegisterView()
     only changes the window list text once. */
  if((hSwitch=WinQuerySwitchHandle(thisPtr->hwndMFldrFrame,0))!=NULLHANDLE)
    {
      SWCNTRL swCtrl;

      if(!WinQuerySwitchEntry(hSwitch, &swCtrl))
        {
          strncpy(swCtrl.szSwtitle, chrBuff, MAXNAMEL+4);
          swCtrl.szSwtitle[MAXNAMEL+3]=0;
          WinChangeSwitchEntry( hSwitch, &swCtrl);
        }
    }
}

#if 0
static void setAllTitles(CWMediaFolder *thisPtr, const char * chrText)
{
  char chrBuff[100]={0};
  HSWITCH hSwitch;

  my_strlcpy(chrBuff,thisPtr->wpQueryTitle(), sizeof(chrBuff));
  strncat(chrBuff," - ", sizeof(chrBuff)-1-strlen(chrBuff));
  strncat(chrBuff, chrText, sizeof(chrBuff)-1-strlen(chrBuff));

  WinSetWindowText(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), chrText);
  //  WinSetWindowText(thisPtr->hwndMFldrFrame, chrBuff);
  thisPtr->wpRegisterView(thisPtr->hwndMFldrFrame, chrBuff);
  if((hSwitch=WinQuerySwitchHandle(thisPtr->hwndMFldrFrame,0))!=NULLHANDLE)
    {
      SWCNTRL swCtrl;
      if(!WinQuerySwitchEntry(hSwitch, &swCtrl))
        {                    
          strncpy(swCtrl.szSwtitle, chrBuff, MAXNAMEL+4);
          swCtrl.szSwtitle[MAXNAMEL+3]=0;
          WinChangeSwitchEntry( hSwitch, &swCtrl);
        }
    }

}
#endif

/* Set the text in the playtime area to 'theText' */
void setPlayTimeText2(CWMediaFolder *thisPtr, char * theText)
{
  setAllTitles(thisPtr, theText);
  //     WinSetWindowText(playControlDialogFromFrameHWND(thisPtr->hwndMFldrFrame), theText);
}

/* This funktion sets the playtime display according to the user settings.
   Supported are remaining time, elapsed time and ID3-tag display */
void setPlayTimeText(CWMediaFolder *thisPtr)
{
  char text[100];
  ULONG ulTemp;
  MMAudio* cwAudio;

  switch(thisPtr->usWhichDisplay)
    {
    case 1:
      /* Remaining */
      ulTemp=thisPtr->ulTotalLength/1000-thisPtr->ulPos;
      sprintf(text,"-%d:%02d",
              ulTemp/60,
              ulTemp%60);
      //      WinSetWindowText(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), text);

      setAllTitles(thisPtr, text);

      break;
    case 2:
      {
        ULONG ulTemp=WinQueryWindowULong(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), QWL_USER);
        SOMClass *cwAudioClass=somhlpGetSomClass("MMAudio");

        /* Show ID3 tag */
        cwAudio=thisPtr->cwQueryCurrentlyPlayingObject();
        if(somIsObj(cwAudio)) {
          if(!methodPtr)
            somhlpQueryCWAudioMethodPtr(thisPtr);
          sprintf(text,"");
          if(methodPtr && somIsObj(cwAudioClass) && cwAudio->somIsA(cwAudioClass) ){/* Query playtime in secs */
            char* chrPtr=text;
            text[0]=0;

            if(ulTemp>=15)
              text[0]=0;
            else if(ulTemp>=12) 
              methodPtr(cwAudio, chrPtr, sizeof(text), IDINFO_GENRE);
            else if(ulTemp>=9)
              methodPtr(cwAudio, chrPtr, sizeof(text), IDINFO_COMMENT);
            else if(ulTemp>=6)
              methodPtr(cwAudio, chrPtr, sizeof(text), IDINFO_ALBUM);
            else if(ulTemp>=3)
              methodPtr(cwAudio, chrPtr, sizeof(text), IDINFO_NAME);
            else if(ulTemp>=0)
              methodPtr(cwAudio, chrPtr, sizeof(text), IDINFO_ARTIST);
          }
          else
            ulTemp=15; /* Show playtime for midi files */
          
          if(ulTemp>=15||!strlen(text)) {
            /* Play time */
            sprintf(text,"%d:%02d",
                    thisPtr->ulPos/60,
                    thisPtr->ulPos%60);
          }
#if 0
          else
            if(!(ulTemp%3)) {
              /* Switch display every 3 secs */
            }
#endif

          setAllTitles(thisPtr, text);

          //   WinSetWindowText(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), text);
          // WinSetWindowText(WinQueryWindow(WinQueryWindow(thisPtr->hwndTopMiddle, QW_PARENT), QW_PARENT), "12345\n6789");

          ulTemp++;
          if(ulTemp==18)
            ulTemp=0;
          WinSetWindowULong(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), QWL_USER, ulTemp);
          break;
        }
      }
      /* Fall through to playtime */
    case 0:
      /* Elapsed time */
      sprintf(text,"%d:%02d",
              thisPtr->ulPos/60,
              thisPtr->ulPos%60);

      setAllTitles(thisPtr, text);
      //  WinSetWindowText(WinWindowFromID(thisPtr->hwndTopMiddle, IDST_PLAYTIME), text);
      break;
    default:
      break;
    }
}

void _resetDisplay(CWMediaFolder *thisPtr)
{
  setAllTitles(thisPtr, "");
  WinSetWindowText(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), "0:00");
  WinSetWindowULong(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), QWL_USER, 0);
  WinPostMsg( WinWindowFromID(playTimeControlFromFrameHWND(thisPtr->hwndMFldrFrame), IDSL_POSBAR),
              SLM_SETSLIDERINFO,
              MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
              MPFROMLONG(0L));
  //  thisPtr->wpRegisterView(thisPtr->hwndMFldrFrame, "");
}

/* This function sets the repeat button checkstate */
void winCheckRepeatButton(CWMediaFolder *thisPtr)
{
  if(thisPtr->ulFlags & FLAG_REPEAT)
    WinCheckButton(playControlDialogFromFrameHWND(thisPtr->hwndMFldrFrame), IDCB_REPEAT, 1);
  else
    WinCheckButton(playControlDialogFromFrameHWND(thisPtr->hwndMFldrFrame), IDCB_REPEAT, 0);
}

/* This function sets the position of the volume slider */
void winSetVolumeSlider(CWMediaFolder *thisPtr)
{
  WinSendMsg( WinWindowFromID(playControlDialogFromFrameHWND(thisPtr->hwndMFldrFrame), IDSL_VOLUME),
              SLM_SETSLIDERINFO,
              MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
              MPFROMLONG( thisPtr->ulVolume));
}


void contextMenu(HWND hwnd, USHORT usItem, BOOL fCheckItem)
{
  POINTL ptl;
  HWND   hwndPopup;

  hwndPopup = WinLoadMenu(hwnd, queryResModuleHandle(), ID_MENUTIMECONTEXT) ;
  if (hwndPopup == NULLHANDLE) {
    DosBeep(100,1000);
    return;
  }
  if(fCheckItem)
    WinSendMsg(hwndPopup,MM_SETITEMATTR,MPFROM2SHORT(usItem, TRUE), MPFROM2SHORT(MIA_CHECKED,MIA_CHECKED));

  WinQueryPointerPos(HWND_DESKTOP, &ptl) ;
  WinMapWindowPoints(HWND_DESKTOP,hwnd,&ptl,1);
  WinPopupMenu(hwnd, hwnd, hwndPopup, 
               ptl.x, ptl.y, 0, PU_HCONSTRAIN | PU_VCONSTRAIN |
               PU_KEYBOARD | PU_MOUSEBUTTON1 | PU_MOUSEBUTTON2 | PU_NONE  ); 
  
}


