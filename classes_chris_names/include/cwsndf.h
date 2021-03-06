
/*
 * This file was generated by the SOM Compiler.
 * FileName: include\cwsndf.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.7
 *     SOM Emitter emith: 2.42
 */

/*
 * 
 * cwsndf.idl:
 */


#ifndef SOM_CWSNDF_h
#define SOM_CWSNDF_h


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
#ifndef CWSNDF
#define CWSNDF SOMObject
#endif
#include <cwaudio.h>

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
#ifndef M_CWSNDF
    #define M_CWSNDF SOMObject
#endif /* M_CWSNDF */

/*
 * End of user-defined types.
 */

#define CWSNDF_MajorVersion 1
#define CWSNDF_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define CWSNDFNewClass cwsndfc
#pragma linkage(cwsndfc, system)
#define CWSNDFClassData cwsndfd
#define CWSNDFCClassData cwsndfx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */

/*
 * Include the meta class file
 */
#include <cwsndf.h>
/*--------------Migration------------*/
#define CWSNDF_classObj CWSNDFClassData.classObject
#define _CWSNDF CWSNDF_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK CWSNDFNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(CWSNDFNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct CWSNDFClassDataStructure {
	SOMClass *classObject;
} SOMDLINK CWSNDFClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct CWSNDFCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK CWSNDFCClassData;

/*
 * New and Renew macros for CWSNDF
 */
#define CWSNDFNew() \
   (_somNew(_CWSNDF ? _CWSNDF : CWSNDFNewClass(CWSNDF_MajorVersion, CWSNDF_MinorVersion)))
