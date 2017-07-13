/*
 * (C) Chris Wohlgemuth 2002-2004
 *
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
/*
 * If you need another license for your project/product (commercial,
 * noncommercial, whatever) contact me at
 * 
 * http://www.os2world.com/cdwriting
 * http://www.geocities.com/SiliconValley/Sector/5785/
 *
 */
/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_cwflac_Source
#define SOM_Module_cwflac_Source
#endif
#define MMFLAC_Class_Source
#define M_MMFLAC_Class_Source

#include <os2.h>
#include "cwflac.ih"
#include "cwaudioinc.h"

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

SOM_Scope PSZ  SOMLINK cwflacM_wpclsQueryInstanceType(M_MMFLAC *somSelf)
{
    /* M_MMFLACData *somThis = M_MMFLACGetData(somSelf); */
    M_MMFLACMethodDebug("M_MMFLAC","cwflacM_wpclsQueryInstanceType");

    return TYPE_FLAC;
}

SOM_Scope PSZ  SOMLINK cwflacM_wpclsQueryInstanceFilter(M_MMFLAC *somSelf)
{
    /* M_MMFLACData *somThis = M_MMFLACGetData(somSelf); */
    M_MMFLACMethodDebug("M_MMFLAC","cwflacM_wpclsQueryInstanceFilter");

    return "*.FLA,*.FLAC";
}

SOM_Scope BOOL  SOMLINK cwflacM_wpclsQueryDefaultHelp(M_MMFLAC *somSelf, 
                                                      PULONG pHelpPanelId, 
                                                      PSZ pszHelpLibrary)
{
    /* M_MMFLACData *somThis = M_MMFLACGetData(somSelf); */
    M_MMFLACMethodDebug("M_MMFLAC","cwflacM_wpclsQueryDefaultHelp");

    return (M_MMFLAC_parent_M_MMAudio_wpclsQueryDefaultHelp(somSelf, 
                                                            pHelpPanelId, 
                                                            pszHelpLibrary));
}

SOM_Scope PSZ  SOMLINK cwflacM_wpclsQueryTitle(M_MMFLAC *somSelf)
{
  static char chrTitle[20]={0};
    /* M_MMFLACData *somThis = M_MMFLACGetData(somSelf); */
    M_MMFLACMethodDebug("M_MMFLAC","cwflacM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_FLACTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle, CLSTITLE_FLAC);

    return chrTitle;
}

SOM_Scope ULONG  SOMLINK cwflacM_wpclsQueryIconData(M_MMFLAC *somSelf, 
                                                    PICONINFO pIconInfo)
{
  HMODULE hmod;
    /* M_MMFLACData *somThis = M_MMFLACGetData(somSelf); */
    M_MMFLACMethodDebug("M_MMFLAC","cwflacM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMFLAC_parent_M_MMAudio_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONFLACFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}
