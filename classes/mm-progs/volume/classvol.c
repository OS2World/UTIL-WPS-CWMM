/*
 * classvol.c (C) Chris Wohlgemuth 2002-2003
 *
 * Vokume applet for audio classes.
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
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_WINWORKPLACE
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_MCIOS2
#define INCL_GPI
#define INCL_PM

#include <os2.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "os2me.h"
#include "common.h"
#include "classvolumeres.h"
#include "mmres.h"
#include "classvolume.h"
#include "mmprogs_defaults.h"
#include "sys_funcs.h"
//#include "cwaudioinc.h"

#ifndef OPEN_DEFAULT 
/*  Open views for the wpOpen() method */
      #define OPEN_UNKNOWN      -1
      #define OPEN_DEFAULT       0
      #define OPEN_CONTENTS      1
      #define OPEN_SETTINGS      2
      #define OPEN_HELP          3
      #define OPEN_RUNNING       4
      #define OPEN_PROMPTDLG     5
      #define OPEN_PALETTE       121                                   /*SPLIT*/
      #define OPEN_USER          0x6500
#endif

LOADEDBITMAP allBMPs[NUM_CTRL_IDX];

/* Format:
   typedef struct
   {
   INT id;
   RECTL rclSource;
   HBITMAP hbmSource;
   RECTL rclDest;
   }CONTROLINFO;
   */
CONTROLINFO ciControls[NUM_CTRL_IDX]={
  {0,{0,0,161,17}, NULLHANDLE,{0}}, /* main */
  /* Achtung destination verwendet fuer position */
  {IDSL_MMPLAYERVOLUME,{15, 2,141,17},NULLHANDLE, {20, 6, 146, 21} }, //{0, 0, 126, 9} /* {10,18,93,13} */ },
  {IDSL_MMPLAYERVOLUMEARM,{0,0,9,9},NULLHANDLE, {0,0,9,9}},
  {IDSL_MMPLAYERVOLUMEARM,{0,0,9,9},NULLHANDLE, {0,0,9,9}},
};


#define NUMPARAMS  2

/* argv[0]: progname
 * argv[1]: HWND object
 */

//#define DEBUG
#ifdef DEBUG
void HlpWriteToTrapLog(const char* chrFormat, ...);
#endif

int IniRestoreInt(char * iniFile, char* chrApp, char *chrKey, int defaultInt);
BOOL IniSaveInt(char * iniFile, char* chrApp, char *chrKey, int theInt);
BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize);
void freeResHandle();
HMODULE queryResModuleHandle(char *chrExePath);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
MRESULT menuInsertMenuSeparator(HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition );
SHORT menuInsertMenuItem( HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition, int iID, char * chrText);

void pmUsage();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);

