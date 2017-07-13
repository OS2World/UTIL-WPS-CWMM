#ifndef REXX_H_INCLUDED
#define REXX_H_INCLUDED


#if __cplusplus
extern "C" {
#endif

LONG RxGetLongFromStemWithoutNumber(RXSTRING args, char * chrTail, ULONG* ulError);
PSZ RxGetStringFromStemWithoutNumber(RXSTRING args, char *chrTail, ULONG* ulError);
LONG RxGetLongFromVar(char * chrVar, ULONG* ulError);
LONG RxSetLongInRexxVar(char* chrVar, ULONG ulVal);

#if __cplusplus
}
#endif

#endif

