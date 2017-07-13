/*
 * This file is (C) Chris Wohlgemuth 2002-2003
 * 
 * It's part of the CW-Multimedia classes distribution
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
#define INCL_GPIBITMAPS

#include "audioshadow.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mediafolderres.h"
//#include "mediafolderinc.h"


extern HMODULE hResource;

void errorResource();
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
void getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

ULONG CWAudioShadow::wpQueryDetailsData(PVOID* ppDetailsData, PULONG pcp)
{
  WPObject *wpObject;
  M_WPObject *mWPObject;
  somId mySomId;
  ULONG rc;

  if((mySomId=somIdFromString(CWAUDIO_CLASS_NAME))!=NULLHANDLE) {
    mWPObject=(M_WPObject*)SOMClassMgrObject->somClassFromId(mySomId);// Get CWAudio class object
    SOMFree(mySomId);
    wpObject=wpQueryShadowedObject(FALSE);//Get linked object
#if 0
    if(somIsObj(wpObject)) {
      if(somResolveByName(wpObject,"wpQueryShadowedObject")){ 
        /* Yes, it's a shadow. Query the linked object. */
        DosBeep(5000, 200);
        //      wpObject=((WPShadow*)wpObject)->wpQueryShadowedObject(FALSE);
      }
    }
#endif
    if(somIsObj(wpObject)) {
      if(wpObject->somIsA(mWPObject)) {/* Check if the object is of class CWAudio or descendent */
        return wpObject->wpQueryDetailsData( ppDetailsData, pcp);
      }
    }
  }

  if((mySomId=somIdFromString("wpQueryDetailsData"))!=NULLHANDLE) {
    rc=((somTD_WPObject_wpQueryDetailsData)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, ppDetailsData, pcp);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return WPShadow::wpQueryDetailsData(ppDetailsData, pcp);

}


















