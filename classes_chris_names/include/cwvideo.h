
/*
 * This file was generated by the SOM Compiler.
 * FileName: include\cwvideo.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.7
 *     SOM Emitter emith: 2.42
 */

/*
 * 
 * CWAudio.idl:
 */


#ifndef SOM_CWVideo_h
#define SOM_CWVideo_h


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
#ifndef CWVideo
#define CWVideo SOMObject
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
#ifndef M_CWVideo
    #define M_CWVideo SOMObject
#endif /* M_CWVideo */

/*
 * End of user-defined types.
 */

/*
 * Passthru lines: File: "C.h", "after"
 */

#include "cwmmres.h"


#define CWVideo_MajorVersion 1
#define CWVideo_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define CWVideoNewClass cwvideoc
#pragma linkage(cwvideoc, system)
#define CWVideoClassData cwvideod
#define CWVideoCClassData cwvideox
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */

/*
 * Include the meta class file
 */
#include <cwvideo.h>
/*--------------Migration------------*/
#define CWVideo_classObj CWVideoClassData.classObject
#define _CWVideo CWVideo_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK CWVideoNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(CWVideoNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct CWVideoClassDataStructure {
	SOMClass *classObject;
} SOMDLINK CWVideoClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct CWVideoCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK CWVideoCClassData;

/*
 * New and Renew macros for CWVideo
 */
#define CWVideoNew() \
   (_somNew(_CWVideo ? _CWVideo : CWVideoNewClass(CWVideo_MajorVersion, CWVideo_MinorVersion)))
#define CWVideoRenew(buf) \
   (_somRenew(_CWVideo ? _CWVideo : CWVideoNewClass(CWVideo_MajorVersion, CWVideo_MinorVersion), buf))

/*
 * Override method: wpModifyPopupMenu
 */
#define CWVideo_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition) \
	WPObject_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition)

/*
 * Override method: wpMenuItemSelected
 */
#define CWVideo_wpMenuItemSelected(somSelf,hwndFrame,ulMenuId) \
	WPObject_wpMenuItemSelected(somSelf,hwndFrame,ulMenuId)

/*
 * Override method: wpOpen
 */
