#define INCL_PM
#define INCL_OS2MM
#define INCL_SW
#define INCL_DOSMISC
#define INCL_DOSMODULEMGR
#include <os2.h>
#include <os2me.h>
#include <stdlib.h>
#include <string.h>

#if 0
int main()
{
  exit();
}
#endif

/*char chrMMDir[CCHMAXPATH];
 */
static void _getMMBase(char* chrMMDir, ULONG ulSize)
{
  PSZ pszMMBase="";

  if(!DosScanEnv("MMBASE", &pszMMBase)) {
    char * chrPtr;
    strncpy(chrMMDir, pszMMBase, ulSize);
    chrMMDir[ulSize-1]=0;
    if((chrPtr=strchr(chrMMDir,';'))!=NULLHANDLE)
      *chrPtr=0;
  }
} 

static HMODULE _getDLLHMod(void)
{
  HMODULE hmod=0;
  char chrPath[CCHMAXPATH];
  _getMMBase( chrPath, sizeof(chrPath));
  if(chrPath[strlen(chrPath)-1]=='\\')
    chrPath[strlen(chrPath)-1]=0;
  strcat(chrPath, "\\DLL\\cddbpage.dll");
  /*WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, chrPath, "", 1234, MB_MOVEABLE);
  //DosBeep(500, 400);       */
  if(!DosQueryModuleHandle(chrPath,&hmod)) {
    return hmod;
  }
  else
    return NULLHANDLE;
}
 
typedef struct {
  HWND hwndHelpInstance;
} MMPAGEINFO;
typedef MMPAGEINFO * PMMPAGEINFO;



/*******************************************************************/
/*  Modeless secondary window procedure                            */
/*******************************************************************/
MRESULT EXPENTRY cddbPageDlgProc (HWND hwnd, ULONG msg,
                                     MPARAM mp1, MPARAM mp2)
{
  PMMPAGEINFO pMMPageInfo = (PMMPAGEINFO)WinQueryWindowPtr (hwnd, QWL_USER);
  
  switch (msg) {
  case WM_INITDLG:
    /* Initialize The Page */
    pMMPageInfo = (PMMPAGEINFO) malloc(sizeof(MMPAGEINFO));
    WinSetWindowPtr (hwnd, QWL_USER, pMMPageInfo);
    
    /* Create a Help Instance */
    /*    pMMPageInfo->hwndHelpInstance = WinCreateHelpInstance(...);*/
    break;
    
  case WM_DESTROY:
    /* Clean up page window resources */
    /*    WinDestroyHelpInstance (pMMPageInfo->hwndHelpInstance); */
    free (pMMPageInfo);
    break;
    
  case WM_COMMAND:
    /* Process All Commands */
    return ((MRESULT) FALSE);
    break;
        
  case WM_CLOSE:
    return ((MRESULT) FALSE);
    break;

#if 0    
  case WM_HELP:
    if (pMMPageInfo->hwndHelpInstance) {
      WinSendMsg(
                 pMMPageInfo->hwndHelpInstance,
                 HM_DISPLAY_HELP,
                 (MPARAM) mp1,
                 HM_RESOURCEID );
    }
    return ((MRESULT)TRUE);
    break;
    
  case WM_TRANSLATEACCEL:
    return (WinDefWindowProc (hwnd, msg, mp1, mp2));
    break;
    
  case HM_QUERY_KEYS_HELP:
    return((MRESULT) IDH_HELPFORKEYS);
    break;
#endif    
  }
  
  return (WinDefSecondaryWindowProc(hwnd, msg, mp1, mp2));
}

HWND InsertCDDBConfigPage(PMCI_DEVICESETTINGS_PARMS pMCIDevSettings)     
{

  HWND  hwndPage=NULLHANDLE;               /* Page window handle    */
  CHAR  szTabText[CCHMAXPATH];  /* Buffer for tab string */
  ULONG ulPageId;               /* Page Identifier       */

  if(pMCIDevSettings)
    DosBeep(1000, 400);       
  if(pMCIDevSettings->hwndCallback)
    DosBeep(500, 400);       
  /*******************************************************************/
  /*   Load a modeless secondary window                              */
  /*******************************************************************/

  hwndPage    = WinLoadSecondaryWindow(
                                       HWND_DESKTOP,
                                       HWND_DESKTOP,
                                       /*                                       pMCIDevSettings->hwndNotebook,
                                       pMCIDevSettings->hwndNotebook,*/
                                       &cddbPageDlgProc,
                                       NULLHANDLE,
                                       /*                            _getDLLHMod(),*/ /* hmod */
                                       1234,
                                       (PVOID)NULLHANDLE);
  /*           (PVOID)pMCIDevSettings);*/

  DosBeep(5000, 400);
  if (!hwndPage) return (NULLHANDLE);
       
  ulPageId = (ULONG)WinSendMsg( pMCIDevSettings->hwndNotebook,
                                BKM_INSERTPAGE,
                                (ULONG)NULL,
                                MPFROM2SHORT( BKA_AUTOPAGESIZE |
                                              BKA_MINOR, BKA_LAST ) );
  /*******************************************************************/
  /*  Associate a secondary window with a notebook page.             */
  /*******************************************************************/
       
  WinSendMsg(pMCIDevSettings->hwndNotebook, BKM_SETPAGEWINDOWHWND,
             MPFROMP( ulPageId ), MPFROMLONG( hwndPage ) );
       
  /*******************************************************************/
  /*  Get Tab Text from DLL                                          */
  /*******************************************************************/

#if 0       
  WinLoadString(WinQueryAnchorBlock( HWND_DESKTOP ), ,
                (USHORT)IDB_EXAMPLE, CCHMAXPATH, szTabText );
#endif
  
  /*******************************************************************/
  /*  Set Tab Text                                                   */
  /*******************************************************************/
       
  WinSendMsg( pMCIDevSettings->hwndNotebook, BKM_SETTABTEXT,
              MPFROMP( ulPageId ), szTabText );
       
  return( hwndPage );
       
}




