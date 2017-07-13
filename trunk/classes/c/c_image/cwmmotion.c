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

#ifndef SOM_Module_cwmmotion_Source
#define SOM_Module_cwmmotion_Source
#endif
#define CWMMotion_Class_Source
#define M_CWMMotion_Class_Source

#include <os2.h>
#include "helpid.h"
#include "cwmmotion.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);


/*
 * SOM_Scope PSZ  SOMLINK cwmmotionM_wpclsQueryInstanceType(M_CWMMotion *somSelf)
 */

/*
 * The prototype for cwmmotionM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwmmotionM_wpclsQueryInstanceType(M_MMMMotion *somSelf)
{
    /* M_CWMMotionData *somThis = M_CWMMotionGetData(somSelf); */
    M_MMMMotionMethodDebug("M_MMMMotion","cwmmotionM_wpclsQueryInstanceType");

    return "MMotion Image";
}


/*
 * SOM_Scope PSZ  SOMLINK cwmmotionM_wpclsQueryInstanceFilter(M_CWMMotion *somSelf)
 */

/*
 * The prototype for cwmmotionM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwmmotionM_wpclsQueryInstanceFilter(M_MMMMotion *somSelf)
{
    /* M_CWMMotionData *somThis = M_CWMMotionGetData(somSelf); */
    M_MMMMotionMethodDebug("M_MMMMotion","cwmmotionM_wpclsQueryInstanceFilter");

    return "*.VID";
}


/*
 * SOM_Scope BOOL  SOMLINK cwmmotionM_wpclsQueryDefaultHelp(M_CWMMotion *somSelf, 
 *                                                          PULONG pHelpPanelId, 
 *                                                          PSZ pszHelpLibrary)
 */

/*
 * The prototype for cwmmotionM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwmmotionM_wpclsQueryDefaultHelp(M_MMMMotion *somSelf, 
                                                         PULONG pHelpPanelId, 
                                                         PSZ pszHelpLibrary)
{
    /* M_CWMMotionData *somThis = M_CWMMotionGetData(somSelf); */
    M_MMMMotionMethodDebug("M_MMMMotion","cwmmotionM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=MMOTIONFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 
}


/*
 * SOM_Scope ULONG  SOMLINK cwmmotionM_wpclsQueryStyle(M_CWMMotion *somSelf)
 */

/*
 * The prototype for cwmmotionM_wpclsQueryStyle was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwmmotionM_wpclsQueryStyle(M_MMMMotion *somSelf)
{
    /* M_CWMMotionData *somThis = M_CWMMotionGetData(somSelf); */
    M_MMMMotionMethodDebug("M_MMMMotion","cwmmotionM_wpclsQueryStyle");

    return (M_MMMMotion_parent_M_MMImage_wpclsQueryStyle(somSelf));
}


/*
 * SOM_Scope PSZ  SOMLINK cwmmotionM_wpclsQueryTitle(M_CWMMotion *somSelf)
 */

/*
 * The prototype for cwmmotionM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwmmotionM_wpclsQueryTitle(M_MMMMotion *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWMMotionData *somThis = M_CWMMotionGetData(somSelf); */
    M_MMMMotionMethodDebug("M_MMMMotion","cwmmotionM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_MMOTIONTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"VIDImage.VID");

    return chrTitle;
}


/*
 * SOM_Scope ULONG  SOMLINK cwmmotionM_wpclsQueryIconData(M_CWMMotion *somSelf, 
 *                                                        PICONINFO pIconInfo)
 */

/*
 * The prototype for cwmmotionM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwmmotionM_wpclsQueryIconData(M_MMMMotion *somSelf, 
                                                       PICONINFO pIconInfo)
{
  HMODULE hmod;

    /* M_CWMMotionData *somThis = M_CWMMotionGetData(somSelf); */
    M_MMMMotionMethodDebug("M_MMMMotion","cwmmotionM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMMMotion_parent_M_MMImage_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICON_IMFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}

