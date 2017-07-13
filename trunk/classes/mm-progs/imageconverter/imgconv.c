/*
 * imgconv.c (C) Chris Wohlgemuth 2002-2003
 *
 * This helper converts image files. It is called by the image classes
 * when the 'convert' menu is chosen.
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
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WIN
#define INCL_OS2MM
#define INCL_MMIOOS2
#define INCL_GPI
#define INCL_PM

#include <os2.h>

#include <sys\types.h>
#include <sys\stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "os2me.h"
#include "mmioos2.h"
#include "common.h"
#include "mmres.h"
#include "mmprogs_defaults.h"

#include "sys_funcs.h"

#ifndef MMIOM_QUERYIMAGE
   #define MMIOM_QUERYIMAGE         MMIOM_START + 39
   #define MMIOM_QUERYIMAGECOUNT    MMIOM_START + 40
   #define MMIOM_SETIMAGE           MMIOM_START + 41
#endif

#define MSG_CONVERTDONE    1L
#define MSG_CONVERTPERCENT 2L
#define MSG_CONVERTERROR   3L

#define ID_TIMER 1
#define TIMER_DELAY        200

#define CONVERTBUFFERSIZE  500000
#define NUMPARAMS  3

/* argv[0]: progname
 * argv[1]: imgfile
 * argv[2]: IO proc name
 */

//#define DEBUG

void HlpSendCommandToObject(char* chrObject, char* command);
BOOL percentRegisterBarClass(void);
BOOL IniSaveWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniRestoreWindowPos(char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniSaveWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL IniRestoreWindowClrs(char * chrIniFile, char* chrApp , HWND hwnd);
BOOL HlpBuildMMProgIniFileName(char* chrProgname, char * chrBuffer, ULONG ulBufferSize);
HMODULE queryResModuleHandle(char *chrExePath);
void freeResHandle();
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);

char logName[]="convert.log";

extern SWP swpWindow;
BOOL bHaveWindowPos=FALSE;

char chrTargetName[CCHMAXPATH]={0};
char chrSourceName[CCHMAXPATH]={0};
char chrProcName[CCHMAXPATH]={0};
char chrIniFile[CCHMAXPATH];

int numArgs;
char* params[NUMPARAMS];

#define NUMIOPROCS 1000
int iIoProc;                  /* Position of IO-Proc in global data area of MMOS2. 0 based */
int iPrivIOProc[NUMIOPROCS]; /* Number of possible IO-Procs. I'm lazy here and use a static array.
                                If there ever will be more than 1000 procs there'll be a problem... */


TID tidThread=0;
BOOL bBreak=FALSE;
PMMFORMATINFO pMemFormatInfo=NULLHANDLE;
HMODULE RESSOURCEHANDLE=0;
BOOL bNoProcGiven=FALSE;
HAB globalHab;
/* BMP info for preview */
BITMAPINFOHEADER2 bmpInfoHeader2;
HBITMAP hBitmap;

void pmUsage();

//#define DEBUG

#ifdef DEBUG
void HlpWriteToTrapLog(const char* chrFormat, ...);
#endif

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


/*
  Create a BMP of another size from an already loaded BMP.
*/
HBITMAP createNewBitmap ( HBITMAP hbm,
                          PBITMAPINFOHEADER2 pBmpInfoHeader2,
                          HWND hwnd)
                          //                          ULONG ulWidth, 
                          //     ULONG ulHeight)
{
  HBITMAP       hbmTarget;
  SIZEL         ImageSize;
  HBITMAP       hbReturnCode;
  HDC           hdc;
  HPS           hps;
  BITMAPINFOHEADER2 bmpih2;
  POINTL aptl[4];
  ULONG ulWidth, ulHeight;
  SWP swp;

  /*
   *  Get position of image frame
   */
  WinQueryWindowPos ( hwnd, &swp);
  
  /* Image size */
  
  ulHeight = bmpInfoHeader2.cy;
  ulWidth = bmpInfoHeader2.cx;

  if(ulWidth <=swp.cx && ulHeight <= swp.cy)
    {
      aptl[0].x=0;
      aptl[1].x=aptl[0].x+ulWidth;
      
      aptl[0].y=0;
      aptl[1].y=aptl[0].y+ulHeight;
    }
  else {
    float fWidth, fHeight, fRes;
    
    fWidth=(float)swp.cx/(float)ulWidth;
    fHeight=(float)swp.cy/(float)ulHeight;
    fRes=( fWidth>fHeight ? fHeight : fWidth);
    
    
    aptl[0].x=0;
    aptl[1].x=aptl[0].x+ulWidth*fRes;
    
      aptl[0].y=0;
      aptl[1].y=aptl[0].y+ulHeight*fRes;
    }
    
    aptl[2].x = 0;              // source lower left
    aptl[2].y = 0;
    
    aptl[3].x = ulWidth;   // source upper right
    aptl[3].y = ulHeight;

#if 0
    sprintf(text, "BMP: %d %d, Window: %d %d , new: %d %d", ulWidth, ulHeight, swp.cx, swp.cy,
            aptl[1].x-aptl[0].x, aptl[1].y-aptl[0].y);
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       text,
                       "Create new bitmap",
                       (HMODULE) NULL,
                       (ULONG) MB_OK | MB_MOVEABLE |
                       MB_ERROR );
