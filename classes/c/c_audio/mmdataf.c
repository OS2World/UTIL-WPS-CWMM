/*
 * (C) Chris Wohlgemuth 2002-2003
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

#ifndef SOM_Module_mmdataf_Source
#define SOM_Module_mmdataf_Source
#endif
#define MMDataFile_Class_Source
#define M_MMDataFile_Class_Source

#include "mmdataf.ih"


SOM_Scope BOOL  SOMLINK mmdatafile_wpModifyPopupMenu(MMDataFile *somSelf, 
                                                     HWND hwndMenu, 
                                                     HWND hwndCnr, 
                                                     ULONG iPosition)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpModifyPopupMenu");

    return (MMDataFile_parent_WPDataFile_wpModifyPopupMenu(somSelf, 
                                                           hwndMenu, 
                                                           hwndCnr, 
                                                           iPosition));
}

SOM_Scope ULONG  SOMLINK mmdatafile_mmFilterPopupMenu(MMDataFile *somSelf, 
                                                      ULONG ulFlags, 
                                                      HWND hwndcnr, 
                                                      BOOL fMultiSelect)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_mmFilterPopupMenu");

    /* Return statement to be customized: */
    return;
}

SOM_Scope ULONG  SOMLINK mmdatafile_mmViewNotify(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_mmViewNotify");

    /* Return statement to be customized: */
    return;
}

SOM_Scope ULONG  SOMLINK mmdatafile_mmDetermineViewFor(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_mmDetermineViewFor");

    /* Return statement to be customized: */
    return;
}

SOM_Scope ULONG  SOMLINK mmdatafile_wpFilterPopupMenu(MMDataFile *somSelf, 
                                                      ULONG ulFlags, 
                                                      HWND hwndCnr, 
                                                      BOOL fMultiSelect)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpFilterPopupMenu");

    return (MMDataFile_parent_WPDataFile_wpFilterPopupMenu(somSelf, 
                                                           ulFlags, 
                                                           hwndCnr, 
                                                           fMultiSelect));
}

SOM_Scope void  SOMLINK mmdatafile_wpInitData(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpInitData");

    MMDataFile_parent_WPDataFile_wpInitData(somSelf);
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpMenuItemHelpSelected(MMDataFile *somSelf, 
                                                          ULONG MenuId)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpMenuItemHelpSelected");

    return (MMDataFile_parent_WPDataFile_wpMenuItemHelpSelected(somSelf, 
                                                                MenuId));
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpMenuItemSelected(MMDataFile *somSelf, 
                                                      HWND hwndFrame, 
                                                      ULONG ulMenuId)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpMenuItemSelected");

    return (MMDataFile_parent_WPDataFile_wpMenuItemSelected(somSelf, 
                                                            hwndFrame, 
                                                            ulMenuId));
}

SOM_Scope ULONG  SOMLINK mmdatafile_wpQueryConcurrentView(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpQueryConcurrentView");

    return (MMDataFile_parent_WPDataFile_wpQueryConcurrentView(somSelf));
}

SOM_Scope ULONG  SOMLINK mmdatafile_wpQueryDefaultView(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpQueryDefaultView");

    return (MMDataFile_parent_WPDataFile_wpQueryDefaultView(somSelf));
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpSetTitle(MMDataFile *somSelf, 
                                              PSZ pszNewTitle)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpSetTitle");

    return (MMDataFile_parent_WPDataFile_wpSetTitle(somSelf, 
                                                    pszNewTitle));
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpSetup(MMDataFile *somSelf, 
                                           PSZ pszSetupString)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpSetup");

    return (MMDataFile_parent_WPDataFile_wpSetup(somSelf, pszSetupString));
}

SOM_Scope MRESULT  SOMLINK mmdatafile_wpDragOver(MMDataFile *somSelf, 
                                                 HWND hwndCnr, 
                                                 PDRAGINFO pdrgInfo)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpDragOver");

    return (MMDataFile_parent_WPDataFile_wpDragOver(somSelf, 
                                                    hwndCnr, 
                                                    pdrgInfo));
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpRestoreState(MMDataFile *somSelf, 
                                                  ULONG ulReserved)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpRestoreState");

    return (MMDataFile_parent_WPDataFile_wpRestoreState(somSelf, 
                                                        ulReserved));
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpSaveState(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpSaveState");

    return (MMDataFile_parent_WPDataFile_wpSaveState(somSelf));
}

SOM_Scope BOOL  SOMLINK mmdatafile_wpSetRealName(MMDataFile *somSelf, 
                                                 PSZ pszName)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpSetRealName");

    return (MMDataFile_parent_WPDataFile_wpSetRealName(somSelf, 
                                                       pszName));
}

