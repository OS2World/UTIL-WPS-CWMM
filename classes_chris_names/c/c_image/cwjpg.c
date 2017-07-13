/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_cwjpg_Source
#define SOM_Module_cwjpg_Source
#endif
#define CWJPG_Class_Source
#define M_CWJPG_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwjpg.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

SOM_Scope PSZ  SOMLINK cwjpgM_wpclsQueryInstanceType(M_CWJPG *somSelf)
{
    /* M_CWJPGData *somThis = M_CWJPGGetData(somSelf); */
    M_CWJPGMethodDebug("M_CWJPG","cwjpgM_wpclsQueryInstanceType");

    return "JPG";
}

SOM_Scope PSZ  SOMLINK cwjpgM_wpclsQueryInstanceFilter(M_CWJPG *somSelf)
{
    /* M_CWJPGData *somThis = M_CWJPGGetData(somSelf); */
    M_CWJPGMethodDebug("M_CWJPG","cwjpgM_wpclsQueryInstanceFilter");

    return "*.JPG";
}

SOM_Scope BOOL  SOMLINK cwjpgM_wpclsQueryDefaultHelp(M_CWJPG *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWJPGData *somThis = M_CWJPGGetData(somSelf); */
    M_CWJPGMethodDebug("M_CWJPG","cwjpgM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=JPGFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWJPG_parent_M_CWImage_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}

SOM_Scope ULONG  SOMLINK cwjpgM_wpclsQueryStyle(M_CWJPG *somSelf)
{
    /* M_CWJPGData *somThis = M_CWJPGGetData(somSelf); */
    M_CWJPGMethodDebug("M_CWJPG","cwjpgM_wpclsQueryStyle");

    return (M_CWJPG_parent_M_CWImage_wpclsQueryStyle(somSelf));
}

SOM_Scope PSZ  SOMLINK cwjpgM_wpclsQueryTitle(M_CWJPG *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWJPGData *somThis = M_CWJPGGetData(somSelf); */
    M_CWJPGMethodDebug("M_CWJPG","cwjpgM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_JPGTITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"JPGImage.JPG");

    return chrTitle;
}

SOM_Scope ULONG  SOMLINK cwjpgM_wpclsQueryIconData(M_CWJPG *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWJPGData *somThis = M_CWJPGGetData(somSelf); */
    M_CWJPGMethodDebug("M_CWJPG","cwjpgM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONJPGFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}
