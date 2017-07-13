#define INCL_WIN
#define INCL_PM

#ifdef __EMX__
#include <os2emx.h>
#else
#include <os2.h>
#endif
#include <stdlib.h>
#include <string.h>

#if __cplusplus
extern "C" {
#endif

SHORT LBFindMatch(HWND hwndLB, char *chrMatchText)
{
  SHORT sItem;
  USHORT a;
  ULONG ulMatchLen=strlen(chrMatchText);

  if(ulMatchLen==0)
    return (SHORT)LIT_NONE;

  sItem=SHORT1FROMMR(WinSendMsg(hwndLB, LM_QUERYITEMCOUNT, 0L, 0L));
  if(sItem==0)
    return (SHORT)LIT_NONE;

  for(a=0;a<sItem;a++)
    {
      SHORT sLength;
      sLength=SHORT1FROMMR(WinSendMsg(hwndLB, LM_QUERYITEMTEXTLENGTH, MPFROMSHORT(a), MPFROMLONG(0L)));

      if(sLength==ulMatchLen) {
        char *chrItemText;
        /* Same length so compare */
        if((chrItemText=malloc(sLength+1))==NULLHANDLE)
          return LIT_NONE;
        WinSendMsg(hwndLB, LM_QUERYITEMTEXT, MPFROM2SHORT(a, sLength+1), MPFROMP(chrItemText));
        if(!strcmp(chrMatchText, chrItemText)) {
          free(chrItemText);
          return a;
        }
        free(chrItemText);
      }
    }
  return (SHORT)LIT_NONE;
}

#if __cplusplus
}
#endif
