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

#ifndef SOM_Module_cwfli_Source
#define SOM_Module_cwfli_Source
#endif
#define CWFLI_Class_Source
#define M_CWFLI_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwfli.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);


/*
 * SOM_Scope PSZ  SOMLINK cwfliM_wpclsQueryTitle(M_CWFLI *somSelf)
 */

/*
 * The prototype for cwfliM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwfliM_wpclsQueryTitle(M_MMFLI *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWFLIData *somThis = M_CWFLIGetData(somSelf); */
    M_MMFLIMethodDebug("M_MMFLI","cwfliM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_FLITITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"FLIVideo.FLI");
    return chrTitle;
}


/*
 * SOM_Scope PSZ  SOMLINK cwfliM_wpclsQueryInstanceType(M_CWFLI *somSelf)
 */

/*
 * The prototype for cwfliM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwfliM_wpclsQueryInstanceType(M_MMFLI *somSelf)
{
    /* M_CWFLIData *somThis = M_CWFLIGetData(somSelf); */
    M_MMFLIMethodDebug("M_MMFLI","cwfliM_wpclsQueryInstanceType");

    return  "FLC/FLI";
}


/*
 * SOM_Scope PSZ  SOMLINK cwfliM_wpclsQueryInstanceFilter(M_CWFLI *somSelf)
 */

/*
 * The prototype for cwfliM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwfliM_wpclsQueryInstanceFilter(M_MMFLI *somSelf)
{
    /* M_CWFLIData *somThis = M_CWFLIGetData(somSelf); */
    M_MMFLIMethodDebug("M_MMFLI","cwfliM_wpclsQueryInstanceFilter");

    return "*.FLI,*.FLC"; /* FLI: MM-Config, to be removed!! */
}


/*
 * SOM_Scope ULONG  SOMLINK cwfliM_wpclsQueryIconData(M_CWFLI *somSelf, 
 *                                                    PICONINFO pIconInfo)
 */

/*
 * The prototype for cwfliM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwfliM_wpclsQueryIconData(M_MMFLI *somSelf, 
                                                   PICONINFO pIconInfo)
{
  HMODULE hmod;
    /* M_CWFLIData *somThis = M_CWFLIGetData(somSelf); */
    M_MMFLIMethodDebug("M_MMFLI","cwfliM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMFLI_parent_M_MMVideo_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONMOVFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}



/*
 * SOM_Scope BOOL  SOMLINK cwfliM_wpclsQueryDefaultHelp(M_CWFLI *somSelf, 
 *                                                      PULONG pHelpPanelId, 
 *                                                      PSZ pszHelpLibrary)
 */

/*
 * The prototype for cwfliM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwfliM_wpclsQueryDefaultHelp(M_MMFLI *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWFLIData *somThis = M_CWFLIGetData(somSelf); */
    M_MMFLIMethodDebug("M_MMFLI","cwfliM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=FLIFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 
}

