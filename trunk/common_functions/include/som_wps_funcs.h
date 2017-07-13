#ifndef SOMWPS_FUNCS_H_INCLUDED
#define SOMWPS_FUNCS_H_INCLUDED

#if  __cplusplus
extern "C" {

#endif

WPObject* somGetFileSystemObject(WPObject* wpObject);
SOMClass* somGetSomClass(char* chrClassName);
BOOL somObjectIsA(WPObject* wpObject, char * chrClassName);
PBYTE somReallocString(WPObject* wpObject, PBYTE oldString, char* newString, PULONG pulError);
BOOL wpsObjectIsInsertedInContainer(WPObject * wpObject);
WPObject* somResolveShadow(WPObject* wpObject);
ULONG wpsDrawTemplate(WPObject* wpObject, RECTL* rcl, HPS hps, ULONG flCnr);
void wpsRefreshObjectRecords(WPObject *somSelf);

#if  __cplusplus
}
#endif

#endif