#endif

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
        DevCloseDC(hdc);
        return(0L);
      }

    /* Now scale the bitmap */
    memcpy(&bmpih2, pBmpInfoHeader2, sizeof(BITMAPINFOHEADER2));    

    bmpih2.cx=aptl[1].x-aptl[0].x;
    bmpih2.cy=aptl[1].y-aptl[0].y;

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
      GpiDestroyPS(hps);
      DevCloseDC(hdc);
      return(0L);
    }

    /* Blit it */
    hbReturnCode = GpiSetBitmap ( hps, hbmTarget );

    GpiWCBitBlt(hps, hbm,4L, aptl, ROP_SRCCOPY, BBO_IGNORE);

    hbReturnCode = GpiSetBitmap( hps, NULLHANDLE );
    GpiDestroyPS(hps);
    DevCloseDC(hdc);
    pBmpInfoHeader2->cbFix=sizeof(BITMAPINFOHEADER2);
    GpiQueryBitmapInfoHeader(hbmTarget, pBmpInfoHeader2);

#if 0
    sprintf(text, "BMP: %d %d, Window: %d %d , new: %d %d", ulWidth, ulHeight, swp.cx, swp.cy,
            pBmpInfoHeader2->cx, pBmpInfoHeader2->cy);
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,
                       text,
                       "Create new bitmap",
                       (HMODULE) NULL,
                       (ULONG) MB_OK | MB_MOVEABLE |
                       MB_ERROR );
#endif
    return(hbmTarget);
}



BOOL createTargetName(char *sourceName, char *chrExt)
{
  char *textPtr;

  strcpy(chrTargetName, sourceName);
  if((textPtr=strrchr(chrTargetName, '.'))!=NULLHANDLE)
    *textPtr=0;
  else
    textPtr=chrTargetName;
  strcat(textPtr,".");
  strcat(textPtr, chrExt);
  
  return TRUE;
}


