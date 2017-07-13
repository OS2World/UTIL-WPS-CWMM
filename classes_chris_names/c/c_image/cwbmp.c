
/*
 *  This file was generated by the SOM Compiler.
 *  Generated using:
 *     SOM incremental update: 2.42
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

#ifndef SOM_Module_cwbmp_Source
#define SOM_Module_cwbmp_Source
#endif
#define CWBitmap_Class_Source
#define M_CWBitmap_Class_Source

#define INCL_DOS
#define INCL_PM
#include <os2.h>
#include <stdio.h>

#include "cwbmp.ih"
#include "except.h"
#include "cwmmres.h"
#include "cwaudioinc.h"

#define STR_BITMAPNAME   "Bitmap"

extern LONG  lNumIOProcs;

HMODULE queryModuleHandle(void);
BOOL insertIOProcMenuItems( HWND hwndMenu );
ULONG launchPMProg(PSZ pszTitle, PSZ wrapperExe, PSZ parameters,  WPObject *thisPtr, ULONG ulView);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
BOOL getBmpInfoHeader(PBITMAPINFOHEADER2  bmpih2, PSZ pszFileName, char* procName, ULONG ulLength, BOOL *bNoIOProcAvailable);
HBITMAP loadBitmap ( PSZ pszFileName, PBITMAPINFOHEADER2 pBMPInfoHeader2);
HBITMAP createNewBitmap ( HBITMAP hbm,
                          PBITMAPINFOHEADER2 pBmpInfoHeader2,
                          ULONG ulWidth, 
                          ULONG ulHeight, 
                          BOOL* pbQuitEarly);


MRESULT EXPENTRY bmpInfoDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  CWBitmap *cwImage;

  switch(msg) {
  case WM_INITDLG :
    cwImage=(CWBitmap*)LONGFROMMP(mp2);
    if(somIsObj(cwImage)) {
      //      CWBitmapData *somThis = CWBitmapGetData(cwImage);

      PBITMAPINFOHEADER2 pbmpih2;

      if((pbmpih2=(PBITMAPINFOHEADER2) _cwmmQueryBitmapInfoHeader(cwImage))!=NULLHANDLE)
        {
          char fName[CCHMAXPATH];
          char chrTemplate[100];

          if(pbmpih2->cbFix==sizeof(BITMAPINFOHEADER2)) {
            getMessage(chrTemplate, IDSTR_SPRINTFPIXEL, sizeof(chrTemplate), 
                       queryModuleHandle(), hwnd);;
            sprintf(fName, chrTemplate,pbmpih2->cx);
            WinSetWindowText(WinWindowFromID(hwnd, IDST_IMAGEWIDTH),fName);
            
            sprintf(fName, chrTemplate, pbmpih2->cy);
            WinSetWindowText(WinWindowFromID(hwnd, IDST_IMAGEHEIGHT),fName);

            getMessage(chrTemplate, IDSTR_SPRINTFBIT, sizeof(chrTemplate), 
                       queryModuleHandle(), hwnd);;            
            sprintf(fName, chrTemplate, pbmpih2->cBitCount);
            WinSetWindowText(WinWindowFromID(hwnd, IDST_COLORDEPTH),fName);
          }
          else if(pbmpih2->cbFix==sizeof(BITMAPINFOHEADER)) {
            /* We shouldn't get a BITMAPINFOHEADER here but who knows...*/
            PBITMAPINFOHEADER pbmpih=(PBITMAPINFOHEADER)pbmpih2;
            getMessage(chrTemplate, IDSTR_SPRINTFPIXEL, sizeof(chrTemplate), 
                       queryModuleHandle(), hwnd);;

            sprintf(fName, chrTemplate,pbmpih->cx);
            WinSetWindowText(WinWindowFromID(hwnd, IDST_IMAGEWIDTH),fName);
            
            sprintf(fName, chrTemplate, pbmpih->cy);
            WinSetWindowText(WinWindowFromID(hwnd, IDST_IMAGEHEIGHT),fName);

            getMessage(chrTemplate, IDSTR_SPRINTFBIT, sizeof(chrTemplate), 
                       queryModuleHandle(), hwnd);;                        
            sprintf(fName, chrTemplate, pbmpih->cBitCount);
            WinSetWindowText(WinWindowFromID(hwnd, IDST_COLORDEPTH),fName);
          }
          
          WinSetWindowText(WinWindowFromID(hwnd, IDST_IMAGEFORMAT), STR_BITMAPNAME);
        }
    }/* somIsObj() */
    return (MRESULT)TRUE;
    /* This prevents switching the notebook page behind the open folder */
    case WM_WINDOWPOSCHANGED:
      {
        MRESULT mr;

        if(WinQueryFocus(HWND_DESKTOP)!=
           WinQueryWindow(WinQueryWindow(hwnd, QW_PARENT), QW_PARENT)) {
          mp2=MPFROMLONG(LONGFROMMP(mp2)|0x80000);/*AWP_ACTIVATE 0x00080000L*/
          mr=WinDefDlgProc(hwnd, msg, mp1, mp2);
          return mr;  
        }
        break;
      }
    case WM_FOCUSCHANGE:
      {
        if(!SHORT1FROMMP(mp2)) {
          if(HWNDFROMMP(mp1)==hwnd) {
            MRESULT mr;

            mr=WinDefDlgProc(hwnd, msg, mp1, mp2);
            WinSendMsg(WinQueryWindow(WinQueryWindow(hwnd, QW_PARENT), QW_PARENT), 
                       WM_SETFOCUS, MPFROMHWND(hwnd), (MPARAM)TRUE);
            return mr;
          }
        }
        break;
      }
    case WM_DESTROY:
      /* The notebook closes and gets destroyed */
      /* Set focus to desktop to prevent PM freeze */
      WinSetFocus(HWND_DESKTOP, HWND_DESKTOP);
      break;
  default:
    break;
  }
  return WinDefDlgProc(hwnd, msg, mp1, mp2);
}

