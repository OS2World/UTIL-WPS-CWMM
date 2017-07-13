
/*
 * This file was generated by the SOM Compiler.
 * FileName: include\cwaudio.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.7
 *     SOM Emitter emith: 2.42
 */

/*
 * 
 * CWAudio.idl:
 */


#ifndef SOM_CWAudio_h
#define SOM_CWAudio_h


#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef CWAudio
#define CWAudio SOMObject
#endif
#include <cwmmdataf.h>

/*
 * Start of user-defined types:
 */
#ifndef SOMClass
    #define SOMClass SOMObject
#endif /* SOMClass */
#ifndef WPFolder
    #define WPFolder SOMObject
#endif /* WPFolder */
#ifndef M_WPObject
    #define M_WPObject SOMObject
#endif /* M_WPObject */
#ifndef WPObject
    #define WPObject SOMObject
#endif /* WPObject */
#ifndef M_WPFileSystem
    #define M_WPFileSystem SOMObject
#endif /* M_WPFileSystem */
#ifndef M_WPDataFile
    #define M_WPDataFile SOMObject
#endif /* M_WPDataFile */
#ifndef M_CWMMDataFile
    #define M_CWMMDataFile SOMObject
#endif /* M_CWMMDataFile */
#ifndef M_CWAudio
    #define M_CWAudio SOMObject
#endif /* M_CWAudio */

/*
 * End of user-defined types.
 */

/*
 * Passthru lines: File: "C.h", "after"
 */

#include "cwmmres.h"


#define CWAudio_MajorVersion 1
#define CWAudio_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define CWAudioNewClass cwaudioc
#pragma linkage(cwaudioc, system)
#define CWAudioClassData cwaudiod
#define CWAudioCClassData cwaudiox
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */

/*
 * Include the meta class file
 */
#include <cwaudio.h>
/*--------------Migration------------*/
#define CWAudio_classObj CWAudioClassData.classObject
#define _CWAudio CWAudio_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK CWAudioNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(CWAudioNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct CWAudioClassDataStructure {
	SOMClass *classObject;
	somMToken cwmmAddWaveInformationPage;
	somMToken cwmmSetTrackInfo;
	somMToken cwmmQueryTrackInfo;
	somMToken cwmmPlayTrack;
	somMToken cwmmAddTrackNamePage;
} SOMDLINK CWAudioClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct CWAudioCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK CWAudioCClassData;

/*
 * New and Renew macros for CWAudio
 */
#define CWAudioNew() \
   (_somNew(_CWAudio ? _CWAudio : CWAudioNewClass(CWAudio_MajorVersion, CWAudio_MinorVersion)))
#define CWAudioRenew(buf) \
   (_somRenew(_CWAudio ? _CWAudio : CWAudioNewClass(CWAudio_MajorVersion, CWAudio_MinorVersion), buf))

/*
 * Override method: wpAddSettingsPages
 */
#define CWAudio_wpAddSettingsPages(somSelf,hwndNotebook) \
	WPObject_wpAddSettingsPages(somSelf,hwndNotebook)

/*
 * Override method: wpQueryDetailsData
 */
#define CWAudio_wpQueryDetailsData(somSelf,ppDetailsData,pcp) \
	WPObject_wpQueryDetailsData(somSelf,ppDetailsData,pcp)

/*
 * Override method: wpRestoreState
 */
#define CWAudio_wpRestoreState(somSelf,ulReserved) \
	WPObject_wpRestoreState(somSelf,ulReserved)

/*
 * Override method: wpSaveState
 */
#define CWAudio_wpSaveState(somSelf) \
	WPObject_wpSaveState(somSelf)

/*
 * Override method: wpModifyPopupMenu
 */
#define CWAudio_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition) \
	WPObject_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition)

/*
 * Override method: wpMenuItemSelected
 */
#define CWAudio_wpMenuItemSelected(somSelf,hwndFrame,ulMenuId) \
	WPObject_wpMenuItemSelected(somSelf,hwndFrame,ulMenuId)

/*
 * Override method: wpObjectReady
 */
#define CWAudio_wpObjectReady(somSelf,ulCode,refObject) \
	WPObject_wpObjectReady(somSelf,ulCode,refObject)

/*
 * Override method: wpRefresh
 */
#define CWAudio_wpRefresh(somSelf,ulView,pReserved) \
	WPFileSystem_wpRefresh(somSelf,ulView,pReserved)

/*
 * Override method: wpSetup
 */
#define CWAudio_wpSetup(somSelf,pszSetupString) \
	WPObject_wpSetup(somSelf,pszSetupString)

/*
 * Override method: wpOpen
 */
#define CWAudio_wpOpen(somSelf,hwndCnr,ulView,param) \
	WPObject_wpOpen(somSelf,hwndCnr,ulView,param)

/*
 * Override method: wpSetTitle
 */
