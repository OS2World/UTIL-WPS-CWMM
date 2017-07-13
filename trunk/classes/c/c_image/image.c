#define INCL_GPI
#define INCL_PM
#define INCL_MMIOOS2
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "os2me.h"
#include "mmioos2.h"

#include "pmdaytip.h"
HAB hab;

HBITMAP hBitmap;

char text[200];

HBITMAP loadBitmap ( PSZ pszFileName,
                     ULONG ulWidth, 
                     ULONG ulHeight, 
                     BOOL* pbQuitEarly)
{
    HBITMAP       hbm;
    MMIOINFO      mmioinfo;
    MMFORMATINFO  mmFormatInfo;
    HMMIO         hmmio;
    ULONG         ulImageHeaderLength;
    MMIMAGEHEADER mmImgHdr;
    ULONG         ulBytesRead;
    ULONG         dwNumRowBytes;
    PBYTE         pRowBuffer;
    ULONG         dwRowCount;
    SIZEL         ImageSize;
    ULONG         dwHeight, dwWidth;
    SHORT          wBitCount;
    FOURCC        fccStorageSystem;
    ULONG         dwPadBytes;
    ULONG         dwRowBits;
    ULONG         ulReturnCode;
    ULONG         dwReturnCode;
    HBITMAP       hbReturnCode;
    LONG          lReturnCode;
    FOURCC        fccIOProc;
    HDC           hdc;
    HPS           hps;
    HAB           hab=WinQueryAnchorBlock(HWND_DESKTOP);

    ulReturnCode = mmioIdentifyFile ( pszFileName,
                                      0L,
                                      &mmFormatInfo,
                                      &fccStorageSystem,
                                      0L,
                                      0L);
    /*
     *  If this file was NOT identified, then this function won't
     *  work, so return an error by indicating an empty bitmap.
     */
    if ( ulReturnCode == MMIO_ERROR )
    {
         return (0L);
    }
    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */
    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {
         WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                        "Image file could not be interpreted by any permanently or temporarily installed IO procedures.",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return ( 0L );
    }
    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
         WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                        "No IMAGE IO Procedures exist which can translate the data in the image file specified.",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
         return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }

    /* Clear out and initialize mminfo structure */
    memset ( &mmioinfo, 0L, sizeof ( MMIOINFO ) );
    mmioinfo.fccIOProc = fccIOProc;
    mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
    hmmio = mmioOpen ( (PSZ) pszFileName,
                       &mmioinfo,
                       MMIO_READ | MMIO_DENYWRITE | MMIO_NOIDENTIFY );
    if ( ! hmmio )
    {
         // If file could not be opened, return with error
         return (0L);
    }

    dwReturnCode = mmioQueryHeaderLength ( hmmio,
                                         (PLONG)&ulImageHeaderLength,
                                           0L,
                                           0L);
    if ( ulImageHeaderLength != sizeof ( MMIMAGEHEADER ) )
    {
         /* We have a problem.....possibly incompatible versions */
         ulReturnCode = mmioClose (hmmio, 0L);
         return (0L);
    }

    ulReturnCode = mmioGetHeader ( hmmio,
                                   &mmImgHdr,
                                   (LONG) sizeof ( MMIMAGEHEADER ),
                                   (PLONG)&ulBytesRead,
                                   0L,
                                   0L);

    if ( ulReturnCode != MMIO_SUCCESS )
    {
         /* Header unavailable */
         ulReturnCode = mmioClose (hmmio, 0L);
         return (0L);
    }

    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */
    dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    dwWidth = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
    wBitCount = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwNumRowBytes = dwRowBits >> 3;

    /*
     *  Account for odd bits used in 1bpp or 4bpp images that are
     *  NOT on byte boundaries.
     */
    if ( dwRowBits % 8 )
    {
         dwNumRowBytes++;
    }
    /*
     *  Ensure the row length in bytes accounts for byte padding.
     *  All bitmap data rows must are aligned on LONG/4-BYTE boundaries.
     *  The data FROM an IOProc should always appear in this form.
     */
    dwPadBytes = ( dwNumRowBytes % 4 );
    if ( dwPadBytes )
    {
         dwNumRowBytes += 4 - dwPadBytes;
    }

    /* Allocate space for ONE row of pels */
    if ( DosAllocMem( (PPVOID)&pRowBuffer,
                      (ULONG)dwNumRowBytes,
                      fALLOC))
    {
         ulReturnCode = mmioClose (hmmio, 0L);
         return(0L);
    }

    /* Create a device context */
    hdc=DevOpenDC(hab, OD_MEMORY,"*",0L, NULL, NULLHANDLE);
    if(hdc==NULLHANDLE)
      {
        DosFreeMem(pRowBuffer);
        mmioClose (hmmio, 0L);
        return(0L);
      }
    

    // ***************************************************
    // Create a memory presentation space that includes
    // the memory device context obtained above.
    // ***************************************************
    ImageSize.cx = ulWidth;
    ImageSize.cy = ulHeight;

    //    ImageSize.cx = dwWidth;
    //    ImageSize.cy = dwHeight;

    hps = GpiCreatePS ( hab,
                        hdc,
                        &ImageSize,
                        PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC );
    if ( !hps )
    {
         WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                        "No HPS...",
                        "Open Image File",
                        (HMODULE) NULL,
                        (ULONG) MB_OK | MB_MOVEABLE |
                                MB_ERROR );
      DosFreeMem(pRowBuffer);
      mmioClose (hmmio, 0L);
      return(0L);
    }

    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // will put all of the bits once we read them in.
    // ***************************************************

    /*!!!!*/
    //        mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cxResolution=ulWidth;
    //     mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cyResolution=ulHeight;

    mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx=ulWidth;
    mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy=ulHeight;

    /*!!!!*/
    hbm = GpiCreateBitmap ( hps,
                            &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
                            0L,
                            NULL,
                            NULL);

    if ( !hbm )
    {
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                     "No HBITMAP...",
                     "Open Image File",
                     (HMODULE) NULL,
                     (ULONG) MB_OK | MB_MOVEABLE |
                     MB_ERROR );

      DosFreeMem(pRowBuffer);
      ulReturnCode = mmioClose (hmmio, 0L);
      return(0L);
    }

    mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx=dwWidth;
    mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy=dwHeight;
        mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cxResolution=ulWidth;
     mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cyResolution=ulHeight;

    sprintf(text, "%d %d", mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx,
            mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy
            );
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
text,
                     "Open Image File",
                     (HMODULE) NULL,
                     (ULONG) MB_OK | MB_MOVEABLE |
                     MB_ERROR );

    // ***************************************************
    // Select the bitmap into the memory device context.
    // ***************************************************
    hbReturnCode = GpiSetBitmap ( hps,
                                  hbm );

    //***************************************************************
    //  LOAD THE BITMAP DATA FROM THE FILE
    //      One line at a time, starting from the BOTTOM
    //*************************************************************** */

    for ( dwRowCount = 0; dwRowCount < dwHeight; dwRowCount++ )
    {
         ulBytesRead = (ULONG) mmioRead ( hmmio,
                                          pRowBuffer,
                                          dwNumRowBytes );
         if ( !ulBytesRead )
         {
              break;
         }
         /*
          *  Allow context switching while previewing.. Couldn't get
          *  it to work. Perhaps will get to it when time is available...
          */
         lReturnCode = GpiSetBitmapBits ( hps,
                                          (LONG) dwRowCount,
                                          (LONG) 1,
                                          (PBYTE) pRowBuffer,
                                          (PBITMAPINFO2) &mmImgHdr.mmXDIBHeader.BMPInfoHeader2);
    }


    ulReturnCode = mmioClose (hmmio, 0L);
    DosFreeMem(pRowBuffer);

    return(hbm);
}



