/*
 * This file is (C) Chris Wohlgemuth 2002
 * 
 * It's part of the media folder distribution
 */

#include "m3ufolder.hh"

extern char chrInstallDir[CCHMAXPATH];

ULONG launchRexx( PSZ pszTitle, PSZ rexxFile, PSZ parameters);
HMODULE queryModuleHandle(PSZ pszClassName);
HMODULE queryResModuleHandle(void);
BOOL _queryHelpLibrary(char * chrBuffer, int iSize);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
HMODULE queryModuleHandle(PSZ pszClassName);

/* Overriden function which returns our class name */
PSZ M_CWM3UFolder::wpclsQueryTitle()
{
  static char chrTitle[40];
  if(getMessage(chrTitle, IDSTR_TITLEM3UFOLDER,sizeof(chrTitle), queryResModuleHandle(), HWND_DESKTOP))
    return chrTitle;
  /* Fall back in case of error */
  return "M3U editor";
}

ULONG M_CWM3UFolder::wpclsQueryStyle()
{
  return (M_CWMediaFolder::wpclsQueryStyle()|CLSSTYLE_NEVERTEMPLATE|CLSSTYLE_DONTTEMPLATE);
}

BOOL M_CWM3UFolder::wpclsCreateDefaultTemplates(WPObject * wpObject)
{
  return TRUE;
}

void M_CWM3UFolder::wpclsInitData()
{
  static BOOL bObjectsCreated=FALSE;

  /* Call parent */
  M_CWMediaFolder::wpclsInitData();
  
  /* Be sure we have our install dir */
  queryModuleHandle("M_CWM3UFolder");
}

BOOL M_CWM3UFolder::wpclsQueryDefaultHelp(PULONG HelpPanelId,PSZ HelpLibrary)
{
  if(HelpLibrary)
    _queryHelpLibrary(HelpLibrary, CCHMAXPATH);

  if(HelpPanelId)
    *HelpPanelId= IDHLP_M3UFOLDERMAIN;
	
  return TRUE;
}

/* Overriden function which returns the custom icon */
ULONG M_CWM3UFolder::wpclsQueryIconData(PICONINFO pIconInfo)
{
  //  HMODULE hResource=queryResModuleHandle();
  HMODULE hResource=queryModuleHandle("M_CWM3UFolder");

  if(!hResource)
    return M_CWProgFolder::wpclsQueryIconData(pIconInfo);

  if (pIconInfo)   {
    pIconInfo->fFormat = ICON_RESOURCE;
    pIconInfo->hmod    = hResource;
    pIconInfo->resid   = IDICON_M3UFILE;
  } /* endif */

  return ( sizeof(ICONINFO) );
}

ULONG M_CWM3UFolder::wpclsQueryIconDataN(PICONINFO pIconInfo, ULONG ulIconIndex)
{
  //HMODULE hResource=queryResModuleHandle();
  HMODULE hResource=queryModuleHandle("M_CWM3UFolder");

  if(!hResource)
    return M_CWProgFolder::wpclsQueryIconDataN(pIconInfo, ulIconIndex);

  if (pIconInfo)   {
    pIconInfo->fFormat = ICON_RESOURCE;
    pIconInfo->hmod    = hResource;
    pIconInfo->resid   = IDICON_M3UFILE;
  } /* endif */
  return ( sizeof(ICONINFO) );
}

