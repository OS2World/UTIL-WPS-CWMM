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

#ifndef SOM_Module_cwvideo_Source
#define SOM_Module_cwvideo_Source
#endif
#define CWVideo_Class_Source
#define M_CWVideo_Class_Source

#define INCL_PM
#include <os2.h>
#include "cwvideo.ih"
#include "cwaudioinc.h" /* For REXX script name */
//#include "cwmmres.h"
//#include "mmres.h"

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
ULONG launchPMProg(PSZ pszTitle, PSZ wrapperExe, PSZ parameters,  CWMMDataFile *thisPtr, ULONG ulView);
BOOL getStringFromRexxScript(PSZ rexxFile, char* chrResult, ULONG ulSize);

char chrMMVideoExt[400]={0};/* Array holding the extensions for wpclsQueryInstanceFilter() */

/*
 * SOM_Scope BOOL  SOMLINK cwvideo_wpModifyPopupMenu(CWVideo *somSelf, 
 *                                                   HWND hwndMenu, 
 *                                                   HWND hwndCnr, 
 *                                                   ULONG iPosition)
 */

/*
 * The prototype for cwvideo_wpModifyPopupMenu was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwvideo_wpModifyPopupMenu(MMVideo *somSelf, 
                                                  HWND hwndMenu, 
                                                  HWND hwndCnr, 
                                                  ULONG iPosition)
{
    /* CWVideoData *somThis = CWVideoGetData(somSelf); */
    MMVideoMethodDebug("MMVideo","cwvideo_wpModifyPopupMenu");
    
    _wpInsertPopupMenuItems(somSelf, hwndMenu, 1, queryResModuleHandle(), ID_MENU_PLAYER, 1);
    _wpInsertPopupMenuItems(somSelf, hwndMenu, 1, queryResModuleHandle(), ID_MENU_START, 1);
    return (MMVideo_parent_CWMMDataFile_wpModifyPopupMenu(somSelf, 
                                                        hwndMenu, 
                                                        hwndCnr, 
                                                        iPosition));
}


/*
 * SOM_Scope BOOL  SOMLINK cwvideo_wpMenuItemSelected(CWVideo *somSelf, 
 *                                                    HWND hwndFrame, 
 *                                                    ULONG ulMenuId)
 */

/*
 * The prototype for cwvideo_wpMenuItemSelected was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwvideo_wpMenuItemSelected(MMVideo *somSelf, 
                                                   HWND hwndFrame, 
                                                   ULONG ulMenuId)
{
    /* CWVideoData *somThis = CWVideoGetData(somSelf); */
    MMVideoMethodDebug("MMVideo","cwvideo_wpMenuItemSelected");

    switch(ulMenuId)
      {
      case ID_MENU_PLAYER:
      case ID_MENU_START:
        //_wpViewObject(somSelf, NULLHANDLE, ID_MENU_PLAYER ,0);
        _wpViewObject(somSelf, NULLHANDLE, ulMenuId,0);
        return TRUE;
      default:
        break;
      }

    return (MMVideo_parent_CWMMDataFile_wpMenuItemSelected(somSelf, 
                                                         hwndFrame, 
                                                         ulMenuId));
}



/*
 * SOM_Scope HWND  SOMLINK cwvideo_wpOpen(CWVideo *somSelf, HWND hwndCnr, 
 *                                        ULONG ulView, ULONG param)
 */

/*
 * The prototype for cwvideo_wpOpen was replaced by the following prototype:
 */
