#ifndef INI_H_INCLUDED
#define INI_H_INCLUDED

#if __cplusplus
extern "C" {
#endif

BOOL IniSaveWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL IniRestoreWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd, ULONG fSize);

#if __cplusplus
}
#endif

#endif

