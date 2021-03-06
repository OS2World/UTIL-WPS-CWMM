
#define INCL_WIN

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void errorResourceVerbose(void)
{
  WinMessageBox(HWND_DESKTOP,0,
                "The resource DLL which contains all the dialogs, graphics and messages cannot be loaded. \
Please check your installation.",
"Problem with class installation",12345,
                MB_OK|MB_MOVEABLE|MB_ERROR);
}

void errorResource(void)
{
#if 0
  /*
    Don't show a message box because on WPS startup this may lead to a deadlock.
    At least I suspect this happens on system where the MM classes don't work.
    Instead there's a new function errorResourceVerbose() which may be called when we
    know, the WPS is up and running. 
   */
  WinMessageBox(HWND_DESKTOP,0,
                "The resource DLL which contains all the dialogs, graphics and messages cannot be loaded. \
Please check your installation.",
"Problem with class installation",12345,
                MB_OK|MB_MOVEABLE|MB_ERROR);
#endif
}


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Show a message box with text strings loaded from */
/* the resource DLL or the EXE file.                */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulResult                                   */
/*                                                  */
/* MBID_ERROR in case of an error.                  */
/* :p.                                              */
/* Result code from WinMessageBox().                */
/*                                                  */
/*                                                  */
/*!!*************************************************/
ULONG messageBox( char* text, ULONG ulTextID , LONG lSizeText,
                  char* title, ULONG ulTitleID, LONG lSizeTitle,
                  HMODULE hResource, HWND hwnd, ULONG ulFlags)
{

  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulTextID,lSizeText,text)) {
    errorResource();
    return MBID_ERROR;
  }
  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulTitleID,lSizeTitle,title)) {
    errorResource();
    return MBID_ERROR;
  }
  return WinMessageBox(  hwnd, hwnd, text, title, 0UL, ulFlags );
}

static ULONG mBox( char* text, ULONG ulTextID , LONG lSizeText,
                   char* title, ULONG ulTitleID, LONG lSizeTitle,
                   HMODULE hResource, HWND hwnd, ULONG ulFlags)
{

  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulTextID,lSizeText,text)) {
    errorResource();
    return MBID_ERROR;
  }
  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulTitleID,lSizeTitle,title)) {
    errorResource();
    return MBID_ERROR;
  }
  return WinMessageBox(  HWND_DESKTOP, hwnd, text, title, 0UL, ulFlags );
}

#if 0
/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Show a message box with text strings loaded from */
/* the resource DLL or the EXE file.                */
/* Unlike messagebox no buffers must be given but   */
/* only the string IDs. Max title length is 256,    */
/* max text length 256.                             */
/* :p.                                              */
/*                                                  */
/* This function is obsolete.                       */
/* Use MsgShowMessageBox() instead.                 */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulResult                                   */
/*                                                  */
/* MBID_ERROR in case of an error.                  */
/* :p.                                              */
/* Result code from WinMessageBox().                */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* This function is obsolete.                       */
/* Use MsgShowMessageBox() instead.                 */
/*                                                  */
/*!!*************************************************/
ULONG showMessageBox2(HWND hwnd, ULONG ulIDTitle, ULONG ulIDText, HMODULE hModule, ULONG ulFlag)
{
  char text[256];
  char title[256];

  return mBox(  text, ulIDText , sizeof(text),
               title, ulIDTitle, sizeof(title),
               hModule, hwnd, ulFlag);
};
#endif

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Show a message box with text strings loaded from */
/* the resource DLL or the EXE file.                */
/* Unlike messagebox() no buffers must be given but */
/* only the string IDs. Max title length is 256,    */
/* max text length 256.                             */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* HWND hwnd input                                  */
/*                                                  */
/* Handle to a window. This will be the owner of    */
/* the message box.                                 */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* ULONG ulIDTitle input                            */
/*                                                  */
/* ID of the string to be used as the title.        */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* ULONG ulIDText input                             */
/*                                                  */
/* ID of the string to be used as the text.         */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* HMODULE hModule input                            */
/*                                                  */
/* Handle to a ressource DLL or NULLHANDLE. If      */
/* this parameter is null the strings will be       */
/* taken from ressources bound to the executable.   */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* ULONG ulFlags input                              */
/*                                                  */
/* Flags specifying the appearance of the message   */
/* box. See WinMessageBox() for more information.   */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* ULONG ulResult                                   */
/*                                                  */
/* MBID_ERROR in case of an error. This may be      */
/* for example if the ressources can't be found.    */
/* :p.                                              */
/* Result code from WinMessageBox().                */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* The parent of the message box is HWND_DESKTOP.   */
/*                                                  */
/*!!*************************************************/
ULONG MsgShowMessageBox(HWND hwnd, ULONG ulIDTitle, ULONG ulIDText, HMODULE hModule, ULONG ulFlag)
{
  char* pText;
  char* pTitle;
  ULONG rc;

#define TLENGTH 256L

  if(NULLHANDLE==(pText=malloc(TLENGTH*2*sizeof(char))))
    return MBID_ERROR;

  pTitle=pText+TLENGTH*sizeof(char);

  rc=mBox( pText, ulIDText , TLENGTH,
           pTitle, ulIDTitle, TLENGTH,
           hModule, hwnd, ulFlag);

  free(pText);
  return rc;
};


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* Load a message string from a resource DLL or the */
/* EXE file.                                        */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* BOOL rc                                          */
/*                                                  */
/* TRUE if string was found in the resource DLL or  */
/* EXE file. FALSE otherwise.                       */
/*                                                  */
/* @@REMARKS                                        */
/*                                                  */
/* This function is obsolete.                       */
/* Use MsgGetMessage() instead.                     */
/*                                                  */
/*!!*************************************************/
BOOL getMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd)
{
  if(!WinLoadString(WinQueryAnchorBlock(hwnd),hResource,ulID,lSizeText,text)) {
    strcpy(text,"");
    return FALSE;
  }
  return TRUE;
}

/*!***********************************************************/
/*                                                           */
/* Load a message string from a resource DLL or the          */
/* EXE file.                                                 */
/*                                                           */
/* @@RETURNS                                                 */
/*                                                           */
/* BOOL rc                                                   */
/*                                                           */
/* TRUE if string was found in the resource DLL or           */
/* EXE file. FALSE otherwise.                                */
/*                                                           */
/* @@REMARKS                                                 */
/*                                                           */
/* If an error occurs an empty string is placed in the       */
/* buffer.                                                   */
/*                                                           */
/*!!**********************************************************/
BOOL MsgGetMessage(char* text,ULONG ulID, LONG lSizeText, HMODULE hResource,HWND hwnd)
{
  return getMessage( text, ulID, lSizeText, hResource, hwnd);
}

void pmUsage()
{
  WinMessageBox(HWND_DESKTOP,0,
                "This helper shouldn't be started by hand. It is called by the multimedia classes. \
If you didn't launch the helper by hand you may have found a bug. Please contact the author.",
                "Problem with multimedia classes",12345,
                MB_OK|MB_MOVEABLE|MB_ERROR);

}

