
/*
 * This file was generated by the SOM Compiler.
 * FileName: include\cwpcx.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.7
 *     SOM Emitter emith: 2.42
 */

/*
 * 
 * CWgif.idl:
 */


#ifndef SOM_CWPCX_h
#define SOM_CWPCX_h


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
#ifndef CWPCX
#define CWPCX SOMObject
#endif
#include <cwimage.h>

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
#ifndef M_WPImageFile
    #define M_WPImageFile SOMObject
#endif /* M_WPImageFile */
#ifndef M_CWImage
    #define M_CWImage SOMObject
#endif /* M_CWImage */
#ifndef M_CWPCX
    #define M_CWPCX SOMObject
#endif /* M_CWPCX */

/*
 * End of user-defined types.
 */

#define CWPCX_MajorVersion 1
#define CWPCX_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define CWPCXNewClass cwpcxc
#pragma linkage(cwpcxc, system)
#define CWPCXClassData cwpcxd
#define CWPCXCClassData cwpcxx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */

/*
 * Include the meta class file
 */
#include <cwpcx.h>
/*--------------Migration------------*/
#define CWPCX_classObj CWPCXClassData.classObject
#define _CWPCX CWPCX_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK CWPCXNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(CWPCXNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct CWPCXClassDataStructure {
	SOMClass *classObject;
} SOMDLINK CWPCXClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct CWPCXCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK CWPCXCClassData;

/*
 * New and Renew macros for CWPCX
 */
#define CWPCXNew() \
   (_somNew(_CWPCX ? _CWPCX : CWPCXNewClass(CWPCX_MajorVersion, CWPCX_MinorVersion)))
