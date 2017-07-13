
#define INCL_REXXSAA
#ifdef __EMX__
#include <os2emx.h>
#else
#include <os2.h>
#include <rexxsaa.h>                   /* needed for RexxStart()     */
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#if  __cplusplus
extern "C" {
#endif

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */
#define MAX_VAR_LEN  270
typedef struct RxStemData {
    SHVBLOCK shvb;                     /* Request block for RxVar    */
    CHAR varname[MAX_VAR_LEN];         /* Buffer for the variable    */
                                       /* name                       */
    ULONG stemlen;                     /* Length of stem.            */
 } RXSTEMDATA;

/*!**************************************************/
/*                                                  */
/* @@DESC                                           */
/*                                                  */
/* To be written...                                 */
/*                                                  */
/*!!*************************************************/
LONG RxGetLongFromStemWithoutNumber(RXSTRING args, char * chrTail, ULONG* ulError)
{
  char text[20];
  RXSTEMDATA ldp={0};

  *ulError=0;  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail  to stem */  
  strncat(ldp.varname+ldp.stemlen, chrTail, sizeof(ldp.varname)-ldp.stemlen);
  ldp.varname[sizeof(ldp.varname)-1]=0;

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if(sizeof(text) > ldp.shvb.shvvalue.strlength) {
    strncpy(text, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
    text[ldp.shvb.shvvalue.strlength]=0;
  }
  else
    strncpy(text, ldp.shvb.shvvalue.strptr, sizeof(text));
  text[sizeof(text)-1]=0;

  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return atol(text);
}

/*!***********************************************************/
/*                                                           */
/* @@DESC                                                    */
/*                                                           */
/* This function returns a string from a stem. The caller    */
/* must free the returned string with free()                 */
/*                                                           */
/*!!**********************************************************/
PSZ RxGetStringFromStemWithoutNumber(RXSTRING args, char *chrTail, ULONG* ulError)
{
  RXSTEMDATA ldp={0};
  char *pText;  
  *ulError=0;
  
                                       /* Initialize data area       */
  strcpy(ldp.varname, args.strptr);
  ldp.stemlen = args.strlength;
  strupr(ldp.varname);                 /* uppercase the name         */

  if (ldp.varname[ldp.stemlen-1] != '.')
    ldp.varname[ldp.stemlen++] = '.';

  /* Add tail to stem */  
  sprintf(ldp.varname+ldp.stemlen, "%s", chrTail);

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if((pText=(char*)malloc(ldp.shvb.shvvalue.strlength+1))!=NULLHANDLE)
     {
       strncpy(pText, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
       pText[ldp.shvb.shvvalue.strlength]=0;
     }
  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return pText;
}

/*!***********************************************************/
/*                                                           */
/* @@DESC                                                    */
/*                                                           */
/* This function returns a long from a REXX var.             */
/*                                                           */
/*!!**********************************************************/
LONG RxGetLongFromVar(char * chrVar, ULONG* ulError)
{
  char text[20];
  RXSTEMDATA ldp={0};
  
  *ulError=0;  

  /* Initialize data area       */
  strcpy(ldp.varname, chrVar);
  ldp.stemlen = strlen(chrVar);
  strupr(ldp.varname);                 /* uppercase the name         */
  
  ldp.varname[sizeof(ldp.varname)-1]=0;

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;

  ldp.shvb.shvcode = RXSHV_SYFET;                    /* Get a value            */
  ldp.shvb.shvret = 0;

  /* The value field of the request block is empty so REXX will allocate memory for us
     which holds the value. */

  /* Now get the value */
  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN) {
    *ulError=1;
    return 0;      /* error on non-zero          */
  }

  if(sizeof(text) > ldp.shvb.shvvalue.strlength) {
    strncpy(text, ldp.shvb.shvvalue.strptr, ldp.shvb.shvvalue.strlength);
    text[ldp.shvb.shvvalue.strlength]=0;
  }
  else
    strncpy(text, ldp.shvb.shvvalue.strptr, sizeof(text));
  text[sizeof(text)-1]=0;

  /* Free mem allocated for us by REXX */
  DosFreeMem(ldp.shvb.shvvalue.strptr);

  return atol(text);
}

/*!***********************************************************/
/*                                                           */
/* @@DESC                                                    */
/*                                                           */
/* To be written...                                          */
/*                                                           */
/*!!**********************************************************/
LONG RxSetLongInRexxVar(char* chrVar, ULONG ulVal)
{
  RXSTEMDATA ldp;
  char valString[34];

  if(!chrVar || !strlen(chrVar))
    return INVALID_ROUTINE;      /* error on non-zero          */

  memset(&ldp, 0, sizeof(ldp));
                                         /* Initialize data area       */
  strcpy(ldp.varname, chrVar);
  ldp.stemlen = strlen(chrVar);
  strupr(ldp.varname);                 /* uppercase the name         */

  ldp.shvb.shvnext = NULL;                           /* Only one request block */
  ldp.shvb.shvname.strptr = ldp.varname;             /* Var name               */
  ldp.shvb.shvname.strlength = strlen(ldp.varname);  /* RxString length        */
  ldp.shvb.shvnamelen = ldp.shvb.shvname.strlength;
  /* Set the value of the var */
  ldp.shvb.shvvalue.strptr =   _ultoa(ulVal, valString, 10);  /* Make a string from the var */
  ldp.shvb.shvvalue.strlength = strlen(valString);
  ldp.shvb.shvvaluelen = ldp.shvb.shvvalue.strlength;
  ldp.shvb.shvcode = RXSHV_SYSET;
  ldp.shvb.shvret = 0;

  if (RexxVariablePool(&ldp.shvb) == RXSHV_BADN)
    return INVALID_ROUTINE;      /* error on non-zero          */

  return VALID_ROUTINE;
}

#if  __cplusplus
}
#endif

