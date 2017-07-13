#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#ifdef __CPLUSPLUS__
extern "C" {
#endif

ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags);
ULONG showMessageBox2(HWND hwnd, ULONG ulIDTitle, ULONG ulIDText, HMODULE hModule, ULONG ulFlag);
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
BOOL MsgGetMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd);
#ifdef __CPLUSPLUS__
}
#endif

#endif
