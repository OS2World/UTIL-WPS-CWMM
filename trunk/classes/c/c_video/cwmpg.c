/*
 * (C) Chris Wohlgemuth 2002-2005
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

#ifndef SOM_Module_cwmpg_Source
#define SOM_Module_cwmpg_Source
#endif
#define CWMPG_Class_Source
#define M_CWMPG_Class_Source

#define INCL_PM
#include <os2.h>
#include "helpid.h"
#include "cwmpg.ih"
#include "cwaudioinc.h" /* For REXX script name */
#include "sys_funcs.h"

extern char chrHelpLibrary[CCHMAXPATH];
char chrMMMPGExt[400]={0};/* Array holding the extensions for wpclsQueryInstanceFilter() */

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG launchPMProg(PSZ pszTitle, PSZ wrapperExe, PSZ parameters,  CWMMDataFile *thisPtr, ULONG ulView);
BOOL getStringFromRexxScript(PSZ rexxFile, char* chrResult, ULONG ulSize);

/*
 * SOM_Scope HWND  SOMLINK cwmpg_wpOpen(CWMPG *somSelf, HWND hwndCnr, 
 *                                      ULONG ulView, ULONG param)
 */

/*
 * The prototype for cwmpg_wpOpen was replaced by the following prototype:
 */
SOM_Scope HWND  SOMLINK cwmpg_wpOpen(MMMPG *somSelf, HWND hwndCnr, 
                                     ULONG ulView, ULONG param)
{
    /* CWMPGData *somThis = CWMPGGetData(somSelf); */
    MMMPGMethodDebug("MMMPG","cwmpg_wpOpen");

    switch(ulView)
      {
      case ID_MENU_START:
      case ID_MENU_PLAYER:
        /*case 0xbbe5:   Player */
        {
          char chrPlayer[CCHMAXPATH]={0};
          char chrParams[CCHMAXPATH+2]="\"";
          char chrTitle[50];
          char *chrPtr=chrParams;
          ULONG ulSize;
          ULONG ulRc;
          
          if(!PrfQueryProfileString(HINI_USERPROFILE, "CWMM", "mpgplayer", NULLHANDLE, chrPlayer,
                                    sizeof(chrPlayer)))
            PrfQueryProfileString(HINI_USERPROFILE, "CWMM", "vb", "vb.exe", chrPlayer,
                                  sizeof(chrPlayer));
          
          chrPtr++;
          ulSize=sizeof(chrParams)-2;
          _wpQueryRealName(somSelf, chrPtr, &ulSize, TRUE);
          strcat(chrPtr,"\"");

          /* Get the Title for the window list */
          if(!getMessage(chrTitle, IDSTR_VIDPLAYERTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
            strcpy(chrTitle, "Player");

          if((ulRc=launchPMProg(chrTitle, chrPlayer, chrParams, somSelf, ulView))==NULLHANDLE) {
            break;
          }
          return NULLHANDLE;
        }
      default:
        break;
      }

    return (MMMPG_parent_MMVideo_wpOpen(somSelf, hwndCnr, ulView, 
                                        param));
}



/*
 * SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryTitle(M_CWMPG *somSelf)
 */

/*
 * The prototype for cwmpgM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryTitle(M_MMMPG *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_MMMPGMethodDebug("M_MMMPG","cwmpgM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_MPGTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"MPGVideo.MPG");

    return chrTitle;
}


/*
 * SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryInstanceType(M_CWMPG *somSelf)
 */

/*
 * The prototype for cwmpgM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryInstanceType(M_MMMPG *somSelf)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_MMMPGMethodDebug("M_MMMPG","cwmpgM_wpclsQueryInstanceType");

    return "MPEG";
}


/*
 * SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryInstanceFilter(M_CWMPG *somSelf)
 */

/*
 * The prototype for cwmpgM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwmpgM_wpclsQueryInstanceFilter(M_MMMPG *somSelf)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_MMMPGMethodDebug("M_MMMPG","cwmpgM_wpclsQueryInstanceFilter");

    /* This list is build in wpclsInitData() */
    return chrMMMPGExt;
}


/*
 * SOM_Scope ULONG  SOMLINK cwmpgM_wpclsQueryIconData(M_CWMPG *somSelf, 
 *                                                    PICONINFO pIconInfo)
 */

/*
 * The prototype for cwmpgM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwmpgM_wpclsQueryIconData(M_MMMPG *somSelf, 
                                                   PICONINFO pIconInfo)
{
  HMODULE hmod;
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_MMMPGMethodDebug("M_MMMPG","cwmpgM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMMPG_parent_M_MMVideo_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONMPGFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}



SOM_Scope void  SOMLINK cwmpgM_wpclsInitData(M_MMMPG *somSelf)
{
  static BOOL bGotMPEGExt=FALSE;

    /* M_MMMPGData *somThis = M_MMMPGGetData(somSelf); */
    M_MMMPGMethodDebug("M_MMMPG","cwmpgM_wpclsInitData");

    /* 
       Taken from the audio classes. I suspect the same is true for the video classes.
       Anyway it wont hurt having this here.

       Get extensions of additional audio procs. These extensions may be specified by
       newly installed IO procs in MMPM2.INI or by using the Multimedia setup. For
       example the MMIOMP3 procedure for reading MP3 files adds the MP3 extension this
       way to the system. Extensions already handled by a specialized class will be
       filtered in the called REXX script e.g. MP3 so only unknown extensions end up here.

       Strangely enough wpclsQueryInstanceFilter() is called during wpclsInitData() so
       we query the extensions here before calling the parent.

       FIXME:
       The check is only done once during initialization. This is a little annoyance for
       the user because new extension specified in the settings will only be picked up
       on next WPS start. 
       */
     if(!bGotMPEGExt)
      {
        /* REXX script: "mpegext.rx" */
        getStringFromRexxScript(MMMPG_GETEXT_RX, chrMMMPGExt, sizeof(chrMMMPGExt));
        /* Add our default extension *.MPG */
        if(chrMMMPGExt[0]==0)
          /* It's the first one */
          strcpy(chrMMMPGExt,"*.MPG,*.VBS");
        else 
          strlcat(chrMMMPGExt, ",*.MPG,*.VBS", sizeof(chrMMMPGExt));
        bGotMPEGExt=TRUE; 
      }

    M_MMMPG_parent_M_MMVideo_wpclsInitData(somSelf);
}

/*
 * SOM_Scope BOOL  SOMLINK cwmpgM_wpclsQueryDefaultHelp(M_CWMPG *somSelf, 
 *                                                      PULONG pHelpPanelId, 
 *                                                      PSZ pszHelpLibrary)
 */

/*
 * The prototype for cwmpgM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwmpgM_wpclsQueryDefaultHelp(M_MMMPG *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_CWMPGData *somThis = M_CWMPGGetData(somSelf); */
    M_MMMPGMethodDebug("M_MMMPG","cwmpgM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=MPGFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 
}






