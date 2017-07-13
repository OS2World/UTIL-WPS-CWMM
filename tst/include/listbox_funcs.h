#ifndef LB_H_INCLUDED
#define LB_H_INCLUDED


#if __cplusplus
extern "C" {
#endif

SHORT LBFindMatch(HWND hwndLB, char *chrMatchText);

#if __cplusplus
}
#endif


#define LBFindMatchInDialogLB(hwndDlg, id, chrMatchText) LBFindMatch(WinWindowFromID(hwndDlg, id), chrMatchText)

#define LBSelectItem(hwndLB, sItem) WinSendMsg(hwndLB, LM_SELECTITEM, MPFROMSHORT(sItem), MPFROMLONG(TRUE))
#define LBSelectItemInDialogLB(hwndDlg, id, sItem) \
             WinSendMsg(WinWindowFromID(hwndDlg, id), LM_SELECTITEM, MPFROMSHORT(sItem), MPFROMLONG(TRUE))

#define LBDeselectItem(hwndLB, sItem) WinSendMsg(hwndLB, LM_SELECTITEM, MPFROMSHORT(sItem), MPFROMLONG(FALSE))
#define LBDeselectItemInDialogLB(hwndDlg, id, sItem) \
             WinSendMsg(WinWindowFromID(hwndDlg, id), LM_SELECTITEM, MPFROMSHORT(sItem), MPFROMLONG(FALSE))

#define LBQueryFirstSelectedItem(hwndLB) \
             SHORT1FROMMR(WinSendMsg(hwndLB, LM_QUERYSELECTION, MPFROMSHORT(LIT_FIRST), 0L))

#define LBInsertItemInDialogLB(hwndDlg, id, sIdx, chrText) \
         SHORT1FROMMR(WinSendMsg(WinWindowFromID(hwndDlg, id), LM_INSERTITEM, MPFROMSHORT(sIdx), MPFROMP(chrText)))

#endif /* LB_H_INCLUDED */
