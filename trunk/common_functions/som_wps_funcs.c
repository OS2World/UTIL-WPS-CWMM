#define INCL_PM
#define INCL_DOS
#include <os2.h>
#include <stdio.h>
/* Include file with definition for WPShadow class */
#include "wpshadow.h"
#include "wpdisk.h"
#include "wpfsys.h" /* For _wpQueryDisk */


/*&************************************************/


/*&&***********************************************/

#if 0
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
BOOL somObjectIsOnCD(WPObject * somSelf)
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

/*!************************************************************/
/*                                                            */
/* @@DESC                                                     */
/* This function returns the pointer to a file system object  */
/* or NULL.                                                   */
/* It follows shadow objects and returns the object handle    */
/* of the linked object if it's a file system object          */
/*                                                            */
/*!!***********************************************************/
WPObject* somGetFileSystemObject(WPObject* wpObject)
{
  if(!somIsObj(wpObject))
    return NULL;//No object given

  /* Check if it's a shadow */
  //if(somResolveByName(wpObject,"wpQueryShadowedObject")){ 
  if(_somIsA(wpObject,_WPShadow)){ 
    /* Yes, it's a shadow. Query the linked object. */
    wpObject=_wpQueryShadowedObject((WPShadow*)wpObject, FALSE);
    if(!somIsObj(wpObject))
      return NULL;//The link is broken
  }

#if 0
  while(somResolveByName(wpObject,"wpQueryShadowedObject")) {
    /* Yes, it's a shadow. Query the linked object. */
    wpObject=_wpQueryShadowedObject((WPShadow*)wpObject, FALSE);
  }
#endif
  
  /* Check if it's a file system object */
  //  if(somResolveByName(wpObject, "wpQueryRealName")){
  if(_somIsA(wpObject, _WPFileSystem)){
    return wpObject;/* Yes */
  }
  else
    return NULL;
}

/*!*******************************************************/
/*                                                       */
/* @@DESC                                                */
/* Get the class object for the class <chrClassName>.    */
/*                                                       */
/*                                                       */
/* @@REMARKS                                             */
/*                                                       */
/* If the class isn't already loaded by SOM this function*/
/* will find it and load it into memory.                 */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* char* chrClassName input                              */
/*                                                       */
/* Class for which the class object will be returned.    */
/*                                                       */
/*                                                       */
/* @@RETURNS                                             */
/*                                                       */
/* SOMClass somCls                                       */
/* Pointer to the class object.                          */
/* In case of an error NULL is returned.                 */
/*                                                       */
/*!!******************************************************/
SOMClass* somGetSomClass(char* chrClassName)
{
  somId    mySomId;
  SOMClass *somClass;

  if((mySomId=somIdFromString(chrClassName))==NULLHANDLE)
    return NULLHANDLE;

  //  somClass=_somClassFromId(SOMClassMgrObject, mySomId);
  /* somFindClass() loads the class if not already done */
  somClass=_somFindClass(SOMClassMgrObject, mySomId, 1, 1);
  SOMFree(mySomId);

  return somClass;
}

#if  __cplusplus
extern "C" {

#endif

/*!*******************************************************/
/*                                                       */
/* @@DESC                                                */
/* Check if the object wpObject is an instance of the    */
/* class <chrClassName>.                                 */
/*                                                       */
/* @@REMARKS                                             */
/*                                                       */
/* The object may be an instance of the specified class  */
/* or of one of the subclasses when this function returns*/
/* TRUE. For example if the object is an instance of     */
/* MMMP3 and MMMP3 is a subclass of MMAudio a check      */
/* for MMAudio would return TRUE.                        */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* WPObject* wpObject input                              */
/* Object which should be tested.                        */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* char* chrClassName input                              */
/*                                                       */
/* Name of the class e.g. MMAudio.                       */
/*                                                       */
/* @@RETURNS                                             */
/*                                                       */
/* BOOL  rc                                              */
/* TRUE if the object is an instance of the class        */
/* <chrClassName>, or one of its subclasses,             */
/* FALSE otherwise.                                      */
/*                                                       */
/*                                                       */
/*!!******************************************************/
BOOL somObjectIsA(WPObject* wpObject, char * chrClassName)
{
  return _somIsA(wpObject, somGetSomClass(chrClassName));
}

#if  __cplusplus
}
#endif


WPObject* somResolveShadow(WPObject* wpObject)
{
  if(!somIsObj(wpObject))
    return NULLHANDLE;
  if(somObjectIsA(wpObject, "WPShadow"))
    return _wpQueryShadowedObject((WPShadow*)wpObject, FALSE);

  return wpObject;
}

