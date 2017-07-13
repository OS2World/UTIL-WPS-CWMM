/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_cwtga_Source
#define SOM_Module_cwtga_Source
#endif
#define CWTarga_Class_Source
#define M_CWTarga_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwtga.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

SOM_Scope PSZ  SOMLINK cwtgaM_wpclsQueryInstanceType(M_CWTarga *somSelf)
{
    /* M_CWTargaData *somThis = M_CWTargaGetData(somSelf); */
    M_CWTargaMethodDebug("M_CWTarga","cwtgaM_wpclsQueryInstanceType");

    return "Targa";
}

SOM_Scope PSZ  SOMLINK cwtgaM_wpclsQueryInstanceFilter(M_CWTarga *somSelf)
{
    /* M_CWTargaData *somThis = M_CWTargaGetData(somSelf); */
    M_CWTargaMethodDebug("M_CWTarga","cwtgaM_wpclsQueryInstanceFilter");

    return "*.TGA";
}

SOM_Scope BOOL  SOMLINK cwtgaM_wpclsQueryDefaultHelp(M_CWTarga *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWTargaData *somThis = M_CWTargaGetData(somSelf); */
    M_CWTargaMethodDebug("M_CWTarga","cwtgaM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=TARGAFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWTarga_parent_M_CWImage_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}

SOM_Scope ULONG  SOMLINK cwtgaM_wpclsQueryStyle(M_CWTarga *somSelf)
{
    /* M_CWTargaData *somThis = M_CWTargaGetData(somSelf); */
    M_CWTargaMethodDebug("M_CWTarga","cwtgaM_wpclsQueryStyle");

    return (M_CWTarga_parent_M_CWImage_wpclsQueryStyle(somSelf));
}

SOM_Scope PSZ  SOMLINK cwtgaM_wpclsQueryTitle(M_CWTarga *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWTargaData *somThis = M_CWTargaGetData(somSelf); */
    M_CWTargaMethodDebug("M_CWTarga","cwtgaM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_TGATITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"TargaImage.TGA");

    return chrTitle;
}

SOM_Scope ULONG  SOMLINK cwtgaM_wpclsQueryIconData(M_CWTarga *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWTargaData *somThis = M_CWTargaGetData(somSelf); */
    M_CWTargaMethodDebug("M_CWTarga","cwtgaM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONTGAFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}
