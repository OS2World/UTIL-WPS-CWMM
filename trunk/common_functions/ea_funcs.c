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
 * If you need another license for your project/product contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 */

#define INCL_PM
#define  INCL_ERRORS
#include <os2.h>
#include <wpfolder.h>
#include "sys_funcs.h"

/*!*************************************************************/
/*                                                             */
/* @@FUNC                                                      */
/*        BOOL EASaveString(WPFileSystem * wpFSObject,         */
/*                          PSZ pKey, PSZ pValue)              */
/*                                                             */
/*                                                             */
/* @@DESC                                                      */
/*        Save an ASCII value to the EA <pKey> of the          */
/*        filesystem object <wpFSObject>.                      */
/*                                                             */
/* @@RETURNS                                                   */
/*                                                             */
/* BOOL rc                                                     */
/*                                                             */
/* TRUE if the string was saved in the extended attribute,     */
/* FALSE otherwise.                                            */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* WPFileSystem* wpFSObject input                              */
/*                                                             */
/* Pointer to a file system object.                            */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* PSZ pKey input                                              */
/*                                                             */
/* Attribute name to be used.                                  */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* PSZ pValue input                                            */
/*                                                             */
/* The null terminated string to be saved in the extended      */
/* attribute :hp2.pKey:ehp2..                                  */
/* :p.                                                         */
/* If this is an empty string or NULL the attribute will       */
/* be deleted from the file.                                   */
/*                                                             */
/* @@REMARKS                                                   */
/*                                                             */
/* Attribute names starting with a point (:hp2..:ehp2.) are    */
/* reserved for the operating system.                          */
/*                                                             */
/*                                                             */
/*                                                             */
/*                                                             */
/*!!************************************************************/
BOOL EASaveString(WPFileSystem * wpFSObject,  PSZ pKey, PSZ pValue)
{
  LONG rc;                             /* Ret code                   */
  PVOID       fealist;                 /* fealist buffer             */
  EAOP2       eaop;                    /* eaop structure             */
  PFEA2       pfea;                    /* pfea structure             */
  HFILE       handle;                  /* file handle                */
  ULONG       act;                     /* open action                */
  ULONG       ulSize;
  USHORT      usSizeEA;

  char chrFileName[CCHMAXPATH];

  //SysWriteToTrapLog("%s %s\n", pKey, pValue);

  if(!somIsObj(wpFSObject) || !pKey || strlen(pKey)==0)
    return FALSE;

  if(pValue) {
    if((fealist=_wpAllocMem(wpFSObject, strlen(pKey)+strlen(pValue)+sizeof(FEA2)+0x100 /* Add some space */,
                            &ulSize))==NULLHANDLE)
      return FALSE;
  }
  else {
    if((fealist=_wpAllocMem(wpFSObject, strlen(pKey)+sizeof(FEA2)+0x100 /* Add some space */,
                            &ulSize))==NULLHANDLE)
      return FALSE;
  }

  ulSize=sizeof(chrFileName);
  _wpQueryRealName(wpFSObject, chrFileName,&ulSize,TRUE);


  if ((rc = DosOpen(chrFileName, &handle, &act,
      0L, 0, OPEN_ACTION_OPEN_IF_EXISTS,
      OPEN_ACCESS_READWRITE + OPEN_SHARE_DENYWRITE +
      OPEN_FLAGS_FAIL_ON_ERROR + OPEN_FLAGS_WRITE_THROUGH, NULL))!=NO_ERROR) {
    _wpFreeMem(wpFSObject,fealist);     /* Free the memory            */
    return FALSE;
  }

  eaop.fpFEA2List = (PFEA2LIST)fealist;/* Set memory for the FEA        */
  eaop.fpGEA2List = NULL;              /* GEA is unused                 */
  pfea = &eaop.fpFEA2List->list[0];    /* point to first FEA            */
  pfea->fEA = '\0';                    /* set the flags to non critical */
                                       /* Size of FEA name field        */
  pfea->cbName = (BYTE)strlen(pKey);
  strcpy((PSZ)pfea->szName, pKey);     /* Set the name of this FEA      */


  /* Size of Value for this one */
  if(!pValue || strlen(pValue)==0) {
    pfea->cbValue = 0;                 /* Delete EA if no Value given   */
  }
  else {
    pfea->cbValue = (SHORT)strlen(pValue)+4; /* 4 bytes for marking as ASCII attribute */

    /* Set the EA value after the FEA2 */
    usSizeEA=0xFFFD;
    memcpy((PSZ)pfea->szName+(pfea->cbName+1), &usSizeEA, 2);             /* It's an ASCII value    */
    usSizeEA=strlen(pValue);
    memcpy((PSZ)pfea->szName+(pfea->cbName+3), &usSizeEA, sizeof(USHORT));/* Length of ASCII value  */
    
    memcpy((PSZ)pfea->szName+(pfea->cbName+5), pValue, usSizeEA);         /* And now the value      */
  }
  pfea->oNextEntryOffset = 0;                                           /* no next entry          */
  eaop.fpFEA2List->cbList =                                             /* Set the total size var */
      sizeof(ULONG) + sizeof(FEA2) + pfea->cbName + pfea->cbValue;

  /* set the file info          */
  rc = DosSetFileInfo(handle, 2, (PSZ)&eaop, sizeof(EAOP2));
  DosClose(handle);                                                     /* Close the File         */
  _wpFreeMem(wpFSObject,fealist);                                       /* Free the memory        */
  return TRUE;
}