#define CWAudio_wpSetTitle(somSelf,pszNewTitle) \
	WPObject_wpSetTitle(somSelf,pszNewTitle)

/*
 * Override method: wpQueryRealName
 */
#define CWAudio_wpQueryRealName(somSelf,pszFilename,pcb,fQualified) \
	WPFileSystem_wpQueryRealName(somSelf,pszFilename,pcb,fQualified)

/*
 * Override method: wpQueryTitle
 */
#define CWAudio_wpQueryTitle(somSelf) \
	WPObject_wpQueryTitle(somSelf)

/*
 * Override method: wpInitData
 */
#define CWAudio_wpInitData(somSelf) \
	WPObject_wpInitData(somSelf)

/*
 * New Method: cwmmAddWaveInformationPage
 */
typedef ULONG   SOMLINK somTP_CWAudio_cwmmAddWaveInformationPage(CWAudio *somSelf,
		HWND hwndNotebook);
#pragma linkage(somTP_CWAudio_cwmmAddWaveInformationPage, system)
typedef somTP_CWAudio_cwmmAddWaveInformationPage *somTD_CWAudio_cwmmAddWaveInformationPage;
#define somMD_CWAudio_cwmmAddWaveInformationPage "::CWAudio::cwmmAddWaveInformationPage"
#define CWAudio_cwmmAddWaveInformationPage(somSelf,hwndNotebook) \
    (SOM_Resolve(somSelf, CWAudio, cwmmAddWaveInformationPage) \
	(somSelf,hwndNotebook))
#ifndef SOMGD_cwmmAddWaveInformationPage
    #if (defined(_cwmmAddWaveInformationPage) || defined(__cwmmAddWaveInformationPage))
        #undef _cwmmAddWaveInformationPage
        #undef __cwmmAddWaveInformationPage
        #define SOMGD_cwmmAddWaveInformationPage 1
    #else
        #define _cwmmAddWaveInformationPage CWAudio_cwmmAddWaveInformationPage
    #endif /* _cwmmAddWaveInformationPage */
#endif /* SOMGD_cwmmAddWaveInformationPage */

/*
 * New Method: cwmmSetTrackInfo
 */
typedef BOOL   SOMLINK somTP_CWAudio_cwmmSetTrackInfo(CWAudio *somSelf,
		char* chrString,
		ULONG ulValue,
		int iWhich);
#pragma linkage(somTP_CWAudio_cwmmSetTrackInfo, system)
typedef somTP_CWAudio_cwmmSetTrackInfo *somTD_CWAudio_cwmmSetTrackInfo;
#define somMD_CWAudio_cwmmSetTrackInfo "::CWAudio::cwmmSetTrackInfo"
#define CWAudio_cwmmSetTrackInfo(somSelf,chrString,ulValue,iWhich) \
    (SOM_Resolve(somSelf, CWAudio, cwmmSetTrackInfo) \
	(somSelf,chrString,ulValue,iWhich))
#ifndef SOMGD_cwmmSetTrackInfo
    #if (defined(_cwmmSetTrackInfo) || defined(__cwmmSetTrackInfo))
        #undef _cwmmSetTrackInfo
        #undef __cwmmSetTrackInfo
        #define SOMGD_cwmmSetTrackInfo 1
    #else
        #define _cwmmSetTrackInfo CWAudio_cwmmSetTrackInfo
    #endif /* _cwmmSetTrackInfo */
#endif /* SOMGD_cwmmSetTrackInfo */

/*
 * New Method: cwmmQueryTrackInfo
 */
typedef ULONG   SOMLINK somTP_CWAudio_cwmmQueryTrackInfo(CWAudio *somSelf,
		char** chrString,
		ULONG ulSize,
		int iWhich);
#pragma linkage(somTP_CWAudio_cwmmQueryTrackInfo, system)
typedef somTP_CWAudio_cwmmQueryTrackInfo *somTD_CWAudio_cwmmQueryTrackInfo;
#define somMD_CWAudio_cwmmQueryTrackInfo "::CWAudio::cwmmQueryTrackInfo"
#define CWAudio_cwmmQueryTrackInfo(somSelf,chrString,ulSize,iWhich) \
    (SOM_Resolve(somSelf, CWAudio, cwmmQueryTrackInfo) \
	(somSelf,chrString,ulSize,iWhich))
#ifndef SOMGD_cwmmQueryTrackInfo
    #if (defined(_cwmmQueryTrackInfo) || defined(__cwmmQueryTrackInfo))
        #undef _cwmmQueryTrackInfo
        #undef __cwmmQueryTrackInfo
        #define SOMGD_cwmmQueryTrackInfo 1
    #else
        #define _cwmmQueryTrackInfo CWAudio_cwmmQueryTrackInfo
    #endif /* _cwmmQueryTrackInfo */
#endif /* SOMGD_cwmmQueryTrackInfo */

