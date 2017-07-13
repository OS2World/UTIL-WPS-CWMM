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

#ifndef SOM_Module_cwaif_Source
#define SOM_Module_cwaif_Source
#endif
#define CWAIF_Class_Source
#define M_CWAIF_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwaif.ih"
#include "cwaudioinc.h"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
HMODULE queryResModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);


/*
 * SOM_Scope PSZ  SOMLINK cwaifM_wpclsQueryInstanceType(M_CWAIF *somSelf)
 */

/*
 * The prototype for cwaifM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwaifM_wpclsQueryInstanceType(M_MMAIF *somSelf)
{
    /* M_CWAIFData *somThis = M_CWAIFGetData(somSelf); */
    M_MMAIFMethodDebug("M_MMAIF","cwaifM_wpclsQueryInstanceType");

    return TYPE_AIF;
    //    return "AIF";
}


/*
 * SOM_Scope PSZ  SOMLINK cwaifM_wpclsQueryInstanceFilter(M_CWAIF *somSelf)
 */

/*
 * The prototype for cwaifM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwaifM_wpclsQueryInstanceFilter(M_MMAIF *somSelf)
{
    /* M_CWAIFData *somThis = M_CWAIFGetData(somSelf); */
    M_MMAIFMethodDebug("M_MMAIF","cwaifM_wpclsQueryInstanceFilter");

    return "*.AIF";
}


/*
 * SOM_Scope BOOL  SOMLINK cwaifM_wpclsQueryDefaultHelp(M_CWAIF *somSelf, 
 *                                                      PULONG pHelpPanelId, 
 *                                                      PSZ pszHelpLibrary)
 */

/*
 * The prototype for cwaifM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwaifM_wpclsQueryDefaultHelp(M_MMAIF *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWAIFData *somThis = M_CWAIFGetData(somSelf); */
    M_MMAIFMethodDebug("M_MMAIF","cwaifM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=AIFFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWAIF_parent_M_CWAudio_wpclsQueryDefaultHelp(somSelf, 
                                                           pHelpPanelId, 
                                                           pszHelpLibrary));*/
}


/*
 * SOM_Scope PSZ  SOMLINK cwaifM_wpclsQueryTitle(M_CWAIF *somSelf)
 */

/*
 * The prototype for cwaifM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwaifM_wpclsQueryTitle(M_MMAIF *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWAIFData *somThis = M_CWAIFGetData(somSelf); */
    M_MMAIFMethodDebug("M_MMAIF","cwaifM_wpclsQueryTitle");
    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_AIFTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle, CLSTITLE_AIF);

    return chrTitle;
}


/*
 * SOM_Scope ULONG  SOMLINK cwaifM_wpclsQueryIconData(M_CWAIF *somSelf, 
 *                                                    PICONINFO pIconInfo)
 */

/*
 * The prototype for cwaifM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwaifM_wpclsQueryIconData(M_MMAIF *somSelf, 
                                                   PICONINFO pIconInfo)
{
  HMODULE hmod;
    /* M_CWAIFData *somThis = M_CWAIFGetData(somSelf); */
    M_MMAIFMethodDebug("M_MMAIF","cwaifM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMAIF_parent_M_MMAudio_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONVOCFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );

}
