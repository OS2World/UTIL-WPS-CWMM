/*
 * This file is (C) Chris Wohlgemuth 2002
 * It is part of the MediaFolder package
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
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_GPILOGCOLORTABLE
#define INCL_SW
#define INCL_GPIREGIONS

#include "mediafolder.hh"
#include "mediafolderres.h"
#include <stdio.h>
#include <stdlib.h>


/* Fonts to use in dialogs */
#define DEFAULT_DIALOG_FONT   "9.WarpSans"


PFNWP  oldButtonProc2;  //place for original button-procedure
HWND hwndBubbleWindow=NULLHANDLE;// The handle of the help window
#define  WM_NEWBUBBLE   WM_USER+100 //Use ATOM later
#define xVal  12      //x-distance of Bubble
#define yVal  8      //y-distance of Bubble

extern HMODULE hResource;

extern CONTROLINFO ciControls[];
extern LOADEDBITMAP allBMPs[];
extern HPOINTER hptrArrowWE; /* Pointer for slider control */

PFNWP  oldButtonProc;  //place for original button-procedure
PFNWP  oldStaticTextProc;
PFNWP  oldStaticProc;
PFNWP  oldCheckBoxProc;
PFNWP  oldSliderPaintProc;
PFNWP  oldPlayTimeProc;

/* Extern */
ULONG cwQueryOSRelease();
void sendConfig();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
void getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
MRESULT cwInsertMenuItem(int iPosition, HWND hwndMenu, HWND hwndSubMenu, int iID, char * chrText);
MRESULT cwInsertMenuSeparator(int iPosition, HWND hwndMenu, HWND hwndSubMenu);
void handleWindowPosChanged(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);/* In dataCddialogProcs.cpp */
void writeLogPrintf(char* logFile, char* format, ...);

/* Local */
MRESULT EXPENTRY staticTextProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_SETWINDOWPARAMS:
      {
        MRESULT mr;
        PWNDPARAMS pwp=(PWNDPARAMS)PVOIDFROMMP(mp1);
        if(pwp->fsStatus==WPM_TEXT) {
          /* The control text changes. Force repaint of background by frame. */
          if(WinIsWindowVisible(hwnd)) {
            WinShowWindow(hwnd, FALSE);
            mr=oldStaticTextProc(hwnd, msg, mp1, mp2);/* Change text */
            WinShowWindow(hwnd, TRUE);/* Force painting of text */
            return mr;
          }
        }
      }
      break;
    case WM_PAINT:
      {
        HPS hps;
        char text[200];

        hps=WinBeginPaint(hwnd, NULLHANDLE, NULL);
        if(WinQueryWindowText(hwnd, sizeof(text), text))
          {
            RECTL rcl;

            WinQueryWindowRect(hwnd, &rcl);
            GpiCreateLogColorTable(hps, LCOL_PURECOLOR,LCOLF_RGB, 0, 0, NULLHANDLE);
            rcl.xLeft+=1;
                        
            WinDrawText(hps, strlen(text), text, &rcl, 0x00eeeeef,0, /*DT_CENTER|*/DT_VCENTER|DT_MNEMONIC);
            rcl.xLeft-=1;
            rcl.yBottom+=1;
            WinDrawText(hps, strlen(text),text, &rcl, 0x00333355,0,DT_VCENTER /*| DT_CENTER*//*|DT_TEXTATTRS*/);

          }
        WinEndPaint(hps);
        return (MRESULT) 0;
      }
    default:
      break;
    }
  return oldStaticTextProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY playTimeTextProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_BUTTON1CLICK:
      WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_APPTERMINATENOTIFY,
                 MPFROM2SHORT(NOTIFY_CLICK, WinQueryWindowUShort(hwnd, QWS_ID)) ,0L);
      return (MRESULT)0;
    case WM_CONTEXTMENU:
      WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_APPTERMINATENOTIFY,
                 MPFROM2SHORT(NOTIFY_CONTEXT, WinQueryWindowUShort(hwnd, QWS_ID)) ,0L);
      return (MRESULT)0;
    case WM_PAINT:
      {
        HPS hps;
        RECTL rcl;
        char text[100];
        CONTROLINFO ci;
        
        ci=ciControls[CTRLIDX_PLAYTIME];//ciVolSliderArm;
        
        hps=WinBeginPaint(hwnd, NULLHANDLE, NULL);
        if(WinQueryWindowText(hwnd, sizeof(text), text))
          {
            POINTL ptl= {0};
            WinDrawBitmap(hps, ci.hbmSource,
                          &ci.rclSource, 
                          &ptl,
                          0, 0,
                          DBM_IMAGEATTRS);
            WinQueryWindowRect(hwnd,&rcl);
            WinDrawText(hps, strlen(text),text, &rcl,0,0,DT_CENTER|DT_VCENTER|DT_TEXTATTRS);
          }
        WinEndPaint(hps);
        return (MRESULT) 0;
      }
    default:
      break;
    }
  return oldPlayTimeProc(hwnd, msg, mp1, mp2);
}


