/*
 * (C) Chris Wohlgemuth 2001-2003
 *
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
#define INCL_GPI
#define INCL_PM
#define INCL_MMIOOS2
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmioos2.h"
#include "mmres.h"
#include "sys_funcs.h"

//#define DEBUG

extern HAB globalHab;
ULONG showMsgBox(ULONG ulIDTitle, ULONG ulIDText, HMODULE hModule);
void writeLog(char* chrFormat, ...);
HMODULE queryModuleHandle(void);
void HlpWriteToTrapLog(const char* chrFormat, ...);
ULONG _checkFileSize(char* chrFileName);

BOOL getBmpInfoHeader(PBITMAPINFOHEADER2  bmpih2, PSZ pszFileName, char* procName, ULONG ulLength, BOOL *bNoIOProcAvailable)
{
    MMIOINFO      mmioinfo;
    MMFORMATINFO  mmFormatInfo;
    HMMIO         hmmio;
    ULONG         ulImageHeaderLength;
    MMIMAGEHEADER mmImgHdr;
    FOURCC        fccStorageSystem;
    FOURCC        fccIOProc;
    ULONG         ulReturnCode;
    ULONG ulBytesRead;
    char *pName;
    
    procName[0]=0;

#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(), trying to identify file: %s\n",
                      pszFileName);
#endif

    /* Check file size */
    if(SysQueryFileSize(pszFileName)==0)
      return FALSE; /* File is empty at the moment, so return without reading. */
    
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
#ifdef DEBUG
        HlpWriteToTrapLog("In getBmpInfoHeader(), mmioIdentifyFile failed, file: %s\n",
                 pszFileName);
#endif
        /* Disabled, because when copying via WPS we end here before the image is available  */
        //   showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
        //*bNoIOProcAvailable=TRUE;
        return (0L);
      }

#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(), file identified\n");
#endif

    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */
    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {
#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(),  mmFormatInfo.fccIOProc == FOURCC_DOS, file: %s\n",
             pszFileName);
#endif

    //    showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
    *bNoIOProcAvailable=TRUE;
      return ( 0L );
    }

    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(),  mmFormatInfo check, file: %s\n",
             pszFileName);
#endif
    /* Disabled, because it fails for templates */
    //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
    *bNoIOProcAvailable=TRUE;
      return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }

#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(), trying to get IO-Proc-Name...\n");
#endif

    if((pName=malloc(mmFormatInfo.lNameLength+1))!=NULLHANDLE)
      {
        LONG lBytesRead;
        mmioGetFormatName(&mmFormatInfo, pName, &lBytesRead, 0,0);
        strncpy(procName, pName, ulLength-1);
        free(pName);
      }

#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(), trying to open the file...\n");
#endif

    /* Clear out and initialize mminfo structure */
    memset ( &mmioinfo, 0L, sizeof ( MMIOINFO ) );
    mmioinfo.fccIOProc = fccIOProc;
    mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
    /*!!!!!!!!!!!!!!!!!!!!!!*/


    hmmio = mmioOpen ( (PSZ) pszFileName,
                       &mmioinfo,
                       MMIO_READ | MMIO_DENYWRITE | MMIO_NOIDENTIFY );
   
    if ( ! hmmio )
    {
      // If file could not be opened, return with error
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_OPENFILEERROR, queryModuleHandle());
      return (0L);
    }

#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(), now querying header length...\n");
#endif

    ulReturnCode = mmioQueryHeaderLength ( hmmio,
                                         (PLONG)&ulImageHeaderLength,
                                           0L,
                                           0L);
    if ( ulImageHeaderLength != sizeof ( MMIMAGEHEADER ) )
    {
         /* We have a problem.....possibly incompatible versions */
         ulReturnCode = mmioClose (hmmio, 0L);
         //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
         return (0L);
    }

#ifdef DEBUG
    HlpWriteToTrapLog("In getBmpInfoHeader(), now querying header...\n");
#endif

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
      //   showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR, queryModuleHandle());
      return (0L);
    }

    memcpy(bmpih2, &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
           sizeof(BITMAPINFOHEADER2)+256*sizeof(RGB2) );
    ulReturnCode = mmioClose (hmmio, 0L);
    return TRUE;
}