#define CWPCXRenew(buf) \
   (_somRenew(_CWPCX ? _CWPCX : CWPCXNewClass(CWPCX_MajorVersion, CWPCX_MinorVersion), buf))
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define CWPCX_cwmmAddImageInformationPage CWImage_cwmmAddImageInformationPage
#define CWPCX_wpQueryBitmapHandle WPImageFile_wpQueryBitmapHandle
#define CWPCX_wpQueryBitmapInfoHeader WPImageFile_wpQueryBitmapInfoHeader
#define CWPCX_wpQueryBitmapData WPImageFile_wpQueryBitmapData
#define CWPCX_wpSetBitmapData WPImageFile_wpSetBitmapData
#define CWPCX_wpReadImageFile WPImageFile_wpReadImageFile
#define CWPCX_wpWriteImageFile WPImageFile_wpWriteImageFile
#define CWPCX_wpAddFileTypePage WPDataFile_wpAddFileTypePage
#define CWPCX_wpPrintMetaFile WPDataFile_wpPrintMetaFile
#define CWPCX_wpPrintPifFile WPDataFile_wpPrintPifFile
#define CWPCX_wpPrintPrinterSpecificFile WPDataFile_wpPrintPrinterSpecificFile
#define CWPCX_wpPrintUnknownFile WPDataFile_wpPrintUnknownFile
#define CWPCX_wpQueryAssociatedProgram WPDataFile_wpQueryAssociatedProgram
#define CWPCX_wpSetAssociatedFileIcon WPDataFile_wpSetAssociatedFileIcon
#define CWPCX_wpQueryAssociatedFileIcon WPDataFile_wpQueryAssociatedFileIcon
#define CWPCX_wpSetRealName WPFileSystem_wpSetRealName
#define CWPCX_wpSetType WPFileSystem_wpSetType
#define CWPCX_wpQueryType WPFileSystem_wpQueryType
#define CWPCX_wpSetAttr WPFileSystem_wpSetAttr
#define CWPCX_wpQueryAttr WPFileSystem_wpQueryAttr
#define CWPCX_wpAddFile1Page WPFileSystem_wpAddFile1Page
#define CWPCX_wpAddFile2Page WPFileSystem_wpAddFile2Page
#define CWPCX_wpAddFile3Page WPFileSystem_wpAddFile3Page
#define CWPCX_wpAddFileMenuPage WPFileSystem_wpAddFileMenuPage
#define CWPCX_wpQueryCreation WPFileSystem_wpQueryCreation
#define CWPCX_wpQueryLastAccess WPFileSystem_wpQueryLastAccess
#define CWPCX_wpQueryLastWrite WPFileSystem_wpQueryLastWrite
#define CWPCX_wpQueryFileSize WPFileSystem_wpQueryFileSize
#define CWPCX_wpQueryEASize WPFileSystem_wpQueryEASize
#define CWPCX_wpSetDateInfo WPFileSystem_wpSetDateInfo
#define CWPCX_wpSetFileSizeInfo WPFileSystem_wpSetFileSizeInfo
#define CWPCX_wpRefresh WPFileSystem_wpRefresh
#define CWPCX_wpQueryRefreshFlags WPFileSystem_wpQueryRefreshFlags
#define CWPCX_wpSetRefreshFlags WPFileSystem_wpSetRefreshFlags
#define CWPCX_wpPrintPlainTextFile WPFileSystem_wpPrintPlainTextFile
#define CWPCX_wpSetTitleAndRenameFile WPFileSystem_wpSetTitleAndRenameFile
#define CWPCX_wpConfirmRenameFileWithExt WPFileSystem_wpConfirmRenameFileWithExt
#define CWPCX_wpVerifyUpdateAccess WPFileSystem_wpVerifyUpdateAccess
#define CWPCX_wpAddUserItemsToPopupMenu WPFileSystem_wpAddUserItemsToPopupMenu
#define CWPCX_wpIsDiskSwapped WPFileSystem_wpIsDiskSwapped
#define CWPCX_wpQueryRealName WPFileSystem_wpQueryRealName
#define CWPCX_wpQueryFilename WPFileSystem_wpQueryFilename
#define CWPCX_wpQueryDisk WPFileSystem_wpQueryDisk
#define CWPCX_wpQueryDateInfo WPFileSystem_wpQueryDateInfo
#define CWPCX_wpConfirmKeepAssoc WPFileSystem_wpConfirmKeepAssoc
#define CWPCX_wpAddObjectGeneralPage WPObject_wpAddObjectGeneralPage
#define CWPCX_wpAddObjectGeneralPage2 WPObject_wpAddObjectGeneralPage2
#define CWPCX_wpAddObjectWindowPage WPObject_wpAddObjectWindowPage
#define CWPCX_wpAddSettingsPages WPObject_wpAddSettingsPages
#define CWPCX_wpAddToObjUseList WPObject_wpAddToObjUseList
#define CWPCX_wpAllocMem WPObject_wpAllocMem
#define CWPCX_wpAppendObject WPObject_wpAppendObject
#define CWPCX_wpAssertObjectMutexSem WPObject_wpAssertObjectMutexSem
#define CWPCX_wpClose WPObject_wpClose
#define CWPCX_wpCnrInsertObject WPObject_wpCnrInsertObject
#define CWPCX_wpCnrRemoveObject WPObject_wpCnrRemoveObject
#define CWPCX_wpCnrSetEmphasis WPObject_wpCnrSetEmphasis
#define CWPCX_wpConfirmDelete WPObject_wpConfirmDelete
#define CWPCX_wpConfirmObjectTitle WPObject_wpConfirmObjectTitle
#define CWPCX_wpCopiedFromTemplate WPObject_wpCopiedFromTemplate
#define CWPCX_wpCopyObject WPObject_wpCopyObject
#define CWPCX_wpCreateAnother WPObject_wpCreateAnother
#define CWPCX_wpCreateFromTemplate WPObject_wpCreateFromTemplate
#define CWPCX_wpCreateShadowObject WPObject_wpCreateShadowObject
#define CWPCX_wpDelete WPObject_wpDelete
#define CWPCX_wpDeleteFromObjUseList WPObject_wpDeleteFromObjUseList
#define CWPCX_wpDisplayHelp WPObject_wpDisplayHelp
#define CWPCX_wpDisplayMenu WPObject_wpDisplayMenu
#define CWPCX_wpDoesObjectMatch WPObject_wpDoesObjectMatch
#define CWPCX_wpDraggedOverObject WPObject_wpDraggedOverObject
#define CWPCX_wpDragOver WPObject_wpDragOver
#define CWPCX_wpDrop WPObject_wpDrop
#define CWPCX_wpDroppedOnObject WPObject_wpDroppedOnObject
#define CWPCX_wpEndConversation WPObject_wpEndConversation
#define CWPCX_wpFilterPopupMenu WPObject_wpFilterPopupMenu
#define CWPCX_wpFindUseItem WPObject_wpFindUseItem
#define CWPCX_wpFormatDragItem WPObject_wpFormatDragItem
#define CWPCX_wpFree WPObject_wpFree
#define CWPCX_wpFreeMem WPObject_wpFreeMem
#define CWPCX_wpHide WPObject_wpHide
#define CWPCX_wpInitData WPObject_wpInitData
#define CWPCX_wpInsertPopupMenuItems WPObject_wpInsertPopupMenuItems
#define CWPCX_wpInsertSettingsPage WPObject_wpInsertSettingsPage
#define CWPCX_wpMenuItemHelpSelected WPObject_wpMenuItemHelpSelected
#define CWPCX_wpMenuItemSelected WPObject_wpMenuItemSelected
#define CWPCX_wpModifyPopupMenu WPObject_wpModifyPopupMenu
#define CWPCX_wpMoveObject WPObject_wpMoveObject
#define CWPCX_wpOpen WPObject_wpOpen
#define CWPCX_wpPrintObject WPObject_wpPrintObject
#define CWPCX_wpQueryConcurrentView WPObject_wpQueryConcurrentView
#define CWPCX_wpQueryButtonAppearance WPObject_wpQueryButtonAppearance
#define CWPCX_wpQueryConfirmations WPObject_wpQueryConfirmations
#define CWPCX_wpQueryDefaultHelp WPObject_wpQueryDefaultHelp
#define CWPCX_wpQueryDefaultView WPObject_wpQueryDefaultView
#define CWPCX_wpQueryDetailsData WPObject_wpQueryDetailsData
#define CWPCX_wpQueryError WPObject_wpQueryError
#define CWPCX_wpSetFolder WPObject_wpSetFolder
#define CWPCX_wpQueryFolder WPObject_wpQueryFolder
#define CWPCX_wpQueryIcon WPObject_wpQueryIcon
#define CWPCX_wpQueryIconData WPObject_wpQueryIconData
#define CWPCX_wpQueryMinWindow WPObject_wpQueryMinWindow
#define CWPCX_wpQueryNameClashOptions WPObject_wpQueryNameClashOptions
#define CWPCX_wpQueryStyle WPObject_wpQueryStyle
#define CWPCX_wpSetTaskRec WPObject_wpSetTaskRec
#define CWPCX_wpFindTaskRec WPObject_wpFindTaskRec
#define CWPCX_wpQueryTitle WPObject_wpQueryTitle
#define CWPCX_wpRegisterView WPObject_wpRegisterView
#define CWPCX_wpReleaseObjectMutexSem WPObject_wpReleaseObjectMutexSem
#define CWPCX_wpRender WPObject_wpRender
#define CWPCX_wpRenderComplete WPObject_wpRenderComplete
#define CWPCX_wpReplaceObject WPObject_wpReplaceObject
#define CWPCX_wpRequestObjectMutexSem WPObject_wpRequestObjectMutexSem
#define CWPCX_wpRestore WPObject_wpRestore
#define CWPCX_wpRestoreData WPObject_wpRestoreData
#define CWPCX_wpRestoreLong WPObject_wpRestoreLong
#define CWPCX_wpRestoreState WPObject_wpRestoreState
#define CWPCX_wpRestoreString WPObject_wpRestoreString
#define CWPCX_wpSaveData WPObject_wpSaveData
#define CWPCX_wpSaveImmediate WPObject_wpSaveImmediate
#define CWPCX_wpSaveDeferred WPObject_wpSaveDeferred
#define CWPCX_wpSaveLong WPObject_wpSaveLong
#define CWPCX_wpSaveState WPObject_wpSaveState
#define CWPCX_wpSaveString WPObject_wpSaveString
#define CWPCX_wpScanSetupString WPObject_wpScanSetupString
#define CWPCX_wpSetConcurrentView WPObject_wpSetConcurrentView
#define CWPCX_wpSetButtonAppearance WPObject_wpSetButtonAppearance
#define CWPCX_wpSetDefaultHelp WPObject_wpSetDefaultHelp
#define CWPCX_wpSetDefaultView WPObject_wpSetDefaultView
#define CWPCX_wpSetError WPObject_wpSetError
#define CWPCX_wpSetIconHandle WPObject_wpSetIconHandle
#define CWPCX_wpQueryScreenGroupID WPObject_wpQueryScreenGroupID
#define CWPCX_wpSetupOnce WPObject_wpSetupOnce
#define CWPCX_wpSetIcon WPObject_wpSetIcon
#define CWPCX_wpSetIconData WPObject_wpSetIconData
#define CWPCX_wpSetMinWindow WPObject_wpSetMinWindow
#define CWPCX_wpSetStyle WPObject_wpSetStyle
#define CWPCX_wpModifyStyle WPObject_wpModifyStyle
#define CWPCX_wpSetTitle WPObject_wpSetTitle
#define CWPCX_wpSetup WPObject_wpSetup
#define CWPCX_wpSwitchTo WPObject_wpSwitchTo
#define CWPCX_wpUnInitData WPObject_wpUnInitData
#define CWPCX_wpViewObject WPObject_wpViewObject
#define CWPCX_wpQueryTrueStyle WPObject_wpQueryTrueStyle
#define CWPCX_wpQueryHandle WPObject_wpQueryHandle
#define CWPCX_wpUnlockObject WPObject_wpUnlockObject
#define CWPCX_wpObjectReady WPObject_wpObjectReady
#define CWPCX_wpIsObjectInitialized WPObject_wpIsObjectInitialized
#define CWPCX_wpCreateShadowObjectExt WPObject_wpCreateShadowObjectExt
#define CWPCX_wpCnrDeleteUseItem WPObject_wpCnrDeleteUseItem
#define CWPCX_wpIsDeleteable WPObject_wpIsDeleteable
#define CWPCX_wpQueryCoreRecord WPObject_wpQueryCoreRecord
#define CWPCX_wpSetObjectID WPObject_wpSetObjectID
#define CWPCX_wpQueryObjectID WPObject_wpQueryObjectID
#define CWPCX_wpSetDefaultIconPos WPObject_wpSetDefaultIconPos
#define CWPCX_wpQueryDefaultIconPos WPObject_wpQueryDefaultIconPos
#define CWPCX_wpCnrRefreshDetails WPObject_wpCnrRefreshDetails
#define CWPCX_wpFindViewItem WPObject_wpFindViewItem
#define CWPCX_wpLockObject WPObject_wpLockObject
#define CWPCX_wpIsLocked WPObject_wpIsLocked
#define CWPCX_wpQueryContainerFlagPtr WPObject_wpQueryContainerFlagPtr
#define CWPCX_wpWaitForClose WPObject_wpWaitForClose
#define CWPCX_somDefaultInit SOMObject_somDefaultInit
#define CWPCX_somDestruct SOMObject_somDestruct
#define CWPCX_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define CWPCX_somDefaultAssign SOMObject_somDefaultAssign
#define CWPCX_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define CWPCX_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define CWPCX_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define CWPCX_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define CWPCX_somDefaultVAssign SOMObject_somDefaultVAssign
#define CWPCX_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define CWPCX_somInit SOMObject_somInit
#define CWPCX_somFree SOMObject_somFree
#define CWPCX_somUninit SOMObject_somUninit
#define CWPCX_somGetClass SOMObject_somGetClass
#define CWPCX_somGetClassName SOMObject_somGetClassName
#define CWPCX_somGetSize SOMObject_somGetSize
#define CWPCX_somIsA SOMObject_somIsA
#define CWPCX_somIsInstanceOf SOMObject_somIsInstanceOf
#define CWPCX_somRespondsTo SOMObject_somRespondsTo
#define CWPCX_somDispatch SOMObject_somDispatch
#define CWPCX_somClassDispatch SOMObject_somClassDispatch
#define CWPCX_somCastObj SOMObject_somCastObj
#define CWPCX_somResetObj SOMObject_somResetObj
#define CWPCX_somDispatchV SOMObject_somDispatchV
#define CWPCX_somDispatchL SOMObject_somDispatchL
#define CWPCX_somDispatchA SOMObject_somDispatchA
#define CWPCX_somDispatchD SOMObject_somDispatchD
#define CWPCX_somPrintSelf SOMObject_somPrintSelf
#define CWPCX_somDumpSelf SOMObject_somDumpSelf
#define CWPCX_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_CWPCX_h */

