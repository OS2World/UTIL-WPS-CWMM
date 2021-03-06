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

#ifndef SOM_Module_cwau_Source
#define SOM_Module_cwau_Source
#endif
#define CWAU_Class_Source
#define M_CWAU_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwau.ih"
#include "cwaudioinc.h"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);


/*
 * SOM_Scope PSZ  SOMLINK cwauM_wpclsQueryInstanceType(M_CWAU *somSelf)
 */

/*
 * The prototype for cwauM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwauM_wpclsQueryInstanceType(M_MMAU *somSelf)
{
    /* M_CWAUData *somThis = M_CWAUGetData(somSelf); */
    M_MMAUMethodDebug("M_MMAU","cwauM_wpclsQueryInstanceType");

    return TYPE_AU;
    //    return "AU";
}


/*
 * SOM_Scope PSZ  SOMLINK cwauM_wpclsQueryInstanceFilter(M_CWAU *somSelf)
 */

/*
 * The prototype for cwauM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwauM_wpclsQueryInstanceFilter(M_MMAU *somSelf)
{
    /* M_CWAUData *somThis = M_CWAUGetData(somSelf); */
    M_MMAUMethodDebug("M_MMAU","cwauM_wpclsQueryInstanceFilter");

    return "*.AU";
}


/*
 * SOM_Scope BOOL  SOMLINK cwauM_wpclsQueryDefaultHelp(M_CWAU *somSelf, 
 *                                                     PULONG pHelpPanelId, 
 *                                                     PSZ pszHelpLibrary)
 */

/*
 * The prototype for cwauM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwauM_wpclsQueryDefaultHelp(M_MMAU *somSelf, 
                                                    PULONG pHelpPanelId, 
                                                    PSZ pszHelpLibrary)
{
    /* M_CWAUData *somThis = M_CWAUGetData(somSelf); */
    M_MMAUMethodDebug("M_MMAU","cwauM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=AUFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 
}


/*
 * SOM_Scope PSZ  SOMLINK cwauM_wpclsQueryTitle(M_CWAU *somSelf)
 */

/*
 * The prototype for cwauM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwauM_wpclsQueryTitle(M_MMAU *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWAUData *somThis = M_CWAUGetData(somSelf); */
    M_MMAUMethodDebug("M_MMAU","cwauM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_AUTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle, CLSTITLE_AU);

    return chrTitle;
}


/*
 * SOM_Scope ULONG  SOMLINK cwauM_wpclsQueryIconData(M_CWAU *somSelf, 
 *                                                   PICONINFO pIconInfo)
 */

/*
 * The prototype for cwauM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwauM_wpclsQueryIconData(M_MMAU *somSelf, 
                                                  PICONINFO pIconInfo)
{
  HMODULE hmod;
    /* M_CWAUData *somThis = M_CWAUGetData(somSelf); */
    M_MMAUMethodDebug("M_MMAU","cwauM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMAU_parent_M_MMAudio_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONVOCFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}

