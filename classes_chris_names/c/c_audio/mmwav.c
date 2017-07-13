
/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using: 
 *      SOM Emitter emitctm: 2.42
 */

#ifndef SOM_Module_mmwav_Source
#define SOM_Module_mmwav_Source
#endif
#define MMWAV_Class_Source
#define M_MMWAV_Class_Source

#include <os2.h>
#include "helpid.h"
#include "mmwav.ih"

extern char chrHelpLibrary[CCHMAXPATH];

HMODULE queryModuleHandle(void);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);


SOM_Scope BOOL  SOMLINK mmwav_wpSaveState(CWWAV *somSelf)
{
    /* CWWAVData *somThis = CWWAVGetData(somSelf); */
    CWWAVMethodDebug("CWWAV","mmwav_wpSaveState");

    /* Set .TYPE EA so the editor and the player are happy. */
    _wpSetType(somSelf, "WAV,Digital Audio" , NULL);

    return (CWWAV_parent_CWAudio_wpSaveState(somSelf));
}


/*
 * The prototype for mmwavM_wpclsQueryTitle was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK mmwavM_wpclsQueryTitle(M_CWWAV *somSelf)
{
  static char chrTitle[20]={0};
    /* M_MMWAVData *somThis = M_MMWAVGetData(somSelf); */
  /*    M_MMWAVMethodDebug("M_MMWAV","mmwavM_wpclsQueryTitle");*/
    M_CWWAVMethodDebug("M_CWWAV","mmwavM_wpclsQueryTitle");

    if(chrTitle[0]==0)
      if(!getMessage(chrTitle, IDSTR_WAVTITLE, sizeof(chrTitle), queryModuleHandle(), HWND_DESKTOP))
        strcpy(chrTitle,"WAVAudio.WAV");

    return chrTitle;
}


/*
 * SOM_Scope PSZ  SOMLINK mmwavM_wpclsQueryInstanceType(M_MMWAV *somSelf)
 */

/*
 * The prototype for mmwavM_wpclsQueryInstanceType was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK mmwavM_wpclsQueryInstanceType(M_CWWAV *somSelf)
{
    /* M_MMWAVData *somThis = M_MMWAVGetData(somSelf); */
  /*    M_MMWAVMethodDebug("M_MMWAV","mmwavM_wpclsQueryInstanceType");*/
    M_CWWAVMethodDebug("M_CWWAV","mmwavM_wpclsQueryInstanceType");

    return "WAV,Digital Audio";

}


/*
 * SOM_Scope PSZ  SOMLINK mmwavM_wpclsQueryInstanceFilter(M_MMWAV *somSelf)
 */

/*
 * The prototype for mmwavM_wpclsQueryInstanceFilter was replaced by the following prototype:
 */
SOM_Scope PSZ  SOMLINK mmwavM_wpclsQueryInstanceFilter(M_CWWAV *somSelf)
{
    /* M_MMWAVData *somThis = M_MMWAVGetData(somSelf); */
    M_CWWAVMethodDebug("M_CWWAV","mmwavM_wpclsQueryInstanceFilter");
    /*     M_MMWAVMethodDebug("M_MMWAV","mmwavM_wpclsQueryInstanceFilter");*/
    return "*.WAV";

}

/*
 * The prototype for mmwavM_wpclsQueryIconData was replaced by the following prototype:
 */
SOM_Scope ULONG  SOMLINK mmwavM_wpclsQueryIconData(M_CWWAV *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_MMWAVData *somThis = M_MMWAVGetData(somSelf); */
  /*    M_MMWAVMethodDebug("M_MMWAV","mmwavM_wpclsQueryIconData");*/
    M_CWWAVMethodDebug("M_CWWAV","mmwavM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONWAVFILE;
	} /* endif */
    
	return ( sizeof(ICONINFO) );
}

/*
 * The prototype for mmwavM_wpclsQueryDefaultHelp was replaced by the following prototype:
 */
SOM_Scope BOOL  SOMLINK mmwavM_wpclsQueryDefaultHelp(M_CWWAV *somSelf, 
                                                     PULONG pHelpPanelId, 
                                                     PSZ pszHelpLibrary)
{
    /* M_MMWAVData *somThis = M_MMWAVGetData(somSelf); */
  /*    M_MMWAVMethodDebug("M_MMWAV","mmwavM_wpclsQueryDefaultHelp");*/
    M_CWWAVMethodDebug("M_CWWAV","mmwavM_wpclsQueryDefaultHelp");

    if(pHelpPanelId)
      *pHelpPanelId=WAVFILE_HELP_PANEL;
    if(pszHelpLibrary)
      strncpy(pszHelpLibrary, chrHelpLibrary, CCHMAXPATH);
    return TRUE; 

    /*    return (M_CWWAV_parent_M_CWAudio_wpclsQueryDefaultHelp(somSelf, 
          pHelpPanelId, 
          pszHelpLibrary));*/
}