#define CWVideo_wpOpen(somSelf,hwndCnr,ulView,param) \
	WPObject_wpOpen(somSelf,hwndCnr,ulView,param)
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define CWVideo_wpAddFileTypePage WPDataFile_wpAddFileTypePage
#define CWVideo_wpPrintMetaFile WPDataFile_wpPrintMetaFile
#define CWVideo_wpPrintPifFile WPDataFile_wpPrintPifFile
#define CWVideo_wpPrintPrinterSpecificFile WPDataFile_wpPrintPrinterSpecificFile
#define CWVideo_wpPrintUnknownFile WPDataFile_wpPrintUnknownFile
#define CWVideo_wpQueryAssociatedProgram WPDataFile_wpQueryAssociatedProgram
#define CWVideo_wpSetAssociatedFileIcon WPDataFile_wpSetAssociatedFileIcon
#define CWVideo_wpQueryAssociatedFileIcon WPDataFile_wpQueryAssociatedFileIcon
#define CWVideo_wpSetRealName WPFileSystem_wpSetRealName
#define CWVideo_wpSetType WPFileSystem_wpSetType
#define CWVideo_wpQueryType WPFileSystem_wpQueryType
#define CWVideo_wpSetAttr WPFileSystem_wpSetAttr
#define CWVideo_wpQueryAttr WPFileSystem_wpQueryAttr
#define CWVideo_wpAddFile1Page WPFileSystem_wpAddFile1Page
#define CWVideo_wpAddFile2Page WPFileSystem_wpAddFile2Page
#define CWVideo_wpAddFile3Page WPFileSystem_wpAddFile3Page
#define CWVideo_wpAddFileMenuPage WPFileSystem_wpAddFileMenuPage
#define CWVideo_wpQueryCreation WPFileSystem_wpQueryCreation
#define CWVideo_wpQueryLastAccess WPFileSystem_wpQueryLastAccess
#define CWVideo_wpQueryLastWrite WPFileSystem_wpQueryLastWrite
#define CWVideo_wpQueryFileSize WPFileSystem_wpQueryFileSize
#define CWVideo_wpQueryEASize WPFileSystem_wpQueryEASize
#define CWVideo_wpSetDateInfo WPFileSystem_wpSetDateInfo
#define CWVideo_wpSetFileSizeInfo WPFileSystem_wpSetFileSizeInfo
#define CWVideo_wpRefresh WPFileSystem_wpRefresh
#define CWVideo_wpQueryRefreshFlags WPFileSystem_wpQueryRefreshFlags
#define CWVideo_wpSetRefreshFlags WPFileSystem_wpSetRefreshFlags
#define CWVideo_wpPrintPlainTextFile WPFileSystem_wpPrintPlainTextFile
#define CWVideo_wpSetTitleAndRenameFile WPFileSystem_wpSetTitleAndRenameFile
#define CWVideo_wpConfirmRenameFileWithExt WPFileSystem_wpConfirmRenameFileWithExt
#define CWVideo_wpVerifyUpdateAccess WPFileSystem_wpVerifyUpdateAccess
#define CWVideo_wpAddUserItemsToPopupMenu WPFileSystem_wpAddUserItemsToPopupMenu
#define CWVideo_wpIsDiskSwapped WPFileSystem_wpIsDiskSwapped
#define CWVideo_wpQueryRealName WPFileSystem_wpQueryRealName
#define CWVideo_wpQueryFilename WPFileSystem_wpQueryFilename
#define CWVideo_wpQueryDisk WPFileSystem_wpQueryDisk
#define CWVideo_wpQueryDateInfo WPFileSystem_wpQueryDateInfo
#define CWVideo_wpConfirmKeepAssoc WPFileSystem_wpConfirmKeepAssoc
#define CWVideo_wpAddObjectGeneralPage WPObject_wpAddObjectGeneralPage
#define CWVideo_wpAddObjectGeneralPage2 WPObject_wpAddObjectGeneralPage2
#define CWVideo_wpAddObjectWindowPage WPObject_wpAddObjectWindowPage
#define CWVideo_wpAddSettingsPages WPObject_wpAddSettingsPages
#define CWVideo_wpAddToObjUseList WPObject_wpAddToObjUseList
#define CWVideo_wpAllocMem WPObject_wpAllocMem
#define CWVideo_wpAppendObject WPObject_wpAppendObject
#define CWVideo_wpAssertObjectMutexSem WPObject_wpAssertObjectMutexSem
#define CWVideo_wpClose WPObject_wpClose
#define CWVideo_wpCnrInsertObject WPObject_wpCnrInsertObject
#define CWVideo_wpCnrRemoveObject WPObject_wpCnrRemoveObject
#define CWVideo_wpCnrSetEmphasis WPObject_wpCnrSetEmphasis
#define CWVideo_wpConfirmDelete WPObject_wpConfirmDelete
#define CWVideo_wpConfirmObjectTitle WPObject_wpConfirmObjectTitle
#define CWVideo_wpCopiedFromTemplate WPObject_wpCopiedFromTemplate
#define CWVideo_wpCopyObject WPObject_wpCopyObject
#define CWVideo_wpCreateAnother WPObject_wpCreateAnother
#define CWVideo_wpCreateFromTemplate WPObject_wpCreateFromTemplate
#define CWVideo_wpCreateShadowObject WPObject_wpCreateShadowObject
#define CWVideo_wpDelete WPObject_wpDelete
#define CWVideo_wpDeleteFromObjUseList WPObject_wpDeleteFromObjUseList
#define CWVideo_wpDisplayHelp WPObject_wpDisplayHelp
#define CWVideo_wpDisplayMenu WPObject_wpDisplayMenu
#define CWVideo_wpDoesObjectMatch WPObject_wpDoesObjectMatch
#define CWVideo_wpDraggedOverObject WPObject_wpDraggedOverObject
#define CWVideo_wpDragOver WPObject_wpDragOver
#define CWVideo_wpDrop WPObject_wpDrop
#define CWVideo_wpDroppedOnObject WPObject_wpDroppedOnObject
#define CWVideo_wpEndConversation WPObject_wpEndConversation
#define CWVideo_wpFilterPopupMenu WPObject_wpFilterPopupMenu
#define CWVideo_wpFindUseItem WPObject_wpFindUseItem
#define CWVideo_wpFormatDragItem WPObject_wpFormatDragItem
#define CWVideo_wpFree WPObject_wpFree
#define CWVideo_wpFreeMem WPObject_wpFreeMem
#define CWVideo_wpHide WPObject_wpHide
#define CWVideo_wpInitData WPObject_wpInitData
#define CWVideo_wpInsertPopupMenuItems WPObject_wpInsertPopupMenuItems
#define CWVideo_wpInsertSettingsPage WPObject_wpInsertSettingsPage
#define CWVideo_wpMenuItemHelpSelected WPObject_wpMenuItemHelpSelected
#define CWVideo_wpMoveObject WPObject_wpMoveObject
#define CWVideo_wpPrintObject WPObject_wpPrintObject
#define CWVideo_wpQueryConcurrentView WPObject_wpQueryConcurrentView
#define CWVideo_wpQueryButtonAppearance WPObject_wpQueryButtonAppearance
#define CWVideo_wpQueryConfirmations WPObject_wpQueryConfirmations
#define CWVideo_wpQueryDefaultHelp WPObject_wpQueryDefaultHelp
#define CWVideo_wpQueryDefaultView WPObject_wpQueryDefaultView
#define CWVideo_wpQueryDetailsData WPObject_wpQueryDetailsData
#define CWVideo_wpQueryError WPObject_wpQueryError
#define CWVideo_wpSetFolder WPObject_wpSetFolder
#define CWVideo_wpQueryFolder WPObject_wpQueryFolder
#define CWVideo_wpQueryIcon WPObject_wpQueryIcon
#define CWVideo_wpQueryIconData WPObject_wpQueryIconData
#define CWVideo_wpQueryMinWindow WPObject_wpQueryMinWindow
#define CWVideo_wpQueryNameClashOptions WPObject_wpQueryNameClashOptions
#define CWVideo_wpQueryStyle WPObject_wpQueryStyle
#define CWVideo_wpSetTaskRec WPObject_wpSetTaskRec
#define CWVideo_wpFindTaskRec WPObject_wpFindTaskRec
#define CWVideo_wpQueryTitle WPObject_wpQueryTitle
#define CWVideo_wpRegisterView WPObject_wpRegisterView
#define CWVideo_wpReleaseObjectMutexSem WPObject_wpReleaseObjectMutexSem
#define CWVideo_wpRender WPObject_wpRender
#define CWVideo_wpRenderComplete WPObject_wpRenderComplete
#define CWVideo_wpReplaceObject WPObject_wpReplaceObject
#define CWVideo_wpRequestObjectMutexSem WPObject_wpRequestObjectMutexSem
#define CWVideo_wpRestore WPObject_wpRestore
#define CWVideo_wpRestoreData WPObject_wpRestoreData
#define CWVideo_wpRestoreLong WPObject_wpRestoreLong
#define CWVideo_wpRestoreState WPObject_wpRestoreState
#define CWVideo_wpRestoreString WPObject_wpRestoreString
#define CWVideo_wpSaveData WPObject_wpSaveData
#define CWVideo_wpSaveImmediate WPObject_wpSaveImmediate
#define CWVideo_wpSaveDeferred WPObject_wpSaveDeferred
#define CWVideo_wpSaveLong WPObject_wpSaveLong
#define CWVideo_wpSaveState WPObject_wpSaveState
#define CWVideo_wpSaveString WPObject_wpSaveString
#define CWVideo_wpScanSetupString WPObject_wpScanSetupString
#define CWVideo_wpSetConcurrentView WPObject_wpSetConcurrentView
#define CWVideo_wpSetButtonAppearance WPObject_wpSetButtonAppearance
#define CWVideo_wpSetDefaultHelp WPObject_wpSetDefaultHelp
#define CWVideo_wpSetDefaultView WPObject_wpSetDefaultView
#define CWVideo_wpSetError WPObject_wpSetError
#define CWVideo_wpSetIconHandle WPObject_wpSetIconHandle
#define CWVideo_wpQueryScreenGroupID WPObject_wpQueryScreenGroupID
#define CWVideo_wpSetupOnce WPObject_wpSetupOnce
#define CWVideo_wpSetIcon WPObject_wpSetIcon
#define CWVideo_wpSetIconData WPObject_wpSetIconData
#define CWVideo_wpSetMinWindow WPObject_wpSetMinWindow
#define CWVideo_wpSetStyle WPObject_wpSetStyle
#define CWVideo_wpModifyStyle WPObject_wpModifyStyle
#define CWVideo_wpSetTitle WPObject_wpSetTitle
#define CWVideo_wpSetup WPObject_wpSetup
#define CWVideo_wpSwitchTo WPObject_wpSwitchTo
#define CWVideo_wpUnInitData WPObject_wpUnInitData
#define CWVideo_wpViewObject WPObject_wpViewObject
#define CWVideo_wpQueryTrueStyle WPObject_wpQueryTrueStyle
#define CWVideo_wpQueryHandle WPObject_wpQueryHandle
#define CWVideo_wpUnlockObject WPObject_wpUnlockObject
#define CWVideo_wpObjectReady WPObject_wpObjectReady
#define CWVideo_wpIsObjectInitialized WPObject_wpIsObjectInitialized
#define CWVideo_wpCreateShadowObjectExt WPObject_wpCreateShadowObjectExt
#define CWVideo_wpCnrDeleteUseItem WPObject_wpCnrDeleteUseItem
#define CWVideo_wpIsDeleteable WPObject_wpIsDeleteable
#define CWVideo_wpQueryCoreRecord WPObject_wpQueryCoreRecord
#define CWVideo_wpSetObjectID WPObject_wpSetObjectID
#define CWVideo_wpQueryObjectID WPObject_wpQueryObjectID
#define CWVideo_wpSetDefaultIconPos WPObject_wpSetDefaultIconPos
#define CWVideo_wpQueryDefaultIconPos WPObject_wpQueryDefaultIconPos
#define CWVideo_wpCnrRefreshDetails WPObject_wpCnrRefreshDetails
#define CWVideo_wpFindViewItem WPObject_wpFindViewItem
#define CWVideo_wpLockObject WPObject_wpLockObject
#define CWVideo_wpIsLocked WPObject_wpIsLocked
#define CWVideo_wpQueryContainerFlagPtr WPObject_wpQueryContainerFlagPtr
#define CWVideo_wpWaitForClose WPObject_wpWaitForClose
#define CWVideo_somDefaultInit SOMObject_somDefaultInit
#define CWVideo_somDestruct SOMObject_somDestruct
#define CWVideo_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define CWVideo_somDefaultAssign SOMObject_somDefaultAssign
#define CWVideo_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define CWVideo_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define CWVideo_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define CWVideo_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define CWVideo_somDefaultVAssign SOMObject_somDefaultVAssign
#define CWVideo_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define CWVideo_somInit SOMObject_somInit
#define CWVideo_somFree SOMObject_somFree
#define CWVideo_somUninit SOMObject_somUninit
#define CWVideo_somGetClass SOMObject_somGetClass
#define CWVideo_somGetClassName SOMObject_somGetClassName
#define CWVideo_somGetSize SOMObject_somGetSize
#define CWVideo_somIsA SOMObject_somIsA
#define CWVideo_somIsInstanceOf SOMObject_somIsInstanceOf
#define CWVideo_somRespondsTo SOMObject_somRespondsTo
#define CWVideo_somDispatch SOMObject_somDispatch
#define CWVideo_somClassDispatch SOMObject_somClassDispatch
#define CWVideo_somCastObj SOMObject_somCastObj
#define CWVideo_somResetObj SOMObject_somResetObj
#define CWVideo_somDispatchV SOMObject_somDispatchV
#define CWVideo_somDispatchL SOMObject_somDispatchL
#define CWVideo_somDispatchA SOMObject_somDispatchA
#define CWVideo_somDispatchD SOMObject_somDispatchD
#define CWVideo_somPrintSelf SOMObject_somPrintSelf
#define CWVideo_somDumpSelf SOMObject_somDumpSelf
#define CWVideo_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_CWVideo_h */