MRESULT drawSlider(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2, USHORT id);
MRESULT EXPENTRY privateSliderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL paintHilitedControl(HWND hwnd, USHORT usControl, BOOL bHilite);
MRESULT paintControls(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY bgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY buttonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

BOOL writeVolumeToIni(char * iniFile, ULONG ulVoume);
int readVolumeFromIni(char * iniFile, int iDefault);
void sendCommandToObject(char* chrObject, char* command);

char logName[]="classvol.log";

extern SWP swpWindow;
BOOL bHaveWindowPos=FALSE;

/* Track to play. Full path */
char chrSourceName[CCHMAXPATH]={0};
char * chrPtrName=NULLHANDLE; /* Pointer to filename */
char chrClassName[100]= {0};

char chrIniFile[CCHMAXPATH];

int numArgs;
char* params[NUMPARAMS];

ULONG ulVolume=75;

HWND hwndObject;

HMODULE RESSOURCEHANDLE=0;

HMODULE BMP_RESSOURCEHANDLE=0;

PFNWP  oldPlayTimeProc;
PFNWP orgBgProc=NULLHANDLE; /* Org proc of background */
PFNWP orgButtonProc=NULLHANDLE;

HPOINTER hptrArrowWE=NULLHANDLE;

static void _loadBmps()
{
  HPS hps;
  HMODULE hResource;

  if(allBMPs[MAIN_BMP_IDX].hbm)
    return;

  hResource=BMP_RESSOURCEHANDLE; //_queryResModuleHandle();
  hps=WinGetPS(HWND_DESKTOP);
  
  /* The main BMP */
  allBMPs[MAIN_BMP_IDX].hbm=GpiLoadBitmap(hps, hResource, IDBMP_MAIN, 0, 0);
  GpiQueryBitmapParameters(allBMPs[MAIN_BMP_IDX].hbm, &allBMPs[MAIN_BMP_IDX].bmpInfoHdr);

#if 0
  /* Stop */
  allBMPs[CTRLIDX_STOPBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_STOP, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_STOPBUTTON].hbm, &allBMPs[CTRLIDX_STOPBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_STOPBUTTON].hbmSource=allBMPs[CTRLIDX_STOPBUTTON].hbm;

  /* Stop sel */
  allBMPs[CTRLIDX_STOPBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_STOPSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_STOPBUTTONSEL].hbm, &allBMPs[CTRLIDX_STOPBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_STOPBUTTONSEL].hbmSource=allBMPs[CTRLIDX_STOPBUTTONSEL].hbm;

  /* Pause */
  allBMPs[CTRLIDX_PAUSEBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PAUSE, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PAUSEBUTTON].hbm, &allBMPs[CTRLIDX_PAUSEBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_PAUSEBUTTON].hbmSource=allBMPs[CTRLIDX_PAUSEBUTTON].hbm;

  /* Pause sel */
  allBMPs[CTRLIDX_PAUSEBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PAUSESEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PAUSEBUTTONSEL].hbm, &allBMPs[CTRLIDX_PAUSEBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_PAUSEBUTTONSEL].hbmSource=allBMPs[CTRLIDX_PAUSEBUTTONSEL].hbm;

  /* Play */
  allBMPs[CTRLIDX_PLAYBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PLAY, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PLAYBUTTON].hbm, &allBMPs[CTRLIDX_PLAYBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_PLAYBUTTON].hbmSource=allBMPs[CTRLIDX_PLAYBUTTON].hbm;

  /* Play sel */
  allBMPs[CTRLIDX_PLAYBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_PLAYSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_PLAYBUTTONSEL].hbm, &allBMPs[CTRLIDX_PLAYBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_PLAYBUTTONSEL].hbmSource=allBMPs[CTRLIDX_PLAYBUTTONSEL].hbm;

  /* Positionslider bg */
  ciControls[CTRLIDX_POSSLIDER].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Positionslider arm */
  allBMPs[CTRLIDX_POSSLIDERARM].hbm=GpiLoadBitmap(hps, hResource, IDBMP_POSSLIDERARM, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_POSSLIDERARM].hbm, &allBMPs[CTRLIDX_POSSLIDERARM].bmpInfoHdr);
  ciControls[CTRLIDX_POSSLIDERARM].hbmSource=allBMPs[CTRLIDX_POSSLIDERARM].hbm;

  /* Playtime text field */
  ciControls[CTRLIDX_PLAYTIME].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;
#endif

  /* Volume slider background */
  ciControls[CTRLIDX_VOLSLIDER].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Volume slider arm */
  allBMPs[CTRLIDX_VOLSLIDERARM].hbm=GpiLoadBitmap(hps, hResource, IDBMP_VOLSLIDERARM, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_VOLSLIDERARM].hbm, &allBMPs[CTRLIDX_VOLSLIDERARM].bmpInfoHdr);
  ciControls[CTRLIDX_VOLSLIDERARM].hbmSource=allBMPs[CTRLIDX_VOLSLIDERARM].hbm;

  /* Volume slider arm selected*/
  allBMPs[CTRLIDX_VOLSLIDERARMSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_VOLSLIDERARMSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_VOLSLIDERARMSEL].hbm, &allBMPs[CTRLIDX_VOLSLIDERARMSEL].bmpInfoHdr);
  ciControls[CTRLIDX_VOLSLIDERARMSEL].hbmSource=allBMPs[CTRLIDX_VOLSLIDERARMSEL].hbm;

  WinReleasePS(hps);
}


