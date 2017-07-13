#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_DOSDEVIOCTL
#define INCL_DOSDEVICES    // DosDevIOCtl

#include <os2.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h> /* For strlcpy() */
#include <stdarg.h>
#include <stdlib.h> /* For getenv() */

#include "sys_funcs.h"
#include "excptLogName.h"

#if  __cplusplus
extern "C" {
#endif
  
/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion returns the drive letter of the    */
/* boot partition.                                  */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* char chrDrive                                    */
/*                                                  */
/* Drive from which the system was booted.          */
/*                                                  */
/*!!*************************************************/
char SysQueryBootDriveLetter(void)
{
  ULONG ulSysValue;

  if(!DosQuerySysInfo(QSV_BOOT_DRIVE, QSV_BOOT_DRIVE,&ulSysValue,sizeof(ulSysValue)))
    return 'a'+ulSysValue-1;

  return 'c';
} 

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion returns the running OS version.    */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulVersion                                  */
/*                                                  */
/*                                                  */
/* 30: Warp 3:p.                                    */
/* 40: Warp 4                                       */
/*                                                  */
/*!!*************************************************/
ULONG SysQueryOSRelease(void)
{
  static ULONG ulVersionMinor=0;

  if(!ulVersionMinor)
    if(DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ulVersionMinor, sizeof(ulVersionMinor)))
      ulVersionMinor=30;/* Default Warp 3 */

  return ulVersionMinor;

}

