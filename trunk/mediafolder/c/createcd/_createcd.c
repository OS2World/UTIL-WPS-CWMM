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
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSFILEMGR

#include <os2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mediafolderinc.h"



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
  int iNumCD;
  char cFirst;
  char setup[100];
  char name[100];
  char id[40];
  int a;

  if(!CDQueryCDDrives(&iNumCD, &cFirst))
    exit(1);
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
      if(!WinCreateObject(CDFLDR_CLASSNAME, name, setup,CDFLDR_LOCATION, CO_UPDATEIFEXISTS))
        printf("Can't create %s\n", name);
      else
        break;
      DosSleep(1000);
    }
  }
  exit(0);
}