SOM_Scope ULONG  SOMLINK cwbmp_cwmmAddImageInformationPage(CWBitmap *somSelf, 
                                                           HWND hwndNotebook)
{
  PAGEINFO pageinfo;
  char pageName[100];

    /* CWBitmapData *somThis = CWBitmapGetData(somSelf); */
    CWBitmapMethodDebug("CWBitmap","cwbmp_cwmmAddImageInformationPage");

  //Clear the pageinfo structure
  memset((PCH)&pageinfo, 0, sizeof(PAGEINFO));
  //Fill the pageinfo structure
  pageinfo.cb = sizeof(PAGEINFO);
  pageinfo.hwndPage = NULLHANDLE;
  pageinfo.usPageStyleFlags = BKA_MAJOR | BKA_STATUSTEXTON;
  pageinfo.usPageInsertFlags = BKA_FIRST;
  //We want page numbers
  pageinfo.usSettingsFlags = SETTINGS_PAGE_NUMBERS;
  //The dialog procedure for this page
  pageinfo.pfnwp = bmpInfoDlgProc;
  //The resource DLL
  pageinfo.resid = queryModuleHandle();
  //pageinfo.resid = queryModuleHandle();
  //The ID of the dialog template
  pageinfo.dlgid = IDDLG_IMAGEINFOPAGE;
  //pageinfo.dlgid = IDDLG_WAVEINFOPAGE;
  //We need a pointer to our WPS-object in the dialog procedure
  //to call class functions
  pageinfo.pCreateParams = somSelf;
  //The ID of the help panel for this page
  //pageinfo.idDefaultHelpPanel = IDDLG_GENERAL2PAGE;

  //Tell the WPS the help library name
  pageinfo.pszHelpLibraryName = NULLHANDLE;
  //We have a major tab so we need a name
  /* pageName: "ISO filesystem" */
  getMessage(pageName, IDSTR_IMAGEINFOPAGENAME, sizeof(pageName),  queryModuleHandle(), hwndNotebook);
  pageinfo.pszName = pageName;
  //Insert the page into the settings notebook
  return _wpInsertSettingsPage(somSelf,hwndNotebook,&pageinfo);
}

/* This private function is for creating a small bitmap handle. It loads the bitmap
   and creates the small bitmap but *does not* keep the original size bitmap.
   If using _wpQueryBitmapHandle() the object would keep (the maybe huge) original
   size bitmap in memory until going dormant. This would eat up all memory rather fast. */