HBITMAP loadBitmap ( PSZ pszFileName, PBITMAPINFOHEADER2 pBMPInfoHeader2)
{
    HBITMAP       hbm;
    MMIOINFO      mmioinfo;
        MMFORMATINFO  mmFormatInfo;
    HMMIO         hmmio;
    // ULONG         ulImageHeaderLength;
        //        MMIMAGEHEADER mmImgHdr;
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

#if 0
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
      //      showMsgBox(IDSTR_CWIMAGETITLE, IDSTR_NOIMGIOPROCERROR);
      return (0L);
    }
#endif
    /*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
#if 0
    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */
    dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    dwWidth = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
    wBitCount = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwNumRowBytes = dwRowBits >> 3;
#endif

    /*
     *  Determine the number of bytes required, per row.
     *      PLANES MUST ALWAYS BE = 1
     */
    dwHeight = pBMPInfoHeader2->cy;
    dwWidth = pBMPInfoHeader2->cx;
    wBitCount = pBMPInfoHeader2->cBitCount;
    dwRowBits = dwWidth * pBMPInfoHeader2->cBitCount;
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

HBITMAP createNewBitmap ( HBITMAP hbm,
                          PBITMAPINFOHEADER2 pBmpInfoHeader2,
                          ULONG ulWidth, 
                          ULONG ulHeight, 
                          BOOL* pbQuitEarly)
{
  HBITMAP       hbmTarget;
  SIZEL         ImageSize;
  HBITMAP       hbReturnCode;
  HDC           hdc;
  HPS           hps;
  BITMAPINFOHEADER2 bmpih2;
  POINTL aptl[4];

  if(!ulWidth || !ulHeight)
    return 0L;

    /* Create a device context */
    hdc=DevOpenDC(globalHab, OD_MEMORY,"*",0L, NULL, NULLHANDLE);
    if(hdc==NULLHANDLE)
      {
        return(0L);
      }
    
    // ***************************************************
    // Create a memory presentation space that includes
    // the memory device context obtained above.
    // ***************************************************
    ImageSize.cx = ulWidth;
    ImageSize.cy = ulHeight;
    hps = GpiCreatePS ( globalHab,
                        hdc,
                        &ImageSize,
                        PU_PELS | GPIT_NORMAL | GPIA_ASSOC );
    if ( !hps )
      {
#ifdef DEBUG
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       "No HPS...",
                       "Create new bitmap",
                       (HMODULE) NULL,
                       (ULONG) MB_OK | MB_MOVEABLE |
                       MB_ERROR );
#endif   
        DevCloseDC(hdc);
        return(0L);
      }
    //    GpiSelectPalette(hps, NULLHANDLE);

    /* Now scale the bitmap */
    memcpy(&bmpih2, pBmpInfoHeader2, sizeof(BITMAPINFOHEADER2));    

    bmpih2.cx=ulWidth;
    bmpih2.cy=ulHeight;
    bmpih2.cbImage=(((ulWidth*(1<<bmpih2.cPlanes)*(1<<bmpih2.cBitCount))+31) /32)*ulHeight;
    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // ***************************************************

    hbmTarget = GpiCreateBitmap ( hps,
                                  &bmpih2,
                                  0L,
                                  NULL,
                                  NULL);
    if ( !hbmTarget )
      {
#ifdef DEBUG
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                     "No target HBITMAP...",
                     "Create new bitmap",
                     (HMODULE) NULL,
                     (ULONG) MB_OK | MB_MOVEABLE |
                     MB_ERROR );
#endif
      GpiDestroyPS(hps);
      DevCloseDC(hdc);
      return(0L);
    }

    /* Blit it */
    hbReturnCode = GpiSetBitmap ( hps,
                                  hbmTarget );
    
    /*GpiBltBit*/
    aptl[0].x=0;
    aptl[0].y=0;
    aptl[1].x=ulWidth;
    aptl[1].y=ulHeight;
    aptl[2].x=0;
    aptl[2].y=0;
    aptl[3].x=pBmpInfoHeader2->cx;
    aptl[3].y=pBmpInfoHeader2->cy;

    GpiWCBitBlt(hps, hbm,4L, aptl, ROP_SRCCOPY, BBO_IGNORE);

    hbReturnCode = GpiSetBitmap( hps, NULLHANDLE );
    GpiDestroyPS(hps);
    DevCloseDC(hdc);
    return(hbmTarget);
}



