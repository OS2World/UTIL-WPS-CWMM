/*
 * This file is (C) Chris Wohlgemuth 2002
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
#define INCL_WIN
#define INCL_GPIBITMAPS
#include <os2.h>

#define INCL_MCIOS2
#include <os2me.h>
#include "except.h"
#include "mediafolderinc.h"


#define  MAIN_BMP_IDX              0
#define  CTRLIDX_POSSLIDER         1
#define  CTRLIDX_POSSLIDERARM      2
#define  CTRLIDX_VOLSLIDER         3
#define  CTRLIDX_VOLSLIDERARM      4
#define  CTRLIDX_VOLSLIDERARMSEL   5
#define  CTRLIDX_PLAYTIME          6
#define  CTRLIDX_TOPLEFT           7
#define  CTRLIDX_TOPRIGHT          8
#define  CTRLIDX_PLAYBUTTON        9
#define  CTRLIDX_PLAYBUTTONSEL     10
#define  CTRLIDX_STOPBUTTON        11
#define  CTRLIDX_STOPBUTTONSEL     12
#define  CTRLIDX_PAUSEBUTTON       13
#define  CTRLIDX_PAUSEBUTTONSEL    14
#define  CTRLIDX_SKIPBACKBUTTON    15
#define  CTRLIDX_SKIPBACKBUTTONSEL 16
#define  CTRLIDX_SKIPFWDBUTTON     17
#define  CTRLIDX_SKIPFWDBUTTONSEL  18
#define  CTRLIDX_BOTTOM            19
#define  CTRLIDX_CHECK             20
#define  CTRLIDX_UNCHECK           21
#define  CTRLIDX_CHECKSEL          22
#define  CTRLIDX_UNCHECKSEL        23

#define  CTRLIDX_RANDOMCHECK             24
#define  CTRLIDX_RANDOMUNCHECK           25
#define  CTRLIDX_RANDOMCHECKSEL          26
#define  CTRLIDX_RANDOMUNCHECKSEL        27

#define  NUM_CTRL_IDX              28


#define SHOWABOUTDIALOG_COMMAND   "SHOWABOUTDIALOG=1"
#define SETUP_SHOWABOUTDIALOG     "SHOWABOUTDIALOG"

/*** Private folder flags: ***/

/* Flag indicating from where a method was called */
#define MFLDR_CURACTION_ADDFOLDERVIEW3  0x00000001
/* Flag telling if folder is in compact view */
#define MFLDR_VIEW_COMPACT              0x00000002
/* The view is currently changing */
#define MFLDR_CHANGING_VIEW             0x00000004
/*** End of private folder flags ***/

#define IDHLP_MEDIAFOLDERMAIN  100


#pragma SOMAsDefault(on)
#include <som.hh>
#include <somcls.hh>
#pragma SOMNoMangling(on)
/*#include "progfolder.h"*/
#include "progfolder.hh"
#include "cwaudio.hh"

/* Forward declaration */
class CWMediaFolder;

#pragma SOMAsDefault(off)
#pragma SOMNonDTS(on)

typedef struct _MYVIEWITEM
{
  BOOL bIsInUseList;
  USEITEM useItem;
  VIEWITEM viewItem;
}MYVIEWITEM; 

typedef struct
{
  INT id;
  RECTL rclSource;
  HBITMAP hbmSource;
  RECTL rclDest;
}CONTROLINFO;

typedef struct
{
  BITMAPINFOHEADER bmpInfoHdr;
  HBITMAP hbm;
}LOADEDBITMAP;


/* The following is used for openThreadFunc(void *arg) when openeing a media folder and
   for storing information when calling cwPlayAudioFile() etc. from the player panel dialog of the folder. */
typedef struct _THREADPARAMS
{
  CWMediaFolder *thisPtr;
  HWND hwndFrame;
}THREADPARAMS;

/* This struct is used in the CWInsertTrack method to specify the REXX script
to be started */
typedef struct _INSERTTRACKPARMS
{
  WPObject* thisPtr;
  char chrRexxScript[CCHMAXPATH];
}INSERTTRACKPARMS; 

typedef  struct _MEDIAFLDRDATA
{
  ULONG ulTotalTime; /* Playtime of all folder tracks in secs */
  char chrPlayTimeFont[50];/* Fontnamelength is 31 chars. Some space for the pointsize added and some to be save.
                              Yeah, I know I'm lazy... */
  BOOL bProcessing;  /* Set to TRUE if a thread is running. this is checked in the container proc and the wait ptr. is set. */
  ULONG ulNumTracks;
  ULONG ulSize;
  BOOL bOpened;
  BYTE *pRandomArray; /* This array holds the already random played tracks. When played the index is set to '1'. */
  int iNumPlayed;     /* # of random played tracks */
  int iNumToPlay;
  HWND hwndPlayObject;/* Object window which handles the play commands so the WPS is not blocked. */  
}MEDIAFLDRDATA;
typedef   MEDIAFLDRDATA*   PMEDIAFLDRDATA;

