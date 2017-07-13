
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_GPI

#include <os2.h>
#include "os2me.h"
#include "common.h"
#include "mmres.h"

extern HAB globalHab;
extern BITMAPINFOHEADER2 bmpInfoHeader2;
extern HBITMAP hBitmap;

void showImageControls(HWND hwnd, BOOL bShow)
{
  /* Preview area */
  WinShowWindow(WinWindowFromID(hwnd, IDSR_IMGPREVIEW), bShow);
  /* Group box */
  WinShowWindow(WinWindowFromID(hwnd, IDGB_PREVIEW), bShow);
}

VOID DrawBitmap ( HWND hwnd )
{
    SWP    swp;
    POINTL aptl[4];
    HPS    hps;
    BOOL   bReturnCode;
    ULONG  ulHeight;
    ULONG  ulWidth;
    RECTL rectl;

    hps = WinBeginPaint ( hwnd,
                          0,
                          NULL);
    /*
     *  Get position of image frame
     */
    bReturnCode = WinQueryWindowPos ( hwnd, &swp);

    WinQueryWindowRect(hwnd, &rectl);
    WinFillRect(hps, &rectl, CLR_WHITE);
    /* Center image */
    ulHeight = bmpInfoHeader2.cy;
    ulWidth = bmpInfoHeader2.cx;
    if(ulWidth <=swp.cx && ulHeight <= swp.cy)
      {
        aptl[0].x=(swp.cx-ulWidth)/2;
        aptl[1].x=aptl[0].x+ulWidth;
        
        aptl[0].y=(swp.cy-ulHeight)/2;
        aptl[1].y=aptl[0].y+ulHeight;
      }
    else {
      float fWidth, fHeight, fRes;

      fWidth=(float)swp.cx/(float)ulWidth;
      fHeight=(float)swp.cy/(float)ulHeight;
      fRes=( fWidth>fHeight ? fHeight : fWidth);


      aptl[0].x=(swp.cx-ulWidth*fRes)/2;
      aptl[1].x=aptl[0].x+ulWidth*fRes;
      
      aptl[0].y=(swp.cy-ulHeight*fRes)/2;
      aptl[1].y=aptl[0].y+ulHeight*fRes;
    }
    
    aptl[2].x = 0;              // source lower left
    aptl[2].y = 0;
    
    aptl[3].x = ulWidth;   // source upper right
    aptl[3].y = ulHeight;

    /*
     *  Call GpiBitBlt and supply 4 aptl structures.  This tells
     *  it to stretch or compress the bitmap depending on what is
     *  in the aptl structures.  See above lines for their current
     *  settings.
     */

    WinDrawBitmap(hps, hBitmap, NULLHANDLE, (PPOINTL)aptl, 0, 0, DBM_NORMAL|DBM_STRETCH);

#if 0
    aptl[0].x=0;
    aptl[0].y=0;
    //aptl[1].x=100;
    //aptl[1].y=100;
    //       WinDrawBitmap(hps, hBitmap, NULLHANDLE, (PPOINTL)aptl, 0, 0, DBM_NORMAL/*|DBM_STRETCH*/);
#endif
    bReturnCode = WinEndPaint (hps);
}

MRESULT EXPENTRY bmpPreviewProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  switch (msg)
    {      
    case WM_PAINT:
      {
        if (!WinIsWindowVisible(hwnd))
          {
            return((MRESULT)NULL);
          }
        DrawBitmap(hwnd);
        return MRTRUE;
      }
    default:
      break;
    }
  return WinDefWindowProc( hwnd, msg, mp1, mp2);
}


#if 0
/*
  Load an image file using IO-Procs
  */
HBITMAP loadBitmap ( PSZ pszFileName, PBITMAPINFOHEADER2 pBMPInfoHeader2)
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
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
      return (0L);
    }

    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */
    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
      return ( 0L );
    }
    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
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
      //    showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_OPENFILEERROR, queryModuleHandle());
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
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR);
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

    /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
    *pBMPInfoHeader2=mmImgHdr.mmXDIBHeader.BMPInfoHeader2;

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
      //    showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOMEMERROR, queryModuleHandle());
      return(0L);
    }

    /* Create a device context */
    hdc=DevOpenDC(globalHab, OD_MEMORY,"*",0L, NULL, NULLHANDLE);
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
    
    ImageSize.cx = dwWidth;
    ImageSize.cy = dwHeight;

    hps = GpiCreatePS ( globalHab,
                        hdc,
                        &ImageSize,
                        PU_PELS | GPIT_NORMAL | GPIA_ASSOC );
    //                        PU_PELS | GPIF_DEFAULT | GPIT_MICRO | GPIA_ASSOC );
    if ( !hps )
      {
#ifdef DEBUG
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       "No HPS...",
                       "Open Image File",
                       (HMODULE) NULL,
                       (ULONG) MB_OK | MB_MOVEABLE |
                       MB_ERROR );
#endif   
        DevCloseDC(hdc);
        DosFreeMem(pRowBuffer);
        mmioClose (hmmio, 0L);
        return(0L);
      }

    //    GpiSelectPalette(hps, NULLHANDLE);
    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // will put all of the bits once we read them in.
    // ***************************************************
    hbm = GpiCreateBitmap ( hps,
                            pBMPInfoHeader2,
                            0L,
                            NULL,
                            NULL);

    if ( !hbm )
    {
#ifdef DEBUG
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                     "No HBITMAP...",
                     "Open Image File",
                     (HMODULE) NULL,
                     (ULONG) MB_OK | MB_MOVEABLE |
                     MB_ERROR );
#endif
      GpiDestroyPS(hps);
      DevCloseDC(hdc);
      DosFreeMem(pRowBuffer);
      ulReturnCode = mmioClose (hmmio, 0L);
      return(0L);
    }

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
                                          (PBITMAPINFO2) pBMPInfoHeader2);
    }

    /* Clean up */
    hbReturnCode = GpiSetBitmap ( hps,
                                  NULLHANDLE );
    ulReturnCode = mmioClose (hmmio, 0L);
    DosFreeMem(pRowBuffer);
    GpiDestroyPS(hps);
    DevCloseDC(hdc);

    return(hbm);
}
#endif