/*
 * New Method: cwmmPlayTrack
 */
typedef void   SOMLINK somTP_CWAudio_cwmmPlayTrack(CWAudio *somSelf,
		BOOL bPlay);
#pragma linkage(somTP_CWAudio_cwmmPlayTrack, system)
typedef somTP_CWAudio_cwmmPlayTrack *somTD_CWAudio_cwmmPlayTrack;
#define somMD_CWAudio_cwmmPlayTrack "::CWAudio::cwmmPlayTrack"
#define CWAudio_cwmmPlayTrack(somSelf,bPlay) \
    (SOM_Resolve(somSelf, CWAudio, cwmmPlayTrack) \
	(somSelf,bPlay))
#ifndef SOMGD_cwmmPlayTrack
    #if (defined(_cwmmPlayTrack) || defined(__cwmmPlayTrack))
        #undef _cwmmPlayTrack
        #undef __cwmmPlayTrack
        #define SOMGD_cwmmPlayTrack 1
    #else
        #define _cwmmPlayTrack CWAudio_cwmmPlayTrack
    #endif /* _cwmmPlayTrack */
#endif /* SOMGD_cwmmPlayTrack */

/*
 * New Method: cwmmAddTrackNamePage
 */
typedef ULONG   SOMLINK somTP_CWAudio_cwmmAddTrackNamePage(CWAudio *somSelf,
		HWND hwndNotebook);
#pragma linkage(somTP_CWAudio_cwmmAddTrackNamePage, system)
typedef somTP_CWAudio_cwmmAddTrackNamePage *somTD_CWAudio_cwmmAddTrackNamePage;
#define somMD_CWAudio_cwmmAddTrackNamePage "::CWAudio::cwmmAddTrackNamePage"
#define CWAudio_cwmmAddTrackNamePage(somSelf,hwndNotebook) \
    (SOM_Resolve(somSelf, CWAudio, cwmmAddTrackNamePage) \
	(somSelf,hwndNotebook))
#ifndef SOMGD_cwmmAddTrackNamePage
    #if (defined(_cwmmAddTrackNamePage) || defined(__cwmmAddTrackNamePage))
        #undef _cwmmAddTrackNamePage
        #undef __cwmmAddTrackNamePage
        #define SOMGD_cwmmAddTrackNamePage 1
    #else
        #define _cwmmAddTrackNamePage CWAudio_cwmmAddTrackNamePage
    #endif /* _cwmmAddTrackNamePage */
