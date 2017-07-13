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

#define INCL_GPIBITMAPS
#define INCL_PM
#define INCL_DOSDEVIOCTL
#define INCL_DOS

#include <os2.h>
#include <stdio.h>
/* Include file with definition for WPShadow class */
#include "wpshadow.h"
#include "wpdisk.h"
#include "wpfsys.h" /* For _wpQueryDisk */
#include "cwaudioinc.h"
#include "sys_funcs.h"

#define MM_INVALID_DRIVES      "mmClsDontCheckDrives"
#define MM_INVALID_ID3DRIVES   "mmClsDontReadID3OnDrives"

BOOL cwQueryCDDrives(int *iNumCD, int * iFirstDrive);
HFILE openDrive(char* drive);
void closeDrive(HFILE hfDrive);

#if 0
/* The following is also used by CWMMDisk so it can't be made static. */
int iFirstCD;
int iNumCD;
BOOL bGotCD=FALSE;
#endif

/* List of drives to be skipped during audio scanning. */
/* 1: skip, 0: scan.                                   */
static char chrInvalidDrives[26]= {0};
/* List of drives on which no ID3 tags should be read. */
/* 1: skip, 0: read.                                   */
static char chrInvalidID3Drives[26]= {0};
/* List of CD drives in the system. */
/* 0: other disk, 1: CD drive, 2: not yet queried */
static char chrCDDrives[26]= {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

#if 0
static BOOL _queryCDOnce(void)
{
  static BOOL bTriedCD=FALSE;

  /* Only search for CDs once to speed up things */
  if(!bTriedCD) {
    bTriedCD=TRUE; /* Don't search for CDs again */ 
    /* Get CD drives */
    if(cwQueryCDDrives(&iNumCD, &iFirstCD)) {
      int a;
      bGotCD=TRUE;
#if 0
      /* Invalidate drive letter in chrValidDrives[] list */
      for(a=iFirstCD;a<iFirstCD+iNumCD; a++) {
        chrInvalidDrives[a-1]=1;
        //SysWriteToTrapLog(" %d\n",a-1 );      
      }
#endif
    }
  }
  return TRUE;
}
#endif

void invalidateDriveInList(ULONG ulDriveNum)
{
  if(ulDriveNum<1 || ulDriveNum>26)
    return;

  chrCDDrives[ulDriveNum-1]=2;
}

/*****************************************************/
/*                                                   */
/* Check if the drive is a CD. Iinsert the           */
/* result into the static var.                       */
/*                                                   */
/* ulDriveNum:  1: a:, 2: b: ...                     */
/*                                                   */
/*****************************************************/
ULONG checkForCDDrive(ULONG ulDriveNum)
{
  //int a;

  if(ulDriveNum<1 || ulDriveNum>26)
    return 2;

  //for(a=0;a<26; a++)
  //SysWriteToTrapLog("%d ", chrCDDrives[a]);

  // SysWriteToTrapLog("\n%s: %d\n", __FUNCTION__, chrCDDrives[ulDriveNum-1]);

  if(chrCDDrives[ulDriveNum-1]!=2)
    return chrCDDrives[ulDriveNum-1];

  if(SysQueryDriveType(ulDriveNum)==QDT_REMOVABLE_DISK)
    {
      /* It's a removable disk (floppy, CD-Rom or other) */
      char chrPlayerID[4];
      ULONG rc;
      HFILE hFile;
  
      sprintf(chrPlayerID, "%c:", 'A'+ulDriveNum-1);
      /* Try to open the drive */
      if((hFile=openDrive(chrPlayerID))!=NULLHANDLE) {
        ULONG ulParamLen;
        ULONG ulDataLen;
        ULONG ulData;
        
        ulParamLen=4;
        ulDataLen=sizeof(ulData);
        rc = DosDevIOCtl(hFile, IOCTL_CDROMDISK, CDROMDISK_DEVICESTATUS,
                         "CD01", 4, &ulParamLen, &ulData,
                         sizeof(ulData), &ulDataLen);
        if(!rc)
          chrCDDrives[ulDriveNum-1]=1; /* It's a CD drive */
        else
          chrCDDrives[ulDriveNum-1]=0; /* Other drive */
        closeDrive(hFile);
      }
      else
        chrCDDrives[ulDriveNum-1]=2; /* Open failed, maybe no CD inserted yet.
                                        Mark as undetermined */
    }
  else
    chrCDDrives[ulDriveNum-1]=0; /* It's not a CD drive */

  //SysWriteToTrapLog("Drive determined as: %d\n", chrCDDrives[ulDriveNum-1]);
  return chrCDDrives[ulDriveNum-1];
}

/*****************************************************/
/*                                                   */
/* Mostly obsolete now because of function           */
/* cwObjectIsOnInvalidDrive() which is more generic. */
/*                                                   */
/*****************************************************/
BOOL cwObjectIsOnCD(WPObject * somSelf)
{
  WPDisk *wpDisk;
  int iDiskNum;

  //  SysWriteToTrapLog("In %s\n", __FUNCTION__);
  
  if((wpDisk=_wpQueryDisk(somSelf))!=NULLHANDLE) {
    //SysWriteToTrapLog("Got disk object...\n");
   if((iDiskNum=_wpQueryLogicalDrive(wpDisk))!=0) {
     //SysWriteToTrapLog("Disk num: %d, status: %d\n", iDiskNum, chrCDDrives[iDiskNum-1]);
      if(chrCDDrives[iDiskNum-1]==2)
        checkForCDDrive(iDiskNum);
      if(chrCDDrives[iDiskNum-1]==1) {
        //SysWriteToTrapLog("Object is on CD\n\n");
        return TRUE; /* The object is on a CD */
      }
    }
  }
  //  SysWriteToTrapLog("Leaving %s, object is not on CD\n", __FUNCTION__);
  return FALSE; /* Object not on CD */
}
#if 0
BOOL cwObjectIsOnCD(WPObject * somSelf)
{
  /* Get the CD drives in the system */ 
  _queryCDOnce();

  if(bGotCD) {
    WPDisk *wpDisk;
    int iDiskNum;
    
    if((wpDisk=_wpQueryDisk(somSelf))!=NULLHANDLE) {
      if((iDiskNum=_wpQueryLogicalDrive(wpDisk))!=0) {
        if((iDiskNum>=iFirstCD && iDiskNum<=iFirstCD+iNumCD)) {
          return TRUE; /* The object is on a CD */
        }
      }
    }
  }
  return FALSE; /* Object not on CD */
}
#endif

/*****************************************************/
/*                                                   */
/* Check key in USER ini specifying which volumes    */
/* should be skipped when querying ID3 tags.         */
/*                                                   */
/* Key is:                                           */
/*                                                   */
/* CWMM->mmClsDontCheckID3OnDrives-><drive list>     */
/*                                                   */
/* The drive list is a string containing the drive   */
/* letters to ignore.                                */
/*                                                   */
/*****************************************************/
static BOOL _queryInvalidID3Drives(void)
{
  static BOOL bQueriedID3Drives=FALSE;

  if(!bQueriedID3Drives)
    {
      char chrProfileStr[30];
      char * chrPtr;

      /* Only query once */
      bQueriedID3Drives=TRUE;
      if(!PrfQueryProfileString(HINI_USERPROFILE, "CWMM", MM_INVALID_ID3DRIVES, NULLHANDLE, chrProfileStr, 
                                sizeof(chrProfileStr)))
        return FALSE; /* Key not set, use all drives */
      strcpy(chrProfileStr,strlwr(chrProfileStr));
      chrPtr=chrProfileStr;

      while(*chrPtr!=0 && *chrPtr>='a' && *chrPtr<='z')
        chrInvalidID3Drives[(*chrPtr++)-'a']=1;

    }
  return TRUE;
}

BOOL cwObjectIsOnInvalidID3Drive(WPObject * somSelf)
{
  WPDisk *wpDisk;
  int iDiskNum;

  /* Get invalid drives list */
  _queryInvalidID3Drives();
      
  if((wpDisk=_wpQueryDisk(somSelf))!=NULLHANDLE) {
    if((iDiskNum=_wpQueryLogicalDrive(wpDisk))!=0)
      return chrInvalidID3Drives[iDiskNum-1]; 
  }  
  return FALSE; /* Object not on invalid drive */
}

/*****************************************************/
/*                                                   */
/* Check key in USER ini specifying which volumes    */
/* should be skipped when querying audio info.       */
/*                                                   */
/* Key is:                                           */
/*                                                   */
/* CWMM->mmClsDontCheckDrives-><drive list>          */
/*                                                   */
/* The drive list is a string containing the drive   */
/* letters to ignore.                                */
/*                                                   */
/*****************************************************/
static BOOL _queryInvalidDrives(void)
{
  static BOOL bQueriedDrives=FALSE;

  if(!bQueriedDrives)
    {
      char chrProfileStr[30];
      char * chrPtr;

      /* Only query once */
      bQueriedDrives=TRUE;
      if(!PrfQueryProfileString(HINI_USERPROFILE, "CWMM", MM_INVALID_DRIVES, NULLHANDLE, chrProfileStr, 
                                sizeof(chrProfileStr)))
        return FALSE; /* Key not set, use all drives */
      strcpy(chrProfileStr,strlwr(chrProfileStr));
      chrPtr=chrProfileStr;

      while(*chrPtr!=0 && *chrPtr>='a' && *chrPtr<='z')
        chrInvalidDrives[(*chrPtr++)-'a']=1;
      //SysWriteToTrapLog("%c: -> %d\n",*chrPtr, (*chrPtr++)-'a' );      

    }
  return TRUE;
}

BOOL cwObjectIsOnInvalidDrive(WPObject * somSelf)
{
  WPDisk *wpDisk;
  int iDiskNum;

  /* Get invalid drives list */
  _queryInvalidDrives();
      
  if((wpDisk=_wpQueryDisk(somSelf))!=NULLHANDLE) {
    if((iDiskNum=_wpQueryLogicalDrive(wpDisk))!=0) {
      return chrInvalidDrives[iDiskNum-1]; 
    }
  }
  
  return FALSE; /* Object not on invalid drive */
}

BOOL mmclsSetObjectType(WPFileSystem * somSelf)
{
  char chrBuf[256];
  ULONG ulLength;

  M_WPObject *m_wpObject=_somGetClass(somSelf);

  /* Set type if not already done */
  strncpy(chrBuf, _wpQueryType(somSelf), sizeof(chrBuf));
  chrBuf[sizeof(chrBuf)-1]=0;
  
  if(!strstr(chrBuf, _wpclsQueryInstanceType(m_wpObject))) {
    ulLength=strlen(chrBuf);
    if(ulLength) {
      if(ulLength+6 < sizeof(chrBuf)) {
        strcat(chrBuf, "\n");
        strcat(chrBuf, _wpclsQueryInstanceType(m_wpObject));
      }
      //  HlpWriteToTrapLog("1 %s\n", chrBuf);
    }
    else
      strcpy(chrBuf, _wpclsQueryInstanceType(m_wpObject));
    //   HlpWriteToTrapLog("2 %s\n", chrBuf);
  }
  _wpSetType(somSelf, chrBuf , NULL);
  return TRUE;
}

BOOL mmclsCreateTheDefaultTemplate(M_WPObject *somSelf, 
                                   WPObject* Folder)
{
  char chrSetup[120];
  WPFolder* wpFolder;
  //  char *chrPtr;

  sprintf(chrSetup, "<MMPM2_MM%sTEMPLATE>", _wpclsQueryInstanceType(somSelf));

#if 0
  HlpWriteToTrapLog("%s\n", chrSetup);
  chrPtr=chrSetup;
  /* Remove spaces in id */
  while(*chrPtr!=0)
    {
      if(*chrPtr==' ')
        *chrPtr='_';
      chrPtr++;
    };
#endif

  if(WinQueryObject(chrSetup))
    return TRUE; /* There's already a template */

  sprintf(chrSetup, "TEMPLATE=YES;NODELETE=YES;OBJECTID=<MMPM2_MM%sTEMPLATE>;", _wpclsQueryInstanceType(somSelf));
  wpFolder=_wpclsQueryFolder(somSelf, MMTEMPLATE_FOLDER_ID, FALSE);
  _wpclsNew(somSelf, _wpclsQueryTitle(somSelf), chrSetup, wpFolder,FALSE);

  return TRUE;
}





