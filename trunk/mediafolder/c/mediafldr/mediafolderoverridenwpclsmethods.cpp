/*
 * This file is (C) Chris Wohlgemuth 1999-2003
 * 
 * It's part of the Audio/Data-CD-Creator distribution
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

#define INCL_DOSERRORS
#define INCL_DOSERRORS
#define INCL_GPIBITMAPS
#define INCL_REXXSAA

#include "mediafolder.hh"
#include "mediafolderres.h"

#pragma SOMAsDefault(pop)
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#include <wpclsmgr.hh> /* For wpModuleForClass() */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sys_funcs.h"

#pragma SOMAsDefault(on)


ULONG EXPENTRY rxCallPMFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);
ULONG EXPENTRY rxCallWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);
ULONG EXPENTRY rxCallCWWPSFunc(PSZ name, LONG argc, RXSTRING argv[], PSZ queuename,PRXSTRING retstring);

HMODULE queryModuleHandle(PSZ pszClassName);

char chrInstallDir[CCHMAXPATH];
HPOINTER hptrArrowWE=NULLHANDLE; /* Pointer for slider control */

HMTX hmtxFileName;

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
  {0,{0,0,590,70}, NULLHANDLE,{0}},/* main */
  //  {IDPB_POSBAR,{356, 17, 548,25},NULLHANDLE, { 0, 0, 191,10}},
  {IDSL_POSBAR,{353, 15, 551,28},NULLHANDLE, { 0, 0, 191,10}},
   {IDPB_SLIDERARM,{0,1,9,8},NULLHANDLE, {0,0,9,9} },
  //  {IDSL_VOLUME,{33, 5,158,14},NULLHANDLE, {0, 0, 126, 9} /* {10,18,93,13} */ },
  {IDSL_VOLUME,{32, 2,158,17},NULLHANDLE, {0, 0, 126, 9} /* {10,18,93,13} */ },
  {IDSL_VOLUMEARM,{0,0,9,9},NULLHANDLE, {0,0,9,9}},
  {IDSL_VOLUMEARM,{0,0,9,9},NULLHANDLE, {0,0,9,9}},
  //  {IDST_PLAYTIME, {351, 25, 551,52},NULLHANDLE, { 0, 0, 190, 27} },
  {IDST_PLAYTIME, {351, 29, 551,52},NULLHANDLE, { 0, 0, 190, 25} },
  {0,{0,0,20,72}, NULLHANDLE,{0}},/* Top left */
  {0,{0,0,20,72}, NULLHANDLE,{0}},/* Top right */
  /* Achtung destination verwendet fuer position */
  {IDPB_PLAY, {0, 0, 36, 36},NULLHANDLE, { 114, 18, 150, 54} },/* Play */
  {IDPB_PLAY, {0, 0, 36,36},NULLHANDLE, { 114, 18, 150, 54} }, /* Playsel */

  {IDPB_STOP, {0, 0, 36, 36},NULLHANDLE, { 40, 18, 76, 54} },/* Stop */
  {IDPB_STOP, {0, 0, 36,36},NULLHANDLE, { 40, 18, 76, 54} }, /* Stop sel */
  {IDPB_PAUSE, {0, 0, 36, 36},NULLHANDLE, { 77, 18, 112, 54} },/* Pause */
  {IDPB_PAUSE, {0, 0, 36,36},NULLHANDLE, { 77, 18, 112, 54} }, /* Pause sel */
  {IDPB_SKIPBACK, {0, 0, 36, 36},NULLHANDLE, { 3, 18, 39, 54} },/* Skip back */
  {IDPB_SKIPBACK, {0, 0, 36,36},NULLHANDLE, { 3, 18, 39, 54} }, /* Skip back sel */
  {IDPB_SKIPFORWARD, {0, 0, 36, 36},NULLHANDLE, { 151, 18, 187, 54} },/* Skip fwd */
  {IDPB_SKIPFORWARD, {0, 0, 36,36},NULLHANDLE, { 151, 18, 187, 54} }, /* Skip fwd sel */
  {0,{0,0,300,20}, NULLHANDLE,{0}},/* Bottom */

  //   {IDCB_REPEAT, {0, 0, 13,13},NULLHANDLE, { 197, 24, 233, 60} }, /* Placeholder for checkbutton */

  {IDCB_REPEAT, {0, 0, 13,13},NULLHANDLE, { 197, 36, 233, 60} }, /* Placeholder for checkbutton */
  {IDCB_REPEAT, {0, 0, 13,13},NULLHANDLE, { 197, 36, 233, 60} }, /* Placeholder for checkbutton */
  {IDCB_REPEAT, {0, 0, 13,13},NULLHANDLE, { 197, 36, 233, 60} }, /* Placeholder for checkbutton selected */
  {IDCB_REPEAT, {0, 0, 13,13},NULLHANDLE, { 197, 36, 233, 60} }, /* Placeholder for checkbutton selected */

  {IDCB_RANDOM, {0, 0, 13,13},NULLHANDLE, { 197, 10, 233, 60} }, /* Placeholder for checkbutton */
  {IDCB_RANDOM, {0, 0, 13,13},NULLHANDLE, { 197, 10, 233, 60} }, /* Placeholder for checkbutton */
  {IDCB_RANDOM, {0, 0, 13,13},NULLHANDLE, { 197, 10, 233, 60} }, /* Placeholder for checkbutton selected */
  {IDCB_RANDOM, {0, 0, 13,13},NULLHANDLE, { 197, 10, 233, 60} } /* Placeholder for checkbutton selected */

};


