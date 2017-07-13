/*
 * Copyright (c) Chris Wohlgemuth 2002 
 * All rights reserved.
 *
 * http://www.geocities.com/SiliconValley/Sector/5785/
 * http://www.os2world.com/cdwriting
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The authors name may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
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

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "mmioos2.h"

char pszFileName[]="";

int main()
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

#if 0
    CHAR          szBuffer[ sizeof( FOURCC ) + MAX_FILENAME_SIZE + 4 ];
    LONG          lNumIOProcs;
    MMFORMATINFO  mmFormatInfo;
    PMMFORMATINFO pmmFormatInfoArray;
    PSZ           pszFourccString;
    ULONG         ulReturnCode;
    LONG          lFormatsRead;
    LONG          index;
    LONG          lBytesRead;
#endif

    printf("in Main\n");
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
      printf("Can't identify file...\n");
         return (0L);
    }

    /*
     *  If mmioIdentifyFile did not find a custom-written IO proc which
     *  can understand the image file, then it will return the DOS IO Proc
     *  info because the image file IS a DOS file.
     */

    if( mmFormatInfo.fccIOProc == FOURCC_DOS )
    {

      printf("Image file could not be interpreted by any permanently or temporarily installed IO procedures.\n");
         return ( 0L );
    }

    /*
     *  Ensure this is an IMAGE IOproc, and that it can read
     *  translated data
     */

    if ( (mmFormatInfo.ulMediaType != MMIO_MEDIATYPE_IMAGE) ||
         ((mmFormatInfo.ulFlags & MMIO_CANREADTRANSLATED) == 0) )
    {
      printf("No IMAGE IO Procedures exist which can translate the data in the image file specified.");
         return (0L);
    }
    else
    {
         fccIOProc = mmFormatInfo.fccIOProc;
    }

    /* Clear out and initialize mminfo structure */
    
    memset ( &mmioinfo,
             0L,
             sizeof ( MMIOINFO ) );
    
    mmioinfo.fccIOProc = fccIOProc;
    mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
    
    hmmio = mmioOpen ( (PSZ) pszFileName,
                       &mmioinfo,
                       MMIO_READ | MMIO_DENYWRITE | MMIO_NOIDENTIFY );
    
    if ( ! hmmio )
      {
        /* If file could not be opened, return with error */
        printf("File can't be opened!\n");
        return (0L);
    }

    dwReturnCode = mmioQueryHeaderLength ( hmmio,
                                         (PLONG)&ulImageHeaderLength,
                                           0L,
                                           0L);

    if ( ulImageHeaderLength != sizeof ( MMIMAGEHEADER ) )
    {
         /* We have a problem.....possibly incompatible versions */
      printf("Error in mmioQueryHeaderLength ()\n");
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
      printf("Can't get header!\n");
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

         printf("No memory!!\n");
         return(0L);
    }


  printf("Done\n");
  return 0;
}




