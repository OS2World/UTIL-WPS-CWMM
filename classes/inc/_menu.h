#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

SHORT menuInsertMenuItem( HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition, int iID, char * chrText);
MRESULT menuInsertMenuSeparator(HWND hwndMenu, HWND hwndSubMenu, SHORT sPosition );

#endif