#endif /* SOMGD_cwmmAddTrackNamePage */
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define CWAudio_cwmmSetObjWindowProc CWMMDataFile_cwmmSetObjWindowProc
#define CWAudio_cwmmQueryObjWindowProc CWMMDataFile_cwmmQueryObjWindowProc
#define CWAudio_cwmmSetRecordTitles CWMMDataFile_cwmmSetRecordTitles
#define CWAudio_wpAddFileTypePage WPDataFile_wpAddFileTypePage
#define CWAudio_wpPrintMetaFile WPDataFile_wpPrintMetaFile
#define CWAudio_wpPrintPifFile WPDataFile_wpPrintPifFile
#define CWAudio_wpPrintPrinterSpecificFile WPDataFile_wpPrintPrinterSpecificFile
#define CWAudio_wpPrintUnknownFile WPDataFile_wpPrintUnknownFile
#define CWAudio_wpQueryAssociatedProgram WPDataFile_wpQueryAssociatedProgram
#define CWAudio_wpSetAssociatedFileIcon WPDataFile_wpSetAssociatedFileIcon
#define CWAudio_wpQueryAssociatedFileIcon WPDataFile_wpQueryAssociatedFileIcon
#define CWAudio_wpSetRealName WPFileSystem_wpSetRealName
#define CWAudio_wpSetType WPFileSystem_wpSetType
#define CWAudio_wpQueryType WPFileSystem_wpQueryType
#define CWAudio_wpSetAttr WPFileSystem_wpSetAttr
#define CWAudio_wpQueryAttr WPFileSystem_wpQueryAttr
#define CWAudio_wpAddFile1Page WPFileSystem_wpAddFile1Page
#define CWAudio_wpAddFile2Page WPFileSystem_wpAddFile2Page
#define CWAudio_wpAddFile3Page WPFileSystem_wpAddFile3Page
#define CWAudio_wpAddFileMenuPage WPFileSystem_wpAddFileMenuPage
#define CWAudio_wpQueryCreation WPFileSystem_wpQueryCreation
#define CWAudio_wpQueryLastAccess WPFileSystem_wpQueryLastAccess
#define CWAudio_wpQueryLastWrite WPFileSystem_wpQueryLastWrite
#define CWAudio_wpQueryFileSize WPFileSystem_wpQueryFileSize
#define CWAudio_wpQueryEASize WPFileSystem_wpQueryEASize
#define CWAudio_wpSetDateInfo WPFileSystem_wpSetDateInfo
#define CWAudio_wpSetFileSizeInfo WPFileSystem_wpSetFileSizeInfo
#define CWAudio_wpQueryRefreshFlags WPFileSystem_wpQueryRefreshFlags
#define CWAudio_wpSetRefreshFlags WPFileSystem_wpSetRefreshFlags
#define CWAudio_wpPrintPlainTextFile WPFileSystem_wpPrintPlainTextFile
#define CWAudio_wpSetTitleAndRenameFile WPFileSystem_wpSetTitleAndRenameFile
#define CWAudio_wpConfirmRenameFileWithExt WPFileSystem_wpConfirmRenameFileWithExt
#define CWAudio_wpVerifyUpdateAccess WPFileSystem_wpVerifyUpdateAccess
#define CWAudio_wpAddUserItemsToPopupMenu WPFileSystem_wpAddUserItemsToPopupMenu
#define CWAudio_wpIsDiskSwapped WPFileSystem_wpIsDiskSwapped
#define CWAudio_wpQueryFilename WPFileSystem_wpQueryFilename
#define CWAudio_wpQueryDisk WPFileSystem_wpQueryDisk
#define CWAudio_wpQueryDateInfo WPFileSystem_wpQueryDateInfo
#define CWAudio_wpConfirmKeepAssoc WPFileSystem_wpConfirmKeepAssoc
#define CWAudio_wpAddObjectGeneralPage WPObject_wpAddObjectGeneralPage
#define CWAudio_wpAddObjectGeneralPage2 WPObject_wpAddObjectGeneralPage2
#define CWAudio_wpAddObjectWindowPage WPObject_wpAddObjectWindowPage
#define CWAudio_wpAddToObjUseList WPObject_wpAddToObjUseList
#define CWAudio_wpAllocMem WPObject_wpAllocMem
#define CWAudio_wpAppendObject WPObject_wpAppendObject
#define CWAudio_wpAssertObjectMutexSem WPObject_wpAssertObjectMutexSem
#define CWAudio_wpClose WPObject_wpClose
#define CWAudio_wpCnrInsertObject WPObject_wpCnrInsertObject
#define CWAudio_wpCnrRemoveObject WPObject_wpCnrRemoveObject
#define CWAudio_wpCnrSetEmphasis WPObject_wpCnrSetEmphasis
#define CWAudio_wpConfirmDelete WPObject_wpConfirmDelete
#define CWAudio_wpConfirmObjectTitle WPObject_wpConfirmObjectTitle
#define CWAudio_wpCopiedFromTemplate WPObject_wpCopiedFromTemplate
#define CWAudio_wpCopyObject WPObject_wpCopyObject
#define CWAudio_wpCreateAnother WPObject_wpCreateAnother
#define CWAudio_wpCreateFromTemplate WPObject_wpCreateFromTemplate
#define CWAudio_wpCreateShadowObject WPObject_wpCreateShadowObject
#define CWAudio_wpDelete WPObject_wpDelete
#define CWAudio_wpDeleteFromObjUseList WPObject_wpDeleteFromObjUseList
#define CWAudio_wpDisplayHelp WPObject_wpDisplayHelp
#define CWAudio_wpDisplayMenu WPObject_wpDisplayMenu
#define CWAudio_wpDoesObjectMatch WPObject_wpDoesObjectMatch
#define CWAudio_wpDraggedOverObject WPObject_wpDraggedOverObject
#define CWAudio_wpDragOver WPObject_wpDragOver
#define CWAudio_wpDrop WPObject_wpDrop
#define CWAudio_wpDroppedOnObject WPObject_wpDroppedOnObject
#define CWAudio_wpEndConversation WPObject_wpEndConversation
#define CWAudio_wpFilterPopupMenu WPObject_wpFilterPopupMenu
#define CWAudio_wpFindUseItem WPObject_wpFindUseItem
#define CWAudio_wpFormatDragItem WPObject_wpFormatDragItem
#define CWAudio_wpFree WPObject_wpFree
#define CWAudio_wpFreeMem WPObject_wpFreeMem
#define CWAudio_wpHide WPObject_wpHide
#define CWAudio_wpInsertPopupMenuItems WPObject_wpInsertPopupMenuItems
#define CWAudio_wpInsertSettingsPage WPObject_wpInsertSettingsPage
#define CWAudio_wpMenuItemHelpSelected WPObject_wpMenuItemHelpSelected
#define CWAudio_wpMoveObject WPObject_wpMoveObject
#define CWAudio_wpPrintObject WPObject_wpPrintObject
#define CWAudio_wpQueryConcurrentView WPObject_wpQueryConcurrentView
#define CWAudio_wpQueryButtonAppearance WPObject_wpQueryButtonAppearance
#define CWAudio_wpQueryConfirmations WPObject_wpQueryConfirmations
#define CWAudio_wpQueryDefaultHelp WPObject_wpQueryDefaultHelp
#define CWAudio_wpQueryDefaultView WPObject_wpQueryDefaultView
#define CWAudio_wpQueryError WPObject_wpQueryError
#define CWAudio_wpSetFolder WPObject_wpSetFolder
#define CWAudio_wpQueryFolder WPObject_wpQueryFolder
#define CWAudio_wpQueryIcon WPObject_wpQueryIcon
#define CWAudio_wpQueryIconData WPObject_wpQueryIconData
#define CWAudio_wpQueryMinWindow WPObject_wpQueryMinWindow
#define CWAudio_wpQueryNameClashOptions WPObject_wpQueryNameClashOptions
#define CWAudio_wpQueryStyle WPObject_wpQueryStyle
#define CWAudio_wpSetTaskRec WPObject_wpSetTaskRec
#define CWAudio_wpFindTaskRec WPObject_wpFindTaskRec
#define CWAudio_wpRegisterView WPObject_wpRegisterView
#define CWAudio_wpReleaseObjectMutexSem WPObject_wpReleaseObjectMutexSem
#define CWAudio_wpRender WPObject_wpRender
#define CWAudio_wpRenderComplete WPObject_wpRenderComplete
#define CWAudio_wpReplaceObject WPObject_wpReplaceObject
#define CWAudio_wpRequestObjectMutexSem WPObject_wpRequestObjectMutexSem
#define CWAudio_wpRestore WPObject_wpRestore
#define CWAudio_wpRestoreData WPObject_wpRestoreData
#define CWAudio_wpRestoreLong WPObject_wpRestoreLong
#define CWAudio_wpRestoreString WPObject_wpRestoreString
#define CWAudio_wpSaveData WPObject_wpSaveData
#define CWAudio_wpSaveImmediate WPObject_wpSaveImmediate
#define CWAudio_wpSaveDeferred WPObject_wpSaveDeferred
#define CWAudio_wpSaveLong WPObject_wpSaveLong
#define CWAudio_wpSaveString WPObject_wpSaveString
#define CWAudio_wpScanSetupString WPObject_wpScanSetupString
#define CWAudio_wpSetConcurrentView WPObject_wpSetConcurrentView
#define CWAudio_wpSetButtonAppearance WPObject_wpSetButtonAppearance
#define CWAudio_wpSetDefaultHelp WPObject_wpSetDefaultHelp
#define CWAudio_wpSetDefaultView WPObject_wpSetDefaultView
#define CWAudio_wpSetError WPObject_wpSetError
#define CWAudio_wpSetIconHandle WPObject_wpSetIconHandle
#define CWAudio_wpQueryScreenGroupID WPObject_wpQueryScreenGroupID
#define CWAudio_wpSetupOnce WPObject_wpSetupOnce
#define CWAudio_wpSetIcon WPObject_wpSetIcon
#define CWAudio_wpSetIconData WPObject_wpSetIconData
#define CWAudio_wpSetMinWindow WPObject_wpSetMinWindow
#define CWAudio_wpSetStyle WPObject_wpSetStyle
#define CWAudio_wpModifyStyle WPObject_wpModifyStyle
#define CWAudio_wpSwitchTo WPObject_wpSwitchTo
#define CWAudio_wpUnInitData WPObject_wpUnInitData
#define CWAudio_wpViewObject WPObject_wpViewObject
#define CWAudio_wpQueryTrueStyle WPObject_wpQueryTrueStyle
#define CWAudio_wpQueryHandle WPObject_wpQueryHandle
#define CWAudio_wpUnlockObject WPObject_wpUnlockObject
#define CWAudio_wpIsObjectInitialized WPObject_wpIsObjectInitialized
#define CWAudio_wpCreateShadowObjectExt WPObject_wpCreateShadowObjectExt
#define CWAudio_wpCnrDeleteUseItem WPObject_wpCnrDeleteUseItem
#define CWAudio_wpIsDeleteable WPObject_wpIsDeleteable
#define CWAudio_wpQueryCoreRecord WPObject_wpQueryCoreRecord
#define CWAudio_wpSetObjectID WPObject_wpSetObjectID
#define CWAudio_wpQueryObjectID WPObject_wpQueryObjectID
#define CWAudio_wpSetDefaultIconPos WPObject_wpSetDefaultIconPos
#define CWAudio_wpQueryDefaultIconPos WPObject_wpQueryDefaultIconPos
#define CWAudio_wpCnrRefreshDetails WPObject_wpCnrRefreshDetails
#define CWAudio_wpFindViewItem WPObject_wpFindViewItem
#define CWAudio_wpLockObject WPObject_wpLockObject
#define CWAudio_wpIsLocked WPObject_wpIsLocked
#define CWAudio_wpQueryContainerFlagPtr WPObject_wpQueryContainerFlagPtr
#define CWAudio_wpWaitForClose WPObject_wpWaitForClose
#define CWAudio_somDefaultInit SOMObject_somDefaultInit
#define CWAudio_somDestruct SOMObject_somDestruct
#define CWAudio_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define CWAudio_somDefaultAssign SOMObject_somDefaultAssign
#define CWAudio_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define CWAudio_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define CWAudio_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define CWAudio_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define CWAudio_somDefaultVAssign SOMObject_somDefaultVAssign
#define CWAudio_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define CWAudio_somInit SOMObject_somInit
#define CWAudio_somFree SOMObject_somFree
#define CWAudio_somUninit SOMObject_somUninit
#define CWAudio_somGetClass SOMObject_somGetClass
#define CWAudio_somGetClassName SOMObject_somGetClassName
#define CWAudio_somGetSize SOMObject_somGetSize
#define CWAudio_somIsA SOMObject_somIsA
#define CWAudio_somIsInstanceOf SOMObject_somIsInstanceOf
#define CWAudio_somRespondsTo SOMObject_somRespondsTo
#define CWAudio_somDispatch SOMObject_somDispatch
#define CWAudio_somClassDispatch SOMObject_somClassDispatch
#define CWAudio_somCastObj SOMObject_somCastObj
#define CWAudio_somResetObj SOMObject_somResetObj
#define CWAudio_somDispatchV SOMObject_somDispatchV
#define CWAudio_somDispatchL SOMObject_somDispatchL
#define CWAudio_somDispatchA SOMObject_somDispatchA
#define CWAudio_somDispatchD SOMObject_somDispatchD
#define CWAudio_somPrintSelf SOMObject_somPrintSelf
#define CWAudio_somDumpSelf SOMObject_somDumpSelf
#define CWAudio_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_CWAudio_h */