static HBITMAP  cwQueryBitmapHandle(CWBitmap *somSelf, 
                                 ULONG ulWidth, 
                                 ULONG ulHeight)
{
  ULONG ulSize;
  char chrName[CCHMAXPATH];
  PBYTE pByte;
  BOOL bGotHeader=FALSE;
  HBITMAP hbm=NULLHANDLE;
  HBITMAP hbm2;
  
  CWBitmapData *somThis = CWBitmapGetData(somSelf); 
  //   CWImageMethodDebug("CWBitmap","cwimg_wpQueryBitmapHandle");
  
  if(ulWidth==0 || ulHeight==0)
    return NULLHANDLE;
  
  ulSize=sizeof(chrName);
  if(!_wpQueryRealName(somSelf, chrName, &ulSize,TRUE))
    return NULLHANDLE;

    
  TRY_LOUD(LOAD_BMP3) {
    if(_cwmmQueryBitmapInfoHeader(somSelf))
      bGotHeader=TRUE;
  }
  CATCH(LOAD_BMP3)
    {
#if 0
      if(MBID_OK==showMsgBox2(IDSTR_CWIMAGETITLE, IDSTR_IMGIOPROCCRASH, queryModuleHandle(),
                              MB_OK | MB_MOVEABLE | MB_WARNING))
        exit(0);
      exit(0);
#endif
    }END_CATCH;
    
    if(!bGotHeader)
      return NULLHANDLE;

    TRY_LOUD(LOAD_BMP4) {
      hbm=loadBitmap ( chrName , (PBITMAPINFOHEADER2) _pBmpInfoHeader2);
    }
    CATCH(LOAD_BMP4)
      {
#if 0
        if(MBID_OK==showMsgBox2(IDSTR_CWIMAGETITLE, IDSTR_IMGIOPROCCRASH, queryModuleHandle(),
                                MB_OK | MB_MOVEABLE | MB_WARNING))
          exit(0);
        exit(0);
#endif
        hbm=NULLHANDLE;
      } END_CATCH;
      
      if(!hbm)
        return NULLHANDLE;
      
      
    TRY_LOUD(QRY_HBM) {
      hbm2=createNewBitmap ( hbm, (PBITMAPINFOHEADER2) _pBmpInfoHeader2, ulWidth, ulHeight , NULLHANDLE);
    }
    CATCH(QRY_HBM)
      {
        hbm2=NULLHANDLE;
      } END_CATCH;
      
      GpiDeleteBitmap(hbm);
      return hbm2;
}


SOM_Scope HBITMAP  SOMLINK cwbmp_cwmmQuerySmallBitmapHandle(CWBitmap *somSelf, 
                                                            ULONG ulSize)
{
  HBITMAP hbm;
    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_cwmmQuerySmallBitmapHandle");

    if(_bNoIOProcAvailable)
      return NULLHANDLE; /* No IO-proc for this image format. Don't try again in the future. */

    if(ulSize<MAX_SIZE_SMALLBITMAP)
      ulSize=MAX_SIZE_SMALLBITMAP;

    if(ulSize>_ulSizeSmallBitmap)
      {
        if(_hBitmapSmall) {
          GpiDeleteBitmap(_hBitmapSmall);
          _hBitmapSmall=NULLHANDLE;
        }
      }

    if(!_hBitmapSmall) {
      _ulSizeSmallBitmap=0;

      /* Get a small bitmap without caching the big size bitmap */
     if((hbm=cwQueryBitmapHandle(somSelf, 
                                  ulSize, 
                                  ulSize))!=NULLHANDLE) {

        _hBitmapSmall=hbm;
        _ulSizeSmallBitmap=ulSize;
      }
    }
    /* Return statement to be customized: */
    return _hBitmapSmall;
}

SOM_Scope void  SOMLINK cwbmp_cwmmFreeBitmaps(CWBitmap *somSelf)
{
    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_cwmmFreeBitmaps");

    _cwmmFreeSmallBitmap(somSelf);
}

