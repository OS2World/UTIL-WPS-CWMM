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
/*
 * If you need another license for your project/product (commercial,
 * noncommercial, whatever) contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 *
 */
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPIBITMAPS
#define INCL_GPILOGCOLORTABLE
#define INCL_SW
#define INCL_GPIREGIONS
#define INCL_WIN

#include <os2.h>

#include "common.h"
#include "classvolumeres.h"
#include "mmres.h"
#include "classvolume.h"
#include <stdio.h>
#include <stdlib.h>


/* Fonts to use in dialogs */
#define DEFAULT_DIALOG_FONT   "9.WarpSans"

/* Bitmap data */
extern LOADEDBITMAP allBMPs[];
extern CONTROLINFO ciControls[];

/* Window procedures */
extern PFNWP orgBgProc; /* Org proc of background */
extern PFNWP orgButtonProc;
extern HPOINTER hptrArrowWE;


/* This custom button proc is only for DnD handling */
MRESULT EXPENTRY buttonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    default:
      break;
    }
  if(orgButtonProc)
    return (*orgButtonProc)(hwnd, msg, mp1, mp2);
  else
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY bgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
      /*****************************************************/
      /* Paint the background                              */
      /*****************************************************/
    case WM_PAINT:
      {
        SWP swp;
        RECTL rcl;
        HPS hps;

        WinQueryWindowPos(hwnd,&swp);
        if(swp.fl & SWP_MINIMIZE)
          break;
  
        hps=WinBeginPaint(hwnd, NULLHANDLE, NULLHANDLE);
        WinQueryWindowRect(hwnd, &rcl);
        WinDrawBitmap(hps, allBMPs[MAIN_BMP_IDX].hbm,
                      NULLHANDLE, 
                      (PPOINTL)&rcl,
                      0, 0,
                      DBM_IMAGEATTRS);    
        WinEndPaint(hps);
        return MRFALSE;
      }
    default:
      break;
    }
  if(orgBgProc)
    return (*orgBgProc)(hwnd, msg, mp1, mp2);
  else
    return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

/* This function paints all our controls with the provided bitmaps */
/* It's called from the guiproc                                    */
MRESULT paintControls(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  USERBUTTON* ubPtr;
  CONTROLINFO ci;
  RECTL rcl;

  ubPtr=(USERBUTTON*)PVOIDFROMMP(mp2);
  switch(SHORT1FROMMP(mp1))
    {
#if 0
    case IDPB_POSBAR:
      ci=ciControls[CTRLIDX_POSSLIDER];//ciPosSlider;
      break;
    case IDPB_SLIDERARM:
      DosBeep(5000, 30);
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
#endif
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
  //  static ULONG ulShowFeedback=0;

#if 0
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
#endif
  return 1;
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
        return WinDefWindowProc(hwnd, msg, mp1, mp2);
      }
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
        WinPostMsg(WinQueryWindow(hwnd, QW_PARENT),WM_CONTROL,
                   MPFROM2SHORT(WinQueryWindowUShort(hwnd, QWS_ID), SLN_CHANGE),
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
            rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM,
                                MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                                MPFROMP(&oi) );
          }
          if(!rc)
            WinFillRect(hps, &rcl3, CLR_PALEGRAY);

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
            rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM,
                                MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                                MPFROMP(&oi) );
          }
          if(!rc)
            WinFillRect(hps, &rcl3, CLR_PALEGRAY);

          /* Paint Slider */
          rc=FALSE;
          if(userSlider) {
            OWNERITEM oi={0};
            oi.hwnd=hwnd;
            oi.hps=hps;
            oi.fsState=SLS_OWNERDRAW;/* More to come */
            oi.rclItem=rcl;
            oi.idItem=SDA_SLIDERARM;
            rc=(BOOL)WinSendMsg(WinQueryWindow(hwnd, QW_PARENT), WM_DRAWITEM,
                                MPFROMSHORT(WinQueryWindowUShort(hwnd, QWS_ID)),
                                MPFROMP(&oi) );

          }
          if(!rc)
            {
              WinFillRect(hps,&rcl, CLR_PALEGRAY);
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
            //            WinSetPointer(HWND_DESKTOP, WinQuerySysPointer(HWND_DESKTOP, SPTR_SIZEWE, FALSE));
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
          WinFillRect(hps, &rcl3, CLR_PALEGRAY);

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
            WinFillRect(hps,&rcl2, CLR_PALEGRAY);        
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

  poi=(POWNERITEM)PVOIDFROMMP(mp2);

  switch(id)
    {
    case IDSL_MMPLAYERVOLUME:
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
      if(ciArm.hbmSource)
        //DosBeep(5000, 50);
      WinDrawBitmap(poi->hps, ciArm.hbmSource,
                    &ciArm.rclSource, 
                    (PPOINTL)&poi->rclItem,
                    0, 0,
                    DBM_IMAGEATTRS);
      //    WinFillRect(poi->hps,&poi->rclItem, CLR_RED);
      return (MRESULT) TRUE;
    case SDA_BACKGROUND:
      {
        WinDrawBitmap(poi->hps,
                      ci.hbmSource,
                      &ci.rclSource, 
                      (PPOINTL)&poi->rclItem,
                      0, 0,
                      DBM_IMAGEATTRS);
        //    WinFillRect(poi->hps,&poi->rclItem, CLR_WHITE);
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



