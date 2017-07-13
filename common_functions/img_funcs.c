/*
  This file conatins functions for loading image files. 
 */

#define  INCL_PM
#define INCL_OS2MM
#define  INCL_MMIOOS2

#include <os2.h>
#include "os2me.h"
#include <string.h> /* for memset() */
#include <stdlib.h> /* for malloc() */
#include "sys_funcs.h"


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function loads a supported image file.      */
/* Every format for which MMOS2 has an IO procedure */
/* may be used here. In contrast to                 */
/* ImgLoadImageFile() a provided BMPINFOHEADER2     */
/* structure is filled with data.                   */
/*                                                  */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* HBITMAP hBitmap                                  */
/*                                                  */
/* Handle to the bitmap or NULL.                    */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* PSZ pszFileName input                            */
/*                                                  */
/* Name of the image file to load.                  */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* PBITMAPINFOHEADER2 pBMPInfoHeader2 in/out        */
/*                                                  */
/* Pointer to a BITMAPINFOHEADER2 structure.        */
/*                                                  */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* The caller must free the bitmap handle after     */
/* use.                                             */
/*                                                  */
/*!!*************************************************/
HBITMAP ImgLoadImageFileAndHeader( PSZ pszFileName, PBITMAPINFOHEADER2 pBMPInfoHeader2)
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
    HAB           hab;

    hab=WinQueryAnchorBlock(HWND_DESKTOP);

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
            return ( 0L );
    }
    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
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
    
    /*
    // ***************************************************
    // Create a memory presentation space that includes
    // the memory device context obtained above.
    // ***************************************************
    */
    ImageSize.cx = dwWidth;
    ImageSize.cy = dwHeight;

    hps = GpiCreatePS ( hab,
                        hdc,
                        &ImageSize,
                        PU_PELS | GPIT_NORMAL | GPIA_ASSOC );
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
       /*
    //    GpiSelectPalette(hps, NULLHANDLE);
    // ***************************************************
    // Create an uninitialized bitmap.  This is where we
    // will put all of the bits once we read them in.
    // ***************************************************
    */

    hbm = GpiCreateBitmap ( hps,
                            &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
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
    /****************************************************
      Select the bitmap into the memory device context.
      ***************************************************/
    hbReturnCode = GpiSetBitmap ( hps,
                                  hbm );
    /***************************************************************
      LOAD THE BITMAP DATA FROM THE FILE
      One line at a time, starting from the BOTTOM
      ****************************************************************/

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

    /* Clean up */
    hbReturnCode = GpiSetBitmap ( hps,
                                  NULLHANDLE );
    ulReturnCode = mmioClose (hmmio, 0L);
    DosFreeMem(pRowBuffer);
    GpiDestroyPS(hps);
    DevCloseDC(hdc);

    return(hbm);
}


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function loads a supported image file.      */
/* Every format for which MMOS2 has an IO procedure */
/* may be used here.                                */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* PSZ pszFileName input                            */
/*                                                  */
/* Name of the image file to load.                  */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* HBITMAP hBitmap                                  */
/*                                                  */
/* Handle to the bitmap of NULL.                    */
/*                                                  */
/*!!*************************************************/
HBITMAP ImgLoadImageFile (  PSZ pszFileName )
{
  BITMAPINFOHEADER2  bmpih2;

  return ImgLoadImageFileAndHeader ( pszFileName, &bmpih2 );
}



/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Build a BITMAPINFOHEADER2 for the given file.    */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* PSZ pszFileName input                            */
/*                                                  */
/* Name of the image file.                          */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* PBITMAPINFOHEADER2 pBMPInfoHeader2 in/out        */
/*                                                  */
/* Pointer to a BITMAPINFOHEADER2 structure.        */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* TRUE on success, FALSE otherwise                 */
/*                                                  */
/*!!*************************************************/
BOOL ImgGetBmpInfoHeader(PSZ pszFileName, PBITMAPINFOHEADER2  bmpih2)
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
        /* Disabled, because when copying via WPS we end here before the image is available  */
        return (0L);
      }

    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */
    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {
      return ( 0L );
    }

    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */
    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
      /* Disabled, because it fails for templates */
      return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }
#if 0
    if((pName=malloc(mmFormatInfo.lNameLength+1))!=NULLHANDLE)
      {
        LONG lBytesRead;
        mmioGetFormatName(&mmFormatInfo, pName, &lBytesRead, 0,0);
        if(procName && ulLength) {
          strncpy(procName, pName, ulLength-1);
          procName[ulLength-1]=0;
        }
        free(pName);
      }
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
      return (0L);
    }

    ulReturnCode = mmioQueryHeaderLength ( hmmio,
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

    memcpy(bmpih2, &mmImgHdr.mmXDIBHeader.BMPInfoHeader2,
           sizeof(BITMAPINFOHEADER2)+256*sizeof(RGB2) );
    ulReturnCode = mmioClose (hmmio, 0L);
    return TRUE;
}