/***********************************************************/
/*                                                         */
/* Move the controls to the position defined by the        */
/* background bitmap.                                      */
/*                                                         */
/***********************************************************/
void adjustDialogControlPos(HWND hwnd)
{
  CONTROLINFO ci;

#if 0
  ULONG ulStyle;
  /* STOP button */
  ci=ciControls[CTRLIDX_STOPBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id), HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);
  /* Pause button */
  ci=ciControls[CTRLIDX_PAUSEBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);
  /* Play button */
  ci=ciControls[CTRLIDX_PLAYBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);

  /* Playtime text */
  ci=ciControls[CTRLIDX_PLAYTIME];
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclSource.xLeft,ci.rclSource.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE|SWP_SIZE); 
  oldPlayTimeProc=WinSubclassWindow(WinWindowFromID(hwnd, IDST_MMPLAYERPLAYTIME), playTimeTextProc);

  /* Pos slider */
  ci=ciControls[CTRLIDX_POSSLIDER];//ciPosSlider;
  WinCreateWindow(hwnd, SLIDER_CLASS, "", WS_VISIBLE | WS_TABSTOP, 0,0, 10, 10, 
                  hwnd, HWND_TOP, ci.id, NULLHANDLE, NULLHANDLE);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft, ci.rclDest.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE | SWP_SIZE);
  
  WinSendMsg(WinWindowFromID(hwnd, ci.id),SLM_SETSLIDERINFO,
             MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,0),
             MPFROM2SHORT( 9, 9)); 
#endif

  /* Vol slider */
  ci=ciControls[CTRLIDX_VOLSLIDER];//ciVolSlider;
  WinCreateWindow(hwnd, SLIDER_CLASS, "", WS_VISIBLE | WS_TABSTOP, 0,0, 10, 10, 
                  hwnd, HWND_TOP, ci.id, NULLHANDLE, NULLHANDLE);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft, ci.rclDest.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE | SWP_SIZE);
  WinSendMsg(WinWindowFromID(hwnd, ci.id),SLM_SETSLIDERINFO,
             MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,0),
             MPFROM2SHORT( 9, 9)); 

}


/****************************************************************
 *  FixSysMenu procedure
 *--------------------------------------------------------------
 *
 *  Name:    FixSysMenu(HWND hwndDlg)
 *
 *  Purpose: This routine removes the Restore, Size, Minimize, and
 *           Maximize options from the system menu of a dialog.
 *
 *  Usage
 *
 *  Method:  Called during the WM_INITDLG of a dialog procedure.
 *
 *  Returns: VOID
 *
 ****************************************************************/
VOID FixSysMenu(HWND hwndDlg)
{
  HWND hwndMenu;                      /* Handle to system menu */
  MENUITEM mi={0};

  hwndMenu = WinWindowFromID(hwndDlg, FID_SYSMENU);


  WinSendMsg(hwndMenu,  /* Delete Restore from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_RESTORE, TRUE),
             MPFROMP(NULL));

  WinSendMsg(hwndMenu,  /* Delete Size from the system menu    */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_SIZE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu, /* Delete Minimize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_MINIMIZE, TRUE),
             MPFROMP(NULL));
  
  WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_MAXIMIZE, TRUE),
             MPFROMP(NULL));

  WinSendMsg(hwndMenu, /* Delete Maximize from the system menu */
             MM_DELETEITEM,
             MPFROM2SHORT(SC_HIDE, TRUE),
             MPFROMP(NULL));

  if(WinSendMsg(hwndMenu, /* Get sys menu menuitem  */
             MM_QUERYITEM,
             MPFROM2SHORT(SC_SYSMENU, FALSE),
             MPFROMP(&mi)))
    {
      char chrText[100];
      menuInsertMenuSeparator( mi.hwndSubMenu, NULLHANDLE, MIT_END);
      if(!getMessage(chrText,  IDSTR_MASTERVOLUMEITEM, sizeof(chrText), RESSOURCEHANDLE, hwndMenu))
         strcpy(chrText, "Master ~Volume");
      menuInsertMenuItem( mi.hwndSubMenu, NULLHANDLE, MIT_END, ID_MENUITEMMASTERVOLUME, chrText);
    }
  return;
}


