/*
 * This file is (C) Chris Wohlgemuth 1999-2002
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

#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSFILEMGR

#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mediafolderinc.h"

#define  LVM_IS_HARDDISK  0  
#define  LVM_IS_PRM       1
#define  LVM_IS_CDROM     2
#define  LVM_IS_NETWORK   3
#define  LVM_IS_UNKNOWN   4

/* Typedefs for LVM */
typedef unsigned long      CARDINAL32;
typedef unsigned long      DoubleWord; /* ULONG */
typedef unsigned char      BOOLEAN;

#define VOLUME_NAME_SIZE      20
#define FILESYSTEM_NAME_SIZE  20

#ifdef ADDRESS
  #undef ADDRESS
#endif
typedef void*               ADDRESS;


HMODULE hMod=NULLHANDLE;

typedef struct _Volume_Control_Record {
  DoubleWord  Volume_Serial_Number;
  ADDRESS     Volume_Handle;
  BOOLEAN     Compatibility_Volume;
  BYTE        Device_Type;
  BYTE        Reserved[2];   /* Padding */
} Volume_Control_Record;

typedef struct _Volume_Control_Array {
  Volume_Control_Record*    Volume_Control_Data;
  CARDINAL32                Count;
} Volume_Control_Array;

typedef struct _Volume_Information_Record {
  CARDINAL32  Volume_Size;
  CARDINAL32  Partition_Count;
  CARDINAL32  Drive_Letter_Conflict;
  BOOLEAN     Compatibility_Volume;
  BOOLEAN     Bootable;
  char        Drive_Letter_Preference;
  char        Current_Drive_Letter;
  char        Initial_Drive_Letter;
  BOOLEAN     New_Volume;
  BYTE        Status;
  BYTE        Reserved_1;            /* PAdding */
  char       Volume_Name[VOLUME_NAME_SIZE];
  char       File_System_Name[FILESYSTEM_NAME_SIZE];
} Volume_Information_Record;

typedef Volume_Control_Array (_System _PFNVCA) ();
typedef _PFNVCA *PFNVCA;

typedef Volume_Information_Record (_System _PFNVIR) ();
typedef _PFNVIR *PFNVIR;

PFN pfnOpen_LVM_Engine=NULLHANDLE;
PFN pfnClose_LVM_Engine=NULLHANDLE;
PFNVCA pfnvcaGet_Volume_Control_Data=NULLHANDLE;
PFNVIR pfnvirGet_Volume_Information=NULLHANDLE;

BOOL loadLVMFuncs(void)
{
  char chrErrorObject[CCHMAXPATH]={0};
  ULONG ulCB;

  /* Try to load LVM */
  if(DosLoadModule(chrErrorObject, sizeof(chrErrorObject), "LVM.DLL", &hMod)!=NO_ERROR)
    return FALSE;
  //  printf("Got LVM.DLL\n");

  for(;;) {
    /* Get proc addresses */
    if(DosQueryProcAddr(hMod, 0, "Open_LVM_Engine", &pfnOpen_LVM_Engine)!=NO_ERROR)
      break;
    //    printf("Got Open_LVM_Engine: %x\n", pfnOpen_LVM_Engine);

    if(DosQueryProcAddr(hMod, 0, "Close_LVM_Engine", &pfnClose_LVM_Engine)!=NO_ERROR)
      break;
    //    printf("Got Close_LVM_Engine\n");

    if(DosQueryProcAddr(hMod, 0, "Get_Volume_Control_Data", (PFN*)&pfnvcaGet_Volume_Control_Data)!=NO_ERROR)
      break;
    //   printf("Got Get_Volume_Control_Data: %x\n", pfnvcaGet_Volume_Control_Data);

    if(DosQueryProcAddr(hMod, 0, "Get_Volume_Information", (PFN*)&pfnvirGet_Volume_Information)!=NO_ERROR)
      break;
    //  printf("Got Get_Volume_Information: %x\n", pfnvirGet_Volume_Information);

    return TRUE;
  }
  DosFreeModule(hMod);
  return FALSE;
}

