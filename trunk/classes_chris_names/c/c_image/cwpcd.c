/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_cwpcd_Source
#define SOM_Module_cwpcd_Source
#endif
#define CWPhotoCD_Class_Source
#define M_CWPhotoCD_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwpcd.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

SOM_Scope PSZ  SOMLINK cwpcdM_wpclsQueryInstanceType(M_CWPhotoCD *somSelf)
{
    /* M_CWPhotoCDData *somThis = M_CWPhotoCDGetData(somSelf); */
    M_CWPhotoCDMethodDebug("M_CWPhotoCD","cwpcdM_wpclsQueryInstanceType");

    return "Photo CD";
}

SOM_Scope PSZ  SOMLINK cwpcdM_wpclsQueryInstanceFilter(M_CWPhotoCD *somSelf)
{
    /* M_CWPhotoCDData *somThis = M_CWPhotoCDGetData(somSelf); */
    M_CWPhotoCDMethodDebug("M_CWPhotoCD","cwpcdM_wpclsQueryInstanceFilter");

    return "*.PCD";
}

SOM_Scope BOOL  SOMLINK cwpcdM_wpclsQueryDefaultHelp(M_CWPhotoCD *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWPhotoCDData *somThis = M_CWPhotoCDGetData(somSelf); */
    M_CWPhotoCDMethodDebug("M_CWPhotoCD","cwpcdM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=PCDFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWPhotoCD_parent_M_CWImage_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}

SOM_Scope ULONG  SOMLINK cwpcdM_wpclsQueryStyle(M_CWPhotoCD *somSelf)
{
    /* M_CWPhotoCDData *somThis = M_CWPhotoCDGetData(somSelf); */
    M_CWPhotoCDMethodDebug("M_CWPhotoCD","cwpcdM_wpclsQueryStyle");

    return (M_CWPhotoCD_parent_M_CWImage_wpclsQueryStyle(somSelf));
}

SOM_Scope PSZ  SOMLINK cwpcdM_wpclsQueryTitle(M_CWPhotoCD *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWPhotoCDData *somThis = M_CWPhotoCDGetData(somSelf); */
    M_CWPhotoCDMethodDebug("M_CWPhotoCD","cwpcdM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_PCDTITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"PCDImage.PCD");

    return chrTitle;

}

SOM_Scope ULONG  SOMLINK cwpcdM_wpclsQueryIconData(M_CWPhotoCD *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWPhotoCDData *somThis = M_CWPhotoCDGetData(somSelf); */
    M_CWPhotoCDMethodDebug("M_CWPhotoCD","cwpcdM_wpclsQueryIconData");


	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONPCDFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );

}