void errorResource();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG cwRequestMutex(HMTX  hmtxBMP, ULONG ulTimeOut);
ULONG cwReleaseMutex(HMTX  hmtxBMP);
ULONG cwCreateMutex(HMTX * hmtxBMP);
ULONG cwCloseMutex(HMTX  hmtxBMP);

MRESULT EXPENTRY topCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY rightCtrlDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) ;
MRESULT EXPENTRY privateSliderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
HMODULE queryResModuleHandle(void);

static void _loadBmps()
{
  HPS hps;
  BITMAPINFOHEADER bmpInfoHdr;
  HMODULE hResource;

  if(allBMPs[MAIN_BMP_IDX].hbm)
    return;

  // hResource=queryResModuleHandle();
  hResource=queryModuleHandle("M_CWMediaFolder");

  hps=WinGetPS(HWND_DESKTOP);
  
  /* The main BMP */
  allBMPs[MAIN_BMP_IDX].hbm=GpiLoadBitmap(hps, hResource, IDBMP_TOPMIDDLE, 0, 0);
  GpiQueryBitmapParameters(allBMPs[MAIN_BMP_IDX].hbm, &allBMPs[MAIN_BMP_IDX].bmpInfoHdr);

  /* Top right bmp */
  allBMPs[CTRLIDX_TOPRIGHT].hbm=GpiLoadBitmap(hps, hResource, IDBMP_TOPRIGHT, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_TOPRIGHT].hbm, &allBMPs[CTRLIDX_TOPRIGHT].bmpInfoHdr);

  /* Top left bmp */
  allBMPs[CTRLIDX_TOPLEFT].hbm=GpiLoadBitmap(hps, hResource, IDBMP_TOPLEFT, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_TOPLEFT].hbm, &allBMPs[CTRLIDX_TOPLEFT].bmpInfoHdr);

  /* Bottom bmp */
  allBMPs[CTRLIDX_BOTTOM].hbm=GpiLoadBitmap(hps, hResource, IDBMP_BOTTOM, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_BOTTOM].hbm, &allBMPs[CTRLIDX_BOTTOM].bmpInfoHdr);

  /* Positionslider bg */
  ciControls[CTRLIDX_POSSLIDER].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Positionslider arm */
  allBMPs[CTRLIDX_POSSLIDERARM].hbm=GpiLoadBitmap(hps, hResource, IDBMP_POSSLIDERARM, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_POSSLIDERARM].hbm, &allBMPs[CTRLIDX_POSSLIDERARM].bmpInfoHdr);
  ciControls[CTRLIDX_POSSLIDERARM].hbmSource=allBMPs[CTRLIDX_POSSLIDERARM].hbm;

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

  /* Playtime text field */
  ciControls[CTRLIDX_PLAYTIME].hbmSource=allBMPs[MAIN_BMP_IDX].hbm;

  /* Skip back */
  allBMPs[CTRLIDX_SKIPBACKBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_SKIPBACK, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_SKIPBACKBUTTON].hbm, &allBMPs[CTRLIDX_SKIPBACKBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_SKIPBACKBUTTON].hbmSource=allBMPs[CTRLIDX_SKIPBACKBUTTON].hbm;

  /* Skip back sel */
  allBMPs[CTRLIDX_SKIPBACKBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_SKIPBACKSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_SKIPBACKBUTTONSEL].hbm, &allBMPs[CTRLIDX_SKIPBACKBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_SKIPBACKBUTTONSEL].hbmSource=allBMPs[CTRLIDX_SKIPBACKBUTTONSEL].hbm;

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

  /* Skip forward */
  allBMPs[CTRLIDX_SKIPFWDBUTTON].hbm=GpiLoadBitmap(hps, hResource, IDBMP_SKIPFORWARD, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_SKIPFWDBUTTON].hbm, &allBMPs[CTRLIDX_SKIPFWDBUTTON].bmpInfoHdr);
  ciControls[CTRLIDX_SKIPFWDBUTTON].hbmSource=allBMPs[CTRLIDX_SKIPFWDBUTTON].hbm;

  /* Skip forward sel */
  allBMPs[CTRLIDX_SKIPFWDBUTTONSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_SKIPFORWARDSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_SKIPFWDBUTTONSEL].hbm, &allBMPs[CTRLIDX_SKIPFWDBUTTONSEL].bmpInfoHdr);
  ciControls[CTRLIDX_SKIPFWDBUTTONSEL].hbmSource=allBMPs[CTRLIDX_SKIPFWDBUTTONSEL].hbm;

  /* Checkbutton checked */
  allBMPs[CTRLIDX_CHECK].hbm=GpiLoadBitmap(hps, hResource, IDBMP_CHECK, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_CHECK].hbm, &allBMPs[CTRLIDX_CHECK].bmpInfoHdr);
  ciControls[CTRLIDX_CHECK].hbmSource=allBMPs[CTRLIDX_CHECK].hbm;
  /* Adjust source pos */
  ciControls[CTRLIDX_CHECK].rclSource.yBottom=0;
  ciControls[CTRLIDX_CHECK].rclSource.yTop=allBMPs[CTRLIDX_CHECK].bmpInfoHdr.cy;
  ciControls[CTRLIDX_CHECK].rclSource.xLeft=0;
  ciControls[CTRLIDX_CHECK].rclSource.xRight=allBMPs[CTRLIDX_CHECK].bmpInfoHdr.cx;

  /* Checkbutton unchecked */
  allBMPs[CTRLIDX_UNCHECK].hbm=GpiLoadBitmap(hps, hResource, IDBMP_UNCHECK, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_UNCHECK].hbm, &allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr);
  ciControls[CTRLIDX_UNCHECK].hbmSource=allBMPs[CTRLIDX_UNCHECK].hbm;
  ciControls[CTRLIDX_UNCHECK].rclSource.yBottom=0;
  ciControls[CTRLIDX_UNCHECK].rclSource.yTop=allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr.cy;
  ciControls[CTRLIDX_UNCHECK].rclSource.xLeft=0;
  ciControls[CTRLIDX_UNCHECK].rclSource.xRight=allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr.cx;

  /* Checkbutton checked selected */
  allBMPs[CTRLIDX_CHECKSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_CHECKSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_CHECKSEL].hbm, &allBMPs[CTRLIDX_CHECKSEL].bmpInfoHdr);
  ciControls[CTRLIDX_CHECKSEL].hbmSource=allBMPs[CTRLIDX_CHECKSEL].hbm;
  /* Adjust source pos */
  ciControls[CTRLIDX_CHECKSEL].rclSource.yBottom=0;
  ciControls[CTRLIDX_CHECKSEL].rclSource.yTop=allBMPs[CTRLIDX_CHECKSEL].bmpInfoHdr.cy;
  ciControls[CTRLIDX_CHECKSEL].rclSource.xLeft=0;
  ciControls[CTRLIDX_CHECKSEL].rclSource.xRight=allBMPs[CTRLIDX_CHECKSEL].bmpInfoHdr.cx;

  /* Checkbutton unchecked selected */
  allBMPs[CTRLIDX_UNCHECKSEL].hbm=GpiLoadBitmap(hps, hResource, IDBMP_UNCHECKSEL, 0, 0);
  GpiQueryBitmapParameters(allBMPs[CTRLIDX_UNCHECKSEL].hbm, &allBMPs[CTRLIDX_UNCHECKSEL].bmpInfoHdr);
  ciControls[CTRLIDX_UNCHECKSEL].hbmSource=allBMPs[CTRLIDX_UNCHECKSEL].hbm;
  ciControls[CTRLIDX_UNCHECKSEL].rclSource.yBottom=0;
  ciControls[CTRLIDX_UNCHECKSEL].rclSource.yTop=allBMPs[CTRLIDX_UNCHECKSEL].bmpInfoHdr.cy;
  ciControls[CTRLIDX_UNCHECKSEL].rclSource.xLeft=0;
  ciControls[CTRLIDX_UNCHECKSEL].rclSource.xRight=allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr.cx;
  /*********************/

  /* Checkbutton checked */
  //  allBMPs[CTRLIDX_CHECK].hbm=GpiLoadBitmap(hps, hResource, IDBMP_CHECK, 0, 0);
  //  GpiQueryBitmapParameters(allBMPs[CTRLIDX_CHECK].hbm, &allBMPs[CTRLIDX_CHECK].bmpInfoHdr);
  ciControls[CTRLIDX_RANDOMCHECK].hbmSource=allBMPs[CTRLIDX_CHECK].hbm;
  /* Adjust source pos */
  ciControls[CTRLIDX_RANDOMCHECK].rclSource.yBottom=0;
  ciControls[CTRLIDX_RANDOMCHECK].rclSource.yTop=allBMPs[CTRLIDX_CHECK].bmpInfoHdr.cy;
  ciControls[CTRLIDX_RANDOMCHECK].rclSource.xLeft=0;
  ciControls[CTRLIDX_RANDOMCHECK].rclSource.xRight=allBMPs[CTRLIDX_CHECK].bmpInfoHdr.cx;

  /* Checkbutton unchecked */
  // allBMPs[CTRLIDX_UNCHECK].hbm=GpiLoadBitmap(hps, hResource, IDBMP_UNCHECK, 0, 0);
  //  GpiQueryBitmapParameters(allBMPs[CTRLIDX_UNCHECK].hbm, &allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr);
  ciControls[CTRLIDX_RANDOMUNCHECK].hbmSource=allBMPs[CTRLIDX_UNCHECK].hbm;
  ciControls[CTRLIDX_RANDOMUNCHECK].rclSource.yBottom=0;
  ciControls[CTRLIDX_RANDOMUNCHECK].rclSource.yTop=allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr.cy;
  ciControls[CTRLIDX_RANDOMUNCHECK].rclSource.xLeft=0;
  ciControls[CTRLIDX_RANDOMUNCHECK].rclSource.xRight=allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr.cx;

  /* Checkbutton checked selected */
  //  allBMPs[CTRLIDX_RANDOMCHECKSEL].hbm=allBMPs[CTRLIDX_CHECKSEL].hbm;
  //  GpiQueryBitmapParameters(allBMPs[CTRLIDX_CHECKSEL].hbm, &allBMPs[CTRLIDX_RANDOMCHECKSEL].bmpInfoHdr);
  ciControls[CTRLIDX_RANDOMCHECKSEL].hbmSource=allBMPs[CTRLIDX_CHECKSEL].hbm;
  /* Adjust source pos */
  ciControls[CTRLIDX_RANDOMCHECKSEL].rclSource.yBottom=0;
  ciControls[CTRLIDX_RANDOMCHECKSEL].rclSource.yTop=allBMPs[CTRLIDX_CHECKSEL].bmpInfoHdr.cy;
  ciControls[CTRLIDX_RANDOMCHECKSEL].rclSource.xLeft=0;
  ciControls[CTRLIDX_RANDOMCHECKSEL].rclSource.xRight=allBMPs[CTRLIDX_CHECKSEL].bmpInfoHdr.cx;


  /* Random checkbutton unchecked selected */
  //  allBMPs[CTRLIDX_RANDOMUNCHECKSEL].hbm=allBMPs[CTRLIDX_UNCHECKSEL].hbm;
  //  GpiQueryBitmapParameters(allBMPs[CTRLIDX_UNCHECKSEL].hbm, &allBMPs[CTRLIDX_RANDOMUNCHECKSEL].bmpInfoHdr);
  ciControls[CTRLIDX_RANDOMUNCHECKSEL].hbmSource=allBMPs[CTRLIDX_UNCHECKSEL].hbm;
  ciControls[CTRLIDX_RANDOMUNCHECKSEL].rclSource.yBottom=0;
  ciControls[CTRLIDX_RANDOMUNCHECKSEL].rclSource.yTop=allBMPs[CTRLIDX_UNCHECKSEL].bmpInfoHdr.cy;
  ciControls[CTRLIDX_RANDOMUNCHECKSEL].rclSource.xLeft=0;
  ciControls[CTRLIDX_RANDOMUNCHECK].rclSource.xRight=allBMPs[CTRLIDX_UNCHECK].bmpInfoHdr.cx;


  WinReleasePS(hps);
}
APIRET APIENTRY DosQueryModFromEIP( HMODULE *phMod,
                                    ULONG *pObjNum,
                                    ULONG BuffLen,
                                    PCHAR pBuff,
                                    ULONG *pOffset,
                                    PVOID Address );

