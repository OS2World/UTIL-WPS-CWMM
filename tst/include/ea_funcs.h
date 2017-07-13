#ifndef EA_FUNCS_H_INCLUDED
#define EA_FUNCS_H_INCLUDED

BOOL EARestoreString(WPFileSystem * wpFSObject,  PSZ pKey, char * chrBuffer, ULONG ulBuffSize);
BOOL EASaveString(WPFileSystem * wpFSObject,  PSZ pKey, PSZ pValue);

#endif
