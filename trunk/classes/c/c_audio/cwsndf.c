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

#ifndef SOM_Module_cwsndf_Source
#define SOM_Module_cwsndf_Source
#endif
#define CWSNDF_Class_Source
#define M_CWSNDF_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwsndf.ih"
#include "cwaudioinc.h"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);


/*
 * SOM_Scope PSZ  SOMLINK cwsndfM_wpclsQueryInstanceType(M_CWSNDF *somSelf)
 */
/*
 * The prototype for cwsndfM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwsndfM_wpclsQueryInstanceType(M_MMSNDF *somSelf)
{
    /* M_CWSNDFData *somThis = M_CWSNDFGetData(somSelf); */
    M_MMSNDFMethodDebug("M_MMSNDF","cwsndfM_wpclsQueryInstanceType");

    return TYPE_SNDF;
    //    return "SND";
}


/*
 * SOM_Scope PSZ  SOMLINK cwsndfM_wpclsQueryInstanceFilter(M_CWSNDF *somSelf)
 */

/*
 * The prototype for cwsndfM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwsndfM_wpclsQueryInstanceFilter(M_MMSNDF *somSelf)
{
    /* M_CWSNDFData *somThis = M_CWSNDFGetData(somSelf); */
    M_MMSNDFMethodDebug("M_MMSNDF","cwsndfM_wpclsQueryInstanceFilter");

    return "*.SND";
}


/*
 * SOM_Scope BOOL  SOMLINK cwsndfM_wpclsQueryDefaultHelp(M_CWSNDF *somSelf, 
 *                                                       PULONG pHelpPanelId, 
 *                                                       PSZ pszHelpLibrary)
 */

/*
 * The prototype for cwsndfM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwsndfM_wpclsQueryDefaultHelp(M_MMSNDF *somSelf, 
                                                      PULONG pHelpPanelId, 
                                                      PSZ pszHelpLibrary)
{
    /* M_CWSNDFData *somThis = M_CWSNDFGetData(somSelf); */
    M_MMSNDFMethodDebug("M_MMSNDF","cwsndfM_wpclsQueryDefaultHelp");


    if(pHelpPanelId)
      *pHelpPanelId=SNDFFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 
}


/*
 * SOM_Scope PSZ  SOMLINK cwsndfM_wpclsQueryTitle(M_CWSNDF *somSelf)
 */

/*
 * The prototype for cwsndfM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwsndfM_wpclsQueryTitle(M_MMSNDF *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWSNDFData *somThis = M_CWSNDFGetData(somSelf); */
    M_MMSNDFMethodDebug("M_MMSNDF","cwsndfM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_SNDFTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle, CLSTITLE_SNDF);

    return chrTitle;
}


/*
 * SOM_Scope ULONG  SOMLINK cwsndfM_wpclsQueryIconData(M_CWSNDF *somSelf, 
 *                                                     PICONINFO pIconInfo)
 */

/*
 * The prototype for cwsndfM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwsndfM_wpclsQueryIconData(M_MMSNDF *somSelf, 
                                                    PICONINFO pIconInfo)
{
  HMODULE hmod;

    /* M_CWSNDFData *somThis = M_CWSNDFGetData(somSelf); */
    M_MMSNDFMethodDebug("M_MMSNDF","cwsndfM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMSNDF_parent_M_MMAudio_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONVOCFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}