SOM_Scope void  SOMLINK cwbmp_cwmmFreeSmallBitmap(CWBitmap *somSelf)
{
    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_cwmmFreeSmallBitmap");

    if(_hBitmapSmall) {
      GpiDeleteBitmap(_hBitmapSmall);
      _hBitmapSmall=NULLHANDLE;
      _ulSizeSmallBitmap=0;
    }
}

SOM_Scope PBYTE  SOMLINK cwbmp_cwmmQueryBitmapInfoHeader(CWBitmap *somSelf)
{
  ULONG ulSize;
  char chrName[CCHMAXPATH];
  PBYTE pByte;
  BOOL bGotHeader=FALSE;

    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_cwmmQueryBitmapInfoHeader");

    if(_pBmpInfoHeader2)
      return _pBmpInfoHeader2;

    if(_bNoIOProcAvailable)
      return NULL; /* No IO-proc for this image format. Don't try again in the future. */

    ulSize=sizeof(chrName);
    if(!_wpQueryRealName(somSelf, chrName, &ulSize,TRUE))
      return NULL;

    if((pByte=_wpAllocMem(somSelf, sizeof(BITMAPINFOHEADER2)+256*sizeof(RGB2), &ulSize))==NULLHANDLE)
      return NULL;

    TRY_LOUD(GET_CWBMPIH) {
      bGotHeader=getBmpInfoHeader( (PBITMAPINFOHEADER2) pByte, chrName, _chrIOProcName, 
                                   sizeof(_chrIOProcName), &_bNoIOProcAvailable);
    }
    CATCH(GET_CWBMPIH)
      {
#if 0
        if(MBID_OK==showMsgBox2(IDSTR_CWIMAGETITLE, IDSTR_IMGIOPROCCRASH, queryModuleHandle(),
                                    MB_OK | MB_MOVEABLE | MB_WARNING))
          exit(0);
        exit(0);
#endif

      } END_CATCH;

    if(!bGotHeader) {
      _wpFreeMem(somSelf,pByte);
      _pBmpInfoHeader2=NULLHANDLE;
      _chrIOProcName[0]=0;
      return NULL;
    }
    _pBmpInfoHeader2=pByte;
    return (PBYTE) _pBmpInfoHeader2;

}

SOM_Scope BOOL  SOMLINK cwbmp_wpModifyPopupMenu(CWBitmap *somSelf, 
                                                HWND hwndMenu, 
                                                HWND hwndCnr, 
                                                ULONG iPosition)
{
    /* CWBitmapData *somThis = CWBitmapGetData(somSelf); */
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpModifyPopupMenu");
    _wpInsertPopupMenuItems(somSelf, hwndMenu, 1, queryModuleHandle(), ID_MENU_IMGEDITOR, 1);
    /* Insert all known image I/O procs */
    insertIOProcMenuItems( hwndMenu );

    return (CWBitmap_parent_WPBitmap_wpModifyPopupMenu(somSelf, 
                                                       hwndMenu, 
                                                       hwndCnr, 
                                                       iPosition));
}

SOM_Scope BOOL  SOMLINK cwbmp_wpMenuItemSelected(CWBitmap *somSelf, 
                                                 HWND hwndFrame, 
                                                 ULONG ulMenuId)
{
    /* CWBitmapData *somThis = CWBitmapGetData(somSelf); */
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpMenuItemSelected");

    if(ulMenuId>=ID_ITEM_FIRSTCONVERT && ulMenuId<=ID_ITEM_FIRSTCONVERT+lNumIOProcs-1)
      {
        char chrEditor[CCHMAXPATH]={0};
        char chrParams[CCHMAXPATH+3+10]="\"";
        char *chrPtr=chrParams;
        char chrID[10];
        ULONG ulSize;
        ULONG ulRc;

        char text[100];
        
        PrfQueryProfileString(HINI_USERPROFILE, "CWMM", "imageconverter", "", chrEditor,
                              sizeof(chrEditor));
        chrPtr++;
        ulSize=sizeof(chrParams)-2-10;
        _wpQueryRealName(somSelf, chrPtr, &ulSize, TRUE);
        strcat(chrPtr,"\" ");
        sprintf(chrID,"%d", ulMenuId-ID_ITEM_FIRSTCONVERT);
        strcat(chrPtr,chrID);
        if((ulRc=launchPMProg("", chrEditor, chrParams, somSelf, ulMenuId-ID_ITEM_FIRSTCONVERT))==NULLHANDLE) {
          /* Error */
#if 0
          sprintf(text, "%d", ulMenuId-ID_ITEM_FIRSTCONVERT);        
          WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                        text,
                        "cwimg_wpMenuItemSelected",
                        0UL, MB_OK | MB_ICONEXCLAMATION|MB_MOVEABLE );
#endif
          return TRUE;
        }
        return TRUE;
      }
    switch(ulMenuId)
      {
      case ID_MENU_IMGEDITOR:
        _wpViewObject(somSelf, NULLHANDLE, ulMenuId, 0);
        return TRUE;
      default:
        break;
      }

    return (CWBitmap_parent_WPBitmap_wpMenuItemSelected(somSelf, 
                                                        hwndFrame, 
                                                        ulMenuId));
}

