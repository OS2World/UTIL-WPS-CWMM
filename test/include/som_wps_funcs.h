#ifndef SOMWPS_FUNCS_H_INCLUDED
#define SOMWPS_FUNCS_H_INCLUDED

WPObject* somGetFileSystemObject(WPObject* wpObject);
SOMClass* somGetSomClass(char* chrClassName);
BOOL somObjectIsA(WPObject* wpObject, char * chrClassName);
PBYTE somReallocString(WPObject* wpObject, PBYTE oldString, char* newString, PULONG pulError);

#endif