#define CWSNDFRenew(buf) \
   (_somRenew(_CWSNDF ? _CWSNDF : CWSNDFNewClass(CWSNDF_MajorVersion, CWSNDF_MinorVersion), buf))
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define CWSNDF_cwmmAddWaveInformationPage CWAudio_cwmmAddWaveInformationPage
#define CWSNDF_cwmmSetTrackInfo CWAudio_cwmmSetTrackInfo
#define CWSNDF_cwmmQueryTrackInfo CWAudio_cwmmQueryTrackInfo
#define CWSNDF_cwmmPlayTrack CWAudio_cwmmPlayTrack
#define CWSNDF_wpAddFileTypePage WPDataFile_wpAddFileTypePage
#define CWSNDF_wpPrintMetaFile WPDataFile_wpPrintMetaFile
#define CWSNDF_wpPrintPifFile WPDataFile_wpPrintPifFile
#define CWSNDF_wpPrintPrinterSpecificFile WPDataFile_wpPrintPrinterSpecificFile
#define CWSNDF_wpPrintUnknownFile WPDataFile_wpPrintUnknownFile
#define CWSNDF_wpQueryAssociatedProgram WPDataFile_wpQueryAssociatedProgram
#define CWSNDF_wpSetAssociatedFileIcon WPDataFile_wpSetAssociatedFileIcon
#define CWSNDF_wpQueryAssociatedFileIcon WPDataFile_wpQueryAssociatedFileIcon
#define CWSNDF_wpSetRealName WPFileSystem_wpSetRealName
#define CWSNDF_wpSetType WPFileSystem_wpSetType
#define CWSNDF_wpQueryType WPFileSystem_wpQueryType
#define CWSNDF_wpSetAttr WPFileSystem_wpSetAttr
#define CWSNDF_wpQueryAttr WPFileSystem_wpQueryAttr
#define CWSNDF_wpAddFile1Page WPFileSystem_wpAddFile1Page
#define CWSNDF_wpAddFile2Page WPFileSystem_wpAddFile2Page
#define CWSNDF_wpAddFile3Page WPFileSystem_wpAddFile3Page
#define CWSNDF_wpAddFileMenuPage WPFileSystem_wpAddFileMenuPage
#define CWSNDF_wpQueryCreation WPFileSystem_wpQueryCreation
#define CWSNDF_wpQueryLastAccess WPFileSystem_wpQueryLastAccess
#define CWSNDF_wpQueryLastWrite WPFileSystem_wpQueryLastWrite
#define CWSNDF_wpQueryFileSize WPFileSystem_wpQueryFileSize
#define CWSNDF_wpQueryEASize WPFileSystem_wpQueryEASize
#define CWSNDF_wpSetDateInfo WPFileSystem_wpSetDateInfo
#define CWSNDF_wpSetFileSizeInfo WPFileSystem_wpSetFileSizeInfo
#define CWSNDF_wpRefresh WPFileSystem_wpRefresh
#define CWSNDF_wpQueryRefreshFlags WPFileSystem_wpQueryRefreshFlags
#define CWSNDF_wpSetRefreshFlags WPFileSystem_wpSetRefreshFlags
#define CWSNDF_wpPrintPlainTextFile WPFileSystem_wpPrintPlainTextFile
#define CWSNDF_wpSetTitleAndRenameFile WPFileSystem_wpSetTitleAndRenameFile
#define CWSNDF_wpConfirmRenameFileWithExt WPFileSystem_wpConfirmRenameFileWithExt
#define CWSNDF_wpVerifyUpdateAccess WPFileSystem_wpVerifyUpdateAccess
#define CWSNDF_wpAddUserItemsToPopupMenu WPFileSystem_wpAddUserItemsToPopupMenu
#define CWSNDF_wpIsDiskSwapped WPFileSystem_wpIsDiskSwapped
#define CWSNDF_wpQueryRealName WPFileSystem_wpQueryRealName
#define CWSNDF_wpQueryFilename WPFileSystem_wpQueryFilename
#define CWSNDF_wpQueryDisk WPFileSystem_wpQueryDisk
#define CWSNDF_wpQueryDateInfo WPFileSystem_wpQueryDateInfo
#define CWSNDF_wpConfirmKeepAssoc WPFileSystem_wpConfirmKeepAssoc
#define CWSNDF_wpAddObjectGeneralPage WPObject_wpAddObjectGeneralPage
#define CWSNDF_wpAddObjectGeneralPage2 WPObject_wpAddObjectGeneralPage2
#define CWSNDF_wpAddObjectWindowPage WPObject_wpAddObjectWindowPage
#define CWSNDF_wpAddSettingsPages WPObject_wpAddSettingsPages
#define CWSNDF_wpAddToObjUseList WPObject_wpAddToObjUseList
#define CWSNDF_wpAllocMem WPObject_wpAllocMem
#define CWSNDF_wpAppendObject WPObject_wpAppendObject
#define CWSNDF_wpAssertObjectMutexSem WPObject_wpAssertObjectMutexSem
#define CWSNDF_wpClose WPObject_wpClose
#define CWSNDF_wpCnrInsertObject WPObject_wpCnrInsertObject
#define CWSNDF_wpCnrRemoveObject WPObject_wpCnrRemoveObject
#define CWSNDF_wpCnrSetEmphasis WPObject_wpCnrSetEmphasis
#define CWSNDF_wpConfirmDelete WPObject_wpConfirmDelete
#define CWSNDF_wpConfirmObjectTitle WPObject_wpConfirmObjectTitle
#define CWSNDF_wpCopiedFromTemplate WPObject_wpCopiedFromTemplate
#define CWSNDF_wpCopyObject WPObject_wpCopyObject
#define CWSNDF_wpCreateAnother WPObject_wpCreateAnother
#define CWSNDF_wpCreateFromTemplate WPObject_wpCreateFromTemplate
#define CWSNDF_wpCreateShadowObject WPObject_wpCreateShadowObject
#define CWSNDF_wpDelete WPObject_wpDelete
#define CWSNDF_wpDeleteFromObjUseList WPObject_wpDeleteFromObjUseList
#define CWSNDF_wpDisplayHelp WPObject_wpDisplayHelp
#define CWSNDF_wpDisplayMenu WPObject_wpDisplayMenu
#define CWSNDF_wpDoesObjectMatch WPObject_wpDoesObjectMatch
#define CWSNDF_wpDraggedOverObject WPObject_wpDraggedOverObject
#define CWSNDF_wpDragOver WPObject_wpDragOver
#define CWSNDF_wpDrop WPObject_wpDrop
#define CWSNDF_wpDroppedOnObject WPObject_wpDroppedOnObject
#define CWSNDF_wpEndConversation WPObject_wpEndConversation
#define CWSNDF_wpFilterPopupMenu WPObject_wpFilterPopupMenu
#define CWSNDF_wpFindUseItem WPObject_wpFindUseItem
#define CWSNDF_wpFormatDragItem WPObject_wpFormatDragItem
#define CWSNDF_wpFree WPObject_wpFree
#define CWSNDF_wpFreeMem WPObject_wpFreeMem
#define CWSNDF_wpHide WPObject_wpHide
#define CWSNDF_wpInitData WPObject_wpInitData
#define CWSNDF_wpInsertPopupMenuItems WPObject_wpInsertPopupMenuItems
#define CWSNDF_wpInsertSettingsPage WPObject_wpInsertSettingsPage
#define CWSNDF_wpMenuItemHelpSelected WPObject_wpMenuItemHelpSelected
#define CWSNDF_wpMenuItemSelected WPObject_wpMenuItemSelected
#define CWSNDF_wpModifyPopupMenu WPObject_wpModifyPopupMenu
#define CWSNDF_wpMoveObject WPObject_wpMoveObject
#define CWSNDF_wpOpen WPObject_wpOpen
#define CWSNDF_wpPrintObject WPObject_wpPrintObject
#define CWSNDF_wpQueryConcurrentView WPObject_wpQueryConcurrentView
#define CWSNDF_wpQueryButtonAppearance WPObject_wpQueryButtonAppearance
#define CWSNDF_wpQueryConfirmations WPObject_wpQueryConfirmations
#define CWSNDF_wpQueryDefaultHelp WPObject_wpQueryDefaultHelp
#define CWSNDF_wpQueryDefaultView WPObject_wpQueryDefaultView
#define CWSNDF_wpQueryDetailsData WPObject_wpQueryDetailsData
#define CWSNDF_wpQueryError WPObject_wpQueryError
#define CWSNDF_wpSetFolder WPObject_wpSetFolder
#define CWSNDF_wpQueryFolder WPObject_wpQueryFolder
#define CWSNDF_wpQueryIcon WPObject_wpQueryIcon
#define CWSNDF_wpQueryIconData WPObject_wpQueryIconData
#define CWSNDF_wpQueryMinWindow WPObject_wpQueryMinWindow
#define CWSNDF_wpQueryNameClashOptions WPObject_wpQueryNameClashOptions
#define CWSNDF_wpQueryStyle WPObject_wpQueryStyle
#define CWSNDF_wpSetTaskRec WPObject_wpSetTaskRec
#define CWSNDF_wpFindTaskRec WPObject_wpFindTaskRec
#define CWSNDF_wpQueryTitle WPObject_wpQueryTitle
#define CWSNDF_wpRegisterView WPObject_wpRegisterView
#define CWSNDF_wpReleaseObjectMutexSem WPObject_wpReleaseObjectMutexSem
#define CWSNDF_wpRender WPObject_wpRender
#define CWSNDF_wpRenderComplete WPObject_wpRenderComplete
#define CWSNDF_wpReplaceObject WPObject_wpReplaceObject
#define CWSNDF_wpRequestObjectMutexSem WPObject_wpRequestObjectMutexSem
#define CWSNDF_wpRestore WPObject_wpRestore
#define CWSNDF_wpRestoreData WPObject_wpRestoreData
#define CWSNDF_wpRestoreLong WPObject_wpRestoreLong
#define CWSNDF_wpRestoreState WPObject_wpRestoreState
#define CWSNDF_wpRestoreString WPObject_wpRestoreString
#define CWSNDF_wpSaveData WPObject_wpSaveData
#define CWSNDF_wpSaveImmediate WPObject_wpSaveImmediate
#define CWSNDF_wpSaveDeferred WPObject_wpSaveDeferred
#define CWSNDF_wpSaveLong WPObject_wpSaveLong
#define CWSNDF_wpSaveState WPObject_wpSaveState
#define CWSNDF_wpSaveString WPObject_wpSaveString
#define CWSNDF_wpScanSetupString WPObject_wpScanSetupString
#define CWSNDF_wpSetConcurrentView WPObject_wpSetConcurrentView
#define CWSNDF_wpSetButtonAppearance WPObject_wpSetButtonAppearance
#define CWSNDF_wpSetDefaultHelp WPObject_wpSetDefaultHelp
#define CWSNDF_wpSetDefaultView WPObject_wpSetDefaultView
#define CWSNDF_wpSetError WPObject_wpSetError
#define CWSNDF_wpSetIconHandle WPObject_wpSetIconHandle
#define CWSNDF_wpQueryScreenGroupID WPObject_wpQueryScreenGroupID
#define CWSNDF_wpSetupOnce WPObject_wpSetupOnce
#define CWSNDF_wpSetIcon WPObject_wpSetIcon
#define CWSNDF_wpSetIconData WPObject_wpSetIconData
#define CWSNDF_wpSetMinWindow WPObject_wpSetMinWindow
#define CWSNDF_wpSetStyle WPObject_wpSetStyle
#define CWSNDF_wpModifyStyle WPObject_wpModifyStyle
#define CWSNDF_wpSetTitle WPObject_wpSetTitle
#define CWSNDF_wpSetup WPObject_wpSetup
#define CWSNDF_wpSwitchTo WPObject_wpSwitchTo
#define CWSNDF_wpUnInitData WPObject_wpUnInitData
#define CWSNDF_wpViewObject WPObject_wpViewObject
#define CWSNDF_wpQueryTrueStyle WPObject_wpQueryTrueStyle
#define CWSNDF_wpQueryHandle WPObject_wpQueryHandle
#define CWSNDF_wpUnlockObject WPObject_wpUnlockObject
#define CWSNDF_wpObjectReady WPObject_wpObjectReady
#define CWSNDF_wpIsObjectInitialized WPObject_wpIsObjectInitialized
#define CWSNDF_wpCreateShadowObjectExt WPObject_wpCreateShadowObjectExt
#define CWSNDF_wpCnrDeleteUseItem WPObject_wpCnrDeleteUseItem
#define CWSNDF_wpIsDeleteable WPObject_wpIsDeleteable
#define CWSNDF_wpQueryCoreRecord WPObject_wpQueryCoreRecord
#define CWSNDF_wpSetObjectID WPObject_wpSetObjectID
#define CWSNDF_wpQueryObjectID WPObject_wpQueryObjectID
#define CWSNDF_wpSetDefaultIconPos WPObject_wpSetDefaultIconPos
#define CWSNDF_wpQueryDefaultIconPos WPObject_wpQueryDefaultIconPos
#define CWSNDF_wpCnrRefreshDetails WPObject_wpCnrRefreshDetails
#define CWSNDF_wpFindViewItem WPObject_wpFindViewItem
#define CWSNDF_wpLockObject WPObject_wpLockObject
#define CWSNDF_wpIsLocked WPObject_wpIsLocked
#define CWSNDF_wpQueryContainerFlagPtr WPObject_wpQueryContainerFlagPtr
#define CWSNDF_wpWaitForClose WPObject_wpWaitForClose
#define CWSNDF_somDefaultInit SOMObject_somDefaultInit
#define CWSNDF_somDestruct SOMObject_somDestruct
#define CWSNDF_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define CWSNDF_somDefaultAssign SOMObject_somDefaultAssign
#define CWSNDF_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define CWSNDF_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define CWSNDF_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define CWSNDF_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define CWSNDF_somDefaultVAssign SOMObject_somDefaultVAssign
#define CWSNDF_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define CWSNDF_somInit SOMObject_somInit
#define CWSNDF_somFree SOMObject_somFree
#define CWSNDF_somUninit SOMObject_somUninit
#define CWSNDF_somGetClass SOMObject_somGetClass
#define CWSNDF_somGetClassName SOMObject_somGetClassName
#define CWSNDF_somGetSize SOMObject_somGetSize
#define CWSNDF_somIsA SOMObject_somIsA
#define CWSNDF_somIsInstanceOf SOMObject_somIsInstanceOf
#define CWSNDF_somRespondsTo SOMObject_somRespondsTo
#define CWSNDF_somDispatch SOMObject_somDispatch
#define CWSNDF_somClassDispatch SOMObject_somClassDispatch
#define CWSNDF_somCastObj SOMObject_somCastObj
#define CWSNDF_somResetObj SOMObject_somResetObj
#define CWSNDF_somDispatchV SOMObject_somDispatchV
#define CWSNDF_somDispatchL SOMObject_somDispatchL
#define CWSNDF_somDispatchA SOMObject_somDispatchA
#define CWSNDF_somDispatchD SOMObject_somDispatchD
#define CWSNDF_somPrintSelf SOMObject_somPrintSelf
#define CWSNDF_somDumpSelf SOMObject_somDumpSelf
#define CWSNDF_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_CWSNDF_h */