#ifndef SOM_M_CWVideo_h
#define SOM_M_CWVideo_h


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
#ifndef M_CWVideo
#define M_CWVideo SOMObject
#endif
#include <cwmmdataf.h>

/*
 * Start of user-defined types:
 */

/*
 * End of user-defined types.
 */

#define M_CWVideo_MajorVersion 1
#define M_CWVideo_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define M_CWVideoNewClass cwvideoMc
#pragma linkage(cwvideoMc, system)
#define M_CWVideoClassData cwvideoMd
#define M_CWVideoCClassData cwvideoMx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define M_CWVideo_classObj M_CWVideoClassData.classObject
#define _M_CWVideo M_CWVideo_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK M_CWVideoNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(M_CWVideoNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct M_CWVideoClassDataStructure {
	SOMClass *classObject;
} SOMDLINK M_CWVideoClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct M_CWVideoCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK M_CWVideoCClassData;

/*
 * New and Renew macros for M_CWVideo
 */
#define M_CWVideoNew() \
   (_somNew(_M_CWVideo ? _M_CWVideo : M_CWVideoNewClass(M_CWVideo_MajorVersion, M_CWVideo_MinorVersion)))
#define M_CWVideoRenew(buf) \
   (_somRenew(_M_CWVideo ? _M_CWVideo : M_CWVideoNewClass(M_CWVideo_MajorVersion, M_CWVideo_MinorVersion), buf))

