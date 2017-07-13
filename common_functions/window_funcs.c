#define INCL_WIN
#include <os2.h>

#include "window_funcs.h"

#if  __cplusplus
extern "C" {
#endif

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion changes the z-order of the window  */
/* <hwnd> so it's on the top.                       */
/*                                                  */
/* @@PARAM                                          */
/*                                                  */
/* HWND hwnd                                        */
/*                                                  */
/* Handle of the window.                            */
/*                                                  */
/* @@RETURNS                                        */
/*                                                  */
/* BOOL rc                                          */
/*                                                  */
/* TRUE on success, FALSE otherwise.                */
/*                                                  */
/*!!*************************************************/
BOOL WinSetWindowToTop(HWND hwnd)
{
  return WinSetWindowPos(HWND_TOP, hwnd, 0, 0, 0, 0, SWP_ZORDER);
}

#if  __cplusplus
}
#endif