/* This function paints all our controls with the provided bitmaps */
/* It's called from the guiproc                                    */
MRESULT paintControls(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  USERBUTTON* ubPtr;
  CONTROLINFO ci;
  RECTL rcl;
  SWP swp;

  ubPtr=(USERBUTTON*)PVOIDFROMMP(mp2);
  switch(SHORT1FROMMP(mp1))
    {
    case IDPB_SKIPFORWARD:
      if(ubPtr->fsState==BDS_DEFAULT||!ubPtr->fsState)
        ci=ciControls[CTRLIDX_SKIPFWDBUTTON];
      else {
        ci=ciControls[CTRLIDX_SKIPFWDBUTTONSEL];
      }
      break;
    case IDPB_SKIPBACK:
      if(ubPtr->fsState==BDS_DEFAULT||!ubPtr->fsState)
        ci=ciControls[CTRLIDX_SKIPBACKBUTTON];
      else {
        ci=ciControls[CTRLIDX_SKIPBACKBUTTONSEL];
      }
      break;
    case IDPB_PLAY:
      if(ubPtr->fsState==BDS_DEFAULT||!ubPtr->fsState)
        ci=ciControls[CTRLIDX_PLAYBUTTON];
      else {
        ci=ciControls[CTRLIDX_PLAYBUTTONSEL];
      }
      break;
    case IDPB_PAUSE:
      if(ubPtr->fsState==BDS_DEFAULT||!ubPtr->fsState)
        ci=ciControls[CTRLIDX_PAUSEBUTTON];
      else {
        ci=ciControls[CTRLIDX_PAUSEBUTTONSEL];
      }
      break;
    case IDPB_STOP:
      if(ubPtr->fsState==BDS_DEFAULT||!ubPtr->fsState)
        ci=ciControls[CTRLIDX_STOPBUTTON];
      else {
        ci=ciControls[CTRLIDX_STOPBUTTONSEL];
      }
      break;
    case IDPB_POSBAR:
      ci=ciControls[CTRLIDX_POSSLIDER];//ciPosSlider;
      break;
    case IDPB_SLIDERARM:
        ci=ciControls[CTRLIDX_POSSLIDERARM];//ciPosSliderArm;
      break;
    case IDSL_VOLUME:
      ci=ciControls[CTRLIDX_VOLSLIDER];//ciVolSlider;
      break;
    case IDSL_VOLUMEARM:
      if(WinQueryCapture(HWND_DESKTOP==hwnd))
        ci=ciControls[CTRLIDX_VOLSLIDERARMSEL];//ciVolSliderArm;
      else
        ci=ciControls[CTRLIDX_VOLSLIDERARM];//ciVolSliderArm;
      break;
    default:
      return (MRESULT)0;
    }
  rcl.yBottom=0;
  rcl.xLeft=0;
  WinDrawBitmap(ubPtr->hps, ci.hbmSource,
                &ci.rclSource, 
                (PPOINTL)&rcl,
                0, 0,
                DBM_IMAGEATTRS);
  if(ubPtr->fsStateOld!=ubPtr->fsState)
    ubPtr->fsState =( ubPtr->fsStateOld==BDS_DEFAULT ? BDS_HILITED : BDS_DEFAULT);
  return (MRESULT)0;
}

/* This function paints all our controls with the provided bitmaps. */
BOOL paintHilitedControl(HWND hwnd, USHORT usControl, BOOL bHilite)
{
  CONTROLINFO ci;
  RECTL rcl;
  SWP swp;
  HPS hps;
  static ULONG ulShowFeedback=0;

  if(!ulShowFeedback) {
    /* Query the INI setting*/
    if(!PrfQueryProfileInt(HINI_USERPROFILE, APPKEY_CWMM, MFLDR_NO_VISUAL_FEEDBACK, 0))
      ulShowFeedback=1; /* Flash play controls */
    else
      ulShowFeedback=2; /* Don't flash play controls while playing */
  }

  if(ulShowFeedback==2)
    return 1;
  
  switch(usControl)
    {
    case IDPB_PLAY:
      if(!bHilite) {
        ci=ciControls[CTRLIDX_PLAYBUTTON];
    }
      else {
        ci=ciControls[CTRLIDX_PLAYBUTTONSEL];
      }
      break;
    case IDPB_PAUSE:
      if(!bHilite) {
        ci=ciControls[CTRLIDX_PAUSEBUTTON];
      }
      else {
        ci=ciControls[CTRLIDX_PAUSEBUTTONSEL];
      }
      break;
    default:
      return 0;
    }
  rcl.yBottom=0;
  rcl.xLeft=0;
  hps=WinGetPS(hwnd);
  WinDrawBitmap(hps, ci.hbmSource,
                &ci.rclSource, 
                (PPOINTL)&rcl,
                0, 0,
                DBM_IMAGEATTRS);
  WinReleasePS(hps);
  return 1;
}