BOOL insertIOProcItems( HWND hwndDrop )
{
  //    CHAR          szBuffer[ sizeof( FOURCC ) + CCHMAXPATH + 4 ];
    MMFORMATINFO  mmFormatInfo;
    PMMFORMATINFO pmmFormatInfoArray;
    ULONG         ulReturnCode;
    LONG          lFormatsRead;
    LONG          index;
    LONG          lBytesRead;
    LONG lNumIOProcs;
    int sIdx;
    memset( &mmFormatInfo,
            '\0',
            sizeof(MMFORMATINFO) );
    
    mmFormatInfo.ulMediaType |= MMIO_MEDIATYPE_IMAGE;
    mmFormatInfo.ulFlags|=MMIO_CANWRITETRANSLATED;    
    ulReturnCode = mmioQueryFormatCount ( &mmFormatInfo,
                                          &lNumIOProcs,
                                          0,
                                          0 );
    
    if( ulReturnCode != MMIO_SUCCESS )
      {
        /*
         * Error - mmioQueryFormatCount failed.
         */
        return FALSE;
      }

    /*
     * Allocate enough memory for n number of FormatInfo blocks
     */
    pmmFormatInfoArray = (PMMFORMATINFO) malloc (lNumIOProcs * sizeof( MMFORMATINFO ) );
    pMemFormatInfo=pmmFormatInfoArray;
    if( pmmFormatInfoArray == NULL )
      {
        /*
         * Could not allocate enough memory for mmFormatInfo array.
         */
        return FALSE;
      }
    
    /*
     * call mmioGetFormats to get info on the formats supported.
     */
    ulReturnCode = mmioGetFormats( &mmFormatInfo,
                                   lNumIOProcs,
                                   pmmFormatInfoArray,
                                   &lFormatsRead,
                                   0,
                                   0 );
    if( ulReturnCode != MMIO_SUCCESS )
      {
        /*
         *  mmioGetFormats failed.
         */
        free(pmmFormatInfoArray);
        return FALSE;
      }
    
    if( lFormatsRead != lNumIOProcs )
      {
        /*
         * Error in MMIO - number of formats read in by
         * mmioGetFormats is not equal to number of formats
         * found by mmioQueryFormatCount.
         */
        free(pmmFormatInfoArray);
        return FALSE;
      }


    for ( index = 0, sIdx=0; index <lNumIOProcs; index++ )
      {
        char szName[CCHMAXPATH];

        mmioGetFormatName(pmmFormatInfoArray, szName, &lBytesRead, 0L, 0L);        
        /* Insert NULL string terminator */
        *( szName + lBytesRead ) = (CHAR)NULL;


        /* Compressed TIF is not supported because the Warp 4 IO-Procs are
           broken. */
        if(pmmFormatInfoArray->fccIOProc!=mmioStringToFOURCC("TFMC",MMIO_TOUPPER) &&
           pmmFormatInfoArray->fccIOProc!=mmioStringToFOURCC("TFIC",MMIO_TOUPPER) &&
           (pmmFormatInfoArray->ulFlags & MMIO_CANWRITETRANSLATED))
          {
            /* Insert into list box */
            WinSendMsg(hwndDrop, LM_INSERTITEM,(MPARAM)LIT_END,
                       (MPARAM)szName);

#ifdef DEBUG
        HlpWriteToTrapLog("------ %d private idx: %d, IO-Proc: %s %s\n", index, sIdx, 
                          pmmFormatInfoArray->szDefaultFormatExt,
                          szName);
#endif
            
            iPrivIOProc[sIdx]=index;

            /* Set text field */
            if(bNoProcGiven) {
              WinSetWindowText(hwndDrop, szName);
              createTargetName(chrSourceName, pmmFormatInfoArray->szDefaultFormatExt);
              iIoProc=iPrivIOProc[sIdx];
              bNoProcGiven=FALSE;
            }
            else {
              if(!stricmp(szName, chrProcName)) {
                WinSetWindowText(hwndDrop, szName);
                /* Create target name */
                createTargetName(chrSourceName, pmmFormatInfoArray->szDefaultFormatExt);
                iIoProc=iPrivIOProc[sIdx];
              }
            }
            sIdx++;
          }
        /*
         *  advance to next entry in mmFormatInfo array
         */        
        pmmFormatInfoArray++;
      }
    return TRUE;
}

