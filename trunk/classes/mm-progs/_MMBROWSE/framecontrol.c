#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_DOSMEMMGR
#define INCL_GPI
#define INCL_GPICONTROL
#define INCL_GPILCIDS
#define INCL_GPIMETAFILES
#define INCL_GPIPRIMITIVES
#define INCL_GPIREGIONS
#define INCL_PM
#define INCL_WIN
#define INCL_WINDIALOGS
#define INCL_WINERRORS
#define INCL_WINFRAMEMGR
#define INCL_WINHELP
#define INCL_WINMENUS
#define INCL_WINMESSAGEMGR
#define INCL_WINMLE
#define INCL_WINPOINTERS
#define INCL_WINSTDDLGS
#define INCL_WINSTDFILE
#define INCL_WINSTDSLIDER
#define INCL_WINSWITCHLIST
#define INCL_WINSYS
#define INCL_WINTIMER
#define INCL_WINWINDOWMGR
#define INCL_REXXSAA

#include <os2.h>
#include "framecontrol.h"

FRAMECTRLDATA fcd;

PFNWP g_pfnwpOrgFrameProc=NULLHANDLE;

static USHORT cwNumAdditionalFrameControls(HWND hwndFolder)
{
  USHORT rc;
  FRAMECTRLDATA * fctrlData;

  fctrlData=&fcd;

  if(!fctrlData)
    return 0;
  
  rc=0;
  
  if(fctrlData->bLeft)rc++;
  if(fctrlData->bRight)rc++;
  if(fctrlData->bTop)rc++;
  if(fctrlData->bBottom)rc++;

  return rc;
}


/**************************************************************/
/*                                                            */
/* This function adjusts the size and position of the client. */
/* It is called from the frame window procedure               */
/*                                                            */
/**************************************************************/
static BOOL cwCalculateClient(HWND hwndFolder, PRECTL pRectl, BOOL which)
{
  FRAMECTRLDATA * fctrlData;
  
  //  which:
  //     TRUE:  Frame rectangle provided, calculate client
  //     FALSE: Client area rectangle provided, calculate frame

  /* Get our control data */
  fctrlData=&fcd;
  if(!fctrlData) 
    return FALSE;

  if (SHORT1FROMMP(which))
    //     TRUE:  Frame rectangle provided, calculate client
    //     FALSE: Client area rectangle provided, calculate frame
    {
      //  TRUE: calculate the rectl of the client;
      //  call default window procedure to subtract child frame
      //  controls from the rectangle's height
      LONG lClientHeight;
      LONG lNewControls;

      lNewControls=0;
      if(fctrlData->bTop)
        lNewControls=fctrlData->sizelTopFrame.cy;
      if(fctrlData->bBottom)
        lNewControls+=fctrlData->sizelBottomFrame.cy;
    
      lClientHeight = pRectl->yTop - pRectl->yBottom;
      if ( lNewControls  > lClientHeight  )
        {
          if(fctrlData->bBottom || fctrlData->bTop)
            // extension is taller than client, so set client height to 0
            pRectl->yTop = pRectl->yBottom;
        }
      else
        {
          //  set the origin of the client and shrink it based upon the
          //  frame control's height
          if(fctrlData->bBottom) {
            pRectl->yBottom +=fctrlData->sizelBottomFrame.cy;
            //pRectl->yTop -= fctrlData->sizelBottomFrame.cy;
          }
          if(fctrlData->bTop) {
            pRectl->yTop -=fctrlData->sizelTopFrame.cy;
            //pRectl->yBottom +=fctrlData->sizelTopFrame.cy;
          }
        }
    }
  else
    {
      //  FALSE: calculate the rectl of the frame;
      //  call default window procedure to subtract child frame
      //  controls from the rectangle's height;
      //  set the origin of the frame and increase it based upon the
      //  static text control's height

      return TRUE;

      if(fctrlData->bBottom) {
        pRectl->yBottom +=fctrlData->sizelBottomFrame.cy;
        // pRectl->yTop -= fctrlData->sizelBottomFrame.cy;
      }
      if(fctrlData->bTop)
        pRectl->yTop -=fctrlData->sizelTopFrame.cy;

      if(fctrlData->bLeft) {
        pRectl->xLeft +=fctrlData->sizelLeftFrame.cx;
      }
      if(fctrlData->bRight)
        pRectl->xRight -=fctrlData->sizelRightFrame.cx;

    }
  return TRUE;/* Everything's ok */
}


/**************************************************************/
/*                                                            */