/* This proc handles the custom painting for check boxes */
MRESULT EXPENTRY checkBoxProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
 
  switch(msg)
    {
    case WM_DESTROY:
      {
        MFLDRCTRLDATA * mFldrCtrlData=(MFLDRCTRLDATA *) WinQueryWindowULong(hwnd,QWL_USER);;
        
        if(!mFldrCtrlData)
          free(mFldrCtrlData);
        break;
      }
    case WM_PAINT:
      {
        short       checkState;
        BOOL bHighLite;
        HPS hps;
        RECTL rcl;
        char text[100];
        CONTROLINFO ci;
        MFLDRCTRLDATA * mFldrCtrlData=(MFLDRCTRLDATA *) WinQueryWindowULong(hwnd,QWL_USER);

        checkState = SHORT1FROMMR(WinSendMsg(hwnd,BM_QUERYCHECK,NULL,NULL));
        bHighLite  = LONGFROMMR(WinSendMsg(hwnd,BM_QUERYHILITE,NULL,NULL));
        if(checkState==0) {
          /* unchecked */
          if(mFldrCtrlData)
            mFldrCtrlData->bPreviousCheckState=0;
          if(bHighLite)
            ci=ciControls[CTRLIDX_UNCHECKSEL];
          else
            ci=ciControls[CTRLIDX_UNCHECK];//ciVolSliderArm;
        }
        else if(checkState==1) {
          /* checked */
          if(mFldrCtrlData)
            mFldrCtrlData->bPreviousCheckState=1;
          if(bHighLite)
            ci=ciControls[CTRLIDX_CHECKSEL];
          else
            ci=ciControls[CTRLIDX_CHECK];
        }
      
        WinQueryWindowRect(hwnd, &rcl);
        hps=WinBeginPaint(hwnd, NULLHANDLE, NULL);
        if(WinQueryWindowText(hwnd, sizeof(text), text))
          {
            POINTL ptl= {0};
            ULONG ulPos;
            char text2[120];
            ptl.y=(rcl.yTop-ci.rclSource.yTop)/2;

            WinDrawBitmap(hps, ci.hbmSource,
                          NULL,//&ci.rclSource, 
                          &ptl,
                          0, 0,
                          DBM_IMAGEATTRS);
            //            WinQueryWindowRect(hwnd,&rcl);
            rcl.xLeft+=ci.rclSource.xRight+2;
            GpiCreateLogColorTable(hps, LCOL_PURECOLOR,LCOLF_RGB, 0, 0, NULLHANDLE);
            rcl.xLeft+=1;
            /* Get mnemonic */
            if(mFldrCtrlData && (ulPos=mFldrCtrlData->ulMnemonicPos) & 0x00010000){
              ulPos&=0x0000FFFF;
              strncpy(text2,text,ulPos);
              text2[ulPos]=0;
              strcat(text2,"~");
              strcat(text2,&text[ulPos]);
              WinDrawText(hps, strlen(text2), text2, &rcl, 0x00eeeeef,0, DT_VCENTER|DT_MNEMONIC);
              rcl.xLeft-=1;
              rcl.yBottom+=1;
              WinDrawText(hps, strlen(text2), text2, &rcl, 0x00333355,0,/*DT_TEXTATTRS|*/DT_VCENTER|DT_MNEMONIC);
            }
            else {
              WinDrawText(hps, strlen(text), text, &rcl, 0x00eeeeef,0, DT_VCENTER|DT_MNEMONIC);
              rcl.xLeft-=1;
              rcl.yBottom+=1;
              WinDrawText(hps, strlen(text),text, &rcl, 0x00333355,0,DT_VCENTER/*|DT_TEXTATTRS*/);
            }
          }/* WinQueryWindowText() */
        //        if(WinQueryFocus(HWND_DESKTOP)==hwnd)
        //   WinSetFocus(HWND_DESKTOP, hwnd);
        WinEndPaint(hps);
        return (MRESULT) 0;
      }
    case BM_SETHILITE:
    case BM_SETDEFAULT:
    case BM_SETCHECK:
      {
        MRESULT mr;
        WinEnableWindowUpdate(hwnd, FALSE);
        mr=oldCheckBoxProc(hwnd, msg, mp1, mp2);
        WinEnableWindowUpdate(hwnd, TRUE);
        WinInvalidateRect(hwnd, NULL,TRUE);
        return mr;
      }
    default:
      break;
    }
  return oldCheckBoxProc(hwnd, msg, mp1, mp2);
}
MRESULT EXPENTRY sliderPaintProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
 
  switch(msg)
    {
    case WM_PAINT:
      {
        short       checkState;
        HPS hps;
        RECTL rcl, rcl2;
        char text[100];
        CONTROLINFO ci;
        MRESULT mr;

        hps=WinBeginPaint(hwnd, NULLHANDLE, NULL);
        WinQueryWindowRect(hwnd, &rcl);
        WinValidateRect(hwnd,&rcl,TRUE);

        mr=oldSliderPaintProc(hwnd, msg, mp1, mp2);

        rcl2=rcl;
        hps=WinGetPS(hwnd);
        rcl2.yBottom=rcl2.yTop-5;
        WinFillRect(hps,&rcl2, CLR_BLUE);
        rcl2=rcl;
        rcl2.yTop=rcl2.yBottom+5;
        WinFillRect(hps,&rcl2, CLR_BLUE);
        WinEndPaint(hps);
        return mr;
      }
   case WM_APPTERMINATENOTIFY:
     {
       HPS hps;
       RECTL rcl,rcl2;

       WinQueryWindowRect(hwnd, &rcl);
       rcl2=rcl;
       hps=WinGetPS(hwnd);
       rcl2.yBottom=rcl2.yTop-5;
       WinFillRect(hps,&rcl2, CLR_BLUE);
       rcl2=rcl;
       rcl2.yTop=rcl2.yBottom+5;
       WinFillRect(hps,&rcl2, CLR_BLUE);

       WinReleasePS(hps);
       return (MRESULT) TRUE;
     }
    default:
      break;
    }
  return oldSliderPaintProc(hwnd, msg, mp1, mp2);
}


#define USERSLIDER 1 /* Set to 0 to let the proc paint the control. With set to 1
                        a WM_DRAWITEM msg is sent to the parent. */