#ifndef SOM_M_CWPCX_h
#define SOM_M_CWPCX_h


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
#ifndef M_CWPCX
#define M_CWPCX SOMObject
#endif
#include <cwimage.h>

/*
 * Start of user-defined types:
 */

/*
 * End of user-defined types.
 */

#define M_CWPCX_MajorVersion 1
#define M_CWPCX_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define M_CWPCXNewClass cwpcxMc
#pragma linkage(cwpcxMc, system)
#define M_CWPCXClassData cwpcxMd
#define M_CWPCXCClassData cwpcxMx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define M_CWPCX_classObj M_CWPCXClassData.classObject
#define _M_CWPCX M_CWPCX_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK M_CWPCXNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(M_CWPCXNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct M_CWPCXClassDataStructure {
	SOMClass *classObject;
} SOMDLINK M_CWPCXClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct M_CWPCXCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK M_CWPCXCClassData;

/*
 * New and Renew macros for M_CWPCX
 */
#define M_CWPCXNew() \
   (_somNew(_M_CWPCX ? _M_CWPCX : M_CWPCXNewClass(M_CWPCX_MajorVersion, M_CWPCX_MinorVersion)))
#define M_CWPCXRenew(buf) \
   (_somRenew(_M_CWPCX ? _M_CWPCX : M_CWPCXNewClass(M_CWPCX_MajorVersion, M_CWPCX_MinorVersion), buf))