SOM_Scope HWND  SOMLINK cwvideo_wpOpen(MMVideo *somSelf, HWND hwndCnr, 
                                       ULONG ulView, ULONG param)
{
    /* CWVideoData *somThis = CWVideoGetData(somSelf); */
    MMVideoMethodDebug("MMVideo","cwvideo_wpOpen");

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

          if(!PrfQueryProfileString(HINI_USERPROFILE, "CWMM", "videoplayer", NULLHANDLE, chrPlayer,
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

    return (MMVideo_parent_CWMMDataFile_wpOpen(somSelf, hwndCnr, 
                                             ulView, param));
}



/*
 * SOM_Scope BOOL  SOMLINK cwvideoM_wpclsCreateDefaultTemplates(M_CWVideo *somSelf, 
 *                                                              WPObject* Folder)
 */

/*
 * The prototype for cwvideoM_wpclsCreateDefaultTemplates was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwvideoM_wpclsCreateDefaultTemplates(M_MMVideo *somSelf, 
                                                             WPObject* Folder)
{
    /* M_CWVideoData *somThis = M_CWVideoGetData(somSelf); */
    M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsCreateDefaultTemplates");

    return TRUE;
}


SOM_Scope PSZ  SOMLINK cwvideoM_wpclsQueryInstanceType(M_MMVideo *somSelf)
{
  /*   static char chrTitle[40]={0}; */
    /* M_MMVideoData *somThis = M_MMVideoGetData(somSelf); */
    M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsQueryInstanceType");

#if 0
    /* The type probably shouldn't be translated to make changing files between languages
       easier.*/
    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_CWVIDEOTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"Digital video");
#endif
    return TYPE_DIGITALVIDEO;
    /*  return (M_MMVideo_parent_M_CWMMDataFile_wpclsQueryInstanceType(somSelf)); */
}

SOM_Scope PSZ  SOMLINK cwvideoM_wpclsQueryInstanceFilter(M_MMVideo *somSelf)
{
    /* M_MMVideoData *somThis = M_MMVideoGetData(somSelf); */
    M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsQueryInstanceFilter");

    /* The list of additional extensions is built in wpclsInitData(). 

       FIXME
       This is a static list read in only once during startup.
       Should probably be changed later on.
       */
    return chrMMVideoExt;

    /* return (M_MMVideo_parent_M_CWMMDataFile_wpclsQueryInstanceFilter(somSelf)); */
}

/*
 * SOM_Scope PSZ  SOMLINK cwvideoM_wpclsQueryTitle(M_CWVideo *somSelf)
 */

/*
 * The prototype for cwvideoM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwvideoM_wpclsQueryTitle(M_MMVideo *somSelf)
{
  static char chrTitle[20]={0};
  /* M_CWVideoData *somThis = M_CWVideoGetData(somSelf); */
  M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_CWVIDEOTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"Digital video");

    return chrTitle;
}


SOM_Scope void  SOMLINK cwvideoM_wpclsInitData(M_MMVideo *somSelf)
{
  static BOOL bGotVideoExt=FALSE;

    /* M_MMVideoData *somThis = M_MMVideoGetData(somSelf); */
    M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsInitData");

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
     if(!bGotVideoExt)
      {
        /* REXX script: "videoext.rx" */
        getStringFromRexxScript(MMVIDEO_GETEXT_RX, chrMMVideoExt, sizeof(chrMMVideoExt));
        bGotVideoExt=TRUE; 
      }

    M_MMVideo_parent_M_CWMMDataFile_wpclsInitData(somSelf);
}

/*
 * SOM_Scope ULONG  SOMLINK cwvideoM_wpclsQueryDefaultView(M_CWVideo *somSelf)
 */

/*
 * The prototype for cwvideoM_wpclsQueryDefaultView was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwvideoM_wpclsQueryDefaultView(M_MMVideo *somSelf)
{
    /* M_CWVideoData *somThis = M_CWVideoGetData(somSelf); */
    M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsQueryDefaultView");

    return ID_MENU_START;
    /*    return (M_CWVideo_parent_M_CWMMDataFile_wpclsQueryDefaultView(somSelf));*/
}


/*
 * SOM_Scope ULONG  SOMLINK cwvideoM_wpclsQueryIconData(M_CWVideo *somSelf, 
 *                                                      PICONINFO pIconInfo)
 */

/*
 * The prototype for cwvideoM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwvideoM_wpclsQueryIconData(M_MMVideo *somSelf, 
                                                     PICONINFO pIconInfo)
{
  HMODULE hmod;

    /* M_CWVideoData *somThis = M_CWVideoGetData(somSelf); */
    M_MMVideoMethodDebug("M_MMVideo","cwvideoM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMVideo_parent_M_CWMMDataFile_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONCWVIDEOFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}