/*                                                            */
/**************************************************************/
static ULONG cwFormatFrame(HWND hwndFolder, USHORT countSWP,PSWP pswp)
{
  /* countSWP:  number of standard framecontrols
     pswp:      Array of SWP describing the framecontrols.
                Since we added the number of our controls to the
                standard controls in the framecontrol there is
                space for the new controls.
                */
  ULONG rc;
  PSWP pswpClient=0, pswpFrameCtl=0;
  int iCount;
  FRAMECTRLDATA * fctrlData;
  int a;
  CNRINFO     CnrInfo;
  HWND hwndClient;
  
  /* The data of our framecontrols is stored in a structure.
     Get a pointer from the window words. */
  fctrlData=&fcd;
  if(!fctrlData)
    return 0;

  iCount=0;
  rc=0;
    
  for (a = 0; a < countSWP; a++)
    {
      /* Find the client window */
      if ( WinQueryWindowUShort( pswp[a].hwnd, QWS_ID ) == 0x8008 ) // FID_CLIENT
        {
          pswpClient=&pswp[a];

          /* Framecontrol at the top */                      
          if(fctrlData->bTop){
            pswpFrameCtl=&pswp[countSWP+iCount++];/* First free position */

            /* Adjust client and insert the new control into the array of SWPs */ 
             
            /* Decrease the height of the client to have place for the new control */
            pswp[a].cy -= fctrlData->sizelTopFrame.cy;

            /* Insert the HWND of the new control into the structure */
            pswpFrameCtl->hwnd=fctrlData->hwndCtlTop;
            /* The x-position is the same as that of the client */
            pswpFrameCtl->x  = pswpClient->x;
            /* The control is placed above the client */
            pswpFrameCtl->y  = pswpClient->y + pswpClient->cy;           
            /* The control is as wide as the client */
            pswpFrameCtl->cx = pswpClient->cx;
            /* Insert the height of the control */ 
            pswpFrameCtl->cy = fctrlData->sizelTopFrame.cy;
            /* Place the control on top of all windows */
            pswpFrameCtl->hwndInsertBehind = HWND_BOTTOM;
            /* Insert necessary flags */
            pswpFrameCtl->fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST | SWP_ZORDER | SWP_SHOW;

            /* Adjust position if flags are set. It's possible to place
               the control above the controls on the left and right or let these controls
               cover the full height of the folder. */
            if(fctrlData->bLeft){
              /* There is a frame control on the left side */
              if(!(fctrlData->ulFlagsLeft & FCTL_POSBELOW)) {
                pswpFrameCtl->x  += fctrlData->sizelLeftFrame.cx;
                pswpFrameCtl->cx -= fctrlData->sizelLeftFrame.cx; 
              }
            }
            /* framecontrol on the right side */
            if(!(fctrlData->ulFlagsRight & FCTL_POSBELOW) && fctrlData->bRight)
              pswpFrameCtl->cx -= fctrlData->sizelRightFrame.cx;

            if(!fctrlData->bCnrAdjusted) {
              hwndClient=pswpClient->hwnd;
              if(WinSendMsg(hwndClient,CM_QUERYCNRINFO,MPFROMP(&CnrInfo),MPFROMSHORT(sizeof(CnrInfo)))){
                if(fctrlData->bBottom)
                  CnrInfo.ptlOrigin.y-=fctrlData->sizelBottomFrame.cy;
                if(fctrlData->bTop)
                  CnrInfo.ptlOrigin.y-=fctrlData->sizelTopFrame.cy;
                //CnrInfo.ptlOrigin.y+=0;
                WinSendMsg(hwndClient,CM_SETCNRINFO,MPFROMP(&CnrInfo),MPFROMSHORT(CMA_PTLORIGIN));
              }
              fctrlData->bCnrAdjusted=TRUE;
            }
            /* One new control added */
            rc++;
          }/* end of if(fctrlData->bTop) */


          /* Bottom frame control */
          if(fctrlData->bBottom){
            pswpFrameCtl=&pswp[countSWP+iCount++];/* First free position */

            //            HlpWriteToTrapLog("2 Client: x: %d y: %d\n", pswpClient->x, pswpClient->y);

            pswpClient->cy -= fctrlData->sizelBottomFrame.cy; /* Reduce height of client */

            pswpFrameCtl->hwnd=fctrlData->hwndCtlBottom;                        
            pswpFrameCtl->x  = pswpClient->x;
            pswpFrameCtl->y  = pswpClient->y;
            pswpClient->y += fctrlData->sizelBottomFrame.cy;
                        
            pswpFrameCtl->cx = pswpClient->cx;
            pswpFrameCtl->cy = fctrlData->sizelBottomFrame.cy;
                        
            pswpFrameCtl->hwndInsertBehind = HWND_BOTTOM;
            pswpFrameCtl->fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST | SWP_ZORDER | SWP_SHOW;
                                                
            /* Adjust position if flags are set */
            if(fctrlData->bLeft){
              if((fctrlData->ulFlagsLeft & FCTL_POSABOVE)) {
                pswpFrameCtl->cx +=fctrlData->sizelLeftFrame.cx;
                pswpFrameCtl->x  -= fctrlData->sizelLeftFrame.cx;
              }
            }
            if(!(fctrlData->ulFlagsRight & FCTL_POSABOVE)&& fctrlData->bRight) {
              pswpFrameCtl->cx -= fctrlData->sizelRightFrame.cx; 
            }
            /* Adjust container view so items do not scroll out of view */
            /*   hwndClient=pswpClient->hwnd;
                 if(WinSendMsg(hwndClient,CM_QUERYCNRINFO,MPFROMP(&CnrInfo),MPFROMSHORT(sizeof(CnrInfo)))){
                 CnrInfo.ptlOrigin.y-=pswpFrameCtl->cy;
                 WinSendMsg(hwndClient,CM_SETCNRINFO,MPFROMP(&CnrInfo),MPFROMSHORT(CMA_PTLORIGIN));
                 };*/

            if(!fctrlData->bCnrAdjusted) {
              hwndClient=pswpClient->hwnd;
              if(WinSendMsg(hwndClient,CM_QUERYCNRINFO,MPFROMP(&CnrInfo),MPFROMSHORT(sizeof(CnrInfo)))){
                if(fctrlData->bBottom)
                  CnrInfo.ptlOrigin.y-=fctrlData->sizelBottomFrame.cy;
                if(fctrlData->bTop)
                  CnrInfo.ptlOrigin.y-=fctrlData->sizelTopFrame.cy;
                //CnrInfo.ptlOrigin.y+=0;
                WinSendMsg(hwndClient,CM_SETCNRINFO,MPFROMP(&CnrInfo),MPFROMSHORT(CMA_PTLORIGIN));
              }
              fctrlData->bCnrAdjusted=TRUE;
            }
          
            rc++; 
          }/* end of if(fctrlData->bBottom) */
        }
    }
  return rc;/* Return additional controls */
}