typedef struct _DROPTHREADPARAMS
{
  CWMediaFolder *thisPtr;
  HWND hwndCnr;
  HWND hwndSource;
  ULONG ulNumObjects;
  POINTL ptl;
  WPObject* wpObject[1];
}DROPTHREADPARAMS;
typedef DROPTHREADPARAMS* PDROPTHREADPARAMS;

typedef struct _MFLDRCTRLDATA
{
  ULONG ulMnemonicPos;
  BOOL  bPreviousCheckState;
}MFLDRCTRLDATA;

class launchPad;

#pragma SOMAsDefault(on)

#ifndef somMD_WPFileSystem_wpRefresh
typedef BOOL   SOMLINK somTP_WPFileSystem_wpRefresh(WPFileSystem *somSelf,
		ULONG ulView,
		PVOID pReserved);
//#pragma linkage(somTP_WPFileSystem_wpRefresh, system)
typedef somTP_WPFileSystem_wpRefresh *somTD_WPFileSystem_wpRefresh;
#define somMD_WPFileSystem_wpRefresh "::WPFileSystem::wpRefresh"
#define WPFileSystem_wpRefresh(somSelf,ulView,pReserved) \
    (SOM_Resolve(somSelf, WPFileSystem, wpRefresh) \
	(somSelf,ulView,pReserved))
#ifndef SOMGD_wpRefresh
    #if (defined(_wpRefresh) || defined(__wpRefresh))
        #undef _wpRefresh
        #undef __wpRefresh
        #define SOMGD_wpRefresh 1
    #else
        #define _wpRefresh WPFileSystem_wpRefresh
    #endif /* _wpRefresh */
#endif /* SOMGD_wpRefresh */
#endif

class M_CWMediaFolder:public M_CWProgFolder
{
/* The name of of the new class */
#pragma SOMClassName(*,"M_CWMediaFolder")
/* Required SOM version */
#pragma SOMClassVersion(*,1,2)
#pragma SOMCallstyle(oidl)
public:
  virtual PSZ  wpclsQueryTitle();
  virtual void wpclsInitData();
  virtual	ULONG wpclsQueryIconData(PICONINFO pIconInfo);
  virtual	ULONG wpclsQueryIconDataN(PICONINFO pIconInfo, ULONG ulIconIndex);
  virtual void wpclsUnInitData();
  virtual BOOL wpclsCreateDefaultTemplates(WPObject * wpObject);
  virtual	ULONG wpclsQueryStyle();
  virtual BOOL wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary);

#pragma SOMReleaseOrder()
#pragma SOMIDLPass(*,"Implementation-End","dllname = mediafld;")
};

class CWMediaFolder:public CWProgFolder
{
#pragma SOMClassName(*,"CWMediaFolder")
#pragma SOMMetaClass(CWMediaFolder,"M_CWMediaFolder")
#pragma SOMClassVersion(CWMediaFolder,1,2)
#pragma SOMCallstyle(oidl)
private:
  /* 
     Added with V0.2.8 (07.09.2003)

     Private flags used by the folder to keep track of current actions.

     Tells the folder:
     MFLDR_CURACTION_ADDFOLDERVIEW3: currently processing the wpAddFolderView3Page() 
     MFLDR_VIEW_COMPACT:             folder is in compact view

   */
  ULONG ulPrivFlags;
  SWP swpUnshrink;
public:
  /* From M3UFolder*/
  WPObject* wpObjM3UFile;
  HWND hwndObject;

  WPObject* currentObject; /* Currently playing object */
  PMINIRECORDCORE mrcPlaying;
  MYVIEWITEM myViewItem; 