/**************************************************************/
/*                                                            */
/* This funktion returns the CD-Drives in the system          */
/*                                                            */
/* iNumCD (output): # of CD-Drives                            */
/* cFirstDrive (output): first cd-Drive letter                */
/* returns TRUE: We have cd-drives                            */
/*                                                            */
/**************************************************************/
BOOL CDQueryCDDrives(int *iNumCD, char * cFirstDrive)
{
  HFILE hfDevice;
  ULONG ulAction;
  ULONG ulLen;
  static char cFirst=0;
  static int iNumCDLocal=0;
  static BOOL haveCD=FALSE;
  static BOOL done=FALSE;			
  struct
  {
    USHORT usCountCD;
    USHORT usFirstCD;
  } CDInfo;
  
  if(!done){
    ulLen = sizeof(CDInfo);
    if(!DosOpen("\\DEV\\CD-ROM2$", &hfDevice, &ulAction, 0,
                FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
                OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY, NULL))
      {
        if(!DosDevIOCtl(hfDevice, 0x82, 0x60, NULL, 0, NULL, &CDInfo, ulLen, &ulLen))
          {
            if(CDInfo.usCountCD) {
              haveCD=TRUE;
              iNumCDLocal=CDInfo.usCountCD;
              cFirst='A'+ CDInfo.usFirstCD;
            }										 
          }
        DosClose(hfDevice);
      }
    done=TRUE;
  }
  *iNumCD=iNumCDLocal;
  *cFirstDrive=cFirst;
  return haveCD;				
}

int main(void)
{
  CARDINAL32 error;
  int iNumCD;
  char cFirst;
  char setup[100];
  char name[100];
  char id[40];
  int a;

  if(!CDQueryCDDrives(&iNumCD, &cFirst))
    exit(1);

  if(!loadLVMFuncs()) {
    for(a=0;a<iNumCD;a++) {
      int b;
      /* Build folder name */
      sprintf(name, "CD-Audio^Drive %c:", cFirst+a);
      /* Build object ID */
      sprintf(id, CDFLDR_ID, cFirst+a);
      /* Build setup string */
      sprintf(setup, "%s=%c:;OBJECTID=%s;", CDFLDR_DRIVE, cFirst+a, id);
      //   DosSleep(1500);
      for(b=1;b<=5;b++) {
        /* Wait to let WPS load the class if not yet done. Then retry */ 
        if(WinCreateObject(CDFLDR_CLASSNAME, name, setup,CDFLDR_LOCATION, CO_UPDATEIFEXISTS))
          /*printf("Can't create %s\n", name);*/
          break;
        DosSleep(5000);
      }
    }
  }
  else {
    /* LVM system */
    printf("This is a LVM system\n");

    pfnOpen_LVM_Engine(TRUE, &error);
    if(!error) {
      Volume_Control_Array vca={0};
      printf("LVM engine opened\n");
      
      vca= pfnvcaGet_Volume_Control_Data(&error);
      if(!error) {
        int a;

        printf("Successfully got volume data\n");
        for(a=0; a<vca.Count; a++) {
          /* Now check all device types */
          if(vca.Volume_Control_Data[a].Device_Type==LVM_IS_CDROM) {
            Volume_Information_Record vir;
            int b;

            vir=pfnvirGet_Volume_Information(vca.Volume_Control_Data[a].Volume_Handle, &error);
            if(!error) {
              //printf("Device_type: %d, drive letter: %c:\n", 
              //     vca.Volume_Control_Data[a].Device_Type, vir.Current_Drive_Letter);

              /* Build folder name */
              sprintf(name, "CD-Audio^Drive %c:", vir.Current_Drive_Letter);
              /* Build object ID */
              sprintf(id, CDFLDR_ID, vir.Current_Drive_Letter);
              /* Build setup string */
              sprintf(setup, "%s=%c:;OBJECTID=%s;", CDFLDR_DRIVE, vir.Current_Drive_Letter, id);

              for(b=1;b<=5;b++) {
                /* Wait to let WPS load the class if not yet done. Then retry */ 

                if(WinCreateObject(CDFLDR_CLASSNAME, name, setup, CDFLDR_LOCATION, CO_UPDATEIFEXISTS))
                  break;

                //      printf("Found:  %s, %s\n", name, setup);
                DosSleep(5000);
              } /* for(b)*/
            }
          }
        }/* for(a) */
      }
      /* Close engine */
      pfnClose_LVM_Engine();
    }
    DosFreeModule(hMod);
  } /* else */
  return (0);
}