BOOL DoConvert(HWND hwnd,
               PSZ    pszSource,          // Source file name
               PSZ    pszTarget,
               FOURCC fccTargetIOProc)    //
{
  MMIOINFO      mmioinfoSource;
  MMIOINFO      mmioinfoTarget;
  HMMIO         hmmioSource;
  HMMIO         hmmioTarget;
  ULONG         ulImageHeaderLength;
  MMIMAGEHEADER mmImgHdr;
  ULONG         ulBytesRead;
  ULONG         dwNumRowBytes;
  PBYTE         pRowBuffer;
  ULONG         dwRowCount;
  ULONG         dwWidth;
  ULONG         dwHeight;
  ULONG         dwPadBytes;
  ULONG         dwRowBits;
  LONG          rc;
  LONG          rcSrcQueryCount = 0;
  LONG          rcTrgQueryCount = 0;
  LONG          rcTrgSetImage   = 0;
  ULONG         iIndex, iCount, iCount2;

  MMFORMATINFO  mmFormatInfo;  
  FOURCC fccSourceIOProc;
  FOURCC        fccStorageSystem;

  rc = mmioIdentifyFile ( pszSource,
                          0L,
                          &mmFormatInfo,
                          &fccStorageSystem,
                          0L,
                          0L);

    /*
     *  If this file was NOT identified, then this function won't
     *  work, so return an error by indicating an empty bitmap.
     */

    if ( rc == MMIO_ERROR )
    {
         return (0L);
    }
    fccSourceIOProc=mmFormatInfo.fccIOProc;

  /*******************************/
  /* Set up/open the SOURCE file */
  /*******************************/
  memset (&mmioinfoSource, 0L, sizeof (MMIOINFO));
  mmioinfoSource.fccIOProc = fccSourceIOProc;
  mmioinfoSource.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
  
  hmmioSource = mmioOpen ((PSZ)pszSource, &mmioinfoSource,
                          MMIO_READ | MMIO_DENYWRITE
                          | MMIO_NOIDENTIFY);
  
  if (!hmmioSource)
    return (FALSE);
  
  /*******************************/
  /* Set up/open the TARGET file */
  /*******************************/

  memset (&mmioinfoTarget, 0L, sizeof (MMIOINFO));
  mmioinfoTarget.fccIOProc = fccTargetIOProc;
  mmioinfoTarget.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
  
  hmmioTarget = mmioOpen ((PSZ)pszTarget,
                          &mmioinfoTarget,
                          MMIO_CREATE | MMIO_WRITE |
                          MMIO_DENYWRITE | MMIO_NOIDENTIFY);
  
  if (!hmmioTarget)
    {
      mmioClose (hmmioSource, 0L);
      return (FALSE);
    }

#ifdef DEBUG
        HlpWriteToTrapLog("Target opened.\n");
#endif

  // find out if source has multiple image support
  rcSrcQueryCount = mmioSendMessage(hmmioSource, MMIOM_QUERYIMAGECOUNT, (LONG)&iCount, (LONG)0);
  if (rcSrcQueryCount) iCount = 1;
  
  // find out if the target has multiple image support
  rcTrgQueryCount = mmioSendMessage(hmmioTarget, MMIOM_QUERYIMAGECOUNT, (LONG)&iCount2, (LONG)0);
  
  for (iIndex=0; iIndex<iCount; iIndex++) { /* loop through known images */

    if (!rcSrcQueryCount && !rcTrgQueryCount) {  /* if Both support images */
      
      /* Determine if the target can write arrays, and if not */
      /* then write the the default image from the source     */

      if (rcTrgSetImage && iIndex > 0) break;  /* Target Can't Write array */
      
      /* Now, determine if the target can write arrays */
      rcTrgSetImage = mmioSendMessage (hmmioTarget, MMIOM_SETIMAGE, (LONG)iIndex, (LONG)0);
      
      if (!rcTrgSetImage) mmioSendMessage (hmmioSource, MMIOM_SETIMAGE, (LONG)iIndex, (LONG)0);
      
    } else if (!rcSrcQueryCount) {      /* Source does but target doesn't */
      /* Use the default image from source to copy to target */
      /* so do set the index of the first, let it default    */
      /* get the base photo cd image (2 of 5)                */
      if (iIndex > 0) break;
    } else if (!rcTrgQueryCount) {      /* Target does but source doesn't */
      /* Use the only image to do a default write to target */
    } else {
      /* neither do: just write one image from here to there */
    }

    /****************************/
    /* Obtain the SOURCE HEADER */
    /****************************/
    mmioQueryHeaderLength (hmmioSource, (PLONG)&ulImageHeaderLength, 0L, 0L);
    if (ulImageHeaderLength != sizeof (MMIMAGEHEADER))
      /* We have a problem.....possibly incompatible versions */
      {
        mmioClose (hmmioSource, 0L);
        mmioClose (hmmioTarget, 0L);
        return (FALSE);
      }

    rc = (LONG)mmioGetHeader (hmmioSource, &mmImgHdr,
                              (LONG)sizeof (MMIMAGEHEADER), (PLONG)&ulBytesRead,
                              0L, 0L);
    
    if (rc != MMIO_SUCCESS)
      /* Header unavailable */
      {
        mmioClose (hmmioSource, 0L);
        mmioClose (hmmioTarget, 0L);
        return (FALSE);
      }
    

    /*************************/
    /* Set the TARGET HEADER */
    /*************************/
    mmioQueryHeaderLength (hmmioTarget, (PLONG)&ulImageHeaderLength, 0L, 0L);
    if (ulImageHeaderLength != sizeof (MMIMAGEHEADER))
      {
        /* We have a problem.....possibly incompatible versions */
        mmioClose (hmmioSource, 0L);
        mmioClose (hmmioTarget, 0L);
        return (FALSE);
      }

    
    /* Use the SAME data as came from the SOURCE FILE.  It must be
       compatible with the OS/2 bitmaps, etc.   */
    rc = (LONG)mmioSetHeader (hmmioTarget, &mmImgHdr,
                              (LONG)sizeof (MMIMAGEHEADER), (PLONG)&ulBytesRead,
                              0L, 0L);
    
    if (rc != MMIO_SUCCESS)
      /* Header unavailable */
      {
        mmioClose (hmmioSource, 0L);
        mmioClose (hmmioTarget, 0L);
        return (FALSE);
      }

#ifdef DEBUG
        HlpWriteToTrapLog("Target header set.\n");
#endif
    
    /* Determine the number of bytes required, per row */
    /*      PLANES MUST ALWAYS BE = 1 */
    dwHeight = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cy;
    dwWidth  = mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cx;
    dwRowBits = dwWidth * mmImgHdr.mmXDIBHeader.BMPInfoHeader2.cBitCount;
    dwNumRowBytes = dwRowBits >> 3;
    
    /* Account for odd bits used in 1bpp or 4bpp images that are NOT on byte boundaries. */
    if (dwRowBits % 8)
      dwNumRowBytes++;
    
    /* Ensure the row length in bytes accounts for byte padding.  All bitmap data rows
       must are aligned on LONG/4-BYTE boundaries.   The data FROM an IOProc
       should always appear in this form. */
    dwPadBytes = (dwNumRowBytes % 4);
    if (dwPadBytes)
      dwNumRowBytes += 4 - dwPadBytes;

    /* Allocate space for one row */
    if (DosAllocMem((PVOID)&pRowBuffer, (ULONG)dwNumRowBytes, fALLOC))
      {
        mmioClose (hmmioSource, 0L);
        mmioClose (hmmioTarget, 0L);
        return(FALSE);
      }

    for (dwRowCount = 0;
         dwRowCount < dwHeight;
         dwRowCount++)
      {
        ulBytesRead = (ULONG)mmioRead (hmmioSource, pRowBuffer, dwNumRowBytes);

        if (ulBytesRead) {
          LONG lWritten;
          lWritten=mmioWrite (hmmioTarget, pRowBuffer, (ULONG)ulBytesRead);
#ifdef DEBUG
        HlpWriteToTrapLog("ulBytesRead: %d, lWritten: %d.\n", ulBytesRead, lWritten);
#endif

        }
        else
          break;

        if(dwHeight)
          WinPostMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTPERCENT), MPFROMLONG(dwRowCount*100/dwHeight));
      }
  }
  WinPostMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTPERCENT), MPFROMLONG(100));
  mmioClose (hmmioTarget, 0L);
  mmioClose (hmmioSource, 0L);
  DosFreeMem(pRowBuffer);
  
  return(TRUE);
}