/* This function returns the module handle of our class-dll */
HMODULE queryModuleHandle(PSZ pszClassName)
{
  static HMODULE hmod=0;

  if(!hmod) {
    //  PSZ pathname;
    ULONG ulObj, ulBufLen, ulOffset;
    char thePath[CCHMAXPATH];

    if(DosQueryModFromEIP( &hmod,
                           &ulObj,
                           CCHMAXPATH,
                           thePath,
                           &ulOffset,
                           queryModuleHandle )!=NO_ERROR) {
      SysWriteToTrapLog("Can't get module handle for class DLL!\n");
      hmod=0;
      return NULLHANDLE; /* Error */
    }

#if 0
    /*
      Sometimes the returned name isn't correct. To prevent loading a bunch of classes just
      to get the module handle the following is replaced by the undocumented DosQueryModFromEIP()
      call. Maybe this even fixes some deadlocks people had during WPS startup with the classes
      because now the WPS isn't urged to load all classes at once.
     */
    pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
      ->wpModuleForClass(pszClassName);
    if(DosQueryModuleHandle(pathname,&hmod)) {
      pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
        ->wpModuleForClass("M_CWMediaFolder");
      if(DosQueryModuleHandle(pathname,&hmod)) {
        pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
          ->wpModuleForClass("CWMediaFolder");
        if(DosQueryModuleHandle(pathname,&hmod)) {
          pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
            ->wpModuleForClass("M_CWAudioShadow");
          if(DosQueryModuleHandle(pathname,&hmod)) {
            pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
              ->wpModuleForClass("CWAudioShadow");           
            if(DosQueryModuleHandle(pathname,&hmod)) {
              pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
                ->wpModuleForClass("M_CWCDFolder");           
              if(DosQueryModuleHandle(pathname,&hmod)) {
                pathname=((WPClassManager*)SOMClassMgrObject)  //Query Pathname of class file
                  ->wpModuleForClass("CWCDFolder");           
                if(DosQueryModuleHandle(pathname,&hmod)) {
                  hmod=0;
                  return NULLHANDLE; /* Error */
                }
              }
            }
          }
        }
      }
    }
#endif

    /* Get install dir. */
    if(!DosQueryModuleName(hmod,sizeof(chrInstallDir),chrInstallDir)) {
      char *found;
      if((found=strrchr(chrInstallDir,'\\'))!=NULLHANDLE) {
        *found=0;
        if((found=strrchr(chrInstallDir,'\\'))!=NULLHANDLE)
          *found=0;
        
        /* Save the dir for config files. */
        //strcpy( chrConfigDir, chrInstallDir);
      }
    }
  }/* end of if(!hmod) */
  return hmod;
}

