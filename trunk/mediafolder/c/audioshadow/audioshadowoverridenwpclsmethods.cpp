/*
 * This file is (C) Chris Wohlgemuth 1999-2002
 * 
 * It's part of the Audio/Data-CD-Creator distribution
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

#include "audioshadow.hh"
//#include "mediafolderres.h"


extern HMODULE hResource;
extern char chrInstallDir[];

/* Overriden function which returns our class name */
PSZ M_CWAudioShadow::wpclsQueryTitle()
{
  return "Audio-Referenz";
}

ULONG M_CWAudioShadow::wpclsQueryDetailsInfo( PCLASSFIELDINFO* ppClassFieldInfo, PULONG pSize)
{
  M_WPObject *mWPObject;// Class object
  somId mySomId;
  ULONG rc;

  if((mySomId=somIdFromString(CWAUDIO_CLASS_NAME))!=NULLHANDLE) {
    mWPObject=(M_WPObject*)SOMClassMgrObject->somClassFromId(mySomId);
    SOMFree(mySomId);
    if(somIsObj(mWPObject)) {
      /* got the class object */
      return mWPObject->wpclsQueryDetailsInfo(ppClassFieldInfo, pSize);
    }
  }

  if((mySomId=somIdFromString("wpclsQueryDetailsInfo"))!=NULLHANDLE) {
    rc=((somTD_M_WPObject_wpclsQueryDetailsInfo)
        somParentNumResolve(__ClassObject->somGetPClsMtabs(),
                            1,
                            __ClassObject->    
                            somGetMethodToken(mySomId))                    
        )(this, ppClassFieldInfo, pSize);
    SOMFree(mySomId);
    return rc;
  }
  else 
    return M_WPShadow::wpclsQueryDetailsInfo(ppClassFieldInfo, pSize);

  return 0;
}
