/*
   Documentation script can't handle the const keyword yet!!!!
*/
/*--->!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function may be used to write data into the */
/* traplog file. The syntax is similar to printf(). */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* The filename of the traplog is defined in the    */
/* include file :hp2.excptLogName.h:ehp2.           */
/*                                                  */
/* !parsing is broken at the moment because of const and (...)              */
/*                                                  */
/*!!*************************************************/
void SysWriteToTrapLog(const char* chrFormat, ...)
{
  char logNameLocal[CCHMAXPATH];
  FILE *fHandle;

  sprintf(logNameLocal,"%c:\\%s", SysQueryBootDriveLetter(), EXCEPTION_LOGFILE_NAME);
  fHandle=fopen(logNameLocal,"a");
  if(fHandle) {
    va_list arg_ptr;

    va_start (arg_ptr, chrFormat);
    vfprintf(fHandle, chrFormat, arg_ptr);
    va_end (arg_ptr);
    fclose(fHandle);
  }
}

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function checks if the given file exists.   */
/* thread.                                          */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* BOOL fExists                                     */
/*                                                  */
/* TRUE: File exists, FALSE otherwise.              */
/*                                                  */
/*!!*************************************************/
BOOL SysCheckFileExists(char* chrFileName)
{
  struct stat statBuf;

  /* Check file path */
  if(stat(chrFileName , &statBuf)==-1)
    return FALSE;
  else
    return TRUE;
}


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function queries the size of a file.        */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulSize                                     */
/*                                                  */
/* Size of the file.                                */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* In case of a nonexistant file 0 is returned. So  */
/* it's necessary to first check if the file exists.*/
/*                                                  */
/*!!*************************************************/ 
ULONG SysQueryFileSize(char* chrFileName)
{
  struct stat statBuf;

  /* Check file path */
  if(stat(chrFileName , &statBuf)==-1)
    return 0;
 
  return statBuf.st_size;
}

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function returns the ID of the calling      */
/* thread.                                          */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulTID                                      */
/*                                                  */
/* ID of the current thread.                        */
/*                                                  */
/*!!*************************************************/
ULONG SysQueryCurrentTID(void)
{
  PPIB ppib;
  PTIB ptib;

  DosGetInfoBlocks( &ptib, &ppib);

  return ptib->tib_ptib2->tib2_ultid;
}

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* To be written...                                 */
/*                                                  */
/*!!*************************************************/
ULONG SysQueryDriveType(ULONG ulDriveNum)
{
  ULONG ulDisk=0;
  ULONG ulParamLen;
  ULONG ulDataLen;
  char chrParam[2]={0};
  char chrData;
  ULONG rc;

  if(ulDriveNum>26 || ulDriveNum<1)
    return QDT_ERROR_DISK;

  //  SysWriteToTrapLog("Drive nr.: %d\n", ulDriveNum);

  /*
    Query fixed, removable and remote drives in the system.
    */
  ulDataLen=sizeof(chrData);
  ulParamLen=sizeof(chrParam);
  chrParam[1]=(BYTE)ulDriveNum-1;
  chrParam[0]=0;
  /* Check if drive is fixed or removable */
  rc = DosDevIOCtl(-1, IOCTL_DISK, DSK_BLOCKREMOVABLE,
                   chrParam, sizeof(chrParam), &ulParamLen, &chrData,
                   sizeof(chrData), &ulDataLen);
  
  if(rc) {
    //   SysWriteToTrapLog("Drive nr.: %d, rc for DSK_BLOCKREMOVABLE", ulDriveNum);
    if(rc==ERROR_NOT_SUPPORTED) {
      /* This may be a remote drive */
      char chrDev[8];
      ULONG cbBuf;
      BYTE buf[sizeof(FSQBUFFER2)+3*CCHMAXPATH];
      PFSQBUFFER2 pBuf=(PFSQBUFFER2)buf;
      
      //      SysWriteToTrapLog("Drive nr.: %d, rc for DSK_BLOCKREMOVABLE-> ERROR_NOT_SUPPORTED\n", ulDriveNum);
      sprintf(chrDev, "%c:", 'a'+(char)(ulDriveNum-1));
      cbBuf=sizeof(buf);
      
      /* Check if local or remote (or anything else) */
      rc= DosQueryFSAttach(chrDev, 0, FSAIL_QUERYNAME,(PFSQBUFFER2) &buf, &cbBuf);
      if(!rc) {
        if(pBuf->iType==FSAT_REMOTEDRV)
          ulDisk=QDT_REMOTE_DRIVE;
      }
    }/* ERROR_NOT_SUPPPORTED */
    else
      ulDisk=QDT_NO_DRIVE; /* Everything we don't understand is no disk */
  }/* rc != NO_ERROR */
  else {
    /* We end here for HDs, floppies and CD-ROMS */
    //   SysWriteToTrapLog("Drive nr.: %d, local ones\n", ulDriveNum);
    if(chrData ) /* chrData is 1 if Fixed disk */
      return QDT_FIXED_DISK;
    else {
      /* Check for floppy drives */
      BYTE               abParm[2] = { 0, (BYTE)ulDriveNum-1 };
      ULONG              ulParmLen = 2;
      ULONG              ulDataLen = 0;
      BIOSPARAMETERBLOCK bpb;
      
      //      SysWriteToTrapLog("Drive nr.: %d, not a fixed disk\n", ulDriveNum);

      /* The disk is local. Look for floppy */
      if (DosDevIOCtl ((HFILE)-1, IOCTL_DISK, DSK_GETDEVICEPARAMS, 
                       (PVOID)&abParm[0], sizeof (abParm), &ulParmLen, (PVOID)&bpb, sizeof (bpb), &ulDataLen) == NO_ERROR) {
        {
          ULONG ulDevType;
          BOOL fRemovable;
          
          if (ulDataLen < 36) 
            return (QDT_NO_DRIVE); /* Error */
          
          ulDevType = bpb.bDeviceType;
          fRemovable = (BOOL)((bpb.fsDeviceAttr & 0x01) ? FALSE : TRUE);
          //    SysWriteToTrapLog("Drive nr.: %d, ulDevType: %d\n", ulDriveNum, ulDevType);

          if ((ulDevType == 7) && fRemovable) {  
            // Check if DEVTYPE_OTHER (7) could be a 1.44MB diskette (Just for the sake, check for a 720kB and 2.88MB while we're at it).
            switch (bpb.usBytesPerSector * bpb.cSectors) {
            case 512*1440:
              return QDT_DISKETTE_DRIVE; //DEVTYPE_35INCH_1MB_DISKETTE;
            case 512*2880:
              return QDT_DISKETTE_DRIVE;//DEVTYPE_35INCH_2MB_DISKETTE;
            case 512*5760:
              return QDT_DISKETTE_DRIVE;//DEVTYPE_35INCH_2MB_DISKETTE;
            }/* switch()*/
          }else {
            switch (ulDevType) {
            case 0: /* 48TPI diskette */
            case 1: /* 96TPI diskette */
            case 2: /* 3.5 inch diskette */
            case 3: /* 8inch single diskette */
            case 4: /* 8inch double diskette */
            case 9: /* 3.5 4MB (2.88 formatted) diskette */
              return QDT_DISKETTE_DRIVE;
            }/* switch() */
          }/* else */
          //          SysWriteToTrapLog("Drive nr.: %d, ????\n", ulDriveNum);
          /* Assume CD-ROM here */
          return (QDT_CDROM_DRIVE); /* For example tape */
          return (QDT_NO_DRIVE); /* For example tape */
        }
      }/* DSK_GETDEVICEPARAMS */
    }/* else */
    ulDisk=QDT_REMOVABLE_DISK;
  }
  
  return ulDisk;
}