SOM_Scope HWND  SOMLINK mmdatafile_wpOpen(MMDataFile *somSelf, 
                                          HWND hwndCnr, ULONG ulView, 
                                          ULONG param)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpOpen");

    return (MMDataFile_parent_WPDataFile_wpOpen(somSelf, hwndCnr, 
                                                ulView, param));
}

SOM_Scope HPOINTER  SOMLINK mmdatafile_wpQueryIcon(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpQueryIcon");

    return (MMDataFile_parent_WPDataFile_wpQueryIcon(somSelf));
}


SOM_Scope void  SOMLINK mmdatafile_wpUnInitData(MMDataFile *somSelf)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpUnInitData");

    MMDataFile_parent_WPDataFile_wpUnInitData(somSelf);
}

SOM_Scope void  SOMLINK mmdatafile_wpObjectReady(MMDataFile *somSelf, 
                                                 ULONG ulCode, 
                                                 WPObject* refObject)
{
    /* MMDataFileData *somThis = MMDataFileGetData(somSelf); */
    MMDataFileMethodDebug("MMDataFile","mmdatafile_wpObjectReady");

    MMDataFile_parent_WPDataFile_wpObjectReady(somSelf, ulCode, 
                                               refObject);
}


SOM_Scope PSZ  SOMLINK mmdatafileM_wpclsQueryTitle(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryTitle");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryTitle(somSelf));
}

SOM_Scope BOOL  SOMLINK mmdatafileM_mmclsDeleteFromTypeList(M_MMDataFile *somSelf, 
                                                            PSZ pszType)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_mmclsDeleteFromTypeList");

    /* Return statement to be customized: */
    return;
}

SOM_Scope BOOL  SOMLINK mmdatafileM_mmclsDeleteFromExtensionList(M_MMDataFile *somSelf, 
                                                                 PSZ pszExt)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_mmclsDeleteFromExtensionList");

    /* Return statement to be customized: */
    return;
}

SOM_Scope BOOL  SOMLINK mmdatafileM_mmclsInitTypesAndExtensions(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_mmclsInitTypesAndExtensions");

    /* Return statement to be customized: */
    return;
}

SOM_Scope BOOL  SOMLINK mmdatafileM_mmclsSetDefaultView(M_MMDataFile *somSelf, 
                                                        ULONG ulView)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_mmclsSetDefaultView");

    /* Return statement to be customized: */
    return;
}

SOM_Scope PSZ  SOMLINK mmdatafileM_wpclsQueryInstanceType(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryInstanceType");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryInstanceType(somSelf));
}

SOM_Scope PSZ  SOMLINK mmdatafileM_wpclsQueryInstanceFilter(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryInstanceFilter");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryInstanceFilter(somSelf));
}

SOM_Scope BOOL  SOMLINK mmdatafileM_wpclsCreateDefaultTemplates(M_MMDataFile *somSelf, 
                                                                WPObject* Folder)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsCreateDefaultTemplates");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsCreateDefaultTemplates(somSelf, 
                                                                         Folder));
}

SOM_Scope void  SOMLINK mmdatafileM_wpclsInitData(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsInitData");

    M_MMDataFile_parent_M_WPDataFile_wpclsInitData(somSelf);
}

SOM_Scope BOOL  SOMLINK mmdatafileM_wpclsQueryDefaultHelp(M_MMDataFile *somSelf, 
                                                          PULONG pHelpPanelId, 
                                                          PSZ pszHelpLibrary)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryDefaultHelp");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryDefaultHelp(somSelf, 
                                                                   pHelpPanelId, 
                                                                   pszHelpLibrary));
}

SOM_Scope ULONG  SOMLINK mmdatafileM_wpclsQueryDefaultView(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryDefaultView");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryDefaultView(somSelf));
}

SOM_Scope ULONG  SOMLINK mmdatafileM_wpclsQueryStyle(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryStyle");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryStyle(somSelf));
}


SOM_Scope void  SOMLINK mmdatafileM_wpclsUnInitData(M_MMDataFile *somSelf)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsUnInitData");

    M_MMDataFile_parent_M_WPDataFile_wpclsUnInitData(somSelf);
}

SOM_Scope ULONG  SOMLINK mmdatafileM_wpclsQueryIconData(M_MMDataFile *somSelf, 
                                                        PICONINFO pIconInfo)
{
    /* M_MMDataFileData *somThis = M_MMDataFileGetData(somSelf); */
    M_MMDataFileMethodDebug("M_MMDataFile","mmdatafileM_wpclsQueryIconData");

    return (M_MMDataFile_parent_M_WPDataFile_wpclsQueryIconData(somSelf, 
                                                                pIconInfo));
}

