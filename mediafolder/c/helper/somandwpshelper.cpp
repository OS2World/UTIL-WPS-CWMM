/*
 * This file is (C) Chris Wohlgemuth 2002
 * 
 * It's part of the Media-Folder distribution
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

#define INCL_DOSERRORS
#define INCL_DOSERRORS
#define INCL_GPIBITMAPS

#include "mediafolder.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mediafolderres.h"
#include "menufolder.hh"
#include "cwcdtrack.hh"

/* typedef for somTP_MMAudio_cwmmQueryTrackInfo */
/* For calling the method in the CWAudio class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
#include "cwmmquerytrackinfo.h"

somTD_MMAudio_cwmmQueryTrackInfo methodPtr=NULL;

/* For calling the method in the CWCDTrack class. Linking to the DLL would require the
   new audio classes. By dynamically querying the method the media folder works without
   installing the audio classes. */  
typedef ULONG   SOMLINK somTP_MMCDTrack_cwmmQueryTrackInfo(MMCDTrack *somSelf,
		PSZ chrString,
		ULONG ulSize,
		int iWhich);
typedef somTP_MMCDTrack_cwmmQueryTrackInfo *somTD_MMCDTrack_cwmmQueryTrackInfo;

somTD_MMCDTrack_cwmmQueryTrackInfo methodPtrCDTrack=NULL;


SOMClass* somhlpGetSomClass(char* chrClassName)
{
  somId    mySomId;
  SOMClass *somClass;

  if((mySomId=somIdFromString(chrClassName))==NULLHANDLE)
    return NULLHANDLE;

  //  somClass=SOMClassMgrObject->somClassFromId(mySomId);
  somClass=SOMClassMgrObject->somFindClass(mySomId, 1, 1);
  SOMFree(mySomId);

  return somClass;
}

BOOL somhlpQueryCWAudioMethodPtr(CWMediaFolder *thisPtr)
{
  somId    mySomId;
  SOMClass *mmAudioClass;

  if(!methodPtr) {
    /* If we don't have the method pointer yet, get it. */
    mmAudioClass=somhlpGetSomClass("MMAudio");
    if(mmAudioClass) {
      /* We have the class object */
      mySomId=somIdFromString("cwmmQueryTrackInfo");/* A new method in MMAudio (replacement for MMAudio) */ 
      methodPtr=(somTD_MMAudio_cwmmQueryTrackInfo) mmAudioClass->somFindSMethod(mySomId);
      SOMFree(mySomId);
      return TRUE;
    }
    return FALSE;
  }
  else
    return TRUE;
#if 0
  if(!methodPtr) {
    /* If we don't have the method pointer yet, get it. */
    /* Get class object of MMAudio. */
    if((mySomId=somIdFromString("MMAudio"))==NULLHANDLE)
      return FALSE;
    mmAudioClass=SOMClassMgrObject->somClassFromId(mySomId);
    SOMFree(mySomId);
    if(mmAudioClass) {
      /* We have the class object */
      mySomId=somIdFromString("cwmmQueryTrackInfo");/* A new method in MMAudio (replacement for MMAudio) */ 
      methodPtr=(somTD_MMAudio_cwmmQueryTrackInfo) mmAudioClass->somFindSMethod(mySomId);
      SOMFree(mySomId);
      return TRUE;
    }
    return FALSE;
  }
  else
    return TRUE;
#endif
}

BOOL somhlpQueryCWCDTrackMethodPtr(CWMediaFolder *thisPtr)
{
  somId    mySomId;
  SOMClass *mmCDTrackClass;

  if(!methodPtrCDTrack) {
    /* If we don't have the method pointer yet, get it. */
    /* Get class object of CWCDTrack. */
    mmCDTrackClass=somhlpGetSomClass("MMCDTrack");
    if(mmCDTrackClass) {
      /* We have the class object */
      mySomId=somIdFromString("cwmmQueryTrackInfo");/* A new method in CWAudio/CWCDTrack (replacement for MMAudio) */ 
      methodPtrCDTrack=(somTD_MMCDTrack_cwmmQueryTrackInfo) mmCDTrackClass->somFindSMethod(mySomId);
      SOMFree(mySomId);
      return TRUE;
    }
    return FALSE;
  }
  else
    return TRUE;
}

SOMClass* somhlpQueryMMAudioClass(void)
{
  somId mySomId;
  static SOMClass *mAudioClass=NULL;


  if(!mAudioClass)
    mAudioClass=somhlpGetSomClass("MMAudio");
  
  return mAudioClass;

#if 0
  if(!mAudioClass) {
    if((mySomId=somIdFromString("MMAudio"))!=NULLHANDLE) {
      mAudioClass=(SOMClass*)SOMClassMgrObject->somClassFromId(mySomId);
      SOMFree(mySomId);
    }
  }
  return mAudioClass;
#endif
}




