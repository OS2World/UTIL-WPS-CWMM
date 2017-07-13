
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

#define INCL_DOSDEVIOCTL
#define INCL_DOS
#include <os2.h>

/**************************************************************/
/*                                                            */
/* This funktion returns the CD-Drives in the system          */
/*                                                            */
/* iNumCD (output): # of CD-Drives                            */
/* iFirstDrive (output): first cd-Drive                       */
/* returns TRUE: We have cd-drives                            */
/*                                                            */
/**************************************************************/
BOOL cwQueryCDDrives(int *iNumCD, int * iFirstDrive)
{
  HFILE hfDevice;
  ULONG ulAction;
  ULONG ulLen;
  static int iFirst=0;
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
              iFirst=1+CDInfo.usFirstCD;
            }                                        
          }
        DosClose(hfDevice);
      }
    done=TRUE;
  }
  *iNumCD=iNumCDLocal;
  *iFirstDrive=iFirst;
  return haveCD;              
}

/****************************************/
/* Input: drive (e.g. o:)               */
/****************************************/
HFILE openDrive(char* drive)
{
	HFILE hfDrive = 0;
	ULONG ulAction;
	ULONG rc;

	rc = DosOpen(drive, &hfDrive, &ulAction, 0,
					 FILE_NORMAL, OPEN_ACTION_OPEN_IF_EXISTS,
					 OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY |
					 OPEN_FLAGS_DASD|OPEN_FLAGS_FAIL_ON_ERROR, NULL);	

	if(rc)
		return NULLHANDLE;//Error

	return hfDrive;
}

void closeDrive(HFILE hfDrive)
{
	DosClose(hfDrive);	
}

#pragma pack(1)

typedef struct{
	UCHAR   ucFirstTrack;
	UCHAR 	ucLastTrack;
	ULONG   ulLeadOut;
}CDINFO;

typedef struct
{
	UCHAR signature[4];
	UCHAR  ucTrackNum;
}TINFOPARAM;

typedef struct
{
	ULONG ulTrackAddress;
	UCHAR ucTCInfo;
}CDTRACKINFO;

#pragma pack()

/******************************************/
/* Result:  
	 0:   Error
	 -1: CD is Data Disk
	 other: # Audio tracks */
/******************************************/
int CDQueryAudioCDTracks(HFILE hfDrive)
{
	ULONG ulParamLen;
	ULONG ulDataLen;
	ULONG rc;
	CDINFO cdInfo;
	TINFOPARAM tip;
	CDTRACKINFO trackInfo;
    //char text[100];	

	ulDataLen=sizeof(cdInfo);
	ulParamLen=4;
		
	if(!hfDrive)
		return 0;
	
	rc = DosDevIOCtl(hfDrive, IOCTL_CDROMAUDIO, CDROMAUDIO_GETAUDIODISK,
						  "CD01", 4, &ulParamLen, &cdInfo,
						  sizeof(cdInfo), &ulDataLen);

	if(rc) {
		return 0;//Error
	}

	ulDataLen=sizeof(trackInfo);
	ulParamLen=sizeof(TINFOPARAM);
	tip.signature[0]='C';
	tip.signature[1]='D';	
	tip.signature[2]='0';
	tip.signature[3]='1';
	tip.ucTrackNum=1;
	
	/* We have a disc. Check if it's audio */
	rc = DosDevIOCtl(hfDrive, IOCTL_CDROMAUDIO, CDROMAUDIO_GETAUDIOTRACK,
						  &tip, sizeof(tip), &ulParamLen, &trackInfo,
						  sizeof(trackInfo), &ulDataLen);

	if(rc) {
		return 0;//Error
	}
		
	if(trackInfo.ucTCInfo & 0x40) {
		return -1;//It's a data track
	}
    return cdInfo.ucLastTrack-cdInfo.ucFirstTrack+1;	
}

