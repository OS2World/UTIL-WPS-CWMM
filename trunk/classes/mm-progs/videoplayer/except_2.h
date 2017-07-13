
#ifndef EXCEPT_H_INCLUDED
#define EXCEPT_H_INCLUDED

#include <setjmp.h>
#include "excptLogName.h"

typedef struct REGREC2_STRUCT {
  PVOID     pNext;
  PFN       pfnHandler;
  jmp_buf   jmpThread;
} REGREC2, *PREGREC2;

typedef struct _LOCALEXCEPTSTRUCT {
  EXCEPTIONREGISTRATIONRECORD  excptRegRecord;
  ULONG                 ulExcpt;
  APIRET                arc;
  jmp_buf   jmpThread;
} LOCALEXCEPTSTRUCT, *PLOCALEXCEPTSTRUCT;

    /********************************************************************
     *                                                                  *
     *   Macros                                                         *
     *                                                                  *
     ********************************************************************/

    /* See except.c for explanations how to use these. */

#define TRY_LOUD(localExceptStruct)                                   \
{  \ 
      LOCALEXCEPTSTRUCT    localExceptStruct;                         \
         localExceptStruct.excptRegRecord.ExceptionHandler = (ERR)excHandler;          \
                localExceptStruct.arc = DosSetExceptionHandler(                  \
                            (PEXCEPTIONREGISTRATIONRECORD)&(localExceptStruct.excptRegRecord)); \
                if (localExceptStruct.arc)                                       \
                    DosBeep(100, 1000);                                    \
                localExceptStruct.ulExcpt = setjmp(localExceptStruct.jmpThread);               \
                if (localExceptStruct.ulExcpt == 0)                              \
                {   /* no exception occured: */

    #define CATCH(localExceptStruct)                                       \
                    DosUnsetExceptionHandler(                              \
                            (PEXCEPTIONREGISTRATIONRECORD)&(localExceptStruct.excptRegRecord));   \
                } /* end if (try_ulExcpt == 0) */                          \
                else { /* exception occured: */                            \
                    DosUnsetExceptionHandler(                              \
                            (PEXCEPTIONREGISTRATIONRECORD)&(localExceptStruct.excptRegRecord));
    #define END_CATCH                                                      \
                } /* end else (excptstruct.ulExcpt == 0) */                \
            } /* end variable scope block */


    /*
     * CRASH:
     *      this macro is helpful for testing
     *      the exception handlers.
     *      This is not for general use. ;-)
     */

    #define CRASH {PSZ p = NULL; *p = 'a'; }

    /********************************************************************
     *                                                                  *
     *   Prototypes                                                     *
     *                                                                  *
     ********************************************************************/

ULONG _System excHandler(PEXCEPTIONREPORTRECORD pERepRec,
                         PLOCALEXCEPTSTRUCT pRegRec2,
                         //          PREGREC2 pRegRec2,
                         PCONTEXTRECORD pCtxRec,
                         PVOID pv);



#endif

























