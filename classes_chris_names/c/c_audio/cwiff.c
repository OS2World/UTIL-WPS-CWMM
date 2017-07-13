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
/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_cwiff_Source
#define SOM_Module_cwiff_Source
#endif
#define CWIFF_Class_Source
#define M_CWIFF_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwiff.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

SOM_Scope PSZ  SOMLINK cwiffM_wpclsQueryInstanceType(M_CWIFF *somSelf)
{
    /* M_CWIFFData *somThis = M_CWIFFGetData(somSelf); */
    M_CWIFFMethodDebug("M_CWIFF","cwiffM_wpclsQueryInstanceType");

    return "IFF";
}

SOM_Scope PSZ  SOMLINK cwiffM_wpclsQueryInstanceFilter(M_CWIFF *somSelf)
{
    /* M_CWIFFData *somThis = M_CWIFFGetData(somSelf); */
    M_CWIFFMethodDebug("M_CWIFF","cwiffM_wpclsQueryInstanceFilter");

    return "*.IFF";
}

SOM_Scope BOOL  SOMLINK cwiffM_wpclsQueryDefaultHelp(M_CWIFF *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWIFFData *somThis = M_CWIFFGetData(somSelf); */
    M_CWIFFMethodDebug("M_CWIFF","cwiffM_wpclsQueryDefaultHelp");


    if(pHelpPanelId)
      *pHelpPanelId=IFFFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWIFF_parent_M_CWAudio_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}

SOM_Scope PSZ  SOMLINK cwiffM_wpclsQueryTitle(M_CWIFF *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWIFFData *somThis = M_CWIFFGetData(somSelf); */
    M_CWIFFMethodDebug("M_CWIFF","cwiffM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_IFFTITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"IFFAudio.IFF");

    return chrTitle;
}

SOM_Scope ULONG  SOMLINK cwiffM_wpclsQueryIconData(M_CWIFF *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWIFFData *somThis = M_CWIFFGetData(somSelf); */
    M_CWIFFMethodDebug("M_CWIFF","cwiffM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONVOCFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );

}