SOM_Scope HWND  SOMLINK cwbmp_wpOpen(CWBitmap *somSelf, HWND hwndCnr, 
                                     ULONG ulView, ULONG param)
{
    /* CWBitmapData *somThis = CWBitmapGetData(somSelf); */
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpOpen");

   switch(ulView)
     {
     case ID_MENU_IMGEDITOR:
        /*case 0xbc2b:   editor */
        {
          char chrEditor[CCHMAXPATH]={0};
          char chrParams[CCHMAXPATH+2]="\"";
          char *chrPtr=chrParams;
          ULONG ulSize;
          ULONG ulRc;

          PrfQueryProfileString(HINI_USERPROFILE, "CWMM", "imageeditor", "ib.exe", chrEditor,
                                sizeof(chrEditor));
          chrPtr++;
          ulSize=sizeof(chrParams)-2;
          _wpQueryRealName(somSelf, chrPtr, &ulSize, TRUE);
          strcat(chrPtr,"\"");
          if((ulRc=launchPMProg("Editor", chrEditor, chrParams, somSelf, ulView))==NULLHANDLE) {
            return NULLHANDLE;
            break;
          }
          return NULLHANDLE;
        }
     default:
       break;
     }

    return (CWBitmap_parent_WPBitmap_wpOpen(somSelf, hwndCnr, 
                                            ulView, param));
}

SOM_Scope BOOL  SOMLINK cwbmp_wpAddSettingsPages(CWBitmap *somSelf, 
                                                 HWND hwndNotebook)
{
  BOOL rc;
    /* CWBitmapData *somThis = CWBitmapGetData(somSelf); */
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpAddSettingsPages");


    rc=(CWBitmap_parent_WPBitmap_wpAddSettingsPages(somSelf, 
                                                        hwndNotebook));
    return rc | (_cwmmAddImageInformationPage(somSelf, hwndNotebook));
}


SOM_Scope void  SOMLINK cwbmp_wpUnInitData(CWBitmap *somSelf)
{
    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpUnInitData");

    /* Free the private BITMAPINFOHEADER2 */
    if(_pBmpInfoHeader2) {
      _wpFreeMem(somSelf, _pBmpInfoHeader2);
      _pBmpInfoHeader2=NULLHANDLE;
    }

    _cwmmFreeBitmaps(somSelf);

    CWBitmap_parent_WPBitmap_wpUnInitData(somSelf);
}

SOM_Scope void  SOMLINK cwbmp_wpInitData(CWBitmap *somSelf)
{
    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpInitData");

    CWBitmap_parent_WPBitmap_wpInitData(somSelf);

    _hBitmapSmall=NULLHANDLE;
    _ulSizeSmallBitmap=0;
    _pBmpInfoHeader2=NULL;
    _chrIOProcName[0]=0;
    _bNoIOProcAvailable=FALSE;
}