#ifndef SOM_M_CWAudio_h
#define SOM_M_CWAudio_h


#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef M_CWAudio
#define M_CWAudio SOMObject
#endif
#include <cwmmdataf.h>

/*
 * Start of user-defined types:
 */

/*
 * End of user-defined types.
 */

#define M_CWAudio_MajorVersion 1
#define M_CWAudio_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define M_CWAudioNewClass cwaudioMc
#pragma linkage(cwaudioMc, system)
#define M_CWAudioClassData cwaudioMd
#define M_CWAudioCClassData cwaudioMx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define M_CWAudio_classObj M_CWAudioClassData.classObject
#define _M_CWAudio M_CWAudio_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK M_CWAudioNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(M_CWAudioNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct M_CWAudioClassDataStructure {
	SOMClass *classObject;
} SOMDLINK M_CWAudioClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct M_CWAudioCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK M_CWAudioCClassData;

/*
 * New and Renew macros for M_CWAudio
 */
#define M_CWAudioNew() \
   (_somNew(_M_CWAudio ? _M_CWAudio : M_CWAudioNewClass(M_CWAudio_MajorVersion, M_CWAudio_MinorVersion)))
#define M_CWAudioRenew(buf) \
   (_somRenew(_M_CWAudio ? _M_CWAudio : M_CWAudioNewClass(M_CWAudio_MajorVersion, M_CWAudio_MinorVersion), buf))

