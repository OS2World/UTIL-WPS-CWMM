#ifndef INI_H_INCLUDED
#define INI_H_INCLUDED

#if __cplusplus
extern "C" {
#endif

BOOL PrfSaveWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd);
BOOL PrfRestoreWindowPos(HINI hini, char * iniFile, char* chrApp, char *chrKey, HWND hwnd, ULONG fSize);
BOOL PrfWriteString(HINI hini, char * iniFile, char* chrApp, char *chrKey, char* chrString);
BOOL PrfQueryString(HINI hini, char * iniFile, char* chrApp, char *chrKey, char* chrDefault, char* chrBuffer, ULONG ulSize);

#if __cplusplus
}
#endif

#endif

