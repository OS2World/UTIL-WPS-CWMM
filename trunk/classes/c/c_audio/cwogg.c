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

#ifndef SOM_Module_cwogg_Source
#define SOM_Module_cwogg_Source
#endif
#define CWOGG_Class_Source
#define M_CWOGG_Class_Source

#include "os2.h"
#include "cwogginc.h"
#include "cwogg.ih"
#include "cwaudio.h"
//#include "cwmmres.h"
#include "mmres.h"
#include "cwaudioinc.h"

#include "sys_funcs.h"

extern char classDLLPath[CCHMAXPATH];

HMODULE queryResModuleHandle(void);
HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);

static PSZ _queryHelpLibraryName(void)
{
  static char helpLibraryName[CCHMAXPATH];
  BOOL bDone=FALSE;

  if(!bDone) {
    if(queryModuleHandle()) {
      char  *ptr;
      strncpy(helpLibraryName, classDLLPath, sizeof(helpLibraryName)-1);
      if((ptr=strrchr(helpLibraryName, '\\'))!=NULLHANDLE) {
        *(++ptr)=0;
        if(strlen(helpLibraryName)+strlen(OGGFILE_HELP_LIBRARY)<sizeof(helpLibraryName)) {
          strcat(helpLibraryName, OGGFILE_HELP_LIBRARY );
          bDone=TRUE;
          return helpLibraryName;
        }
      }
    }
    return NULLHANDLE;
  }

  return helpLibraryName;
}


/*
 * SOM_Scope ULONG  SOMLINK cwogg_cwmmQueryTrackInfo(MMOGG *somSelf, 
 *                                                   char** chrString, 
 *                                                   ULONG ulSize, 
 *                                                   int iWhich)
 */

/*
 * The prototype for cwogg_cwmmQueryTrackInfo was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwogg_cwmmQueryTrackInfo(MMOGG *somSelf, 
                                                  PSZ chrString, 
                                                  ULONG ulSize, 
                                                  int iWhich)
{
  /*     MMOGGData *somThis = MMOGGGetData(somSelf); */
  /*      MMAudioData *somThis = MMAudioGetData(somSelf); */
    MMOGGMethodDebug("MMOGG","cwogg_cwmmQueryTrackInfo");

    if(iWhich<1||iWhich>IDINFO_LASTINFO)
      return FALSE;
    /*
    switch(iWhich)
      {
      case IDINFO_BITRATE:
        return _ulBitRate;
      default:
        break;
      }
      */
    return (MMOGG_parent_MMAudio_cwmmQueryTrackInfo(somSelf, 
                                                    chrString, 
                                                    ulSize, iWhich));
}

/*
 * The prototype for cwogg_wpSaveState was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwogg_wpSaveState(MMOGG *somSelf)
{
  //  char chrBuf[256];
  //  ULONG ulLength;
  BOOL rc;

  /*  MMAudioData *somThis = MMAudioGetData(somSelf);*/
    MMOGGMethodDebug("MMOGG","cwogg_wpSaveState");

    /* call parent first. By doing so, the MP3 type is written after the Digital Audio type.
     This way the default class the WPS picks is MMOGG and not MMAudio. */
    rc=(MMOGG_parent_MMAudio_wpSaveState(somSelf));

    return rc;

#if 0
    strncpy(chrBuf, _wpQueryType(somSelf), sizeof(chrBuf));
    chrBuf[sizeof(chrBuf)-1]=0;

    if(!strstr(chrBuf, TYPE_OGG)) {
      ulLength=strlen(chrBuf);
      if(ulLength) {
        if(ulLength+6 < sizeof(chrBuf)) {
          strcat(chrBuf, "\n");
          strcat(chrBuf, TYPE_OGG);
        }
      }
      else
        strcpy(chrBuf, TYPE_OGG);
    }
    return rc;
#endif
}

/*
 * The prototype for cwoggM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwoggM_wpclsQueryTitle(M_MMOGG *somSelf)
{
  static char chrTitle[20]={0};
    /* M_CWOGGData *somThis = M_CWOGGGetData(somSelf); */
    M_MMOGGMethodDebug("M_MMOGG","cwoggM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_OGGTITLE, sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle, CLSTITLE_OGG);

    return chrTitle;
}

/*
 * The prototype for cwoggM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwoggM_wpclsQueryInstanceType(M_MMOGG *somSelf)
{
    /* M_CWOGGData *somThis = M_CWOGGGetData(somSelf); */
    M_MMOGGMethodDebug("M_MMOGG","cwoggM_wpclsQueryInstanceType");

    return TYPE_OGG;
    //    return "OGGS,Digital Audio";
}

/*
 * The prototype for cwoggM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK cwoggM_wpclsQueryInstanceFilter(M_MMOGG *somSelf)
{
    /* M_CWOGGData *somThis = M_CWOGGGetData(somSelf); */
    M_MMOGGMethodDebug("M_MMOGG","cwoggM_wpclsQueryInstanceFilter");

    return "*.OGG";
}

/*
 * The prototype for cwoggM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK cwoggM_wpclsQueryIconData(M_MMOGG *somSelf, 
                                                   PICONINFO pIconInfo)
{
  HMODULE hmod;
    /* M_CWOGGData *somThis = M_CWOGGGetData(somSelf); */
    M_MMOGGMethodDebug("M_MMOGG","cwoggM_wpclsQueryIconData");

    hmod=queryModuleHandle();
    if(!hmod)
      return M_MMOGG_parent_M_MMAudio_wpclsQueryIconData(somSelf, pIconInfo);

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = hmod;
      pIconInfo->resid   = ID_ICONOGGFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}

/*
 * The prototype for cwoggM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK cwoggM_wpclsQueryDefaultHelp(M_MMOGG *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
  PSZ pszHelpLibName;

    /* M_CWOGGData *somThis = M_CWOGGGetData(somSelf); */
    M_MMOGGMethodDebug("M_MMOGG","cwoggM_wpclsQueryDefaultHelp");

    if((pszHelpLibName=_queryHelpLibraryName())!=NULLHANDLE)
      {
        if(pHelpPanelId)
          *pHelpPanelId=OGGFILE_HELP_PANEL;
        if(pszHelpLibrary)
          strncpy(pszHelpLibrary, pszHelpLibName, CCHMAXPATH);
        return TRUE; 
      }

    return (M_MMOGG_parent_M_MMAudio_wpclsQueryDefaultHelp(somSelf, 
                                                           pHelpPanelId, 
                                                           pszHelpLibrary));
}