BOOL userSlider=USERSLIDER;
#define SLOFFSET 3 /* The offset of the active slider area to the window border */
MRESULT EXPENTRY privateSliderProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  switch (msg)
    {      
    case WM_CREATE:
      {
        PCREATESTRUCT pCreate=(CREATESTRUCT*)PVOIDFROMMP(mp2);

        /* Initialize the slider data */
        WinSetWindowUShort(hwnd, SLIDERARMWIDTH,20);
        WinSetWindowUShort(hwnd, SLIDERARMHEIGHT,10);
        WinSetWindowUShort(hwnd, SLINCREMENTS,100);
        /* Dimensions of slider. The slider active area is smaller than the window to
           allow drawing of active state. */
        WinSetWindowULong(hwnd, SLIDERCX,pCreate->cx-2*SLOFFSET);
        WinSetWindowULong(hwnd, SLIDERCY,pCreate->cy-2*SLOFFSET);
        WinSetWindowULong(hwnd, SLIDERX,SLOFFSET);
        WinSetWindowULong(hwnd, SLIDERY,SLOFFSET);
        WinSetWindowULong(hwnd, SLIDERARMPOS, 0);
        WinSetWindowULong(hwnd, SLDRAGGING,FALSE);

        return (MRESULT)0;
      }
    case SLM_SETSLIDERINFO:
      switch(SHORT1FROMMP(mp1))
        {
          case SMA_SLIDERARMPOSITION:
            /*  SMA_RANGEVALUE */
            if(!WinQueryWindowULong(hwnd, SLDRAGGING)) {
              if(SHORT2FROMMP(mp1)==SMA_RANGEVALUE) {
                WinSetWindowULong(hwnd, SLIDERARMPOS, LONGFROMMP(mp2));
                WinInvalidateRect(hwnd, NULLHANDLE,TRUE);
                return (MRESULT)0;
              }
              else if(SHORT2FROMMP(mp1)==SMA_INCREMENTVALUE) {
                LONG lInc=LONGFROMMP(mp2)*(WinQueryWindowULong(hwnd, SLIDERCX)-WinQueryWindowUShort(hwnd, SLIDERARMWIDTH));
                lInc/=WinQueryWindowUShort(hwnd, SLINCREMENTS);
                WinSetWindowULong(hwnd, SLIDERARMPOS, lInc);
                WinInvalidateRect(hwnd, NULLHANDLE,TRUE);
                return (MRESULT)0;
              }
            }
            break;
        case SMA_SLIDERARMDIMENSIONS:
          WinSetWindowUShort(hwnd, SLIDERARMWIDTH, SHORT1FROMMP(mp2));
          WinSetWindowUShort(hwnd, SLIDERARMHEIGHT, SHORT2FROMMP(mp2));
          return (MRESULT)0;
        default:
          break;
        }
      break;
    case WM_SIZE:
      WinSetWindowULong(hwnd, SLIDERCX, SHORT1FROMMP(mp2)-2*SLOFFSET);
      WinSetWindowULong(hwnd, SLIDERCY, SHORT2FROMMP(mp2)-2*SLOFFSET);
      WinInvalidateRect(hwnd, NULLHANDLE,TRUE);
      return (MRESULT)0;
    case WM_BUTTON1DOWN:
      {
        SHORT x=SHORT1FROMMP( mp1);
        SHORT y=SHORT2FROMMP( mp1);
        LONG  lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
        USHORT usWidth=WinQueryWindowUShort(hwnd, SLIDERARMWIDTH);

        if(x<=lPos+usWidth+SLOFFSET && x>=lPos+SLOFFSET && 
           y<=WinQueryWindowUShort(hwnd, SLIDERY)+WinQueryWindowUShort(hwnd, SLIDERCY)
           && y>=WinQueryWindowUShort(hwnd, SLIDERY)) {
          WinSetWindowUShort(hwnd, PTRPOSINSLARM, x-lPos-SLOFFSET);
        }
        WinSetFocus(HWND_DESKTOP, hwnd);
      break;
      }

    case WM_FOCUSCHANGE:
      {
        HPS hps;
        RECTL rcl;
        POINTL ptl;

        if(SHORT1FROMMP(mp2)) {
          hps=WinGetPS(hwnd);
          WinQueryWindowRect(hwnd, &rcl);        
          GpiSetLineType(hps, LINETYPE_DOT);
          ptl.x=rcl.xLeft;
          ptl.y=rcl.yBottom;
          GpiMove(hps,&ptl);
          ptl.x=rcl.xRight-1;
          GpiLine(hps,&ptl);
          ptl.y=rcl.yTop-1;
          GpiLine(hps,&ptl);
          ptl.x=rcl.xLeft;
          GpiLine(hps,&ptl);
          ptl.y=rcl.yBottom;
          GpiLine(hps,&ptl);
          WinReleasePS(hps);
        }
        else
          WinInvalidateRect(hwnd, NULLHANDLE,TRUE);

        break;
      }
    case WM_CHAR:
      if(WinQueryFocus(HWND_DESKTOP)==hwnd) {
        if(SHORT2FROMMP(mp2)==VK_TAB && (SHORT1FROMMP(mp1) & KC_VIRTUALKEY))
          if(SHORT1FROMMP(mp1) & (KC_KEYUP|KC_PREVDOWN)) 
            return (MRESULT)TRUE;

#if 0
        /**/
        if(SHORT2FROMMP(mp2)==VK_RIGHT && (SHORT1FROMMP(mp1) & KC_VIRTUALKEY))
          /* if(SHORT1FROMMP(mp1) & (KC_KEYUP|KC_PREVDOWN))*/ {
            DosBeep(5000, 100);
            return (MRESULT)TRUE;
          }
        if(SHORT2FROMMP(mp2)==VK_LEFT && (SHORT1FROMMP(mp1) & KC_VIRTUALKEY))
          if(SHORT1FROMMP(mp1) & (KC_KEYUP|KC_PREVDOWN)) {
            DosBeep(500, 100);
            return (MRESULT)TRUE;
          }
#endif

        return WinDefWindowProc(hwnd, msg, mp1, mp2);
      }

#if 0
      if(WinQueryFocus(HWND_DESKTOP)==hwnd) {
        /* We have the focus */
        if((SHORT1FROMMP(mp1) & (KC_VIRTUALKEY))==(KC_VIRTUALKEY)) {
          LONG  lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
          USHORT usWidth=WinQueryWindowUShort(hwnd, SLIDERARMWIDTH);
          ULONG ulCx=WinQueryWindowULong(hwnd, SLIDERCX);

          switch(SHORT2FROMMP(mp2))
            {
            case VK_RIGHT:
              if(SHORT1FROMMP(mp1) & (KC_KEYUP|KC_PREVDOWN)) {
                lPos+=1;
                if(lPos>ulCx-usWidth)
                  lPos=ulCx-usWidth;
                else {
                  WinPostMsg( hwnd,
                              SLM_SETSLIDERINFO,
                              MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
                              MPFROMLONG( (LONG) lPos ));
                  lPos=(lPos*100)/(WinQueryWindowULong(hwnd, SLIDERCX)-WinQueryWindowUShort(hwnd, SLIDERARMWIDTH));

                  WinPostMsg( WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                              MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_CHANGE),
                              MPFROMLONG(lPos));

                  if(SHORT1FROMMP(mp1) & KC_LONEKEY) {
                    /* Post SLN_CHANGE notification */
                    WinPostMsg( WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                                MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_CHANGE),
                                MPFROMLONG(lPos));
                    WinSetWindowULong(hwnd, SLDRAGGING, FALSE);
                  }
                  else {
                    WinSetWindowULong(hwnd, SLDRAGGING, TRUE);
                    /* Post SLN_SLIDERTRACK notification */
                    WinPostMsg(WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                               MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_SLIDERTRACK),
                               MPFROMLONG(lPos));
                  }

                }
                WinSetWindowULong(hwnd, SLIDERARMPOS, lPos);
              }
              return (MRESULT)TRUE;
            case VK_LEFT:
              if(SHORT1FROMMP(mp1) & (KC_KEYUP|KC_PREVDOWN)) {
                lPos-=1;
                if(lPos<0) {
                  lPos=0;
                }
                else {
                  WinPostMsg( hwnd,
                              SLM_SETSLIDERINFO,
                              MPFROM2SHORT(SMA_SLIDERARMPOSITION, SMA_RANGEVALUE),
                              MPFROMLONG( (LONG) lPos ));
                  lPos=(lPos*100)/(WinQueryWindowULong(hwnd, SLIDERCX)-WinQueryWindowUShort(hwnd, SLIDERARMWIDTH));
                  /* Post SLN_CHANGE notification */
                  WinPostMsg( WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                              MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_CHANGE),
                              MPFROMLONG(lPos));

                  if(SHORT1FROMMP(mp1) & KC_LONEKEY) {
                    /* Post SLN_CHANGE notification */
                    WinPostMsg( WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                                MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_CHANGE),
                                MPFROMLONG(lPos));
                    WinSetWindowULong(hwnd, SLDRAGGING, FALSE);
                  }
                  else {
                    WinSetWindowULong(hwnd, SLDRAGGING, TRUE);
                    /* Post SLN_SLIDERTRACK notification */
                    WinPostMsg(WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                               MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_SLIDERTRACK),
                               MPFROMLONG(lPos));
                  }

                }
                WinSetWindowULong(hwnd, SLIDERARMPOS, lPos);

              }
              return (MRESULT)TRUE;
            default:
              break;
            }
        }

      }
