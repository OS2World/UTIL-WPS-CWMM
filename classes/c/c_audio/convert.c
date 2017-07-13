/*
 * (C) Chris Wohlgemuth 2002-2003
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
#define INCL_DOSERRORS
#define INCL_DOSQUEUES   /* Queue values */
#define INCL_OS2MM
#define INCL_MMIOOS2

#include <os2.h>
#include <os2me.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <process.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


char buffer[500000]={0};

int main(int argc, char * argv[])
{
  HMMIO hmmio, hmmioTarget;
  MMIOINFO mmioinfo;
  MMAUDIOHEADER mmAudioHeader,mmAudioHeaderTarget;
  LONG lBytesRead=0;
  LONG lBytesWritten=0;
  char * tracks;
  ULONG rc;
  char target[CCHMAXPATH]="r:\\out.wav";

  /* Set stdout to binary */
  _fsetmode(stdout,"b");
  fflush(stdout);

  if(argc==1)
    exit(0);

  tracks=argv[1];
 
  memset(&mmioinfo,0, sizeof(mmioinfo));
  mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
  mmioinfo.ulFlags=MMIO_READ|MMIO_DENYNONE;
  /* open source */
  if((hmmio=mmioOpen(argv[1], &mmioinfo,MMIO_READ))==NULLHANDLE)
    {
      fprintf(stderr,"mmioOpen error with file %s\n", tracks[0]);
      fprintf(stderr,"DONE");
      exit(1);
    }
  memset(&mmAudioHeader,0,sizeof(MMAUDIOHEADER));
  rc = mmioGetHeader(hmmio, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                     &lBytesRead, 0, 0);
  if(rc!=MMIO_SUCCESS) {
    mmioClose(hmmio, 0);
    fprintf(stderr,"mmioGetHeader error!\n");
    exit(1);
  }
  
  /* open target */
  memset(&mmioinfo,0, sizeof(mmioinfo));
  mmioinfo.ulTranslate = MMIO_TRANSLATEHEADER | MMIO_TRANSLATEDATA;
  //mmioinfo.aulInfo[3] = MMIO_MEDIATYPE_AUDIO;
  mmioinfo.ulFlags=MMIO_WRITE|MMIO_CREATE;
  mmioinfo.fccIOProc=mmioStringToFOURCC("WAVE", MMIO_TOUPPER);
  if((hmmioTarget=mmioOpen(target, &mmioinfo, MMIO_WRITE|MMIO_CREATE))==NULLHANDLE)
    {
      char chrError[CCHMAXPATH];
      rc=mmioGetLastError(hmmioTarget);
      mciGetErrorString(mmioinfo.ulErrorRet ,chrError, sizeof(chrError));
      fprintf(stderr,"mmioOpen error with file %s:\n%s\n%d", target, chrError,mmioinfo.ulErrorRet);
      mmioClose(hmmio,0);
      exit(1);
    }
#if 0
  memset(&mmAudioHeader,0,sizeof(MMAUDIOHEADER));
  rc = mmioGetHeader(hmmioTarget, &mmAudioHeaderTarget,sizeof(MMAUDIOHEADER),
                     &lBytesRead, 0, 0);
#endif
  /* Set parameters */
  rc = mmioSetHeader(hmmioTarget, &mmAudioHeader,sizeof(MMAUDIOHEADER),
                     &lBytesWritten, 0, 0);

  /* convert */
  lBytesWritten=0;
  do{
    rc=mmioRead(hmmio, buffer, sizeof(buffer));
    if(rc!=0 && rc!=MMIO_ERROR) {
      mmioWrite(hmmioTarget, buffer, rc);
      //      fwrite(buffer, sizeof(char), rc, stdout);
      lBytesWritten+=rc;
    }
  }while(rc!=0 && rc!=MMIO_ERROR /*&& lBytesWritten <1000000*/);
  if(rc==MMIO_ERROR) {
    fprintf(stderr,"Error while reading audio data!\n");
    fprintf(stderr,"DONE");
  }

  /* close source */
  mmioClose(hmmio,0);
  /* close target */
  mmioClose(hmmioTarget,0);
  exit(0);
}