SOM_Scope ULONG  SOMLINK cwbmp_wpQueryDetailsData(CWBitmap *somSelf, 
                                                  PVOID* ppDetailsData, 
                                                  PULONG pcp)
{
    BOOL rcParent;
    PIMAGEDETAILS pImageDetails;

    CWBitmapData *somThis = CWBitmapGetData(somSelf);
    CWBitmapMethodDebug("CWBitmap","cwbmp_wpQueryDetailsData");

    rcParent= (CWBitmap_parent_WPBitmap_wpQueryDetailsData(somSelf, 
                                                     ppDetailsData, 
                                                     pcp));

    /* The WPS asks for data to be displayed */
    if(ppDetailsData)
      {
        ULONG ulWidth, ulHeight, ulDepth;
        PBITMAPINFOHEADER2 pBmpInfoHeader2;       
        
        pBmpInfoHeader2=(PBITMAPINFOHEADER2) _cwmmQueryBitmapInfoHeader(somSelf);
        
        if(pBmpInfoHeader2) {
          if(pBmpInfoHeader2->cbFix==sizeof(BITMAPINFOHEADER2)) {
            ulWidth=pBmpInfoHeader2->cx;
            ulHeight=pBmpInfoHeader2->cy;
            ulDepth=pBmpInfoHeader2->cBitCount;
          }
          else {
            PBITMAPINFOHEADER pBmpInfoHeader=(PBITMAPINFOHEADER)pBmpInfoHeader2;
            ulWidth=pBmpInfoHeader->cx;
            ulHeight=pBmpInfoHeader->cy;
            ulDepth=pBmpInfoHeader->cBitCount;
          }
        }
        
        /* Fill the structure with the info to be displayed */      
        pImageDetails=(PIMAGEDETAILS)*ppDetailsData;  
        pImageDetails->ulWidth=ulWidth;
        pImageDetails->ulHeight=ulHeight;
        pImageDetails->ulDepth=ulDepth;
        pImageDetails->pszFormat=STR_BITMAPNAME;
        
        *ppDetailsData=((PBYTE) (*ppDetailsData))+sizeof(*pImageDetails);          
      }
    else
      {
        /* Buffer size is queried */
        if(pcp)
          *pcp+=sizeof(*pImageDetails);
      }    
    return rcParent;
}


SOM_Scope ULONG  SOMLINK cwbmpM_wpclsQueryDefaultView(M_CWBitmap *somSelf)
{
    /* M_CWBitmapData *somThis = M_CWBitmapGetData(somSelf); */
    M_CWBitmapMethodDebug("M_CWBitmap","cwbmpM_wpclsQueryDefaultView");

    return ID_MENU_IMGEDITOR;
}

SOM_Scope ULONG  SOMLINK cwbmpM_wpclsQueryIconData(M_CWBitmap *somSelf, 
                                                   PICONINFO pIconInfo)
{
    /* M_CWBitmapData *somThis = M_CWBitmapGetData(somSelf); */
    M_CWBitmapMethodDebug("M_CWBitmap","cwbmpM_wpclsQueryIconData");

	if (pIconInfo)   {
      pIconInfo->fFormat = ICON_RESOURCE;
      pIconInfo->hmod    = queryModuleHandle();
      pIconInfo->resid   = ID_ICONBMPFILE;
	} /* endif */

	return ( sizeof(ICONINFO) );
}



SOM_Scope ULONG  SOMLINK cwbmpM_wpclsQueryDetailsInfo(M_CWBitmap *somSelf, 
                                                      PCLASSFIELDINFO* ppClassFieldInfo, 
                                                      PULONG pSize)
{
  ULONG cParentColumns;
  PCLASSFIELDINFO pCfi;
  int i;
  ULONG rc;

    /* M_CWBitmapData *somThis = M_CWBitmapGetData(somSelf); */
    M_CWBitmapMethodDebug("M_CWBitmap","cwbmpM_wpclsQueryDetailsInfo");

    cParentColumns=(M_CWBitmap_parent_M_WPBitmap_wpclsQueryDetailsInfo(somSelf, 
                                                                       ppClassFieldInfo, 
                                                                       pSize));

  if(pSize)
    *pSize+=sizeof(IMAGEDETAILS);

  if(ppClassFieldInfo)
    {
      if(*ppClassFieldInfo)
        {
          pCfi=*ppClassFieldInfo;
          for(i=0; i<cParentColumns;i++)
            pCfi=(pCfi->pNextFieldInfo ? pCfi->pNextFieldInfo: pCfi);

          pCfi->pNextFieldInfo=cfiImageFieldInfo;
        }
      else
        *ppClassFieldInfo=cfiImageFieldInfo;
    }
    return (cParentColumns+NUM_IMAGE_DETAILS_FIELDS);
}