#endif
      break;
    case WM_BUTTON1MOTIONSTART:
      {
        SHORT x=SHORT1FROMMP( mp1);
        SHORT y=SHORT2FROMMP( mp1);
        LONG  lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
        USHORT usWidth=WinQueryWindowUShort(hwnd, SLIDERARMWIDTH);

        if(x<=lPos+usWidth+SLOFFSET && x>=lPos+SLOFFSET && 
           y<=WinQueryWindowUShort(hwnd, SLIDERY)+WinQueryWindowUShort(hwnd, SLIDERCY)
           && y>=WinQueryWindowUShort(hwnd, SLIDERY)){
          //        if(x<=lPos+usWidth && y<=WinQueryWindowUShort(hwnd, SLIDERY)+WinQueryWindowUShort(hwnd, SLIDERCY)
          //      && y>=WinQueryWindowUShort(hwnd, SLIDERY)) {
          WinSetWindowULong(hwnd, SLDRAGGING, TRUE);
          if(hptrArrowWE)
            WinSetPointer(HWND_DESKTOP, hptrArrowWE);

          WinSetCapture(HWND_DESKTOP, hwnd);
        }
        break;
      }
    case WM_BUTTON1MOTIONEND:
      if(WinQueryWindowULong(hwnd, SLDRAGGING)) {
        LONG lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
        WinSetWindowULong(hwnd, SLDRAGGING,FALSE);
        WinSetCapture(HWND_DESKTOP, NULLHANDLE);
          lPos=(lPos*100)/(WinQueryWindowULong(hwnd, SLIDERCX)-WinQueryWindowUShort(hwnd, SLIDERARMWIDTH));
        /* Post SLN_CHANGE notification */
        WinPostMsg(WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL, MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_CHANGE),
                   MPFROMLONG(lPos));
      }
      break;

    case WM_MOUSEMOVE:
      if(WinQueryWindowULong(hwnd, SLDRAGGING)) {
        HPS hps;
        RECTL rcl, rcl2, rcl3;
        LONG lTemp;
        SHORT x=SHORT1FROMMP(mp1);
        LONG  lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
        USHORT usWidth=WinQueryWindowUShort(hwnd, SLIDERARMWIDTH);
       
        lTemp=lPos;
     
        rcl.xLeft=WinQueryWindowULong(hwnd, SLIDERX);
        rcl.yBottom=WinQueryWindowULong(hwnd, SLIDERY);
        rcl.xRight=rcl.xLeft+WinQueryWindowULong(hwnd, SLIDERCX);
        rcl.yTop=rcl.yBottom+WinQueryWindowULong(hwnd, SLIDERCY);

        rcl2=rcl3=rcl;
        rcl.xLeft=x-WinQueryWindowUShort(hwnd, PTRPOSINSLARM);
        if(rcl.xLeft<rcl2.xLeft)/* Make sure we stop at the left border */
          rcl.xLeft=rcl2.xLeft;

        rcl.xRight=rcl.xLeft+usWidth;
        if(rcl.xRight>rcl2.xRight)
          {/* Make sure we stop at the right border */
            rcl.xRight=rcl2.xRight;
            rcl.xLeft=rcl.xRight-usWidth;
          }
        lPos=rcl.xLeft-WinQueryWindowULong(hwnd, SLIDERX);/* Save position zero based */
        WinSetWindowULong(hwnd, SLIDERARMPOS, lPos);
        if(lPos!=lTemp) {
          BOOL rc;

          hps=WinGetPS(hwnd);
          /* Paint Background not necessary here */

          /* Shaft */
          /* Left part */
          rcl3.xRight=rcl.xLeft;

          rc=FALSE;

          if(userSlider) {
            OWNERITEM oi={0};
            oi.hwnd=hwnd;
            oi.hps=hps;
            oi.fsState=SLS_OWNERDRAW;/* More to come */
            oi.rclItem=rcl3;
            oi.idItem=SDA_SLIDERSHAFT;
            rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                                MPFROMP(&oi) );
          }
          if(!rc)
            WinFillRect(hps, &rcl3, CLR_GREEN);

          /* Right part */
          rcl3.xRight=rcl2.xRight;
          rcl3.xLeft=rcl.xRight;
          rc=FALSE;
          if(userSlider) {
            OWNERITEM oi={0};
            oi.hwnd=hwnd;
            oi.hps=hps;
            oi.fsState=SLS_OWNERDRAW;/* More to come */
            oi.rclItem=rcl3;
            oi.idItem=SDA_SLIDERSHAFT;
            rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                                MPFROMP(&oi) );
          }
          if(!rc)
            WinFillRect(hps, &rcl3, CLR_WHITE);

          /* Paint Slider */
          rc=FALSE;
          if(userSlider) {
            OWNERITEM oi={0};
            oi.hwnd=hwnd;
            oi.hps=hps;
            oi.fsState=SLS_OWNERDRAW;/* More to come */
            oi.rclItem=rcl;
            oi.idItem=SDA_SLIDERARM;
            rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                                MPFROMP(&oi) );

          }
          if(!rc)
            {
              WinFillRect(hps,&rcl, CLR_BLUE);
              WinDrawBorder(hps, &rcl, 2, 2, 0, 0 ,0x0400);
            }

          WinReleasePS(hps);
          lPos=(lPos*100)/(WinQueryWindowULong(hwnd, SLIDERCX)-WinQueryWindowUShort(hwnd, SLIDERARMWIDTH));
           /* Post SLN_SLIDERTRACK notification */
          WinPostMsg(WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL, 
                     MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_SLIDERTRACK),
                     MPFROMLONG(lPos));
        }
        return MRTRUE;
      }
      else {
        if(hptrArrowWE) {
          /* Not dragging */
          /* Change pointer to drag pointer when over the arm */
          SHORT x=SHORT1FROMMP( mp1);
          SHORT y=SHORT2FROMMP( mp1);
          LONG  lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
          USHORT usWidth=WinQueryWindowUShort(hwnd, SLIDERARMWIDTH);
          if(x<=lPos+usWidth+SLOFFSET && x>=lPos+SLOFFSET && 
             y<=WinQueryWindowUShort(hwnd, SLIDERY)+WinQueryWindowUShort(hwnd, SLIDERCY)
             && y>=WinQueryWindowUShort(hwnd, SLIDERY)) {
            WinSetPointer(HWND_DESKTOP, hptrArrowWE);
            return MRTRUE;
          }
        }
      }
      break;
    case WM_PAINT:
      {

        HPS hps, hps2;
        RECTL rcl, rcl2, rcl3;
        POINTL ptl;
        LONG  lPos=WinQueryWindowULong(hwnd, SLIDERARMPOS);
        USHORT usWidth=WinQueryWindowUShort(hwnd, SLIDERARMWIDTH);
        BOOL rc;

        WinQueryWindowRect(hwnd, &rcl);

        /* Shaft */
        rcl2.xLeft=WinQueryWindowULong(hwnd, SLIDERX);
        rcl2.yBottom=WinQueryWindowULong(hwnd, SLIDERY);
        rcl2.xRight=rcl2.xLeft+WinQueryWindowULong(hwnd, SLIDERCX)-1;
        rcl2.yTop=rcl2.yBottom+WinQueryWindowULong(hwnd, SLIDERCY)-1;

        /* Background */
        hps2=WinGetPS(hwnd);
        GpiExcludeClipRectangle(hps2,&rcl2);
        rc=FALSE;
        if(userSlider) {
          OWNERITEM oi={0};
          oi.hwnd=hwnd;
          oi.hps=hps2;
          oi.fsState=SLS_OWNERDRAW;/* More to come */
          oi.rclItem=rcl;
          oi.idItem=SDA_BACKGROUND;
          rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                    MPFROMP(&oi) );
        }
        if(!rc)
          WinFillRect(hps2, &rcl, CLR_PALEGRAY);
        /* Focus */
        if(WinQueryFocus(HWND_DESKTOP)==hwnd) {
          GpiSetLineType(hps2, LINETYPE_DOT);
          ptl.x=rcl.xLeft;
          ptl.y=rcl.yBottom;
          GpiMove(hps2,&ptl);
          ptl.x=rcl.xRight-1;
          GpiLine(hps2,&ptl);
          ptl.y=rcl.yTop-1;
          GpiLine(hps2,&ptl);
          ptl.x=rcl.xLeft;
          GpiLine(hps2,&ptl);
          ptl.y=rcl.yBottom;
          GpiLine(hps2,&ptl);
        }

        WinReleasePS(hps2);
        rcl2.yTop+=1;
        rcl2.xRight+=1;
        hps=WinBeginPaint(hwnd, NULLHANDLE, NULLHANDLE);
        rcl3=rcl=rcl2;
        /* Arm pos */        
        rcl2.xLeft+=lPos;
        /* Arm size */        
        rcl2.xRight=rcl2.xLeft+usWidth;
        
        /* Shaft */
        /* Left part */
        rcl3.xRight=rcl2.xLeft;
        rc=FALSE;
        if(userSlider) {
          OWNERITEM oi={0};
          oi.hwnd=hwnd;
          oi.hps=hps;
          oi.fsState=SLS_OWNERDRAW;/* More to come */
          oi.rclItem=rcl3;
          oi.idItem=SDA_SLIDERSHAFT;
          rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                    MPFROMP(&oi) );

        }
        if(!rc)
          WinFillRect(hps, &rcl3, CLR_GREEN);

        /* Right part */
        rcl3.xRight=rcl.xRight;
        rcl3.xLeft=rcl2.xRight;
        rc=FALSE;
        if(userSlider) {
          OWNERITEM oi={0};
          oi.hwnd=hwnd;
          oi.hps=hps;
          oi.fsState=SLS_OWNERDRAW;/* More to come */
          oi.rclItem=rcl3;
          oi.idItem=SDA_SLIDERSHAFT;
          rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                              MPFROMP(&oi) );
        }
        if(!rc)
          WinFillRect(hps, &rcl3, CLR_WHITE);

        rc=FALSE;
        if(userSlider) {
          OWNERITEM oi={0};
          oi.hwnd=hwnd;
          oi.hps=hps;
          oi.fsState=SLS_OWNERDRAW;/* More to come */
          oi.rclItem=rcl2;
          oi.idItem=SDA_SLIDERARM;
          rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM, MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                              MPFROMP(&oi) );
        }
        if(!rc)
          {
            WinFillRect(hps,&rcl2, CLR_BLUE);        
            WinDrawBorder(hps, &rcl2, 2, 2, 0, 0 ,0x0400);
          }

        WinEndPaint(hps);
        return (MRESULT)0;
      }
      break;
    default:
      break;
    }
  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