/*!*******************************************************/
/*                                                       */
/* @@DESC                                                */
/* Realloc memory for a new string. If an error occurs   */
/* the old string is returned. In pulError the error     */
/* code of wpAllocMem() is returned. The memory of the   */
/* old string is freed if the function succeeds.         */
/*                                                       */
/* @@REMARKS                                             */
/* The memory containing the old string must have been   */
/* allocated using wpAllocMem(). Don't call this         */
/* functions with strings located in memory allocated    */
/* using DosAllocMem() or malloc(). pulError contains    */
/* the error code as returned by wpAllocMem().           */
/* If the string is no longer needed free the memory     */
/* using wpFreeMem().                                    */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* WPObject* wpObject input                              */
/* Object for which the wpAllocMem() method is called.   */
/*                                                       */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* PBYTE oldString input                                 */
/*                                                       */
/* Ponter to the already allocated string memory. If     */
/* this function succeeds, this memory will be freed.    */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* char* newString input                                 */
/*                                                       */
/* String which should be put into a new memory block    */
/*                                                       */
/* @@PARAM                                               */
/*                                                       */
/* PULONG pulError in/out                                */
/*                                                       */
/* Return code of wpAllocMem().                          */
/*                                                       */
/* @@RETURNS                                             */
/*                                                       */
/* PBYTE pByte                                           */
/* Pointer to the memory containing the newString.       */
/* If an error ocurred, the pointer to the old string    */
/* is returned. The strings are always null terminated.  */
/*                                                       */
/*                                                       */
/*!!******************************************************/
PBYTE somReallocString(WPObject* wpObject, PBYTE oldString, char* newString, PULONG pulError)
{
  ULONG ulLen;
  PBYTE pNewText;

  if(!newString)
    return oldString;

  ulLen=strlen(newString);
  /* Alloc mem for new string */
  if( (pNewText=_wpAllocMem(wpObject, ulLen, pulError))!=NULLHANDLE) {
    /* Copy string into memory */
    strcpy(pNewText, newString);
    if(oldString)
      _wpFreeMem(wpObject, oldString);
    return pNewText;
  }

  return oldString;;
}


BOOL wpsObjectIsInsertedInContainer(WPObject * wpObject)
{

  if(_wpFindUseItem(wpObject, USAGE_RECORD, NULLHANDLE))
    return TRUE;

  return FALSE;
}

/*
  This function draws the template (but not the icon) if the object is a template.
 */
ULONG wpsDrawTemplate(WPObject* wpObject, RECTL* rcl, HPS hps, ULONG flCnr)
{
  HPOINTER hptrTemplate;
  HMODULE hmodTemplate;
  
  if(!DosQueryModuleHandle("PMWP",&hmodTemplate))
    {
      ULONG ulIconSize;
      
      hptrTemplate=WinLoadPointer(HWND_DESKTOP, hmodTemplate, 20);
      WinDrawPointer(hps, rcl->xLeft, rcl->yBottom, hptrTemplate, (flCnr &  CV_MINI ? DP_MINI:DP_NORMAL));
      WinDestroyPointer(hptrTemplate);
      if((ulIconSize=WinQuerySysValue(HWND_DESKTOP,SV_CXICON))!=0)
        {
          ulIconSize=(rcl->xRight-rcl->xLeft-ulIconSize/2)/2;
          rcl->xLeft+=ulIconSize/2;
          rcl->yBottom+=ulIconSize;
        }
      return DP_MINI;
    }
  
  return DP_NORMAL;
}


//#include "sys_funcs.h"
/*
  This function redraws an object in all containers it is inserted in.
  */
void wpsRefreshObjectRecords(WPObject *somSelf)
{
  PUSEITEM pUse;
  //    SysWriteToTrapLog("!!!  %s: %s\n", __FUNCTION__, _wpQueryTitle(somSelf));
    for(pUse=_wpFindUseItem(somSelf, USAGE_RECORD, NULL );pUse;
        pUse=_wpFindUseItem(somSelf, USAGE_RECORD, pUse ))
      {
        PRECORDITEM pRecordItem=(PRECORDITEM)++pUse;
        WinPostMsg(pRecordItem->hwndCnr, CM_INVALIDATERECORD,MPFROMP(&pRecordItem->pRecord),
                   MPFROM2SHORT(1,CMA_NOREPOSITION/* |CMA_ERASE*/));
        //      SysWriteToTrapLog("!!!  %s: %s %x\n", __FUNCTION__, _wpQueryTitle(somSelf), pRecordItem->hwndCnr);
      }/* for() */
}