  HWND hwndTopMiddle;/* */
  HWND hwndBottom;
  HWND hwndMFldrFrame;
  int iWave;
  ULONG ulFlags;
  BOOL bPaused;
  ULONG ulVolume;
  ULONG ulStartPosition;
  ULONG ulTotalLength;/* Total length of track in ms */
  ULONG ulPos;/* Position in wave/track in secs */
  //char chrTBFlyFontName[CCHMAXPATH+1];
  //  RGB rgbTBFlyForeground;
  //  RGB rgbTBFlyBackground;
  //  BOOL bTBFlyOverEnabled;
  //  int iTBFlyOverDelay;
  PFNWP pfnwpFrame;
  USHORT usLastSelMenuItem;
  USHORT usWhichDisplay;
  MEDIAFLDRDATA* mfData;/* Allocated in wpInitData() */
  launchPad* lPad;
#if 0
  virtual BOOL wpMenuItemHelpSelected(ULONG MenuId);
  virtual ULONG wpFilterPopupMenu(ULONG ulFlags, HWND hwndCnr, BOOL fMultiSelect);
  virtual BOOL cwIsWaveFile(WPObject* wpObject);
  virtual ULONG cwSetWriteFlags(ULONG ulNewWriteFlags,ULONG ulMask);
  virtual ULONG cwQueryWriteFlags();
#endif
  virtual BOOL    wpSetupOnce(PSZ pSetupString);
  virtual BOOL    wpAddToContent(WPObject* Object);
  virtual BOOL    wpDeleteFromContent(WPObject* Object);
  virtual ULONG   wpAddFolderView1Page(HWND hwndNotebook);
  virtual ULONG   wpAddFolderView2Page(HWND hwndNotebook);
  virtual ULONG   wpAddFolderView3Page(HWND hwndNotebook);
  virtual ULONG   wpFilterPopupMenu(ULONG ulFlags, HWND hwndCnr, BOOL fMultiSelect);
  virtual HWND    wpOpen(HWND hwndCnr,ULONG ulView,ULONG ulParam);
  virtual BOOL    wpRestoreState(ULONG ulReserved);
  virtual BOOL    wpSaveState();
  virtual MRESULT wpDragOver(HWND hwndCnr,PDRAGINFO pDragInfo);
  virtual MRESULT wpDrop(HWND hwndCnr,PDRAGINFO pDragInfo,PDRAGITEM pDragItem);  
  virtual BOOL    wpAddSettingsPages(HWND hwndNotebook);
  virtual ULONG   wpInsertSettingsPage(HWND hwndNotebook, PPAGEINFO ppageinfo);
  virtual BOOL    wpMenuItemSelected(HWND hwndFrame,ULONG ulMenuId);
  virtual BOOL    wpModifyPopupMenu(HWND hwndMenu, HWND hwndCnr, ULONG ulPosition);
  virtual BOOL    wpSetup(PSZ pSetupString);
  virtual void    wpInitData();
  virtual void    wpUnInitData();
  virtual BOOL    wpRefresh(ULONG ulView, PVOID pReserved);
  // The following was introduced by CWProgFolder
  virtual BOOL    cwClose(HWND hwndFrame);
  // These are our new methods
  virtual ULONG   cwPlayAudioFile(HWND hwndFrame, ULONG ulWhich);
  virtual BOOL    cwStopAudioFile(HWND hwndFrame);
  virtual BOOL    cwPauseAudioFile(HWND hwndFrame);
  virtual BOOL    cwAddToolbarOptionPage(HWND hwndNotebook);
  virtual MMAudio* cwQueryCurrentlyPlayingObject();
  virtual void    cwSetStatusLineText(char * chrText);
  virtual BOOL    cwSeekAudioFile(LONG lPosSec);
  virtual void    cwPlayTimeCallback(HWND hwndFrame, LONG lPosSec);
  virtual ULONG   cwPlayRandomAudioFile(HWND hwndFrame);
  virtual BOOL    cwInsertAllTracks(char* chrM3UFile);
  virtual ULONG   cwQueryMFldrFlags();
  virtual BOOL    cwShrinkFolderToCompact(HWND hwndFrame);
  virtual BOOL    cwUnshrinkFolder(HWND hwndFrame);
#pragma SOMReleaseOrder(\
"cwPlayAudioFile",\
"cwStopAudioFile",\
"cwPauseAudioFile",\
"cwAddToolbarOptionPage",\
"cwQueryCurrentlyPlayingObject",\
"cwSetStatusLineText",\
"cwSeekAudioFile",\
"cwPlayTimeCallback",\
"cwPlayRandomAudioFile",\
"cwInsertAllTracks",\
"cwQueryMFldrFlags", \
"cwShrinkFolderToCompact",\
"cwUnshrinkFolder")

#if 0
  virtual MRESULT wpDrop(HWND hwndCnr,PDRAGINFO pDragInfo,PDRAGITEM pDragItem);
  virtual BOOL cwClose();
  virtual BOOL wpSetupOnce(PSZ pSetupString);
  virtual BOOL wpSetup(PSZ pSetupString);

#pragma SOMReleaseOrder(\
"cwIsWaveFile",\
"cwSetWriteFlags",\
"cwQueryWriteFlags",\
"cwEnableGrab",\
"cwQueryGrabEnabled",\
"cwEnableGrabControls",\
"cwEnableSelectControls",\
"cwFillTrackList",\
"cwSetStatusTime",\
"cwForceStatusUpdate",\
"cwEnableWrite",\
"cwQueryWriteEnabled",\
"cwEnableWriteControls",\
"cwSetStatusText",\
"cwWriteAudioTracks",\
"cwIsMp3File",\
"cwCreateContentsFile")
#endif
#pragma SOMIDLPass(CWMediaFolder,"Implementation-End","dllname = mediafld;")
};

/************/
#pragma SOMAsDefault(pop)

#pragma SOMNonDTS(pop)