/* This function returns the module handle of our ressource DLL */
HMODULE queryResModuleHandle(void)
{
  static HMODULE hmod=0;

  if(!hmod) {
    char path[CCHMAXPATH];  
    char buf[CCHMAXPATH];
    char* found;
    APIRET rc;
    
    /* Get the language code of our system and load the  
       resource DLL with the right language */
    do {
      static char chrLang[]="en_EN";
      PCSZ pszLang=chrLang;
      char *chrPtr;
      
      /* Get Language var */
      if(NO_ERROR!=DosScanEnv("LANG", &pszLang))
        {
          /* No language var set */
          pszLang=chrLang; /* Be safe. I don't know if a failing DosScanEnv()
                              may change the contents of the pointer. */
        }

      // SysWriteToTrapLog("LANG: %s\n", pszLang);

      /* Skip leading spaces */
      chrPtr=(char*)pszLang;
      while(*chrPtr==' ')
        chrPtr++;
      
      /* Check if value seems to be valid. The var must be something like xx_XX thus length is 5 */
      if(strlen(chrPtr)<5)
        break;
      
      if(DosQueryModuleName(queryModuleHandle("M_CWMediaFolder"),sizeof(path),path)!=NO_ERROR)
        break;
      
      found=strrchr(path,'\\');
      if(!found)
        break;
      *found=0;
      
      /* Extract the first two chars */                
      sprintf(buf, RESDLLNAME, chrPtr[0], chrPtr[1]);
      strcat(path,buf);

      if(DosLoadModule(buf,sizeof(buf),path, &hmod)==NO_ERROR)
        break;
      
      /* NLS DLL not found. Try to load default */
      found=strrchr(path,'\\');
      if(!found)
        break;
      
      *found=0;
      sprintf(buf, DEFRESDLLNAME);
      strcat(path,buf);

      rc=DosLoadModule(buf,sizeof(buf),path, &hmod);
      if(rc==NO_ERROR)
        break;
      hmod=NULLHANDLE;
    }while(FALSE);
  }
  return hmod;
}


