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

#ifndef SOM_Module_cwmpg_Source
#define SOM_Module_cwmpg_Source
#endif
#define CWMPG_Class_Source
#define M_CWMPG_Class_Source

#define INCL_PM
#include <os2.h>
#include "helpid.h"
#include "cwmpg.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG launchPMProg(PSZ pszTitle, PSZ wrapperExe, PSZ parameters,  CWMMDataFile *thisPtr, ULONG ulView);

SOM_Scope HWND  SOMLINK cwmpg_wpOpen(CWMPG *somSelf, HWND hwndCnr, 
                                     ULONG ulView, ULONG param)
{
    /* CWMPGData *somThis = CWMPGGetData(somSelf); */
    CWMPGMethodDebug("CWMPG","cwmpg_wpOpen");

    switch(ulView)
      {
      case ID_MENU_START:
      case ID_MENU_PLAYER:
        /*case 0xbbe5:   Player */
        {
          char chrPlayer[CCHMAXPATH]={0};
          char chrParams[CCHMAXPATH+2]="\"";
          char *chrPtr=chrParams;
          ULONG ulSize;
          ULONG ulRc;
          
          PrfQueryProfileString(HINI_USERPROFILE, "CWMM", "mpgplayer", "vb.exe", chrPlayer,sizeof(chrPlayer));
          chrPtr++;
          ulSize=sizeof(chrParams)-2;
          _wpQueryRealName(somSelf, chrPtr, &ulSize, TRUE);
          strcat(chrPtr,"\"");
          if((ulRc=launchPMProg("Spieler", chrPlayer, chrParams, somSelf, ulView))==NULLHANDLE) {
            break;
          }
          return NULLHANDLE;
        }
      default:
        break;
      }

    return (CWMPG_parent_CWVideo_wpOpen(somSelf, hwndCnr, ulView, 
                                        param));
}


SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryTitle(M_CWMPG *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_CWMPGMethodDebug("M_CWMPG","cwmpgM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_MPGTITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"MPGVideo.MPG");

    return chrTitle;
}

SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryInstanceType(M_CWMPG *somSelf)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_CWMPGMethodDebug("M_CWMPG","cwmpgM_wpclsQueryInstanceType");

    return "MPEG";
}

SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryInstanceFilter(M_CWMPG *somSelf)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_CWMPGMethodDebug("M_CWMPG","cwmpgM_wpclsQueryInstanceFilter");

    return "*.MPG,*.VBS";
}

SOM_Scope ULONG  SOMLINK cwmpgM_wpclsQueryIconData(M_CWMPG *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_CWMPGMethodDebug("M_CWMPG","cwmpgM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONMPGFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}


SOM_Scope BOOL  SOMLINK cwmpgM_wpclsQueryDefaultHelp(M_CWMPG *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_CWMPGMethodDebug("M_CWMPG","cwmpgM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=MPGFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWMPG_parent_M_CWVideo_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}

