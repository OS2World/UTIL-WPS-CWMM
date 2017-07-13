#define INCL_GPIBITMAPS
#define INCL_PM
#include <os2.h>
#include <stdio.h>
/* Include file with definition for WPShadow class */
#include "wpshadow.h"
#include "wpdisk.h"
#include "wpfsys.h" /* For _wpQueryDisk */

BOOL cwQueryCDDrives(int *iNumCD, int * iFirstDrive);

int iFirstCD;
int iNumCD;
BOOL bGotCD=FALSE;


static BOOL _queryCDOnce(void)
{
  static BOOL bTriedCD=FALSE;

  /* Only search for CDs once to speed up things */
  if(!bTriedCD) {
    bTriedCD=TRUE; /* Don't search for CDs again */ 
    /* Get CD drives */
    if(cwQueryCDDrives(&iNumCD, &iFirstCD))
      bGotCD=TRUE;
  }
  return TRUE;
}

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

/**************************************************************/
/*                                                            */
/* This function returns the pointer to a file system object  */
/* or NULL.                                                   */
/* It follows shadow objects and returns the object handle    */
/* of the linked object if it's a file system object          */
/*                                                            */
/**************************************************************/
WPObject* cwGetFileSystemObject(WPObject* wpObject)
{
  if(!somIsObj(wpObject)) return NULL;//No object given
  

  /* Check if it's a shadow */
  if(somResolveByName(wpObject,"wpQueryShadowedObject")){ 
    /* Yes, it's a shadow. Query the linked object. */
    wpObject=_wpQueryShadowedObject((WPShadow*)wpObject, FALSE);
  }

#if 0
  while(somResolveByName(wpObject,"wpQueryShadowedObject")) {
    /* Yes, it's a shadow. Query the linked object. */
    wpObject=_wpQueryShadowedObject((WPShadow*)wpObject, FALSE);
  }
#endif
  
  if(!somIsObj(wpObject)) return NULL;//The link is broken
  
  /* Check if it's a file system object */
  if(somResolveByName(wpObject, "wpQueryRealName")){
    return wpObject;/* Yes */
  }
  else
    return NULL;
}

SOMClass* cwGetSomClass(char* chrClassName)
{
  somId    mySomId;
  SOMClass *somClass;

  if((mySomId=somIdFromString(chrClassName))==NULLHANDLE)
    return NULLHANDLE;

  somClass=_somClassFromId(SOMClassMgrObject, mySomId);
  SOMFree(mySomId);

  return somClass;
}