void _Optlink convertThreadFunc (void *arg)
{
  HAB  hab;
  HMQ  hmq;
  HWND hwnd=(HWND)arg;

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {

      DoConvert(hwnd,
                chrSourceName,
                chrTargetName,  pMemFormatInfo[iIoProc].fccIOProc);
      
      

      WinPostMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTDONE), 0);      
      DosSleep(1000);
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  tidThread=0;
  bBreak=FALSE;
}

/* Start the convert thread. hwnd is the HWND of our main dialog */
void convertImageFile(HWND hwnd)
{
  tidThread=_beginthread(convertThreadFunc,NULL,  65535*32, (void*)hwnd); //Fehlerbehandlung fehlt
  if(tidThread==-1) {
    DosBeep(100, 500);
    tidThread=0;
  }
}

VOID DrawBitmap ( HWND hwnd )
{
    SWP    swp;
    POINTL aptl[4];
    HPS    hps;
    BOOL   bReturnCode;
    ULONG  ulHeight;
    ULONG  ulWidth;

    hps = WinBeginPaint ( hwnd,
                          0,
                          NULL);
    /*
     *  Get position of image frame
     */
    bReturnCode = WinQueryWindowPos ( hwnd, &swp);

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
/* This Proc handles the on-the-fly data CD writing */
MRESULT EXPENTRY decodeStatusDialogProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  char text[CCHMAXPATH*4 +10];
  //  char title[CCHMAXPATH*4];
  SWCNTRL swctl;
  PID pid;
  int iPercent;

  switch (msg)
    {      
    case WM_INITDLG:
#if 0
      sprintf(text,"1: %s, 2: %s, 3: %s 4: %s 5: %s 6: %s",params[1],params[2],params[3],
              params[4], params[4],params[4]);
      WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, text,
                     params[4],
                     0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
#endif

      WinSendMsg(WinWindowFromID(hwnd,IDST_IMGCONVERTNAME),EM_SETTEXTLIMIT,MPFROMSHORT((SHORT)CCHMAXPATH),0);
      
      /* Filename */
      WinSetWindowText(WinWindowFromID(hwnd,IDST_IMGCONVERTNAME), chrSourceName);
      
      /* Set dialog font to WarpSans for Warp 4 and above */
      if(SysQueryOSRelease()>=40) {
        WinSetPresParam(hwnd,
                        PP_FONTNAMESIZE,(ULONG)sizeof(DEFAULT_DIALOG_FONT),
                        DEFAULT_DIALOG_FONT );
      }

      /* Subclass preview area */
      WinSubclassWindow(WinWindowFromID(hwnd, IDSR_BMP), bmpPreviewProc);

      /* Create a small BMP */
      if(hBitmap) {
        HBITMAP hBitmapTemp;

        hBitmapTemp=createNewBitmap ( hBitmap, &bmpInfoHeader2, WinWindowFromID(hwnd, IDSR_BMP));
        GpiDeleteBitmap(hBitmap);
        hBitmap=hBitmapTemp;
      }

      /* Set percent bars to 0. */
      WinSetWindowText(WinWindowFromID(hwnd,IDBAR_IMGCONVERTPROGRESS),"0#0%");
      WinSendMsg(WinWindowFromID(hwnd,IDEF_IMGCONVERTTARGETNAME), EM_SETTEXTLIMIT,
                 MPFROMSHORT((SHORT)CCHMAXPATH),0);
      WinEnableWindow( WinWindowFromID(hwnd,IDPB_IMGCONVERTABORT), FALSE);

      insertIOProcItems( WinWindowFromID(hwnd, IDDD_IMGIOPROC) );

      WinSetWindowText( WinWindowFromID(hwnd,IDEF_IMGCONVERTTARGETNAME), chrTargetName );

      IniRestoreWindowPos(chrIniFile, INI_IMGCONV_APP, INI_WINDOWPOS_KEY, hwnd);
      IniRestoreWindowClrs(chrIniFile, INI_IMGCONV_APP , hwnd);

      /* Add switch entry */
      memset(&swctl,0,sizeof(swctl));
      WinQueryWindowProcess(hwnd,&pid,NULL);
      swctl.hwnd=hwnd;
      swctl.uchVisibility=SWL_VISIBLE;
      swctl.idProcess=pid;
      swctl.bProgType=PROG_DEFAULT;
      swctl.fbJump=SWL_JUMPABLE;
      WinAddSwitchEntry(&swctl);

      WinSetFocus(HWND_DESKTOP, hwnd);

      return (MRESULT) TRUE;
      /* WM_APPTERMINATENOTIFY messages are sent from the helper programs e.g. format checker. */
    case WM_APPTERMINATENOTIFY:
      switch(LONGFROMMP(mp1))
        {
        case MSG_CONVERTERROR:
        case MSG_CONVERTDONE:
          WinEnableWindow( WinWindowFromID(hwnd,IDPB_IMGCONVERTOK), TRUE);
          WinEnableWindow( WinWindowFromID(hwnd,IDPB_IMGCONVERTABORT), FALSE);
          break;
        case MSG_CONVERTPERCENT:
          iPercent=LONGFROMMP(mp2);
          if(iPercent>100)
            iPercent=100;
          if(iPercent<0)
            iPercent=0;

          /* Update track percent bar value. The helper prog sends us the actual decoded %. */         
          sprintf(text,"%d#%d%%", iPercent, iPercent);
          WinSetWindowText(WinWindowFromID(hwnd,IDBAR_IMGCONVERTPROGRESS), text);
          break;
        default:
          break;
        }
      return FALSE;
    case WM_CLOSE:
      if(tidThread) {
        bBreak=TRUE;
        if(!WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, ID_TIMER, TIMER_DELAY))
          {
            /* Save window position */
            IniSaveWindowPos(chrIniFile, INI_IMGCONV_APP, INI_WINDOWPOS_KEY, hwnd);
            IniSaveWindowClrs(chrIniFile, INI_IMGCONV_APP , hwnd);
            WinDismissDlg(hwnd,0);
          }
        else
          return FALSE;
      }
      /* Save window position */
      IniSaveWindowPos(chrIniFile, INI_IMGCONV_APP, INI_WINDOWPOS_KEY, hwnd);
      IniSaveWindowClrs(chrIniFile, INI_IMGCONV_APP , hwnd);
      WinDismissDlg(hwnd,0);
      return FALSE;
    case WM_CONTROL:
      if(SHORT1FROMMP(mp1)==IDDD_IMGIOPROC)
        {
         if(SHORT2FROMMP(mp1)==EN_CHANGE) {
           SHORT sIdx;

           sIdx=SHORT1FROMMR(WinSendMsg(HWNDFROMMP(mp2),LM_QUERYSELECTION, MPFROMLONG(LIT_FIRST),0));

           iIoProc=iPrivIOProc[sIdx];

           createTargetName(chrSourceName, pMemFormatInfo[iIoProc].szDefaultFormatExt);
           WinSetWindowText( WinWindowFromID(hwnd,IDEF_IMGCONVERTTARGETNAME), chrTargetName );           
         }
        }
      break;
    case WM_TIMER:
      if(SHORT1FROMMP(mp1)==ID_TIMER)
        {
          if(!tidThread) {
            /* Convert thread ended. Quit. */
            WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, ID_TIMER);
            WinPostMsg(hwnd, WM_CLOSE, 0, 0);
          }
          return MRFALSE;
        }
      break;
    case WM_COMMAND:
      switch(SHORT1FROMMP(mp1))
        {
        case IDPB_IMGCONVERTBROWSE:
          {
            char chrTitle[200];
            FILEDLG fd = { 0 };
            /* User pressed the browse button */
            fd.cbSize = sizeof( fd );
            /* It's an centered 'Open'-dialog */
            fd.fl = FDS_OPEN_DIALOG|FDS_CENTER;
            /* Title: "Search CDRecord/2" */
            //     getMessage(text,IDSTR_FDLGSEARCHCDR2TITLE,sizeof(text), hSettingsResource,hwnd);
            /* Set the title of the file dialog */
            fd.pszTitle = chrTitle;
            if(!getMessage(chrTitle, IDSTR_IMAGEBROWSETITLE, sizeof(chrTitle), RESSOURCEHANDLE, hwnd))
              fd.pszTitle = "Image name";
            /* Only show * files */
            //sprintf(fd.szFullFile,"%s","*");
            strcpy(fd.szFullFile, chrTargetName);
            if( WinFileDlg( HWND_DESKTOP, hwnd, &fd ) == NULLHANDLE )
              {
                /* WinFileDlg failed */
                break;
              }
            if( fd.lReturn == DID_OK )
              {
                WinSetWindowText( WinWindowFromID(hwnd,IDEF_IMGCONVERTTARGETNAME), fd.szFullFile );
              }
            break;
          }
        case IDPB_IMGCONVERTCLOSE:
          bBreak=TRUE;
          if(!WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, ID_TIMER, TIMER_DELAY))
            WinPostMsg(hwnd, WM_CLOSE, 0, 0);/* Timer error so do a hard quit */
          break;
        case IDPB_IMGCONVERTABORT:
          bBreak=TRUE;
          WinEnableWindow( WinWindowFromID(hwnd,IDPB_IMGCONVERTABORT), FALSE);
          break;
        case IDPB_IMGCONVERTOK:
          {
            // FSALLOCATE fsAlloc;
            // long long lFreeSpace;
            // ULONG ulDiskNum;
            // char cLetter;
            
            /* Get target name */
            WinQueryWindowText( WinWindowFromID(hwnd,IDEF_IMGCONVERTTARGETNAME), 
                                sizeof(chrTargetName), chrTargetName );
#if 0
            /* Check if diskspace is sufficient */
            cLetter=tolower(chrTargetName[0]);
            ulDiskNum=cLetter-'a'+1;
            if(DosQueryFSInfo(ulDiskNum, FSIL_ALLOC,&fsAlloc,sizeof(fsAlloc)))
              lFreeSpace=0;
            else
              lFreeSpace=fsAlloc.cUnitAvail*fsAlloc.cbSector*fsAlloc.cSectorUnit;

             if(lFreeSpace<lAudioSize) {
               /*
                 Text:   
                 Title: 
                 */       
               getMessage(title, IDSTR_CONVERTNOSPACETEXT,sizeof(title), RESSOURCEHANDLE, hwnd);
               sprintf(text,title,lAudioSize/1000000 );
               getMessage(title, IDSTR_CONVERTNOSPACETITLE,sizeof(title), RESSOURCEHANDLE, hwnd);
               WinMessageBox( HWND_DESKTOP, hwnd, text,
                              title,
                              1234UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
             }
             else {
               /* Start decoding an audio file */
               WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTOK), FALSE);            
               WinEnableWindow( WinWindowFromID(hwnd,IDPB_CONVERTABORT), TRUE);
               convertAudioFile(hwnd);
             }