#if 0
ULONG SysQueryDriveType_(ULONG ulDriveNum)
{
  static char chrDisks[26]={0};
  static BOOL bInit=FALSE;
  int a;

  if(ulDriveNum>26 || ulDriveNum<1)
    return QDT_ERROR_DISK;

  if(!bInit) {
    /*
      Query fixed, removable and remote drives in the system.
      */
    for(a=0; a<26; a++) {
      ULONG ulParamLen;
      ULONG ulDataLen;
      char chrParam[2]={0};
      char chrData;
      ULONG rc;

      ulDataLen=sizeof(chrData);
      ulParamLen=sizeof(chrParam);
      chrParam[1]=a;
      chrParam[0]=0;
      /* Check if drive is fixed or removable */
      rc = DosDevIOCtl(-1, IOCTL_DISK, DSK_BLOCKREMOVABLE,
                       chrParam, sizeof(chrParam), &ulParamLen, &chrData,
                       sizeof(chrData), &ulDataLen);
      
      if(rc) {
        if(rc==ERROR_NOT_SUPPORTED) {
          /* This may be a remote drive */
          char chrDev[8];
          ULONG cbBuf;
          BYTE buf[sizeof(FSQBUFFER2)+3*CCHMAXPATH];
          PFSQBUFFER2 pBuf=(PFSQBUFFER2)buf;
          
          sprintf(chrDev, "%c:", 'a'+a);
          cbBuf=sizeof(buf);
          
          /* Check if local or remote (or anything else) */
          rc= DosQueryFSAttach(chrDev, 0, FSAIL_QUERYNAME,(PFSQBUFFER2) &buf, &cbBuf);
          if(!rc) {
            if(pBuf->iType==FSAT_REMOTEDRV)
              chrDisks[a]=QDT_REMOTE_DRIVE;
          }
        }/* ERROR_NOT_SUPPPORTED */
        else
          chrDisks[a]=QDT_NO_DRIVE; /* Everything we don't understand is no disk */
      }/* rc */
      else
        chrDisks[a]=(chrData & QDT_FIXED_DISK); /* chrData is 1 if Fixed disk */
    }/* for */
    bInit=TRUE;
  }/* bInit */
  return chrDisks[ulDriveNum-1];
}
#endif

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* To be written...                                 */
/*                                                  */
/*!!*************************************************/
ULONG SysQueryFreeDriveSpace(ULONG ulDriveNum, ULONG *ulTotal, ULONG * ulFree, ULONG* ulBytesUnit)
{
  FSALLOCATE fsa;
  ULONG rc;

  if(!ulTotal || !ulFree || !ulBytesUnit)
    return 1;

  if( ulDriveNum<1 || ulDriveNum > 26)
    return ERROR_INVALID_DRIVE;

  if((rc=DosQueryFSInfo(ulDriveNum, FSIL_ALLOC, &fsa, sizeof(FSALLOCATE)))!=NO_ERROR)
    return rc;

  *ulTotal=fsa.cUnit;
  *ulFree=fsa.cUnitAvail;
  *ulBytesUnit=fsa.cbSector*fsa.cSectorUnit;

  return(NO_ERROR);
}

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function returns the home directory of the  */
/* current user.                                    */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG rc                                         */
/*                                                  */
/* NO_ERROR is returned if the function succeeds.   */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* ULONG ulDriveNum input                           */
/*                                                  */
/* The number of the drive to be queried (1-26).    */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* char* chrBuffer in/out                           */
/*                                                  */
/* Pointer to a buffer where the volume name will   */
/* be placed.                                       */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* ULONG ulSize input                               */
/*                                                  */
/* Size of the name buffer.                         */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* The returned name is always terminated by zero.  */
/*                                                  */
/*!!*************************************************/