#ifndef SOM_M_CWSNDF_h
#define SOM_M_CWSNDF_h


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
#ifndef M_CWSNDF
#define M_CWSNDF SOMObject
#endif
#include <cwaudio.h>

/*
 * Start of user-defined types:
 */

/*
 * End of user-defined types.
 */

#define M_CWSNDF_MajorVersion 1
#define M_CWSNDF_MinorVersion 2

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define M_CWSNDFNewClass cwsndfMc
#pragma linkage(cwsndfMc, system)
#define M_CWSNDFClassData cwsndfMd
#define M_CWSNDFCClassData cwsndfMx
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define M_CWSNDF_classObj M_CWSNDFClassData.classObject
#define _M_CWSNDF M_CWSNDF_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK M_CWSNDFNewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(M_CWSNDFNewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct M_CWSNDFClassDataStructure {
	SOMClass *classObject;
} SOMDLINK M_CWSNDFClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct M_CWSNDFCClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK M_CWSNDFCClassData;

/*
 * New and Renew macros for M_CWSNDF
 */
#define M_CWSNDFNew() \
   (_somNew(_M_CWSNDF ? _M_CWSNDF : M_CWSNDFNewClass(M_CWSNDF_MajorVersion, M_CWSNDF_MinorVersion)))
