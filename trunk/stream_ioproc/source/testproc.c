/* This program (install.c) is Public Domain */

#define INCL_OS2MM
#define INCL_DOS
#include <os2.h>
#include <os2me.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "streamproc.h"


//#include <minstall.h>
//If you don't have minstall.h, this is all that is needed
typedef struct _INSTMCISENDCOMMAND
   {
   USHORT wDeviceID;
   USHORT wMessage;
   ULONG  dwParam1;
   ULONG  dwParam2;
   USHORT wUserParm;
   } INSTMCISENDCOMMAND;

typedef INSTMCISENDCOMMAND *PINSTMCISENDCOMMAND;
#define IM_MCI_SEND_COMMAND         0x057C
#define IM_MCI_EXTENDED_SYSINFO     0x057E
#define IM_LOG_ERROR       0x0573


int deleteProc(char *Fourcc) {
    MMINIFILEINFO mmIniFileInfo;
    ULONG ulFlags = 0L;
    ULONG rc;

    memset( &mmIniFileInfo, '\0', sizeof(MMINIFILEINFO) );
    mmIniFileInfo.fccIOProc = mmioStringToFOURCC(Fourcc,0);
    ulFlags = MMIO_FINDPROC;
    rc = mmioIniFileHandler( &mmIniFileInfo,
                             ulFlags);
    if (rc) {
        printf("Couldn't find IOProc %s.  Code: %ld\n",Fourcc,rc);
        return rc;
    }

    ulFlags = MMIO_REMOVEPROC;
    rc = mmioIniFileHandler( &mmIniFileInfo,
                             ulFlags);
    if (rc) {
        printf("Couldn't remove IOProc %s.  Code: %ld\n",Fourcc,rc);
        return rc;
    } else {
        printf("Removed IOProc %s.\n",Fourcc);
    } /* endif */

    {
       char dllname[DLLNAME_SIZE] = "DLL\\";
       char fulldllname[DLLNAME_SIZE] = "";

       strncat(dllname, mmIniFileInfo.szDLLName,sizeof(dllname));
       strncat(dllname, ".DLL",sizeof(dllname));
       rc = DosSearchPath(2,"MMBASE",dllname,fulldllname,sizeof(fulldllname));
       if (rc) {
           printf("Couldn't find %s. code:%ld\n", dllname,rc);
           return rc;
       }

       rc = DosReplaceModule(fulldllname,NULL,NULL);
       if (rc) {
           printf("Couldn't unlock %s. code:%ld\n", fulldllname,rc);
       } else {
           printf("Unlocked %s.\n", fulldllname);
       } /* endif */

       rc = DosDelete(fulldllname);
       if (rc) {
           printf("Couldn't delete %s. code:%ld\n", fulldllname,rc);
           return rc;
       } else {
            printf("Deleted %s.\n", fulldllname);
       } /* endif */
    }

    return 0;
}

int queryNumStorageProcs(void)
{
  MMFORMATINFO mmfinfo;
  LONG lNum=0;

  memset( &mmfinfo, '\0', sizeof(MMFORMATINFO) );
  mmfinfo.ulIOProcType=MMIO_IOPROC_STORAGESYSTEM;
  mmfinfo.ulMediaType=MMIO_MEDIATYPE_AUDIO;

  mmioQueryFormatCount(&mmfinfo, &lNum, 0, 0);
  return lNum;
}

BOOL getFormats(LONG lNum)
{
  MMFORMATINFO mmfinfo;
  PMMFORMATINFO pmmfi;

  LONG lFormatsRead;
  int a;

  memset( &mmfinfo, '\0', sizeof(MMFORMATINFO) );
  mmfinfo.ulIOProcType=MMIO_IOPROC_STORAGESYSTEM;
  //  mmfinfo.ulMediaType=MMIO_MEDIATYPE_AUDIO;

  if((pmmfi=(PMMFORMATINFO)malloc(lNum*sizeof(MMFORMATINFO)))==NULLHANDLE)
    return FALSE;

  printf("mmioGetFormats() returned: %d \n",
         mmioGetFormats(&mmfinfo, lNum, pmmfi, &lFormatsRead,0,0));

  printf("Info for %d formats read.\n\n", lFormatsRead);

  for(a=0;a<lFormatsRead;a++)
    {
      char name[1000];
      LONG lBytesRead;

      if(0==mmioGetFormatName(pmmfi, name, &lBytesRead, 0L, 0L))
        {
          name[lBytesRead]=0;
          printf("%2d: %s\n", a+1, name);
        }
      pmmfi++;
    }
  return TRUE;
}

int installProc(char * dllPath) {
    MMINIFILEINFO mmIniFileInfo;
    ULONG ulFlags = 0L;
    ULONG rc;
#if 0

    memset( &mmIniFileInfo, '\0', sizeof(MMINIFILEINFO) );
    mmIniFileInfo.fccIOProc = FOURCC_STREAM;//mmioStringToFOURCC(,0);
    ulFlags = MMIO_FINDPROC;
    rc = mmioIniFileHandler( &mmIniFileInfo,
                             ulFlags);
    if (rc) {
        printf("Couldn't find IOProc %s.  Code: %ld\n",Fourcc,rc);
        return rc;
    }

    ulFlags = MMIO_REMOVEPROC;
    rc = mmioIniFileHandler( &mmIniFileInfo,
                             ulFlags);
    if (rc) {
        printf("Couldn't remove IOProc %s.  Code: %ld\n",Fourcc,rc);
        return rc;
    } else {
        printf("Removed IOProc %s.\n",Fourcc);
    } /* endif */

    {
       char dllname[DLLNAME_SIZE] = "DLL\\";
       char fulldllname[DLLNAME_SIZE] = "";

       strncat(dllname, mmIniFileInfo.szDLLName,sizeof(dllname));
       strncat(dllname, ".DLL",sizeof(dllname));
       rc = DosSearchPath(2,"MMBASE",dllname,fulldllname,sizeof(fulldllname));
       if (rc) {
           printf("Couldn't find %s. code:%ld\n", dllname,rc);
           return rc;
       }

       rc = DosReplaceModule(fulldllname,NULL,NULL);
       if (rc) {
           printf("Couldn't unlock %s. code:%ld\n", fulldllname,rc);
       } else {
           printf("Unlocked %s.\n", fulldllname);
       } /* endif */

       rc = DosDelete(fulldllname);
       if (rc) {
           printf("Couldn't delete %s. code:%ld\n", fulldllname,rc);
           return rc;
       } else {
            printf("Deleted %s.\n", fulldllname);
       } /* endif */
    }
#endif
    return 0;
}

int main(int argc, char **argv) {

  printf("Test stream io proc...\n");
  
  printf("Installed storage procs: %d\n", queryNumStorageProcs());
  getFormats(queryNumStorageProcs());

  //  getFormats(45);
  installProc("STREAMIO.DLL");
  return 0;
}