BOOL _queryHelpLibrary(char * chrBuffer, int iSize)
{
  char chrBuf[CCHMAXPATH];

  queryModuleHandle("M_CWMediaFolder");
  sprintf(chrBuf,"%s\\bin\\mediafld.hlp",chrInstallDir);
  strncpy(chrBuffer, chrBuf, iSize);
  chrBuffer[iSize-1]=0;
  return TRUE;
}


BOOL M_CWMediaFolder::wpclsQueryDefaultHelp(PULONG HelpPanelId, PSZ HelpLibrary)
{
  if(HelpLibrary)
    _queryHelpLibrary(HelpLibrary, CCHMAXPATH);

  //SysWriteToTrapLog("%s\n", HelpLibrary);
  if(HelpPanelId)
    *HelpPanelId= IDHLP_MEDIAFOLDERMAIN;
	
  return TRUE;
}

/* Overriden function which returns our class name */
PSZ M_CWMediaFolder::wpclsQueryTitle()
{
  static char chrTitle[40];

  if(getMessage(chrTitle, IDSTR_TITLEMEDIAFOLDER,sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
    return chrTitle;
  /* Fall back in case of error */
  return "Media-Player";
}

/* Overriden function which returns the custom icon */
ULONG M_CWMediaFolder::wpclsQueryIconData(PICONINFO pIconInfo)
{
  //  HMODULE hResource=queryResModuleHandle();
  /* The icon is bound to the class DLL not the language DLL */
  HMODULE hResource=queryModuleHandle("M_CWMediaFolder");

  if(!hResource)
    return M_CWProgFolder::wpclsQueryIconData(pIconInfo);

  if (pIconInfo)   {
    pIconInfo->fFormat = ICON_RESOURCE;
    pIconInfo->hmod    = hResource;
    pIconInfo->resid   = IDICON_FOLDER1;
  } /* endif */

  return ( sizeof(ICONINFO) );
}

ULONG M_CWMediaFolder::wpclsQueryIconDataN(PICONINFO pIconInfo, ULONG ulIconIndex)
{
  //HMODULE hResource=queryResModuleHandle();
  /* The icon is bound to the class DLL not the language DLL */
  HMODULE hResource=queryModuleHandle("M_CWMediaFolder");
  if(!hResource)
    return M_CWProgFolder::wpclsQueryIconDataN(pIconInfo, ulIconIndex);

  if (pIconInfo)   {
    pIconInfo->fFormat = ICON_RESOURCE;
    pIconInfo->hmod    = hResource;
    pIconInfo->resid   = IDICON_FOLDER2;
  } /* endif */
  return ( sizeof(ICONINFO) );
}

void M_CWMediaFolder::wpclsInitData()
{
  COUNTRYCODE country= {0};
  COUNTRYINFO countryInfo= {0};
  ULONG ulInfoLen=0;
  APIRET rc;
  char path[CCHMAXPATH];  
  char* found;
  char buf[CCHMAXPATH];
  HATOMTBL hatSystem;

  cwCreateMutex(&hmtxFileName);

  /* Call parent */
  M_CWProgFolder::wpclsInitData();

  /* Get slider cursor */
  if(!hptrArrowWE)
    hptrArrowWE=WinLoadPointer(HWND_DESKTOP,  queryModuleHandle("M_CWMediaFolder"), IDPTR_ARROWWE);
  /* Register the slider window class */
  WinRegisterClass(WinQueryAnchorBlock(HWND_DESKTOP), SLIDER_CLASS, privateSliderProc,0, SLADDBYTES);

  /* Register media folder REXX functions */
  RexxRegisterFunctionExe("MFldrCallFunc", (PFN)rxCallCWWPSFunc);

  _loadBmps();

}

void M_CWMediaFolder::wpclsUnInitData()
{
  /* Don't deregister the function here. If doing it the function
     may be unavailable if the class object goes dormant. */
  /* RexxDeregisterFunction("MFldrCallFunc"); */

  cwCloseMutex( hmtxFileName);
  M_CWProgFolder::wpclsUnInitData();
}


ULONG M_CWMediaFolder::wpclsQueryStyle()
{
  /* CWProgFolder returns *_NEVERTEMPLATE but we want a template */

  /* We don't want an automatic template because we create it during
     installation because we need one with an ID so we can create a shadow
     without problems. So we return CLSSTYLE_DONTTEMPLATE. 
     Another way would be an override of wpclsCreateDefaultTemplate()
     but this works, too, and we have to override this method anyway. We must
     disable CLSSTYLE_NEVERTEMPLATE or 'Create another' and the template checkbox
     will be lost. */
  return (M_CWProgFolder::wpclsQueryStyle()&~CLSSTYLE_NEVERTEMPLATE)|CLSSTYLE_DONTTEMPLATE;
}

void writeLog(char* logFile, char* logText);

static void createMediaFolderShadow(M_WPObject * m_wpObject)
{
  char chrSetup[CCHMAXPATH];

  /* Get special settings from the INI file */
  if(PrfQueryProfileInt(HINI_USERPROFILE, APPKEY_CWMM, MFLDR_NO_TEMPLATE_SHADOW, 0))
    return;
 
  if(!WinQueryObject(ID_MEDIALDRTMPL_SHADOW))
    {
      sprintf(chrSetup,"SHADOWID=%s;OBJECTID=%s;", ID_MEDIAFLDRTEMPLATE,ID_MEDIALDRTMPL_SHADOW);
      WinCreateObject("WPShadow", m_wpObject->wpclsQueryTitle(), chrSetup, ID_MMFOLDER, CO_UPDATEIFEXISTS);
    }
};

BOOL M_CWMediaFolder::wpclsCreateDefaultTemplates(WPObject * wpObject)
{
  HOBJECT hObject;
  WPObject * wpTemplate;
  char chrSetup[CCHMAXPATH*2];
  BOOL  rc;

#ifdef WRITE_TEMPLATE_LOG
  writeLog( "template.log", "In wpclsCreateDefaultTemplates()\n");
  rc=FALSE;
  TRY_LOUD(TEMPLATE) {
    do {

      if((hObject=WinQueryObject(ID_MEDIAFLDRTEMPLATE))!=NULLHANDLE) {
        writeLog( "template.log", "1 WinQueryObject() gibt hObject zurck. Template bereits vorhanden.\n");
        /* There's an object with the ID. Check if it's the right template */
        if((wpTemplate=wpclsQueryObject(hObject))==NULLHANDLE) {
          /* A handle but no object?! Shouldn't happen, fall back to default. */
          writeLog( "template.log", "2 wpTemplate zu hObject kann nicht gefunden werden!!! -> break\n");
          break;
        }

        if(somIsObj(wpTemplate)) {
          writeLog( "template.log", "3 wpTemplate ist ein SOMObject. Test, ob ein CWAudio-Objekt...\n");
          if(wpTemplate->somIsInstanceOf(this)) {
            ULONG ulStyle;

            writeLog( "template.log", "4 wpTemplate ist ein CWAudio-Objekt.\n");

            /* It's an instance of our class. Make sure it's a template. */
            ulStyle=wpTemplate->wpQueryStyle();
            writeLog( "template.log", "5 ulStyle=wpTemplate->wpQueryStyle(); done\n");
            wpTemplate->wpSetStyle(ulStyle|OBJSTYLE_TEMPLATE);
            writeLog( "template.log", "6 wpTemplate->wpSetStyle(ulStyle|OBJSTYLE_TEMPLATE); done\n");
            rc=TRUE;
            break;
            /* Ok we have a template */
          }
          else
            writeLog( "template.log", "7 Nein, kein CWAudio-Objekt. Entfernen...\n");
          /* The object isn't our template. Probably left over from previous installation
             so remove it. */
          ((WPFileSystem*)wpTemplate)->wpSetAttr(FILE_NORMAL); /* Remove system, readonly, hidden flags */
          writeLog( "template.log", "8 ((WPFileSystem*)wpTemplate)->wpSetAttr(FILE_NORMAL); done\n");
          wpTemplate->wpSetStyle(0);/* Remove styles so deleting is possible */
          writeLog( "template.log", "9 wpTemplate->wpSetStyle(0); done\n");
          wpTemplate->wpDelete(NULL);
          writeLog( "template.log", "10 wpTemplate->wpDelete(NULL); done\n");
        }/* somIsObj() */
        else
          writeLog( "template.log", "11 wpTemplate ist kein SOMObject!!?\n");
      }/* if((hObject=WinQueryObject(ID_AUDIOTEMPLATE))!=NULLHANDLE) */
      else
        writeLog( "template.log", "12 WinQueryObject() gibt NULLHANDLE zurck. Kein Template vorhanden.\n");

      wpTemplate=wpObject;
      /* Create a new template */
      /* Get Multimedia template folder */
      writeLog( "template.log", "13 Neues Template erstellen...\n");
      if((hObject=WinQueryObject(ID_MMTEMPLATEFOLDER))!=NULLHANDLE) {
        writeLog( "template.log", "14 Multimediaordner gefunden...\n");
        /* There's an object with the ID. */
        if((wpTemplate=wpclsQueryObject(hObject))==NULLHANDLE) {
          writeLog( "template.log", "15 Objekt-Ptr. zu Multimediaordner nicht gefunden!!\n");
          /* A handle but no object?! Shouldn't happen, fall back to default. */
          wpTemplate=wpObject;
        }
        writeLog( "template.log", "15b Objekt-Ptr. zu Multimediaordner gefunden!!\n");
      }
      else
        writeLog( "template.log", "16 Multimediaordner nicht gefunden. Benutze Schablonen-Ordner\n");

      sprintf(chrSetup,
              "TEMPLATE=YES;OBJECTID=%s", ID_MEDIAFLDRTEMPLATE);
      writeLog( "template.log", "16b erstelle Template...\n");
      if((wpTemplate=wpclsNew(MEDIAFLDR_TEMPLATE_NAME, chrSetup, (WPFolder*)wpTemplate, FALSE))!=NULLHANDLE) {
        writeLog( "template.log", "17 wpTemplate=wpclsNew() war erfogreich.\n");
        sprintf(chrSetup,
                //                "DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;");
                "DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;DETAILSTODISPLAY=0,1,4,12,13,14,15,16,17,18,19,20,21;SORTBYATTR=6,24,25,26,27,28,29,30,31,32,33");
        if(somIsObj(wpTemplate)) {
          writeLog( "template.log", "18 wpTemplate ist ein SOMObject.\n");
          wpTemplate->wpSetup(chrSetup);
          writeLog( "template.log", "19 wpTemplate->wpSetup(chrSetup); done\n");
        }
        rc=TRUE;
      }
      writeLog( "template.log", "20 Verlasse while()\n");
    }while(FALSE);
  }
  CATCH(TEMPLATE)
    {
    } END_CATCH;
    if(rc)
      writeLog( "template.log", "21 Verlasse wpclsCreateDefaultTemplates() mit rc=TRUE\n\n");
    else
      writeLog( "template.log", "21 Verlasse wpclsCreateDefaultTemplates() mit rc=FALSE\n\n");
    return rc;
    /* Above is for debugging only */
#else

  rc=FALSE;
  TRY_LOUD(TEMPLATE) {
    do {
      if((hObject=WinQueryObject(ID_MEDIAFLDRTEMPLATE))!=NULLHANDLE) {
        /* There's an object with the ID. Check if it's the right template */
        if((wpTemplate=wpclsQueryObject(hObject))==NULLHANDLE) {
          /* A handle but no object?! Shouldn't happen, fall back to default. */
          break;
        }

        if(somIsObj(wpTemplate)) {
          if(wpTemplate->somIsInstanceOf(this)) {
            ULONG ulStyle;

            /* It's an instance of our class. Make sure it's a template. */
            ulStyle=wpTemplate->wpQueryStyle();
            wpTemplate->wpSetStyle(ulStyle|OBJSTYLE_TEMPLATE);
            rc=TRUE;
            /* Now create a shadow in the MM folder. */
            createMediaFolderShadow(this);
#if 0
            if(!WinQueryObject(ID_MEDIALDRTMPL_SHADOW))
              {
                sprintf(chrSetup,"SHADOWID=%s;OBJECTID=%s;", ID_MEDIAFLDRTEMPLATE,ID_MEDIALDRTMPL_SHADOW);
                WinCreateObject("WPShadow", wpclsQueryTitle(), chrSetup, ID_MMFOLDER, CO_UPDATEIFEXISTS);
              }
#endif
            break;
            /* Ok we have a template */
          }

          /* The object isn't our template. Probably left over from previous installation
             so remove it. */
          ((WPFileSystem*)wpTemplate)->wpSetAttr(FILE_NORMAL); /* Remove system, readonly, hidden flags */
          wpTemplate->wpSetStyle(0);/* Remove styles so deleting is possible */
          wpTemplate->wpDelete(NULL);
        }/* somIsObj() */
      }/* if((hObject=WinQueryObject(ID_AUDIOTEMPLATE))!=NULLHANDLE) */

      wpTemplate=wpObject;
      /* Create a new template */
      /* Get Multimedia template folder */
      if((hObject=WinQueryObject(ID_MMTEMPLATEFOLDER))!=NULLHANDLE) {
        /* There's an object with the ID. */
        if((wpTemplate=wpclsQueryObject(hObject))==NULLHANDLE) {
          /* A handle but no object?! Shouldn't happen, fall back to default. */
          wpTemplate=wpObject;
        }
      }

      sprintf(chrSetup,
              "TEMPLATE=YES;OBJECTID=%s", ID_MEDIAFLDRTEMPLATE);
      // if((wpTemplate=wpclsNew(MEDIAFLDR_TEMPLATE_NAME, chrSetup, (WPFolder*)wpTemplate, FALSE))!=NULLHANDLE) {
      if((wpTemplate=wpclsNew(wpclsQueryTitle(), chrSetup, (WPFolder*)wpTemplate, FALSE))!=NULLHANDLE) {
        sprintf(chrSetup,
                //                "DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;");
                "DEFAULTVIEW=DETAILS;DETAILSCLASS=CWAudioShadow;SORTCLASS=CWAudioShadow;DETAILSTODISPLAY=0,1,4,12,13,14,15,16,17,18,19,20,21;SORTBYATTR=6,24,25,26,27,28,29,30,31,32,33");
        if(somIsObj(wpTemplate)) {
          wpTemplate->wpSetup(chrSetup);
          /* Now create a shadow in the MM folder. */
          createMediaFolderShadow(this);
#if 0
          if(!WinQueryObject(ID_MEDIALDRTMPL_SHADOW))
            {
              sprintf(chrSetup,"SHADOWID=%s;OBJECTID=%s;", ID_MEDIAFLDRTEMPLATE,ID_MEDIALDRTMPL_SHADOW);
              WinCreateObject("WPShadow", wpclsQueryTitle(), chrSetup, ID_MMFOLDER, CO_UPDATEIFEXISTS);
            }
#endif
        }
        rc=TRUE;
      }
    }while(FALSE);
  }
  CATCH(TEMPLATE)
    {
    } END_CATCH;

    return rc;
#endif    

}










