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
 
/************************************************************************/
/* Put all #defines here                                                */
/************************************************************************/

#define INCL_32                         /* force 32 bit compile */
#define INCL_GPIBITMAPS
#define INCL_DOSFILEMGR
#define INCL_WIN
#define INCL_GPI
#define INCL_PM                         

#define MEMCHECK

/************************************************************************/
/* Put all #includes here                                               */
/************************************************************************/

#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <os2medef.h>
#include <mmioos2.h>
#include <streamproc.h>

#define DEBUG

#ifdef DEBUG
void writeLog(const char* chrFormat, ...)
{
  char logNameLocal[CCHMAXPATH];
  FILE *fHandle;

  sprintf(logNameLocal,"r:\\streamio.log");
  fHandle=fopen(logNameLocal,"a");
  if(fHandle) {
    va_list arg_ptr;
    void *tb;

    va_start (arg_ptr, chrFormat);
    vfprintf(fHandle, chrFormat, arg_ptr);
    va_end (arg_ptr);
    fclose(fHandle);
  }
}
#endif


/************************************************************************/
/* STREAM IOProc                                                        */
/*                                                                      */
/* ARGUMENTS:                                                           */
/*                                                                      */
/*     PSZ pmmioStr - pointer to MMIOINFO block                         */
/*     USHORT usMsg - MMIO message being sent                           */
/*     LONG lParam1 - filename or other parameter depending on message  */
/*     LONG lParam2 - used with some messages as values                 */
/*                                                                      */
/*                                                                      */
/*  RETURN:                                                             */
/*                                                                      */
/*      MMIOM_OPEN                                                      */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_READ                                                      */
/*          Success           - Returns the number of bytes actually    */
/*                              read.  Return 0L if no more bytes can   */
/*                              be read.                                */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_WRITE                                                     */
/*          Success           - Returns the number of bytes actually    */
/*                              written.                                */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_SEEK                                                      */
/*          Success           - Returns the new file position           */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_CLOSE                                                     */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*          Other             - MMIO_WARNING, file was closed but the   */
/*                              IOProc expected additional data         */
/*                                                                      */
/*      MMIOM_GETFORMATNAME                                             */
/*          Success           - Returns the number of bytes read into   */
/*                              the buffer (size of format name)        */
/*          Failure           - Return 0                                */
/*                                                                      */
/*      MMIOM_GETFORMATINFO                                             */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_QUERYHEADERLENGTH                                         */
/*          Success           - Returns the size of the header in bytes */
/*          Failure           - Return 0                                */
/*                                                                      */
/*      MMIOM_IDENTIFYFILE                                              */
/*          Success           - MMIO_SUCCESS     (0)                    */
/*          Failure           - MMIO_ERROR      (-1)                    */
/*                                                                      */
/*      MMIOM_GETHEADER                                                 */
/*          Success           - Returns number of bytes copied into     */
/*                              the header structure.                   */
/*          Failure           - Return 0                                */
/*          Other             - If length passed in was not large       */
/*                              enough to hold header then,             */
/*                              MMIOERR_INVALID_BUFFER_LENGTH is set    */
/*                              in ulErrorRet.                          */
/*                            - If header is bad,                       */
/*                              MMIOERR_INVALID_STRUCTURE is set in     */
/*                              ulErrorRet                              */
/*                                                                      */
/*      MMIOM_SETHEADER                                                 */
/*          Success           - Returns number of bytes written         */
/*          Failure           - Return 0                                */
/*          Other             - If header is bad,                       */
/*                              MMIOERR_INVALID_STRUCTURE is set in     */
/*                              ulErrorRet                              */
/*                                                                      */
/*  DESCRIPTION:                                                        */
/*                                                                      */
/*      This routine will translate JPEG image data into                */
/*      OS/2 2.0 memory bitmap data and back again.                     */
/*                                                                      */
/*                                                                      */
/*  GLOBAL VARS REFERENCED:                                             */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  GLOBAL VARS MODIFIED:                                               */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  NOTES:                                                              */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  SIDE EFFECTS:                                                       */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/************************************************************************/

