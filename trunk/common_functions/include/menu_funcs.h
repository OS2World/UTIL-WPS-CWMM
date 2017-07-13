#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#if  __cplusplus
extern "C" {
#endif
SHORT menuInsertMenuItem( HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition, int iID, char * chrText);
MRESULT menuInsertMenuSeparator(HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition );
SHORT menuQueryItemCount(HWND hwndMenu);
MRESULT menuCheckItem(HWND hwndMenu, USHORT usID, BOOL fIncludeSubMenu, BOOL fCheck);
MRESULT menuRemoveItem(HWND hwndMenu, USHORT usID, BOOL fIncludeSubMenu);
MRESULT menuCheckForItem(HWND hwndMenu, USHORT usID, BOOL fIncludeSubMenu);
#if  __cplusplus
}
#endif

#endif