/*
 * Override method: wpclsQueryInstanceType
 */
#define M_CWPCX_wpclsQueryInstanceType(somSelf) \
	M_WPFileSystem_wpclsQueryInstanceType(somSelf)

/*
 * Override method: wpclsQueryInstanceFilter
 */
#define M_CWPCX_wpclsQueryInstanceFilter(somSelf) \
	M_WPFileSystem_wpclsQueryInstanceFilter(somSelf)

/*
 * Override method: wpclsQueryDefaultHelp
 */
#define M_CWPCX_wpclsQueryDefaultHelp(somSelf,pHelpPanelId,pszHelpLibrary) \
	M_WPObject_wpclsQueryDefaultHelp(somSelf,pHelpPanelId,pszHelpLibrary)

/*
 * Override method: wpclsQueryStyle
 */
#define M_CWPCX_wpclsQueryStyle(somSelf) \
	M_WPObject_wpclsQueryStyle(somSelf)

/*
 * Override method: wpclsQueryTitle
 */
#define M_CWPCX_wpclsQueryTitle(somSelf) \
	M_WPObject_wpclsQueryTitle(somSelf)

/*
 * Override method: wpclsQueryIconData
 */
#define M_CWPCX_wpclsQueryIconData(somSelf,pIconInfo) \
	M_WPObject_wpclsQueryIconData(somSelf,pIconInfo)
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define M_CWPCX_wpclsQueryAwakeObject M_WPFileSystem_wpclsQueryAwakeObject
#define M_CWPCX_wpclsFileSysExists M_WPFileSystem_wpclsFileSysExists
#define M_CWPCX_wpclsQueryObjectFromPath M_WPFileSystem_wpclsQueryObjectFromPath
#define M_CWPCX_wpclsCreateDefaultTemplates M_WPObject_wpclsCreateDefaultTemplates
#define M_CWPCX_wpclsInitData M_WPObject_wpclsInitData
#define M_CWPCX_wpclsMakeAwake M_WPObject_wpclsMakeAwake
#define M_CWPCX_wpclsNew M_WPObject_wpclsNew
#define M_CWPCX_wpclsQueryDefaultView M_WPObject_wpclsQueryDefaultView
#define M_CWPCX_wpclsQueryDetails M_WPObject_wpclsQueryDetails
#define M_CWPCX_wpclsQueryDetailsInfo M_WPObject_wpclsQueryDetailsInfo
#define M_CWPCX_wpclsQueryExtendedCriteria M_WPObject_wpclsQueryExtendedCriteria
#define M_CWPCX_wpclsQueryFolder M_WPObject_wpclsQueryFolder
#define M_CWPCX_wpclsQueryIcon M_WPObject_wpclsQueryIcon
#define M_CWPCX_wpclsQuerySearchInfo M_WPObject_wpclsQuerySearchInfo
#define M_CWPCX_wpclsUnInitData M_WPObject_wpclsUnInitData
#define M_CWPCX_wpclsFindObjectFirst M_WPObject_wpclsFindObjectFirst
#define M_CWPCX_wpclsFindObjectNext M_WPObject_wpclsFindObjectNext
#define M_CWPCX_wpclsFindObjectEnd M_WPObject_wpclsFindObjectEnd
#define M_CWPCX_wpclsSetError M_WPObject_wpclsSetError
#define M_CWPCX_wpclsQueryError M_WPObject_wpclsQueryError
#define M_CWPCX_wpclsQuerySettingsPageSize M_WPObject_wpclsQuerySettingsPageSize
#define M_CWPCX_wpclsQueryObject M_WPObject_wpclsQueryObject
#define M_CWPCX_wpclsQueryButtonAppearance M_WPObject_wpclsQueryButtonAppearance
#define M_CWPCX_wpclsDecUsage M_WPObject_wpclsDecUsage
#define M_CWPCX_wpclsIncUsage M_WPObject_wpclsIncUsage
#define M_CWPCX_wpclsInsertMultipleObjects M_WPObject_wpclsInsertMultipleObjects
#define M_CWPCX_wpclsFindOneObject M_WPObject_wpclsFindOneObject
#define M_CWPCX_wpclsSetSettingsPageSize M_WPObject_wpclsSetSettingsPageSize
#define M_CWPCX_wpclsSetIconData M_WPObject_wpclsSetIconData
#define M_CWPCX_wpclsSetIcon M_WPObject_wpclsSetIcon
#define M_CWPCX_wpclsObjectFromHandle M_WPObject_wpclsObjectFromHandle
#define M_CWPCX_wpclsRemoveObjects M_WPObject_wpclsRemoveObjects
#define M_CWPCX__get_somDataAlignment SOMClass__get_somDataAlignment
#define M_CWPCX__get_somInstanceDataOffsets SOMClass__get_somInstanceDataOffsets
#define M_CWPCX__get_somDirectInitClasses SOMClass__get_somDirectInitClasses
#define M_CWPCX__set_somDirectInitClasses SOMClass__set_somDirectInitClasses
#define M_CWPCX_somNew SOMClass_somNew
#define M_CWPCX_somNewNoInit SOMClass_somNewNoInit
#define M_CWPCX_somRenew SOMClass_somRenew
#define M_CWPCX_somRenewNoInit SOMClass_somRenewNoInit
#define M_CWPCX_somRenewNoZero SOMClass_somRenewNoZero
#define M_CWPCX_somRenewNoInitNoZero SOMClass_somRenewNoInitNoZero
#define M_CWPCX_somAllocate SOMClass_somAllocate
#define M_CWPCX_somDeallocate SOMClass_somDeallocate
#define M_CWPCX__get_somClassAllocate SOMClass__get_somClassAllocate
#define M_CWPCX__get_somClassDeallocate SOMClass__get_somClassDeallocate
#define M_CWPCX_somGetInstanceInitMask SOMClass_somGetInstanceInitMask
#define M_CWPCX_somGetInstanceDestructionMask SOMClass_somGetInstanceDestructionMask
#define M_CWPCX_somGetInstanceAssignmentMask SOMClass_somGetInstanceAssignmentMask
#define M_CWPCX_somInitClass SOMClass_somInitClass
#define M_CWPCX_somInitMIClass SOMClass_somInitMIClass
#define M_CWPCX_somAddStaticMethod SOMClass_somAddStaticMethod
#define M_CWPCX_somAddDynamicMethod SOMClass_somAddDynamicMethod
#define M_CWPCX_somOverrideSMethod SOMClass_somOverrideSMethod
#define M_CWPCX_somClassReady SOMClass_somClassReady
#define M_CWPCX_somGetApplyStub SOMClass_somGetApplyStub
#define M_CWPCX_somGetClassData SOMClass_somGetClassData
#define M_CWPCX_somSetClassData SOMClass_somSetClassData
#define M_CWPCX_somGetClassMtab SOMClass_somGetClassMtab
#define M_CWPCX_somGetInstanceOffset SOMClass_somGetInstanceOffset
#define M_CWPCX_somGetInstancePartSize SOMClass_somGetInstancePartSize
#define M_CWPCX_somGetInstanceSize SOMClass_somGetInstanceSize
#define M_CWPCX_somGetInstanceToken SOMClass_somGetInstanceToken
#define M_CWPCX_somGetMemberToken SOMClass_somGetMemberToken
#define M_CWPCX_somGetMethodData SOMClass_somGetMethodData
#define M_CWPCX_somGetRdStub SOMClass_somGetRdStub
#define M_CWPCX_somGetMethodDescriptor SOMClass_somGetMethodDescriptor
#define M_CWPCX_somGetMethodIndex SOMClass_somGetMethodIndex
#define M_CWPCX_somGetMethodToken SOMClass_somGetMethodToken
#define M_CWPCX_somGetName SOMClass_somGetName
#define M_CWPCX_somGetNthMethodData SOMClass_somGetNthMethodData
#define M_CWPCX_somGetNthMethodInfo SOMClass_somGetNthMethodInfo
#define M_CWPCX_somGetNumMethods SOMClass_somGetNumMethods
#define M_CWPCX_somGetNumStaticMethods SOMClass_somGetNumStaticMethods
#define M_CWPCX_somGetParent SOMClass_somGetParent
#define M_CWPCX_somGetParents SOMClass_somGetParents
#define M_CWPCX_somGetPClsMtab SOMClass_somGetPClsMtab
#define M_CWPCX_somGetPClsMtabs SOMClass_somGetPClsMtabs
#define M_CWPCX_somGetVersionNumbers SOMClass_somGetVersionNumbers
#define M_CWPCX_somSetMethodDescriptor SOMClass_somSetMethodDescriptor
#define M_CWPCX_somFindMethod SOMClass_somFindMethod
#define M_CWPCX_somFindMethodOk SOMClass_somFindMethodOk
#define M_CWPCX_somFindSMethod SOMClass_somFindSMethod
#define M_CWPCX_somFindSMethodOk SOMClass_somFindSMethodOk
#define M_CWPCX_somLookupMethod SOMClass_somLookupMethod
#define M_CWPCX_somCheckVersion SOMClass_somCheckVersion
#define M_CWPCX_somDescendedFrom SOMClass_somDescendedFrom
#define M_CWPCX_somSupportsMethod SOMClass_somSupportsMethod
#define M_CWPCX_somDefinedMethod SOMClass_somDefinedMethod
#define M_CWPCX_somOverrideMtab SOMClass_somOverrideMtab
#define M_CWPCX_somDefaultInit SOMObject_somDefaultInit
#define M_CWPCX_somDestruct SOMObject_somDestruct
#define M_CWPCX_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define M_CWPCX_somDefaultAssign SOMObject_somDefaultAssign
#define M_CWPCX_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define M_CWPCX_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define M_CWPCX_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define M_CWPCX_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define M_CWPCX_somDefaultVAssign SOMObject_somDefaultVAssign
#define M_CWPCX_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define M_CWPCX_somInit SOMObject_somInit
#define M_CWPCX_somFree SOMObject_somFree
#define M_CWPCX_somUninit SOMObject_somUninit
#define M_CWPCX_somGetClass SOMObject_somGetClass
#define M_CWPCX_somGetClassName SOMObject_somGetClassName
#define M_CWPCX_somGetSize SOMObject_somGetSize
#define M_CWPCX_somIsA SOMObject_somIsA
#define M_CWPCX_somIsInstanceOf SOMObject_somIsInstanceOf
#define M_CWPCX_somRespondsTo SOMObject_somRespondsTo
#define M_CWPCX_somDispatch SOMObject_somDispatch
#define M_CWPCX_somClassDispatch SOMObject_somClassDispatch
#define M_CWPCX_somCastObj SOMObject_somCastObj
#define M_CWPCX_somResetObj SOMObject_somResetObj
#define M_CWPCX_somDispatchV SOMObject_somDispatchV
#define M_CWPCX_somDispatchL SOMObject_somDispatchL
#define M_CWPCX_somDispatchA SOMObject_somDispatchA
#define M_CWPCX_somDispatchD SOMObject_somDispatchD
#define M_CWPCX_somPrintSelf SOMObject_somPrintSelf
#define M_CWPCX_somDumpSelf SOMObject_somDumpSelf
#define M_CWPCX_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_M_CWPCX_h */