/* This Proc handles the main dialog */
MRESULT EXPENTRY decodeStatusDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  //  char text[CCHMAXPATH*4 +10];
  //  char title[CCHMAXPATH*4];
  SWCNTRL swctl;
  PID pid;

  switch (msg)
    {      
    case WM_INITDLG:
      {

        /* Add switch entry */
        memset(&swctl,0,sizeof(swctl));
        WinQueryWindowProcess(hwnd,&pid,NULL);
        swctl.hwnd=hwnd;
        swctl.uchVisibility=SWL_VISIBLE;
        swctl.idProcess=pid;
        swctl.bProgType=PROG_DEFAULT;
        swctl.fbJump=SWL_JUMPABLE;
        WinAddSwitchEntry(&swctl);
        
        adjustDialogControlPos(hwnd);
        
        /* Set volume slider*/
        WinSendMsg( WinWindowFromID(hwnd, IDSL_MMPLAYERVOLUME),
                    SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMLONG( ulVolume));
                        
        FixSysMenu(hwnd);

        /* Size dialog */
        if(allBMPs[MAIN_BMP_IDX].hbm) {
          SWP swp;
          POINTL ptl;

          WinQueryWindowPos(WinWindowFromID(hwnd, FID_TITLEBAR), &swp);         
          WinSetWindowPos(WinWindowFromID(hwnd, ID_CLASSVOLCLIENT), NULLHANDLE, 
                          WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME), 
                          WinQuerySysValue(HWND_DESKTOP, SV_CYDLGFRAME), 
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx,
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy,
                          SWP_SIZE|SWP_MOVE);
          orgBgProc=WinSubclassWindow(WinWindowFromID(hwnd, ID_CLASSVOLCLIENT), bgProc);
          
          /* Get mouse position */
          WinQueryPointerPos(HWND_DESKTOP, &ptl);
          if(ptl.x+allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx > WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN))
            ptl.x=WinQuerySysValue(HWND_DESKTOP, SV_CXSCREEN)-allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx-20;

          WinSetWindowPos(hwnd, HWND_TOP, ptl.x/*-allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx/2*/,
                          ptl.y-allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy ,
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cx+WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME)*2,
                          allBMPs[MAIN_BMP_IDX].bmpInfoHdr.cy+swp.cy+WinQuerySysValue(HWND_DESKTOP, SV_CXDLGFRAME)*2
                          , SWP_SIZE|SWP_MOVE|SWP_ACTIVATE);
        }
        /* Set dialog font to WarpSans for Warp 4 and above */
        if(SysQueryOSRelease()>=40) {
          WinSetPresParam(WinWindowFromID(hwnd, IDST_MMPLAYERPLAYTIME),
                          PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                          DEFAULT_DIALOG_FONT );
        }
        return (MRESULT) TRUE;
      }
    case WM_CLOSE:
      writeVolumeToIni(chrIniFile,  ulVolume);
      break;
      /* The play control handling for starting, stopping etc. */
    case WM_COMMAND:
      {
        switch(SHORT1FROMMP(mp1))
          {
          case ID_MENUITEMMASTERVOLUME:
            {
              HOBJECT hObject;
              /* User selected master volume item from sys menu */
              if((hObject=WinQueryObject("<MMPM2_MMVOLUME>"))!=NULLHANDLE) {
                WinOpenObject(hObject,OPEN_DEFAULT,TRUE);
              }
              break;
            }
          default:
            break;
          }/* switch() */

      return (MRESULT)FALSE;
      }/* case */

      /*****************************************************/
      /* Draw the sliders.                                 */
      /*****************************************************/
    case WM_DRAWITEM:
      switch(SHORT1FROMMP(mp1))
        {
        case IDSL_MMPLAYERVOLUME:
          return drawSlider(hwnd, msg, mp1, mp2, SHORT1FROMMP(mp1));
        default:
          break;
        }              
      break;

      /*****************************************************/
      /* Control messages                                  */
      /*****************************************************/
    case WM_CONTROL:
      {
        SWP swp;
        
        switch(SHORT2FROMMP(mp1))
          {
          case BN_PAINT:
            /* Paint the custom controls (buttons, sliders ) */
            WinQueryWindowPos(hwnd,&swp);
            if(swp.fl & SWP_MINIMIZE)
              break;
            return paintControls(hwnd, msg, mp1, mp2);
              /*****************************************************/
              /* Handle position and volume changes.               */
              /*****************************************************/
            case SLN_SLIDERTRACK:
              {
                switch(SHORT1FROMMP(mp1))
                  {
                  case IDSL_MMPLAYERVOLUME:
                    {
                      ULONG ulValue;
                      char command[200];

                      ulValue=LONGFROMMP(mp2);

                      ulVolume=ulValue;
                      sprintf(command,"MMAUDIOVOLUME=%d", ulVolume);
                      sendCommandToObject(chrSourceName, command);
                      break;
                    }
                  default:
                    break;
                  }/* switch */
                return (MRESULT) 0;
                break;
              }
            case SLN_CHANGE:
              {
                switch(SHORT1FROMMP(mp1))
                  {
                  case IDSL_MMPLAYERVOLUME:
                    {
                      ULONG ulValue;
                      
                      ulValue=LONGFROMMP(mp2);
                      ulVolume=ulValue;
                      //     sprintf(chrCommand,"SET wave%ld AUDIO VOLUME %ld WAIT", ulPid, ulVolume);
                      //     mciSendString(chrCommand, retMsg, sizeof(retMsg), 0, 0);
                      break;
                    }
                  default:
                    break;
                  }/* switch */
                return (MRESULT)TRUE;
              }
          default:
            break;
          }/* switch */
        break;
      }/* WM_CONTROL */
    default:
      break;
    }/* switch */
  
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}