/*!*************************************************************/
/*                                                             */
/* @@FUNC                                                      */
/*        BOOL EARestoreString(WPFileSystem * wpFSObject,      */
/*             PSZ pKey, char * chrBuffer, ULONG ulBuffSize)   */
/*                                                             */
/*                                                             */
/* @@DESC                                                      */
/*        Read an ASCII value from the EA <pKey> of the        */
/*        filesystem object <wpFSObject>.                      */
/*                                                             */
/* @@RETURNS                                                   */
/*                                                             */
/* BOOL rc                                                     */
/*                                                             */
/* TRUE if the string was queried from the extended attribute, */
/* FALSE in case of an error or if there's no attribute        */
/* with the specified name.                                    */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* WPFileSystem* wpFSObject input                              */
/*                                                             */
/* Pointer to a file system object.                            */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* PSZ pKey input                                              */
/*                                                             */
/* Attribute name.                                             */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* char* chrBuffer in/out                                      */
/*                                                             */
/* Buffer into which the string will be placed.                */
/*                                                             */
/* @@PARAM                                                     */
/*                                                             */
/* ULONG ulBuffSize input                                      */
/*                                                             */
/* Size of buffer :hp2.chrBuffer:ehp2..                        */
/*                                                             */
/* @@REMARKS                                                   */
/*                                                             */
/* The string is always null terminated. If the buffer isn't   */
/* large enough at most ulBuffSize-1 characters will be read.  */
/*                                                             */
/*                                                             */
/*!!************************************************************/
BOOL EARestoreString(WPFileSystem * wpFSObject,  PSZ pKey, char * chrBuffer, ULONG ulBuffSize)
{
  LONG rc;                             /* Ret code                   */
  UCHAR       geabuff[300];            /* buffer for GEA             */
  PVOID       fealist;                 /* fealist buffer             */
  EAOP2       eaop;                    /* eaop structure             */
  PGEA2       pgea;                    /* pgea structure             */
  PFEA2       pfea;                    /* pfea structure             */
  HFILE       handle;                  /* file handle                */
  ULONG       act;                     /* open action                */
  ULONG ulSize;
  USHORT     *ptrUs;

  char chrFileName[CCHMAXPATH];

  if((fealist=_wpAllocMem(wpFSObject, 0x00010000L /* Add some space */,
                          &ulSize))==NULLHANDLE)
    return FALSE;

  ulSize=sizeof(chrFileName);
  _wpQueryRealName(wpFSObject, chrFileName,&ulSize,TRUE);

  if ((rc = DosOpen(chrFileName, &handle, &act,
      0L, 0, OPEN_ACTION_OPEN_IF_EXISTS,
      OPEN_ACCESS_READONLY + OPEN_SHARE_DENYREADWRITE +
      OPEN_FLAGS_FAIL_ON_ERROR + OPEN_FLAGS_WRITE_THROUGH,NULL))!=NO_ERROR) {
    _wpFreeMem(wpFSObject,fealist);    /* Free the memory            */
    return FALSE;
  }                                    /* get the file status info   */


                                       /* FEA and GEA lists          */
  eaop.fpGEA2List = (PGEA2LIST)geabuff;
  eaop.fpFEA2List = (PFEA2LIST)fealist;
  eaop.oError = 0;                     /* no error occurred yet      */
  pgea = &eaop.fpGEA2List->list[0];    /* point to first GEA         */
  eaop.fpGEA2List->cbList = sizeof(ULONG) + sizeof(GEA2) +
    strlen(pKey);
  eaop.fpFEA2List->cbList = (ULONG)0xffff;

                                       /* fill in the EA name length */
  pgea->cbName = (BYTE)strlen(pKey);
  strcpy(pgea->szName, pKey);          /* fill in the name           */
  pgea->oNextEntryOffset = 0;          /* fill in the next offset    */
                                       /* read the extended attribute*/
  rc = DosQueryFileInfo(handle, 3, (PSZ)&eaop, sizeof(EAOP2));
  DosClose(handle);                    /* close the file             */
  if (eaop.fpFEA2List->cbList <= sizeof(ULONG))
    rc = ERROR_EAS_NOT_SUPPORTED;      /* this is error also         */

  if (rc) {                            /* failure?                   */
    _wpFreeMem(wpFSObject,fealist);    /* Free the memory            */
    return FALSE;
  }

  pfea = &(eaop.fpFEA2List->list[0]);  /* point to the first FEA     */

#if 0
  ldp.shvb.shvname.strptr = ldp.varname;
  ldp.shvb.shvname.strlength = ldp.stemlen;
  ldp.shvb.shvnamelen = ldp.stemlen;
  ldp.shvb.shvvalue.strptr = ((PSZ)pfea->szName+(pfea->cbName+1));
  ldp.shvb.shvvalue.strlength = pfea->cbValue;
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
#endif

  ptrUs=(USHORT*)((PSZ)pfea->szName+(pfea->cbName+1));
  if(*ptrUs!=0xfffd) {
    /* Not an ASCII EA */
    _wpFreeMem(wpFSObject,fealist);    /* Free the memory            */
    return FALSE;
  }
  ptrUs++; /* Move to length */
  ulSize=(ULONG)*ptrUs;

  ptrUs++; /* Move to value */
  if(ulSize==0) {
    /* Not an ASCII EA */
    _wpFreeMem(wpFSObject,fealist);    /* Free the memory            */
    return FALSE;
  }
  //  HlpWriteToTrapLog("Length: %x, Value: %s\n", ulSize, (char*)ptrUs);
  memset(chrBuffer,0,ulBuffSize);
  memcpy(chrBuffer, (char*)ptrUs, (ulSize>=ulBuffSize ? ulBuffSize-1 : ulSize));
  _wpFreeMem(wpFSObject,fealist);    /* Free the memory            */
  return TRUE;
}
