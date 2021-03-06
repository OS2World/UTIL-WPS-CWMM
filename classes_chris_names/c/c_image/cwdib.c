/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_cwdib_Source
#define SOM_Module_cwdib_Source
#endif
#define CWDIB_Class_Source
#define M_CWDIB_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwdib.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

SOM_Scope PSZ  SOMLINK cwdibM_wpclsQueryInstanceType(M_CWDIB *somSelf)
{
    /* M_CWDIBData *somThis = M_CWDIBGetData(somSelf); */
    M_CWDIBMethodDebug("M_CWDIB","cwdibM_wpclsQueryInstanceType");

    return "DIB";
}

SOM_Scope PSZ  SOMLINK cwdibM_wpclsQueryInstanceFilter(M_CWDIB *somSelf)
{
    /* M_CWDIBData *somThis = M_CWDIBGetData(somSelf); */
    M_CWDIBMethodDebug("M_CWDIB","cwdibM_wpclsQueryInstanceFilter");

    return "*.DIB";
}

SOM_Scope BOOL  SOMLINK cwdibM_wpclsQueryDefaultHelp(M_CWDIB *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWDIBData *somThis = M_CWDIBGetData(somSelf); */
    M_CWDIBMethodDebug("M_CWDIB","cwdibM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=DIBFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWDIB_parent_M_CWImage_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}

SOM_Scope ULONG  SOMLINK cwdibM_wpclsQueryStyle(M_CWDIB *somSelf)
{
    /* M_CWDIBData *somThis = M_CWDIBGetData(somSelf); */
    M_CWDIBMethodDebug("M_CWDIB","cwdibM_wpclsQueryStyle");

    return (M_CWDIB_parent_M_CWImage_wpclsQueryStyle(somSelf));
}

SOM_Scope PSZ  SOMLINK cwdibM_wpclsQueryTitle(M_CWDIB *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWDIBData *somThis = M_CWDIBGetData(somSelf); */
    M_CWDIBMethodDebug("M_CWDIB","cwdibM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_DIBTITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"DIBImage.DIB");

    return chrTitle;
}

SOM_Scope ULONG  SOMLINK cwdibM_wpclsQueryIconData(M_CWDIB *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWDIBData *somThis = M_CWDIBGetData(somSelf); */
    M_CWDIBMethodDebug("M_CWDIB","cwdibM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONDIBFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}