/*
 * Override method: wpclsCreateDefaultTemplates
 */
#define M_CWAudio_wpclsCreateDefaultTemplates(somSelf,Folder) \
	M_WPObject_wpclsCreateDefaultTemplates(somSelf,Folder)

/*
 * Override method: wpclsQueryDetailsInfo
 */
#define M_CWAudio_wpclsQueryDetailsInfo(somSelf,ppClassFieldInfo,pSize) \
	M_WPObject_wpclsQueryDetailsInfo(somSelf,ppClassFieldInfo,pSize)

/*
 * Override method: wpclsQueryTitle
 */
#define M_CWAudio_wpclsQueryTitle(somSelf) \
	M_WPObject_wpclsQueryTitle(somSelf)

/*
 * Override method: wpclsInitData
 */
#define M_CWAudio_wpclsInitData(somSelf) \
	M_WPObject_wpclsInitData(somSelf)

/*
 * Override method: wpclsUnInitData
 */
#define M_CWAudio_wpclsUnInitData(somSelf) \
	M_WPObject_wpclsUnInitData(somSelf)

/*
 * Override method: wpclsQueryDefaultView
 */
#define M_CWAudio_wpclsQueryDefaultView(somSelf) \
	M_WPObject_wpclsQueryDefaultView(somSelf)

/*
 * Override method: wpclsQueryIconData
 */