LONG EXPENTRY IOProc_Entry (PVOID  pmmioStr,
                             USHORT usMsg,
                             LONG   lParam1,
                             LONG   lParam2)

    {
    PMMIOINFO   pmmioinfo;                      /* MMIOINFO block */

    pmmioinfo = (PMMIOINFO) pmmioStr;

DosBeep(5000, 30);
#ifdef DEBUG
    writeLog("MSG: %d %x\n", usMsg,usMsg);
#endif

    switch (usMsg)
        {
        /*#############################################################*
         * When Closing the file, perform the following:
         * 1) Setup Variables
         * 2) Process the Image buffer
         * 3) Compress the Image to appropriate format
         *#############################################################*/
        case MMIOM_CLOSE:
            {
            /************************************************************
             * Declare local variables.
             ************************************************************/


            LONG            lRetCode;
            USHORT          rc;

#ifdef DEBUG
            writeLog("MMIO_CLOSE\n");
#endif

            return (MMIO_ERROR);
#if 0
            /***********************************************************
             * Check for valid MMIOINFO block.
             ***********************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            /***********************************************************
             * Set up our working file status variable.
             ***********************************************************/
            pJPGInfo = (PJPGFILESTATUS)pmmioinfo->pExtraInfoStruct;
            
            /***********************************************************
             * Assume success for the moment....
             ***********************************************************/
            lRetCode = MMIO_SUCCESS;
            
            /************************************************************
             * see if we are in Write mode and have a buffer to write out.
             *    We have no image buffer in UNTRANSLATED mode.
             ************************************************************/
            if ((pmmioinfo->ulFlags & MMIO_WRITE) && (pJPGInfo->lpRGBBuf))
                {
                  /* Write the buffer to disk */
                  }

            /***********************************************************
             * Close the file 
             ***********************************************************/
            return (lRetCode);
#endif
            }  /* end case of MMIOM_CLOSE */
            
        /*#############################################################*
         * Get the NLS format Information.
         *#############################################################*/
        case MMIOM_GETFORMATINFO:
            {
            /***********************************************************
             * Declare local variables.
             ***********************************************************/
            PMMFORMATINFO       pmmformatinfo;

#ifdef DEBUG
            writeLog("MMIO_GETFORMATINFO\n");
#endif
#if 0
            /************************************************************
             * Set pointer to MMFORMATINFO structure.
             ************************************************************/
            pmmformatinfo = (PMMFORMATINFO) lParam1;

            /************************************************************
             * Fill in the values for the MMFORMATINFO structure.
             ************************************************************/
            pmmformatinfo->ulStructLen  = sizeof (MMFORMATINFO);
            pmmformatinfo->fccIOProc    = FOURCC_STREAM;
            pmmformatinfo->ulIOProcType = MMIO_IOPROC_FILEFORMAT;
            pmmformatinfo->ulMediaType  = MMIO_MEDIATYPE_IMAGE;

            pmmformatinfo->ulFlags      = MMIO_CANREADTRANSLATED |       
              MMIO_CANSEEKTRANSLATED        |
              MMIO_CANWRITETRANSLATED     
              /* MMIO_CANREADUNTRANSLATED      |
                 MMIO_CANSEEKUNTRANSLATED */;

            strcpy ((PSZ) pmmformatinfo->szDefaultFormatExt, pszJPGExt);
            if (GetNLSData( &pmmformatinfo->ulCodePage,
                            &pmmformatinfo->ulLanguage ))
               {
               return( -1L );
               }

            if (GetFormatStringLength( FOURCC_JPG,
                                       &(pmmformatinfo->lNameLength) ))
               {
               return( -1L );
               }
#endif

            return (MMIO_ERROR);

            /************************************************************
             * Return success back to the application.
             ************************************************************/
            return (MMIO_SUCCESS);
            } /* end case of MMIOM_GETFORMATINFO */

        /*#############################################################*
         * Get the NLS format name.
         *#############################################################*/
        case MMIOM_GETFORMATNAME:
            {
            LONG lBytesCopied;

#ifdef DEBUG
            writeLog("MMIO_GETFORMATNAME\n");
#endif

            /************************************************************
             * Copy format string into buffer supplied by
             * lParam1.  Only put in the amount of my string up to the
             * allocated amount which is in lParam2.  Leave enough room
             * for the NULL termination.
             ************************************************************/
            lBytesCopied = GetFormatString( FOURCC_STREAM,
                                            (char *)lParam1,
                                            lParam2 );

            return (lBytesCopied);
            } /* end case of MMIOM_GETFORMATNAME */

        /*#############################################################*
         * Identify whether this file can be processed.
         *#############################################################*/
        case MMIOM_IDENTIFYFILE:
            {

            /************************************************************
             * Declare local variables.
             ************************************************************/

            BOOL bValidJPG= FALSE;

            ULONG           ulTempFlags = MMIO_READ | MMIO_DENYWRITE |
                                          MMIO_NOIDENTIFY;
                                           /* flags used for temp open  */
                                           /* and close                 */
#ifdef DEBUG
            writeLog("MMIO_IDENTIFYFILE\n");
#endif

            /************************************************************
             * We need either a file name (lParam1) or file handle (lParam2)
             ************************************************************/
            if (!lParam1 && !lParam2)
                return (MMIO_ERROR);


            return (MMIO_ERROR);

            } /* end case of MMIOM_IDENTIFYFILE */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_OPEN:
            {

            /************************************************************
             * Declare local variables
             ************************************************************/
#ifdef DEBUG
            writeLog("MMIO_OPEN\n");
#endif
            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            return (MMIO_ERROR);

            } /* end case of MMIOM_OPEN */

        /*#############################################################*/
        /*#############################################################*/
        case MMIOM_READ:
            {

            /************************************************************
             * Declare Local Variables
             ************************************************************/
              /*            PJPGFILESTATUS   pJPGInfo; */
            LONG            rc;
            LONG            lBytesToRead;

#ifdef DEBUG
            writeLog("MMIO_READ\n");
#endif

            /************************************************************
             * Check for valid MMIOINFO block.
             ************************************************************/
            if (!pmmioinfo)
                return (MMIO_ERROR);

            return (MMIO_ERROR);

            }   /* end case  of MMIOM_READ */

        /*#############################################################*/
        /*#############################################################*/

        /* 
         * If the IOProc has a child IOProc, then pass the message on to the Child, otherwise 
         * return Unsupported Message
         */
        default: 
            {
             /*
              * Declare Local Variables.
              */
              //             PMMFILESTATUS       pVidInfo;
             LONG                lRC;

             /************************************************************
              * Check for valid MMIOINFO block.
              ************************************************************/
             if (!pmmioinfo)
                 return (MMIO_ERROR);

             /* !!!!!!!!!!!! FIXME !!!!!!!!!!!!!*/ 
             return (MMIOERR_UNSUPPORTED_MESSAGE);
            }   /* end case of Default */

        } /* end SWITCH statement for MMIO messages */

    return (0);
    }      /* end of window procedure */