MRESULT EXPENTRY fnwpFrameProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
    {
    case WM_CALCVALIDRECTS:
      {
        MRESULT mr;
        PRECTL pRectl, pRectlNew;
        PSWP pswp;
        POINTL ptl;
        HWND hwndClient;
        SWP swp;
        
        pswp=(PSWP)mp2; 
        if(pswp->fl&SWP_SIZE) {
          pRectl=(PRECTL)mp1;        
          pRectlNew=pRectl;
          pRectlNew++;
          
          if(WinSendMsg(hwnd,WM_QUERYBORDERSIZE,(MPARAM)&ptl,0)) {
            pRectl->yBottom+=ptl.y;
            pRectl->yTop-=ptl.y;
            pRectl->xRight-=ptl.x;
            pRectl->xLeft+=ptl.x;
          }else {
            /* Ok, then use some defaults. Better than nothing... */
            pRectl->yBottom+=4;
            pRectl->yTop-=4;
            pRectl->xRight-=4;
            pRectl->xLeft+=4;
          }
          *pRectlNew=*pRectl;
          return (MRESULT)0;
        }
      }
    case WM_QUERYFRAMECTLCOUNT:
      /* The system asks for the total count of framecontrols.
         First we ask the old frame procedure for the number of standard
         controls. After that we add our controls to the count. */
      if(!g_pfnwpOrgFrameProc)
        return WinDefWindowProc(hwnd,msg,mp1,mp2);/* Error occured. Now we have a real problem... */
      
      return MRFROMSHORT(SHORT1FROMMR((*g_pfnwpOrgFrameProc)(hwnd,msg,mp1,mp2))
                         + cwNumAdditionalFrameControls(hwnd));

    case WM_CALCFRAMERECT:
      {
        MRESULT mr;

        if(!g_pfnwpOrgFrameProc)
          return WinDefWindowProc(hwnd,msg,mp1,mp2);/* Error occured. Now we have a real problem... */
                
        mr=(*g_pfnwpOrgFrameProc)(hwnd,msg,mp1,mp2);
        
        //     mp2:
        //
        //     TRUE:  Frame rectangle provided, calculate client
        //     FALSE: Client area rectangle provided, calculate frame
        
        /* Calculate new size.
           This is done in a member function of our folder class.
           We get the pointer to our folder object by querying the
           window ptr. of the hidden child window. */ 
        //    ((CWProgFolder*)WinQueryWindowPtr(WinWindowFromID(hwnd, ID_FOLDERSTOREWINDOW),0))
        // ->
        cwCalculateClient(hwnd, (PRECTL)mp1, SHORT1FROMMP( mp2));
        
        return mr;
      }

    case WM_FORMATFRAME:
      {
        SHORT countSwp;
        SWP  *sav;
      
        if(!g_pfnwpOrgFrameProc)
          return WinDefWindowProc(hwnd,msg,mp1,mp2);/* Error occured. Now we have a real problem... */

        /* Query the count of standard frame controls */
        countSwp = (int) (*g_pfnwpOrgFrameProc)(hwnd, msg, mp1, mp2);
        
        /*
         * Reformat the frame to move the client
         * over and make room for the framecontrols.
         * Add the new controls to the array of
         * frame controls.
         * After doing so add our own framecontrols
         * to the total number of framecontrols.
         * This is done in the member function of the class.
         */

        return MRFROMSHORT(countSwp+
                           cwFormatFrame(hwnd, countSwp,(PSWP)mp1));//Our Frame(s) added
      }
    case WM_DESTROY:
      {
        break;
      }

    default:
      break;
    }
  return (*g_pfnwpOrgFrameProc)(hwnd, msg, mp1, mp2);
}
