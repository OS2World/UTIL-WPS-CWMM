#define INCL_WIN
#define INCL_GPI
#define INCL_DOS
#define INCL_DOSERRORS

#include <os2.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#if  __cplusplus
extern "C" {
#endif


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion inserts a separator into menu      */
/* <hwndMenu> and submenu <hwndSubMenu> at          */
/* position <iPosition>.                             */
/*                                                  */
/*                                                  */
/*                                                  */
/*                                                  */
/*                                                  */
/*!!*************************************************/
MRESULT menuInsertMenuSeparator(HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition )
{
  MENUITEM mi={0};

  /* Fill the MENUITEM structure */
  mi.iPosition=sPosition;
  mi.afStyle=MIS_SEPARATOR;
  if(hwndSubMenu)
    mi.afStyle|=MIS_SUBMENU;
  mi.id=0;
  mi.afAttribute=NULLHANDLE;                
  mi.hwndSubMenu=hwndSubMenu;
  mi.hItem=NULLHANDLE;

  return WinSendMsg(hwndMenu,MM_INSERTITEM,(MPARAM)&mi,
                    (MPARAM)NULL);                 
}


/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* This funktion inserts an item into menu          */
/* <hwndMenu> and submenu <hwndSubMenu> at          */
/* position <iPosition>.                            */
/*                                                  */
/*!!*************************************************/
SHORT menuInsertMenuItem( HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition, USHORT iID, char * chrText)
{
  MENUITEM mi={0};

  /* Fill the MENUITEM structure */
  mi.iPosition=sPosition;
  mi.afStyle=MIS_TEXT;
  if(hwndSubMenu)
    mi.afStyle|=MIS_SUBMENU;
  mi.id=iID;
  mi.afAttribute=NULLHANDLE;                
  mi.hwndSubMenu=hwndSubMenu;
  mi.hItem=NULLHANDLE;
 
  return SHORT1FROMMR(WinSendMsg(hwndMenu,MM_INSERTITEM,(MPARAM)&mi,
                    (MPARAM)chrText));                 
}

SHORT menuQueryItemCount(HWND hwndMenu)
{
  return SHORT1FROMMR(WinSendMsg( hwndMenu, MM_QUERYITEMCOUNT, MPFROMLONG(0L),
                     MPFROMLONG(0L))); 
}

MRESULT menuCheckItem(HWND hwndMenu, USHORT usID, BOOL fIncludeSubMenu, BOOL fCheck)
{
  return WinSendMsg( hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(usID, fIncludeSubMenu),
                     MPFROM2SHORT(MIA_CHECKED, (fCheck ? MIA_CHECKED : FALSE))); 
}

MRESULT menuRemoveItem(HWND hwndMenu, USHORT usID, BOOL fIncludeSubMenu)
{
  return WinSendMsg( hwndMenu, MM_SETITEMATTR, MPFROM2SHORT(usID, fIncludeSubMenu),
                     MPFROMLONG(0L)); 
}

MRESULT menuCheckForItem(HWND hwndMenu, USHORT usID, BOOL fIncludeSubMenu)
{
  MENUITEM mi;
  return WinSendMsg( hwndMenu, MM_QUERYITEM, MPFROM2SHORT(usID, fIncludeSubMenu),
                     MPFROMP(&mi)); 
}


#if __cplusplus
}
#endif