MRESULT drawSlider(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2, USHORT id)
{
  POWNERITEM poi;
  CONTROLINFO ci, ciArm;
  RECTL rcl;
  POINTL ptl= {0};

  poi=(POWNERITEM)PVOIDFROMMP(mp2);

  switch(id)
    {
    case IDSL_POSBAR:
      ci=ciControls[CTRLIDX_POSSLIDER];
      ciArm=ciControls[CTRLIDX_POSSLIDERARM];
      break;
    case IDSL_VOLUME:
      ci=ciControls[CTRLIDX_VOLSLIDER];//ciVolSlider;
      //      if(WinQueryCapture(HWND_DESKTOP==hwnd))
      ciArm=ciControls[CTRLIDX_VOLSLIDERARMSEL];//ciVolSliderArm;
      // else
      //  ciArm=ciControls[CTRLIDX_VOLSLIDERARM];//ciVolSliderArm;
      break;
    default:
      return (MRESULT)FALSE;
    }
  
  switch(poi->idItem)
    {
    case SDA_SLIDERARM:
      WinDrawBitmap(poi->hps, ciArm.hbmSource,
                    &ciArm.rclSource, 
                    (PPOINTL)&poi->rclItem,
                    0, 0,
                    DBM_IMAGEATTRS);
      return (MRESULT) TRUE;
    case SDA_BACKGROUND:
      {
        WinDrawBitmap(poi->hps,
                      ci.hbmSource,
                      &ci.rclSource, 
                      (PPOINTL)&poi->rclItem,
                      0, 0,
                      DBM_IMAGEATTRS);
        //        WinFillRect(poi->hps,&poi->rclItem, CLR_WHITE);
        return (MRESULT) TRUE;
      }
    case SDA_SLIDERSHAFT:
      {
        RECTL rcl;

        rcl=ci.rclSource;
        rcl.xLeft+=poi->rclItem.xLeft;
        rcl.xRight=rcl.xLeft+(poi->rclItem.xRight-poi->rclItem.xLeft);
        rcl.yBottom+=SLOFFSET;
        rcl.yTop-=SLOFFSET;

        if(!WinDrawBitmap(poi->hps, ci.hbmSource,
                          &rcl,
                          //&ci.rclSource, 
                          (PPOINTL)&poi->rclItem,
                          0, 0,
                          DBM_IMAGEATTRS))
          return (MRESULT)FALSE;
        return (MRESULT)TRUE;
      }
    default:
      return (MRESULT)TRUE;
    }
  /* Done */
  return (MRESULT)TRUE;
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
  ULONG ulStyle;
  char text[CCHMAXPATH];
  ULONG ulLen;
  HWND hwndTemp;
  POINTL ptl;
  SWP swp;

  /* Pos slider */
  ci=ciControls[CTRLIDX_POSSLIDER];//ciPosSlider;
  WinCreateWindow(hwnd, SLIDER_CLASS, "", WS_VISIBLE | WS_TABSTOP, 0,0, 10, 10, 
                  hwnd, HWND_TOP, ci.id, NULLHANDLE, NULLHANDLE);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclSource.xLeft, ci.rclSource.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE | SWP_SIZE);
  
  WinSendMsg(WinWindowFromID(hwnd, ci.id),SLM_SETSLIDERINFO,
             MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,0),
             MPFROM2SHORT( 9, 9)); 
    /* Vol slider */
  ci=ciControls[CTRLIDX_VOLSLIDER];//ciVolSlider;
  WinCreateWindow(hwnd, SLIDER_CLASS, "", WS_VISIBLE | WS_TABSTOP, 0,0, 10, 10, 
                  hwnd, HWND_TOP, ci.id, NULLHANDLE, NULLHANDLE);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclSource.xLeft, ci.rclSource.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE | SWP_SIZE);
  WinSendMsg(WinWindowFromID(hwnd, ci.id),SLM_SETSLIDERINFO,
             MPFROM2SHORT(SMA_SLIDERARMDIMENSIONS,0),
             MPFROM2SHORT( 9, 9)); 
  /* Playtime text */
  ci=ciControls[CTRLIDX_PLAYTIME];
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclSource.xLeft,ci.rclSource.yBottom,
                  ci.rclSource.xRight-ci.rclSource.xLeft,
                  ci.rclSource.yTop-ci.rclSource.yBottom, SWP_MOVE|SWP_SIZE); 
  oldPlayTimeProc=WinSubclassWindow(WinWindowFromID(hwnd, IDST_PLAYTIME), playTimeTextProc);
  /* Skip back button */
  ci=ciControls[CTRLIDX_SKIPBACKBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);
  /* STOP button */
  ci=ciControls[CTRLIDX_STOPBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
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
  /* Skip forward button */
  ci=ciControls[CTRLIDX_SKIPFWDBUTTON];
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle|BS_USERBUTTON);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  ci.rclDest.xRight-ci.rclDest.xLeft,
                  ci.rclDest.yTop-ci.rclDest.yBottom, SWP_MOVE|SWP_SIZE);
  /* Repeat Checkbox */
  ci=ciControls[CTRLIDX_CHECK];//ciPosSlider;