static MRESULT EXPENTRY transparencyExampleProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  RECTL rcl;
  HPS hps;
  HBITMAP hbm, hbmOld;
  BITMAPINFOHEADER2 *ptr;
  RGB2* rgb2;
  int a, r,g,b,r2,g2,b2;
  char *pvImage;
  POINTL ptl= {0};
  switch(msg)
    {
    case WM_PAINT:
      hps=WinBeginPaint(hwnd, NULLHANDLE, NULL);

      WinQueryWindowRect(hwnd, &rcl);
      WinDrawBitmap(hps, hBitmap, NULL, (PPOINTL)&ptl, 0,0,DBM_NORMAL);
#if 0  
      if(pbmpInfo2 && pvImageData) {
        hbm=GpiCreateBitmap(hps, (PBITMAPINFOHEADER2)pbmpInfo2, 0, NULLHANDLE, pbmpInfo2);
        hbmOld=GpiSetBitmap(hps,hbm);
        
        ptr=malloc(sizeof(BITMAPINFOHEADER2)+sizeof(RGB2)*256);
        if(ptr) {
          memcpy(ptr, pbmpInfo2, sizeof(BITMAPINFOHEADER2)+sizeof(RGB2)*256);
          
          GpiDrawBits(hps, pvImageData, (PBITMAPINFO2) ptr, 4L, ptl, ROP_SRCCOPY, BBO_IGNORE);
          free(ptr);
        }/* if(ptr) */
        GpiSetBitmap(hps, hbmOld);
        GpiDeleteBitmap(hbm);
      }/* End of if(pbmpInfo2 && pvImageData) */
      else
        WinFillRect(hps, &rcl, CLR_WHITE);
#endif
      //      WinFillRect(hps, &rcl, CLR_WHITE);
      
        WinEndPaint(hps);
      return (MRESULT)0;
    default:
      break;
    }
  return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

MRESULT EXPENTRY imageDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2) 
{
  SHORT sLen, sPos;
  MRESULT mr;
  RECTL rcl;
  HWND hwndPtr;
  ULONG ulValue;

    switch (msg)
    {
    case WM_INITDLG:
      {
        WinSubclassWindow(WinWindowFromID(hwnd, IDST_IMAGERECT),transparencyExampleProc);
        break;
      }
    case WM_DESTROY:
      break;
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
      {
      case DID_CANCEL:
        return (MRESULT)0;
      default:
        break;
      }
    break;
    case WM_CONTROL:
      {
        break;
      }
    default:
      break;
    }
    return WinDefDlgProc(hwnd, msg, mp1, mp2);
}


int main (int argc, char *argv[])
{
  HMQ  hmq;
  QMSG qmsg;
  char text[CCHMAXPATH];
  char title[CCHMAXPATH];
  short a;
  HWND hwndClient;
  ULONG result;
  ULONG rc;
  HINI hIni;
  char *chrPtr;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  

      if((hBitmap=loadBitmap("D:\\Arbeitsoberflaeche\\BIKES.nnn", 100, 100,NULL))!=NULLHANDLE) {
        if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, imageDialogProc, NULLHANDLE, 100, 0) == DID_ERROR )
          {
            WinDestroyMsgQueue( hmq );
            WinTerminate( hab );
            DosBeep(100,600);
          }   
      }
    }
    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);
  }
  return 0;
}