SOM_Scope void  SOMLINK cwbmpM_wpclsInitData(M_CWBitmap *somSelf)
{
  PCLASSFIELDINFO pCfi;
  USHORT i;

    /* M_CWBitmapData *somThis = M_CWBitmapGetData(somSelf); */
    M_CWBitmapMethodDebug("M_CWBitmap","cwbmpM_wpclsInitData");

    M_CWBitmap_parent_M_WPBitmap_wpclsInitData(somSelf);

    /* Load strings for details titles */
    if(getMessage(chrWidth, IDSTR_IMAGEDETAIL_WIDTH, sizeof(chrWidth), queryModuleHandle(), HWND_DESKTOP))
      pszImageColTitles[0]=chrWidth;
    if(getMessage(chrHeight, IDSTR_IMAGEDETAIL_HEIGHT, sizeof(chrHeight), queryModuleHandle(), HWND_DESKTOP))
      pszImageColTitles[1]=chrHeight;
    if(getMessage(chrDepth, IDSTR_IMAGEDETAIL_DEPTH, sizeof(chrDepth), queryModuleHandle(), HWND_DESKTOP))
      pszImageColTitles[2]=chrDepth;
    if(getMessage(chrFormat, IDSTR_IMAGEDETAIL_FORMAT, sizeof(chrFormat), queryModuleHandle(), HWND_DESKTOP))
      pszImageColTitles[3]=chrFormat;

    /* Setup the static data for the details view */
    for(i=0,pCfi=cfiImageFieldInfo;i<NUM_IMAGE_DETAILS_FIELDS;i++,pCfi++)
      {
        memset((PCH)pCfi,0,sizeof(CLASSFIELDINFO));
        
        pCfi->cb=sizeof(CLASSFIELDINFO);
        pCfi->flData=CFA_LEFT|CFA_SEPARATOR|CFA_FIREADONLY;
        pCfi->flTitle=CFA_CENTER|CFA_SEPARATOR|CFA_HORZSEPARATOR|CFA_STRING|CFA_FITITLEREADONLY;
        pCfi->pNextFieldInfo=pCfi+1;
        pCfi->pTitleData=(PVOID)pszImageColTitles[i];
        pCfi->flCompare=COMPARE_SUPPORTED|SORTBY_SUPPORTED;
        
        switch(i)
          {
          case 0: /* Width */
            pCfi->flData|=CFA_ULONG;
            pCfi->offFieldData=(ULONG)(FIELDOFFSETUL(IMAGEDETAILS, ulWidth));
            pCfi->ulLenFieldData=sizeof(ULONG);
            pCfi->ulLenCompareValue=sizeof(ULONG);
            pCfi->DefaultComparison=CMP_GREATER;
            break;
          case 1: /* Height */
            pCfi->flData|=CFA_ULONG;
            pCfi->offFieldData=(ULONG)(FIELDOFFSETUL(IMAGEDETAILS,ulHeight));
            pCfi->ulLenFieldData=sizeof(ULONG);
            pCfi->ulLenCompareValue=sizeof(ULONG);
            pCfi->DefaultComparison=CMP_GREATER;
            break;
          case 2: /* Depth */
            pCfi->flData|=CFA_ULONG;
            pCfi->offFieldData=(ULONG)(FIELDOFFSETUL(IMAGEDETAILS,ulDepth));
            pCfi->ulLenFieldData=sizeof(ULONG);
            pCfi->ulLenCompareValue=sizeof(ULONG);
            pCfi->DefaultComparison=CMP_GREATER;
            break;
          case 3: /* Format */
            pCfi->flData|=CFA_STRING;
            pCfi->offFieldData=(ULONG)(FIELDOFFSETUL(IMAGEDETAILS,pszFormat));
            pCfi->ulLenFieldData=sizeof(PSZ);
            pCfi->DefaultComparison=CMP_EQUAL;
            break;
          }/* switch(i) */
      }/* for() */
    cfiImageFieldInfo[NUM_IMAGE_DETAILS_FIELDS].pNextFieldInfo=NULL;
}