#define M_CWSNDFRenew(buf) \
   (_somRenew(_M_CWSNDF ? _M_CWSNDF : M_CWSNDFNewClass(M_CWSNDF_MajorVersion, M_CWSNDF_MinorVersion), buf))

/*
 * Override method: wpclsQueryInstanceType
 */
#define M_CWSNDF_wpclsQueryInstanceType(somSelf) \
	M_WPFileSystem_wpclsQueryInstanceType(somSelf)

/*
 * Override method: wpclsQueryInstanceFilter
 */
#define M_CWSNDF_wpclsQueryInstanceFilter(somSelf) \
	M_WPFileSystem_wpclsQueryInstanceFilter(somSelf)

/*
 * Override method: wpclsQueryDefaultHelp
 */
#define M_CWSNDF_wpclsQueryDefaultHelp(somSelf,pHelpPanelId,pszHelpLibrary) \
	M_WPObject_wpclsQueryDefaultHelp(somSelf,pHelpPanelId,pszHelpLibrary)

/*
 * Override method: wpclsQueryTitle
 */
#define M_CWSNDF_wpclsQueryTitle(somSelf) \
	M_WPObject_wpclsQueryTitle(somSelf)

/*
 * Override method: wpclsQueryIconData
 */
#define M_CWSNDF_wpclsQueryIconData(somSelf,pIconInfo) \
	M_WPObject_wpclsQueryIconData(somSelf,pIconInfo)
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define M_CWSNDF_wpclsQueryAwakeObject M_WPFileSystem_wpclsQueryAwakeObject
#define M_CWSNDF_wpclsFileSysExists M_WPFileSystem_wpclsFileSysExists
#define M_CWSNDF_wpclsQueryObjectFromPath M_WPFileSystem_wpclsQueryObjectFromPath
#define M_CWSNDF_wpclsCreateDefaultTemplates M_WPObject_wpclsCreateDefaultTemplates
#define M_CWSNDF_wpclsInitData M_WPObject_wpclsInitData
#define M_CWSNDF_wpclsMakeAwake M_WPObject_wpclsMakeAwake
#define M_CWSNDF_wpclsNew M_WPObject_wpclsNew
#define M_CWSNDF_wpclsQueryDefaultView M_WPObject_wpclsQueryDefaultView
#define M_CWSNDF_wpclsQueryDetails M_WPObject_wpclsQueryDetails
#define M_CWSNDF_wpclsQueryDetailsInfo M_WPObject_wpclsQueryDetailsInfo
#define M_CWSNDF_wpclsQueryExtendedCriteria M_WPObject_wpclsQueryExtendedCriteria
#define M_CWSNDF_wpclsQueryFolder M_WPObject_wpclsQueryFolder
#define M_CWSNDF_wpclsQueryIcon M_WPObject_wpclsQueryIcon
#define M_CWSNDF_wpclsQuerySearchInfo M_WPObject_wpclsQuerySearchInfo
#define M_CWSNDF_wpclsQueryStyle M_WPObject_wpclsQueryStyle
#define M_CWSNDF_wpclsUnInitData M_WPObject_wpclsUnInitData
#define M_CWSNDF_wpclsFindObjectFirst M_WPObject_wpclsFindObjectFirst
#define M_CWSNDF_wpclsFindObjectNext M_WPObject_wpclsFindObjectNext
#define M_CWSNDF_wpclsFindObjectEnd M_WPObject_wpclsFindObjectEnd
#define M_CWSNDF_wpclsSetError M_WPObject_wpclsSetError
#define M_CWSNDF_wpclsQueryError M_WPObject_wpclsQueryError
#define M_CWSNDF_wpclsQuerySettingsPageSize M_WPObject_wpclsQuerySettingsPageSize
#define M_CWSNDF_wpclsQueryObject M_WPObject_wpclsQueryObject
#define M_CWSNDF_wpclsQueryButtonAppearance M_WPObject_wpclsQueryButtonAppearance
#define M_CWSNDF_wpclsDecUsage M_WPObject_wpclsDecUsage
#define M_CWSNDF_wpclsIncUsage M_WPObject_wpclsIncUsage
#define M_CWSNDF_wpclsInsertMultipleObjects M_WPObject_wpclsInsertMultipleObjects
#define M_CWSNDF_wpclsFindOneObject M_WPObject_wpclsFindOneObject
#define M_CWSNDF_wpclsSetSettingsPageSize M_WPObject_wpclsSetSettingsPageSize
#define M_CWSNDF_wpclsSetIconData M_WPObject_wpclsSetIconData
#define M_CWSNDF_wpclsSetIcon M_WPObject_wpclsSetIcon
#define M_CWSNDF_wpclsObjectFromHandle M_WPObject_wpclsObjectFromHandle
#define M_CWSNDF_wpclsRemoveObjects M_WPObject_wpclsRemoveObjects
#define M_CWSNDF__get_somDataAlignment SOMClass__get_somDataAlignment
#define M_CWSNDF__get_somInstanceDataOffsets SOMClass__get_somInstanceDataOffsets
#define M_CWSNDF__get_somDirectInitClasses SOMClass__get_somDirectInitClasses
#define M_CWSNDF__set_somDirectInitClasses SOMClass__set_somDirectInitClasses
#define M_CWSNDF_somNew SOMClass_somNew
#define M_CWSNDF_somNewNoInit SOMClass_somNewNoInit
#define M_CWSNDF_somRenew SOMClass_somRenew
#define M_CWSNDF_somRenewNoInit SOMClass_somRenewNoInit
#define M_CWSNDF_somRenewNoZero SOMClass_somRenewNoZero
#define M_CWSNDF_somRenewNoInitNoZero SOMClass_somRenewNoInitNoZero
#define M_CWSNDF_somAllocate SOMClass_somAllocate
#define M_CWSNDF_somDeallocate SOMClass_somDeallocate
#define M_CWSNDF__get_somClassAllocate SOMClass__get_somClassAllocate
#define M_CWSNDF__get_somClassDeallocate SOMClass__get_somClassDeallocate
#define M_CWSNDF_somGetInstanceInitMask SOMClass_somGetInstanceInitMask
#define M_CWSNDF_somGetInstanceDestructionMask SOMClass_somGetInstanceDestructionMask
#define M_CWSNDF_somGetInstanceAssignmentMask SOMClass_somGetInstanceAssignmentMask
#define M_CWSNDF_somInitClass SOMClass_somInitClass
#define M_CWSNDF_somInitMIClass SOMClass_somInitMIClass
#define M_CWSNDF_somAddStaticMethod SOMClass_somAddStaticMethod
#define M_CWSNDF_somAddDynamicMethod SOMClass_somAddDynamicMethod
#define M_CWSNDF_somOverrideSMethod SOMClass_somOverrideSMethod
#define M_CWSNDF_somClassReady SOMClass_somClassReady
#define M_CWSNDF_somGetApplyStub SOMClass_somGetApplyStub
#define M_CWSNDF_somGetClassData SOMClass_somGetClassData
#define M_CWSNDF_somSetClassData SOMClass_somSetClassData
#define M_CWSNDF_somGetClassMtab SOMClass_somGetClassMtab
#define M_CWSNDF_somGetInstanceOffset SOMClass_somGetInstanceOffset
#define M_CWSNDF_somGetInstancePartSize SOMClass_somGetInstancePartSize
#define M_CWSNDF_somGetInstanceSize SOMClass_somGetInstanceSize
#define M_CWSNDF_somGetInstanceToken SOMClass_somGetInstanceToken
#define M_CWSNDF_somGetMemberToken SOMClass_somGetMemberToken
#define M_CWSNDF_somGetMethodData SOMClass_somGetMethodData
#define M_CWSNDF_somGetRdStub SOMClass_somGetRdStub
#define M_CWSNDF_somGetMethodDescriptor SOMClass_somGetMethodDescriptor
#define M_CWSNDF_somGetMethodIndex SOMClass_somGetMethodIndex
#define M_CWSNDF_somGetMethodToken SOMClass_somGetMethodToken
#define M_CWSNDF_somGetName SOMClass_somGetName
#define M_CWSNDF_somGetNthMethodData SOMClass_somGetNthMethodData
#define M_CWSNDF_somGetNthMethodInfo SOMClass_somGetNthMethodInfo
#define M_CWSNDF_somGetNumMethods SOMClass_somGetNumMethods
#define M_CWSNDF_somGetNumStaticMethods SOMClass_somGetNumStaticMethods
#define M_CWSNDF_somGetParent SOMClass_somGetParent
#define M_CWSNDF_somGetParents SOMClass_somGetParents
#define M_CWSNDF_somGetPClsMtab SOMClass_somGetPClsMtab
#define M_CWSNDF_somGetPClsMtabs SOMClass_somGetPClsMtabs
#define M_CWSNDF_somGetVersionNumbers SOMClass_somGetVersionNumbers
#define M_CWSNDF_somSetMethodDescriptor SOMClass_somSetMethodDescriptor
#define M_CWSNDF_somFindMethod SOMClass_somFindMethod
#define M_CWSNDF_somFindMethodOk SOMClass_somFindMethodOk
#define M_CWSNDF_somFindSMethod SOMClass_somFindSMethod
#define M_CWSNDF_somFindSMethodOk SOMClass_somFindSMethodOk
#define M_CWSNDF_somLookupMethod SOMClass_somLookupMethod
#define M_CWSNDF_somCheckVersion SOMClass_somCheckVersion
#define M_CWSNDF_somDescendedFrom SOMClass_somDescendedFrom
#define M_CWSNDF_somSupportsMethod SOMClass_somSupportsMethod
#define M_CWSNDF_somDefinedMethod SOMClass_somDefinedMethod
#define M_CWSNDF_somOverrideMtab SOMClass_somOverrideMtab
#define M_CWSNDF_somDefaultInit SOMObject_somDefaultInit
#define M_CWSNDF_somDestruct SOMObject_somDestruct
#define M_CWSNDF_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define M_CWSNDF_somDefaultAssign SOMObject_somDefaultAssign
#define M_CWSNDF_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define M_CWSNDF_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define M_CWSNDF_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define M_CWSNDF_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define M_CWSNDF_somDefaultVAssign SOMObject_somDefaultVAssign
#define M_CWSNDF_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define M_CWSNDF_somInit SOMObject_somInit
#define M_CWSNDF_somFree SOMObject_somFree
#define M_CWSNDF_somUninit SOMObject_somUninit
#define M_CWSNDF_somGetClass SOMObject_somGetClass
#define M_CWSNDF_somGetClassName SOMObject_somGetClassName
#define M_CWSNDF_somGetSize SOMObject_somGetSize
#define M_CWSNDF_somIsA SOMObject_somIsA
#define M_CWSNDF_somIsInstanceOf SOMObject_somIsInstanceOf
#define M_CWSNDF_somRespondsTo SOMObject_somRespondsTo
#define M_CWSNDF_somDispatch SOMObject_somDispatch
#define M_CWSNDF_somClassDispatch SOMObject_somClassDispatch
#define M_CWSNDF_somCastObj SOMObject_somCastObj
#define M_CWSNDF_somResetObj SOMObject_somResetObj
#define M_CWSNDF_somDispatchV SOMObject_somDispatchV
#define M_CWSNDF_somDispatchL SOMObject_somDispatchL
#define M_CWSNDF_somDispatchA SOMObject_somDispatchA
#define M_CWSNDF_somDispatchD SOMObject_somDispatchD
#define M_CWSNDF_somPrintSelf SOMObject_somPrintSelf
#define M_CWSNDF_somDumpSelf SOMObject_somDumpSelf
#define M_CWSNDF_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_M_CWSNDF_h */