#endif
             /* Start converting image file */
             WinPostMsg(hwnd, WM_APPTERMINATENOTIFY, MPFROMLONG(MSG_CONVERTPERCENT), MPFROMLONG(0));
             WinEnableWindow( WinWindowFromID(hwnd,IDPB_IMGCONVERTOK), FALSE);            
             WinEnableWindow( WinWindowFromID(hwnd,IDPB_IMGCONVERTABORT), TRUE);
             convertImageFile(hwnd);
             break;
          }
        default:
          break;
        }
      return (MRESULT) FALSE;
    default:
      break;
    }/* switch */
  
  return WinDefDlgProc( hwnd, msg, mp1, mp2);
}

int main (int argc, char *argv[])
{
  HAB  hab;
  HMQ  hmq;
  short a;

  /* Create a copy of the args */
  /* argv[0]: progname
   * argv[1]: imgfile
   * argv[2]: IO proc name
   */

  numArgs=argc;

  //  sprintf(text,"");
  for(a=0;a<argc;a++)
    {
      params[a]=argv[a];
    }

  hab=WinInitialize(0);
  if(hab) {
    hmq=WinCreateMsgQueue(hab,0);
    if(hmq) {  
      /* Check if user started prog by hand */   
      if(argc<NUMPARAMS-1) {/* Not the right num of params */
        pmUsage();
      }
      else {
        /* Save source name */
        strcpy(chrSourceName,params[1]);
        if(argc==NUMPARAMS)
          strcpy(chrProcName,params[2]);
        else {
          bNoProcGiven=TRUE;
          strcpy(chrProcName, "");
        }
        /* Get our ressource dll */  
        //   RESSOURCEHANDLE=0;
        RESSOURCEHANDLE=queryResModuleHandle(argv[0]);

        HlpBuildMMProgIniFileName(argv[0], chrIniFile, sizeof(chrIniFile));
        /* Register the percent bar window class */
        percentRegisterBarClass();
        
        globalHab=WinQueryAnchorBlock(HWND_DESKTOP);        
        hBitmap=loadBitmap ( chrSourceName, &bmpInfoHeader2);
        
        if( WinDlgBox( HWND_DESKTOP, NULLHANDLE, decodeStatusDialogProc, 
                       RESSOURCEHANDLE, IDDLG_IMGCONVERT, 0) == DID_ERROR )
          {
            char text[CCHMAXPATH];
            char title[CCHMAXPATH];
            /*
              Text:   
              Title:  "Installation problem"                       
              */             
            messageBox( text, IDSTR_CONVERTDIALOGERROR , sizeof(text),
                        title, IDSTR_INSTALLERRORTITLE , sizeof(title),
                        RESSOURCEHANDLE, HWND_DESKTOP, MB_OK | MB_ICONEXCLAMATION | MB_MOVEABLE);

            if(hBitmap)
              GpiDeleteBitmap(hBitmap);
            freeResHandle();
            WinDestroyMsgQueue( hmq );
            WinTerminate( hab );
            DosBeep(100,600);
            return( 1 );
          }
        if(pMemFormatInfo)    
          free(pMemFormatInfo);
        if(hBitmap)
          GpiDeleteBitmap(hBitmap);
      }
      freeResHandle();
      WinDestroyMsgQueue(hmq);
    }
    WinTerminate(hab);
  }
  return 0;
}