ULONG SysQueryDiskName(ULONG ulDriveNum, char *chrBuffer, ULONG ulSize)
{
  FSINFO fsIBuf;
  ULONG rc;

  if(!chrBuffer || !ulSize)
    return 1;

  if( ulDriveNum<1 || ulDriveNum > 26)
    return ERROR_INVALID_DRIVE;

  if((rc=DosQueryFSInfo(ulDriveNum, FSIL_VOLSER, &fsIBuf, sizeof(FSINFO)))!=NO_ERROR)
    return rc;
  strncpy(chrBuffer, fsIBuf.vol.szVolLabel, ulSize);
  chrBuffer[ulSize-1]=0;

  return(NO_ERROR);
}

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This function returns the home directory of the  */
/* current user.                                    */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* BOOL fSuccess                                    */
/*                                                  */
/* TRUE if the home dir is specified and does exist */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* char* chrBuf in/out                              */
/*                                                  */
/* Pointer to a buffer where the home directory     */
/* name will be placed.                             */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* ULONG ulSize input                               */
/*                                                  */
/* Size of the name buffer.                         */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* The returned string is always terminated by zero.*/
/* .p:                                              */
/* If a directory is specified in the HOME          */
/* environment variable but doesn't exist FALSE is  */
/* returned and a message is written to the apps    */
/* exception log.                                   */
/*                                                  */
/*!!*************************************************/
BOOL SysQueryHomeDir(char* chrBuf, ULONG ulSize)
{
  char *chrHome=getenv("HOME");

  if(!chrHome || !chrBuf)
    return FALSE;

  if(strlcpy(chrBuf, chrHome, ulSize)>=ulSize) {
    chrBuf[0]=0; 
    return FALSE;
  }
  if(SysCheckFileExists(chrHome))
    return TRUE;

  /* HOME dir specified but does not exist */
  SysWriteToTrapLog("%s, %s: the in config.sys specified HOME directory '%s' does not exist.\n",
                    __DATE__, __TIME__, chrHome);
  return FALSE;
}

char* my_strlcpy(char* string1, char* string2, size_t count)
{

  if((strlen(string2))<count)
    strcpy(string1, string2);
  else {
    strncpy(string1, string2, count);
    string1[count-1]=0;
  }
  return string1;
}

/********************************************************************/
/*	$OpenBSD: strlcpy.c,v 1.8 2003/06/17 21:56:24 millert Exp $	*/

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */


/*
 * Copy src to string dst of size siz.  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz == 0).
 * Returns strlen(src); if retval >= siz, truncation occurred.
 */
size_t
strlcpy(char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0 && --n != 0) {
		do {
			if ((*d++ = *s++) == 0)
				break;
		} while (--n != 0);
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}


/*	$OpenBSD: strlcat.c,v 1.11 2003/06/17 21:56:24 millert Exp $	*/

/*
 * Copyright (c) 1998 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * Appends src to string dst of size siz (unlike strncat, siz is the
 * full size of dst, not space left).  At most siz-1 characters
 * will be copied.  Always NUL terminates (unless siz <= strlen(dst)).
 * Returns strlen(src) + MIN(siz, strlen(initial dst)).
 * If retval >= siz, truncation occurred.
 */
size_t
strlcat(char *dst, const char *src, size_t siz)
{
	register char *d = dst;
	register const char *s = src;
	register size_t n = siz;
	size_t dlen;

	/* Find the end of dst and adjust bytes left but don't go past end */
	while (n-- != 0 && *d != '\0')
		d++;
	dlen = d - dst;
	n = siz - dlen;

	if (n == 0)
		return(dlen + strlen(s));
	while (*s != '\0') {
		if (n != 1) {
			*d++ = *s;
			n--;
		}
		s++;
	}
	*d = '\0';

	return(dlen + (s - src));	/* count does not include NUL */
}

/********************************************************************/
#if  __cplusplus
}
#endif