#define M_CWAudio_wpclsQueryIconData(somSelf,pIconInfo) \
	M_WPObject_wpclsQueryIconData(somSelf,pIconInfo)
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define M_CWAudio_wpclsQueryAwakeObject M_WPFileSystem_wpclsQueryAwakeObject
#define M_CWAudio_wpclsFileSysExists M_WPFileSystem_wpclsFileSysExists
#define M_CWAudio_wpclsQueryObjectFromPath M_WPFileSystem_wpclsQueryObjectFromPath
#define M_CWAudio_wpclsQueryInstanceType M_WPFileSystem_wpclsQueryInstanceType
#define M_CWAudio_wpclsQueryInstanceFilter M_WPFileSystem_wpclsQueryInstanceFilter
#define M_CWAudio_wpclsMakeAwake M_WPObject_wpclsMakeAwake
#define M_CWAudio_wpclsNew M_WPObject_wpclsNew
#define M_CWAudio_wpclsQueryDefaultHelp M_WPObject_wpclsQueryDefaultHelp
#define M_CWAudio_wpclsQueryDetails M_WPObject_wpclsQueryDetails
#define M_CWAudio_wpclsQueryExtendedCriteria M_WPObject_wpclsQueryExtendedCriteria
#define M_CWAudio_wpclsQueryFolder M_WPObject_wpclsQueryFolder
#define M_CWAudio_wpclsQueryIcon M_WPObject_wpclsQueryIcon
#define M_CWAudio_wpclsQuerySearchInfo M_WPObject_wpclsQuerySearchInfo
#define M_CWAudio_wpclsQueryStyle M_WPObject_wpclsQueryStyle
#define M_CWAudio_wpclsFindObjectFirst M_WPObject_wpclsFindObjectFirst
#define M_CWAudio_wpclsFindObjectNext M_WPObject_wpclsFindObjectNext
#define M_CWAudio_wpclsFindObjectEnd M_WPObject_wpclsFindObjectEnd
#define M_CWAudio_wpclsSetError M_WPObject_wpclsSetError
#define M_CWAudio_wpclsQueryError M_WPObject_wpclsQueryError
#define M_CWAudio_wpclsQuerySettingsPageSize M_WPObject_wpclsQuerySettingsPageSize
#define M_CWAudio_wpclsQueryObject M_WPObject_wpclsQueryObject
#define M_CWAudio_wpclsQueryButtonAppearance M_WPObject_wpclsQueryButtonAppearance
#define M_CWAudio_wpclsDecUsage M_WPObject_wpclsDecUsage
#define M_CWAudio_wpclsIncUsage M_WPObject_wpclsIncUsage
#define M_CWAudio_wpclsInsertMultipleObjects M_WPObject_wpclsInsertMultipleObjects
#define M_CWAudio_wpclsFindOneObject M_WPObject_wpclsFindOneObject
#define M_CWAudio_wpclsSetSettingsPageSize M_WPObject_wpclsSetSettingsPageSize
#define M_CWAudio_wpclsSetIconData M_WPObject_wpclsSetIconData
#define M_CWAudio_wpclsSetIcon M_WPObject_wpclsSetIcon
#define M_CWAudio_wpclsObjectFromHandle M_WPObject_wpclsObjectFromHandle
#define M_CWAudio_wpclsRemoveObjects M_WPObject_wpclsRemoveObjects
#define M_CWAudio__get_somDataAlignment SOMClass__get_somDataAlignment
#define M_CWAudio__get_somInstanceDataOffsets SOMClass__get_somInstanceDataOffsets
#define M_CWAudio__get_somDirectInitClasses SOMClass__get_somDirectInitClasses
#define M_CWAudio__set_somDirectInitClasses SOMClass__set_somDirectInitClasses
#define M_CWAudio_somNew SOMClass_somNew
#define M_CWAudio_somNewNoInit SOMClass_somNewNoInit
#define M_CWAudio_somRenew SOMClass_somRenew
#define M_CWAudio_somRenewNoInit SOMClass_somRenewNoInit
#define M_CWAudio_somRenewNoZero SOMClass_somRenewNoZero
#define M_CWAudio_somRenewNoInitNoZero SOMClass_somRenewNoInitNoZero
#define M_CWAudio_somAllocate SOMClass_somAllocate
#define M_CWAudio_somDeallocate SOMClass_somDeallocate
#define M_CWAudio__get_somClassAllocate SOMClass__get_somClassAllocate
#define M_CWAudio__get_somClassDeallocate SOMClass__get_somClassDeallocate
#define M_CWAudio_somGetInstanceInitMask SOMClass_somGetInstanceInitMask
#define M_CWAudio_somGetInstanceDestructionMask SOMClass_somGetInstanceDestructionMask
#define M_CWAudio_somGetInstanceAssignmentMask SOMClass_somGetInstanceAssignmentMask
#define M_CWAudio_somInitClass SOMClass_somInitClass
#define M_CWAudio_somInitMIClass SOMClass_somInitMIClass
#define M_CWAudio_somAddStaticMethod SOMClass_somAddStaticMethod
#define M_CWAudio_somAddDynamicMethod SOMClass_somAddDynamicMethod
#define M_CWAudio_somOverrideSMethod SOMClass_somOverrideSMethod
#define M_CWAudio_somClassReady SOMClass_somClassReady
#define M_CWAudio_somGetApplyStub SOMClass_somGetApplyStub
#define M_CWAudio_somGetClassData SOMClass_somGetClassData
#define M_CWAudio_somSetClassData SOMClass_somSetClassData
#define M_CWAudio_somGetClassMtab SOMClass_somGetClassMtab
#define M_CWAudio_somGetInstanceOffset SOMClass_somGetInstanceOffset
#define M_CWAudio_somGetInstancePartSize SOMClass_somGetInstancePartSize
#define M_CWAudio_somGetInstanceSize SOMClass_somGetInstanceSize
#define M_CWAudio_somGetInstanceToken SOMClass_somGetInstanceToken
#define M_CWAudio_somGetMemberToken SOMClass_somGetMemberToken
#define M_CWAudio_somGetMethodData SOMClass_somGetMethodData
#define M_CWAudio_somGetRdStub SOMClass_somGetRdStub
#define M_CWAudio_somGetMethodDescriptor SOMClass_somGetMethodDescriptor
#define M_CWAudio_somGetMethodIndex SOMClass_somGetMethodIndex
#define M_CWAudio_somGetMethodToken SOMClass_somGetMethodToken
#define M_CWAudio_somGetName SOMClass_somGetName
#define M_CWAudio_somGetNthMethodData SOMClass_somGetNthMethodData
#define M_CWAudio_somGetNthMethodInfo SOMClass_somGetNthMethodInfo
#define M_CWAudio_somGetNumMethods SOMClass_somGetNumMethods
#define M_CWAudio_somGetNumStaticMethods SOMClass_somGetNumStaticMethods
#define M_CWAudio_somGetParent SOMClass_somGetParent
#define M_CWAudio_somGetParents SOMClass_somGetParents
#define M_CWAudio_somGetPClsMtab SOMClass_somGetPClsMtab
#define M_CWAudio_somGetPClsMtabs SOMClass_somGetPClsMtabs
#define M_CWAudio_somGetVersionNumbers SOMClass_somGetVersionNumbers
#define M_CWAudio_somSetMethodDescriptor SOMClass_somSetMethodDescriptor
#define M_CWAudio_somFindMethod SOMClass_somFindMethod
#define M_CWAudio_somFindMethodOk SOMClass_somFindMethodOk
#define M_CWAudio_somFindSMethod SOMClass_somFindSMethod
#define M_CWAudio_somFindSMethodOk SOMClass_somFindSMethodOk
#define M_CWAudio_somLookupMethod SOMClass_somLookupMethod
#define M_CWAudio_somCheckVersion SOMClass_somCheckVersion
#define M_CWAudio_somDescendedFrom SOMClass_somDescendedFrom
#define M_CWAudio_somSupportsMethod SOMClass_somSupportsMethod
#define M_CWAudio_somDefinedMethod SOMClass_somDefinedMethod
#define M_CWAudio_somOverrideMtab SOMClass_somOverrideMtab
#define M_CWAudio_somDefaultInit SOMObject_somDefaultInit
#define M_CWAudio_somDestruct SOMObject_somDestruct
#define M_CWAudio_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define M_CWAudio_somDefaultAssign SOMObject_somDefaultAssign
#define M_CWAudio_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define M_CWAudio_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define M_CWAudio_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define M_CWAudio_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define M_CWAudio_somDefaultVAssign SOMObject_somDefaultVAssign
#define M_CWAudio_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define M_CWAudio_somInit SOMObject_somInit
#define M_CWAudio_somFree SOMObject_somFree
#define M_CWAudio_somUninit SOMObject_somUninit
#define M_CWAudio_somGetClass SOMObject_somGetClass
#define M_CWAudio_somGetClassName SOMObject_somGetClassName
#define M_CWAudio_somGetSize SOMObject_somGetSize
#define M_CWAudio_somIsA SOMObject_somIsA
#define M_CWAudio_somIsInstanceOf SOMObject_somIsInstanceOf
#define M_CWAudio_somRespondsTo SOMObject_somRespondsTo
#define M_CWAudio_somDispatch SOMObject_somDispatch
#define M_CWAudio_somClassDispatch SOMObject_somClassDispatch
#define M_CWAudio_somCastObj SOMObject_somCastObj
#define M_CWAudio_somResetObj SOMObject_somResetObj
#define M_CWAudio_somDispatchV SOMObject_somDispatchV
#define M_CWAudio_somDispatchL SOMObject_somDispatchL
#define M_CWAudio_somDispatchA SOMObject_somDispatchA
#define M_CWAudio_somDispatchD SOMObject_somDispatchD
#define M_CWAudio_somPrintSelf SOMObject_somPrintSelf
#define M_CWAudio_somDumpSelf SOMObject_somDumpSelf
#define M_CWAudio_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_M_CWAudio_h */