#if 0
  ptl.x=ci.rclDest.xLeft;
  ptl.y=ci.rclDest.yBottom;
  WinMapDlgPoints(hwnd, &ptl, 1, FALSE);

  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ptl.x , ptl.y,
                  0, 0, SWP_MOVE);
#endif
  WinQueryWindowPos(WinWindowFromID(hwnd, ci.id), &swp);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  131, swp.cy, SWP_MOVE);

  /* Remove BS_USERBUTTON style otherwise BS_AUTOCHECKBOX doesn't work. BS_USERBUTTON is necessary because eStyler
     from eCS only checks during creation of the button for this style... OS/2 does it always when painting a control. */
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  ulStyle&=~BS_USERBUTTON;
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle | BS_AUTOCHECKBOX);
  /* Subclass */
  oldCheckBoxProc=WinSubclassWindow(WinWindowFromID(hwnd, ci.id),checkBoxProc);
  /* Find mnemonic */
  hwndTemp=WinWindowFromID(hwnd, ci.id);
  if((ulLen=WinQueryWindowText( hwndTemp, sizeof(text), text))!=0) {
    int a;
    
    text[CCHMAXPATH-1]=0;
    for(a=0;a<ulLen;a++) {
      if(WinSendMsg( hwndTemp, WM_MATCHMNEMONIC,MPFROMSHORT((USHORT)text[a]),0))
        break;/* Ok, mnemonic found */
    }
    if(a<=ulLen) {
      MFLDRCTRLDATA * mFldrCtrlData=(MFLDRCTRLDATA *)malloc(sizeof(MFLDRCTRLDATA));

      if(mFldrCtrlData) {
        mFldrCtrlData->ulMnemonicPos=0x00010000+a;
        WinSetWindowULong(hwndTemp, QWL_USER, (ULONG)mFldrCtrlData);/* For mnemonic */
      }
    }
  }

  /* Random Checkbox */
  ci=ciControls[CTRLIDX_RANDOMCHECK];
  WinQueryWindowPos(WinWindowFromID(hwnd, ci.id), &swp);
  WinSetWindowPos(WinWindowFromID(hwnd, ci.id),HWND_TOP, ci.rclDest.xLeft , ci.rclDest.yBottom,
                  131, swp.cy, SWP_MOVE|SWP_SIZE);
  /* Remove BS_USERBUTTON style otherwise BS_AUTOCHECKBOX doesn't work. BS_USERBUTTON is necessary because eStyler
     from eCS only checks during creation of the button for this style... OS/2 does it always when painting a control. */
  ulStyle=WinQueryWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE);
  ulStyle&=~BS_USERBUTTON;
  WinSetWindowULong(WinWindowFromID(hwnd, ci.id),QWL_STYLE,ulStyle | BS_AUTOCHECKBOX);
  /* Subclass */
  oldCheckBoxProc=WinSubclassWindow(WinWindowFromID(hwnd, ci.id),checkBoxProc);
  /* Find mnemonic */
  hwndTemp=WinWindowFromID(hwnd, ci.id);
  if((ulLen=WinQueryWindowText( hwndTemp, sizeof(text), text))!=0) {
    int a;
    
    text[CCHMAXPATH-1]=0;
    for(a=0;a<ulLen;a++) {
      if(WinSendMsg( hwndTemp, WM_MATCHMNEMONIC,MPFROMSHORT((USHORT)text[a]),0))
        break;/* Ok, mnemonic found */
    }
    if(a<=ulLen) {
      MFLDRCTRLDATA * mFldrCtrlData=(MFLDRCTRLDATA *)malloc(sizeof(MFLDRCTRLDATA));

      if(mFldrCtrlData) {
        mFldrCtrlData->ulMnemonicPos=0x00010000+a;
        WinSetWindowULong(hwndTemp, QWL_USER, (ULONG)mFldrCtrlData);/* For mnemonic */
      }
    }
  }

}