int readVolumeFromIni(char * iniFile, int iDefault)
{
  return IniRestoreInt(iniFile, INI_CLASSVOLUME_APP, INI_VOLUME_KEY, iDefault);
}

BOOL writeVolumeToIni(char * iniFile,  ULONG ulVolume)
{
  
  return IniSaveInt(iniFile, INI_CLASSVOLUME_APP, INI_VOLUME_KEY, ulVolume);
}

void sendCommandToObject(char* chrObject, char* command)
{
  static HOBJECT hObject;

  if(!hObject) {
    hObject=WinQueryObject(chrObject);
  }
  if(hObject!=NULLHANDLE)
    WinSetObjectData(hObject, command);
}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  char text[CCHMAXPATH];
  char title[CCHMAXPATH];
  short a;

  /* Create a copy of the args */
  /* argv[0]: progname
   * argv[1]: HWND object
   */

  numArgs=argc;

  strcpy(text,"");
  for(a=0;a<argc;a++)
    {
      params[a]=argv[a];
    }

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      /* Check if user started prog by hand */   
      if(argc!=NUMPARAMS ) {/* Not the right num of params */
        pmUsage();
      }
      else {
        /* Save source name */
        strcpy(chrSourceName, params[1]);
        //      if((chrPtrName=strrchr(chrSourceName, '\\'))!=NULLHANDLE)
        //    ++chrPtrName;
        //SysWriteToTrapLog("Name: %s\n", chrSourceName);
        /* Get our ressource dll */  
        RESSOURCEHANDLE=queryResModuleHandle(argv[0]);
        /* Get data from INI file */
        HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));
        ulVolume=readVolumeFromIni(chrIniFile, DEFAULT_VOLUME);

        /* Load pointer for slider */
        hptrArrowWE=WinLoadPointer(HWND_DESKTOP, BMP_RESSOURCEHANDLE /*_queryResModuleHandle()*/, IDPTR_ARROWWE);
        /* Register the slider window class */
        WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP), SLIDER_CLASS, privateSliderProc,0, SLADDBYTES);

        _loadBmps();

        if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, decodeStatusDialogProc, 
                       RESSOURCEHANDLE, IDDLG_CLASSVOLUME, 0) == DID_ERROR )
          {
            /*
              Text:   
              Title:  "Installation problem"                       
              */             
            messageBox( text, IDSTR_CONVERTDIALOGERROR , sizeof(text),
                        title, IDSTR_INSTALLERRORTITLE , sizeof(title),
                        RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);
            freeResHandle();
            WinDestroyMsgQueue( hmq );
            WinTerminate( hab );
            DosBeep(100,600);
            return( 1 );
          }
      }
      freeResHandle();
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}