/*
 * Override method: wpclsCreateDefaultTemplates
 */
#define M_CWVideo_wpclsCreateDefaultTemplates(somSelf,Folder) \
	M_WPObject_wpclsCreateDefaultTemplates(somSelf,Folder)

/*
 * Override method: wpclsQueryTitle
 */
#define M_CWVideo_wpclsQueryTitle(somSelf) \
	M_WPObject_wpclsQueryTitle(somSelf)

/*
 * Override method: wpclsQueryDefaultView
 */
#define M_CWVideo_wpclsQueryDefaultView(somSelf) \
	M_WPObject_wpclsQueryDefaultView(somSelf)

/*
 * Override method: wpclsQueryIconData
 */
#define M_CWVideo_wpclsQueryIconData(somSelf,pIconInfo) \
	M_WPObject_wpclsQueryIconData(somSelf,pIconInfo)
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define M_CWVideo_wpclsQueryAwakeObject M_WPFileSystem_wpclsQueryAwakeObject
#define M_CWVideo_wpclsFileSysExists M_WPFileSystem_wpclsFileSysExists
#define M_CWVideo_wpclsQueryObjectFromPath M_WPFileSystem_wpclsQueryObjectFromPath
#define M_CWVideo_wpclsQueryInstanceType M_WPFileSystem_wpclsQueryInstanceType
#define M_CWVideo_wpclsQueryInstanceFilter M_WPFileSystem_wpclsQueryInstanceFilter
#define M_CWVideo_wpclsInitData M_WPObject_wpclsInitData
#define M_CWVideo_wpclsMakeAwake M_WPObject_wpclsMakeAwake
#define M_CWVideo_wpclsNew M_WPObject_wpclsNew
#define M_CWVideo_wpclsQueryDefaultHelp M_WPObject_wpclsQueryDefaultHelp
#define M_CWVideo_wpclsQueryDetails M_WPObject_wpclsQueryDetails
#define M_CWVideo_wpclsQueryDetailsInfo M_WPObject_wpclsQueryDetailsInfo
#define M_CWVideo_wpclsQueryExtendedCriteria M_WPObject_wpclsQueryExtendedCriteria
#define M_CWVideo_wpclsQueryFolder M_WPObject_wpclsQueryFolder
#define M_CWVideo_wpclsQueryIcon M_WPObject_wpclsQueryIcon
#define M_CWVideo_wpclsQuerySearchInfo M_WPObject_wpclsQuerySearchInfo
#define M_CWVideo_wpclsQueryStyle M_WPObject_wpclsQueryStyle
#define M_CWVideo_wpclsUnInitData M_WPObject_wpclsUnInitData
#define M_CWVideo_wpclsFindObjectFirst M_WPObject_wpclsFindObjectFirst
#define M_CWVideo_wpclsFindObjectNext M_WPObject_wpclsFindObjectNext
#define M_CWVideo_wpclsFindObjectEnd M_WPObject_wpclsFindObjectEnd
#define M_CWVideo_wpclsSetError M_WPObject_wpclsSetError
#define M_CWVideo_wpclsQueryError M_WPObject_wpclsQueryError
#define M_CWVideo_wpclsQuerySettingsPageSize M_WPObject_wpclsQuerySettingsPageSize
#define M_CWVideo_wpclsQueryObject M_WPObject_wpclsQueryObject
#define M_CWVideo_wpclsQueryButtonAppearance M_WPObject_wpclsQueryButtonAppearance
#define M_CWVideo_wpclsDecUsage M_WPObject_wpclsDecUsage
#define M_CWVideo_wpclsIncUsage M_WPObject_wpclsIncUsage
#define M_CWVideo_wpclsInsertMultipleObjects M_WPObject_wpclsInsertMultipleObjects
#define M_CWVideo_wpclsFindOneObject M_WPObject_wpclsFindOneObject
#define M_CWVideo_wpclsSetSettingsPageSize M_WPObject_wpclsSetSettingsPageSize
#define M_CWVideo_wpclsSetIconData M_WPObject_wpclsSetIconData
#define M_CWVideo_wpclsSetIcon M_WPObject_wpclsSetIcon
#define M_CWVideo_wpclsObjectFromHandle M_WPObject_wpclsObjectFromHandle
#define M_CWVideo_wpclsRemoveObjects M_WPObject_wpclsRemoveObjects
#define M_CWVideo__get_somDataAlignment SOMClass__get_somDataAlignment
#define M_CWVideo__get_somInstanceDataOffsets SOMClass__get_somInstanceDataOffsets
#define M_CWVideo__get_somDirectInitClasses SOMClass__get_somDirectInitClasses
#define M_CWVideo__set_somDirectInitClasses SOMClass__set_somDirectInitClasses
#define M_CWVideo_somNew SOMClass_somNew
#define M_CWVideo_somNewNoInit SOMClass_somNewNoInit
#define M_CWVideo_somRenew SOMClass_somRenew
#define M_CWVideo_somRenewNoInit SOMClass_somRenewNoInit
#define M_CWVideo_somRenewNoZero SOMClass_somRenewNoZero
#define M_CWVideo_somRenewNoInitNoZero SOMClass_somRenewNoInitNoZero
#define M_CWVideo_somAllocate SOMClass_somAllocate
#define M_CWVideo_somDeallocate SOMClass_somDeallocate
#define M_CWVideo__get_somClassAllocate SOMClass__get_somClassAllocate
#define M_CWVideo__get_somClassDeallocate SOMClass__get_somClassDeallocate
#define M_CWVideo_somGetInstanceInitMask SOMClass_somGetInstanceInitMask
#define M_CWVideo_somGetInstanceDestructionMask SOMClass_somGetInstanceDestructionMask
#define M_CWVideo_somGetInstanceAssignmentMask SOMClass_somGetInstanceAssignmentMask
#define M_CWVideo_somInitClass SOMClass_somInitClass
#define M_CWVideo_somInitMIClass SOMClass_somInitMIClass
#define M_CWVideo_somAddStaticMethod SOMClass_somAddStaticMethod
#define M_CWVideo_somAddDynamicMethod SOMClass_somAddDynamicMethod
#define M_CWVideo_somOverrideSMethod SOMClass_somOverrideSMethod
#define M_CWVideo_somClassReady SOMClass_somClassReady
#define M_CWVideo_somGetApplyStub SOMClass_somGetApplyStub
#define M_CWVideo_somGetClassData SOMClass_somGetClassData
#define M_CWVideo_somSetClassData SOMClass_somSetClassData
#define M_CWVideo_somGetClassMtab SOMClass_somGetClassMtab
#define M_CWVideo_somGetInstanceOffset SOMClass_somGetInstanceOffset
#define M_CWVideo_somGetInstancePartSize SOMClass_somGetInstancePartSize
#define M_CWVideo_somGetInstanceSize SOMClass_somGetInstanceSize
#define M_CWVideo_somGetInstanceToken SOMClass_somGetInstanceToken
#define M_CWVideo_somGetMemberToken SOMClass_somGetMemberToken
#define M_CWVideo_somGetMethodData SOMClass_somGetMethodData
#define M_CWVideo_somGetRdStub SOMClass_somGetRdStub
#define M_CWVideo_somGetMethodDescriptor SOMClass_somGetMethodDescriptor
#define M_CWVideo_somGetMethodIndex SOMClass_somGetMethodIndex
#define M_CWVideo_somGetMethodToken SOMClass_somGetMethodToken
#define M_CWVideo_somGetName SOMClass_somGetName
#define M_CWVideo_somGetNthMethodData SOMClass_somGetNthMethodData
#define M_CWVideo_somGetNthMethodInfo SOMClass_somGetNthMethodInfo
#define M_CWVideo_somGetNumMethods SOMClass_somGetNumMethods
#define M_CWVideo_somGetNumStaticMethods SOMClass_somGetNumStaticMethods
#define M_CWVideo_somGetParent SOMClass_somGetParent
#define M_CWVideo_somGetParents SOMClass_somGetParents
#define M_CWVideo_somGetPClsMtab SOMClass_somGetPClsMtab
#define M_CWVideo_somGetPClsMtabs SOMClass_somGetPClsMtabs
#define M_CWVideo_somGetVersionNumbers SOMClass_somGetVersionNumbers
#define M_CWVideo_somSetMethodDescriptor SOMClass_somSetMethodDescriptor
#define M_CWVideo_somFindMethod SOMClass_somFindMethod
#define M_CWVideo_somFindMethodOk SOMClass_somFindMethodOk
#define M_CWVideo_somFindSMethod SOMClass_somFindSMethod
#define M_CWVideo_somFindSMethodOk SOMClass_somFindSMethodOk
#define M_CWVideo_somLookupMethod SOMClass_somLookupMethod
#define M_CWVideo_somCheckVersion SOMClass_somCheckVersion
#define M_CWVideo_somDescendedFrom SOMClass_somDescendedFrom
#define M_CWVideo_somSupportsMethod SOMClass_somSupportsMethod
#define M_CWVideo_somDefinedMethod SOMClass_somDefinedMethod
#define M_CWVideo_somOverrideMtab SOMClass_somOverrideMtab
#define M_CWVideo_somDefaultInit SOMObject_somDefaultInit
#define M_CWVideo_somDestruct SOMObject_somDestruct
#define M_CWVideo_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define M_CWVideo_somDefaultAssign SOMObject_somDefaultAssign
#define M_CWVideo_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define M_CWVideo_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define M_CWVideo_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define M_CWVideo_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define M_CWVideo_somDefaultVAssign SOMObject_somDefaultVAssign
#define M_CWVideo_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define M_CWVideo_somInit SOMObject_somInit
#define M_CWVideo_somFree SOMObject_somFree
#define M_CWVideo_somUninit SOMObject_somUninit
#define M_CWVideo_somGetClass SOMObject_somGetClass
#define M_CWVideo_somGetClassName SOMObject_somGetClassName
#define M_CWVideo_somGetSize SOMObject_somGetSize
#define M_CWVideo_somIsA SOMObject_somIsA
#define M_CWVideo_somIsInstanceOf SOMObject_somIsInstanceOf
#define M_CWVideo_somRespondsTo SOMObject_somRespondsTo
#define M_CWVideo_somDispatch SOMObject_somDispatch
#define M_CWVideo_somClassDispatch SOMObject_somClassDispatch
#define M_CWVideo_somCastObj SOMObject_somCastObj
#define M_CWVideo_somResetObj SOMObject_somResetObj
#define M_CWVideo_somDispatchV SOMObject_somDispatchV
#define M_CWVideo_somDispatchL SOMObject_somDispatchL
#define M_CWVideo_somDispatchA SOMObject_somDispatchA
#define M_CWVideo_somDispatchD SOMObject_somDispatchD
#define M_CWVideo_somPrintSelf SOMObject_somPrintSelf
#define M_CWVideo_somDumpSelf SOMObject_somDumpSelf
#define M_CWVideo_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_M_CWVideo_h */